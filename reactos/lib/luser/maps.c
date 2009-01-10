#include <ddk/ntddk.h>
#include <ddk/ntifs.h>
#include <ndk/ntndk.h>
#include <luser.h>
#include <lunix.h>
#include <lstorage.h>
#include <unixmap.h>

#define MEMFD 100
ULONG MemSize;

unsigned long strtoul(char *, int, int);

int MapOpen()
{
    char mapsname[] = "/proc/self/maps";
    int mapfd = unix_open(mapsname, O_RDONLY, 0);
    if (mapfd == -1) unix_abort();
    return mapfd;
}

void MapClose(int mapfd)
{
    unix_close(mapfd);
}

int MapRead(int mapfd, unix_map_t *map)
{
    char *place, *dash, *space, *off, mapline[256];
    int i, protmask = 0, rb;
    int protconv[] = {
        PAGE_NOACCESS,
        PAGE_READONLY,
        PAGE_READWRITE,
        PAGE_READWRITE,
        PAGE_EXECUTE,
        PAGE_EXECUTE_READ,
        PAGE_EXECUTE_READWRITE,
        PAGE_EXECUTE_READWRITE
    };
    place = mapline;
    while ((rb = unix_read(mapfd, place, 1)) != -1 && *place >= ' ') place++;
    *place = 0;
    if (rb < 1) return 0;
    //Printf("Map:[%s]\n", mapline);
    dash = strchr(mapline, '-');
    if (!dash) return 0;
    *dash = 0;
    memset(map, 0, sizeof(*map));
    map->start = strtoul(mapline, 0, 16);
    //Printf("%s -> %08x\n", mapline, map->start);
    dash++;
    space = strchr(dash, ' ');
    if (!space) return 0;
    *space = 0;
    map->end = strtoul(dash, 0, 16);
    space++;
    for (i = 0; i < 3; i++)
        if (space[i] != '-') 
            protmask |= 1<<i;
    map->prot = protconv[protmask];
    space = strchr(space, ' ');
    if (!space) return 0;
    *space = 0;
    space++;
    off = space;
    space = strchr(space, ' ');
    *space = 0;
    space++;
    map->offset = strtoul(off, 0, 16);
    map->isram = !!strstr(space, "/tmp/mem");
    //Printf("Map: %08x-%08x @ %08x %c\n", map->start, map->end, map->offset, map->isram ? 'r':'.');
    return 1;
}

ULONG_PTR MapGetTableAddr()
{
    return __readcr3();
}

HARDWARE_PTE MapGetFirstLevelPTEWithCR(ULONG_PTR CR, ULONG_PTR Addr)
{
    HARDWARE_PTE Pte;
    ULONG_PTR PageTableAddr = CR;
    ULONG_PTR PageTableEntryAddr = 
        (4 * ((Addr >> 22) & 0x3ff)) + PageTableAddr;
    unix_lseek(MEMFD, PageTableEntryAddr, 0);
    unix_read(MEMFD, (char *)&Pte, sizeof(Pte));
    return Pte;
}

HARDWARE_PTE MapGetFirstLevelPTE(ULONG_PTR Addr)
{
    return MapGetFirstLevelPTEWithCR(MapGetTableAddr(), Addr);
}

HARDWARE_PTE MapGetSecondLevelPTE(HARDWARE_PTE FirstLvlPte, ULONG_PTR Addr)
{
    HARDWARE_PTE SecondPte;
    ULONG_PTR PageTableEntryAddr = 
        (4 * ((Addr >> 12) & 0x3ff)) + (FirstLvlPte.PageFrameNumber << 12);
    unix_lseek(MEMFD, PageTableEntryAddr, 0);
    unix_read(MEMFD, (char *)&SecondPte, sizeof(SecondPte));
    return SecondPte;
}

VOID MapRefreshAddr(ULONG_PTR TableAddr)
{
    unix_map_t mapping;
    int map = MapOpen();
    if (map == -1) unix_abort();
    while (MapRead(map, &mapping))
    {
        // Go through maps and evict this page table if mapped
        if (mapping.offset <= TableAddr && mapping.offset + mapping.end - mapping.start > TableAddr)
        {
            //Printf("Found mapped %x-%x and evicted it\n", mapping.start, mapping.end);
            unix_msync((PVOID)mapping.start, mapping.end - mapping.start, MS_SYNC);
            break;
        }
    }
    MapClose(map);
}

HARDWARE_PTE MapGetPTEWithRefresh(ULONG_PTR Addr)
{
    HARDWARE_PTE FirstLevel, SecondLevel = { };
    ULONG_PTR TableAddr = MapGetTableAddr();
    //MapRefreshAddr(TableAddr);
    FirstLevel = MapGetFirstLevelPTEWithCR(TableAddr, Addr);
    if (!FirstLevel.Valid) return SecondLevel;
    //MapRefreshAddr(FirstLevel.PageFrameNumber << 12);
    return MapGetSecondLevelPTE(FirstLevel, Addr);
}

PHARDWARE_PTE MapFindRevMapping(ULONG_PTR Addr)
{
    PHARDWARE_PTE pte;
    unix_map_t mapping;
    int res = 0, map = MapOpen();
    Addr &= ~0xfff;
    while (MapRead(map, &mapping))
    {
        if (mapping.offset <= Addr && mapping.offset + (mapping.end - mapping.start > Addr))
        {
            res = mapping.start + (Addr - mapping.offset);
            break;
        }
    }
    MapClose(map);
    pte = (PHARDWARE_PTE)res;
    return pte;
}

void MapDump()
{
    int cr = LuserReadCR(3);
    int i,j,map = MapOpen();
    unix_map_t mapping = { };
    HARDWARE_PTE First, SecondMapped, Second, *Mapped, SecondHard = { };

    Printf(",---- CR3 is %x\n", cr);

    for (i = 0; i < 1024; i++)
    {
        unix_lseek(MEMFD, cr + i * sizeof(HARDWARE_PTE), 0);
        unix_read(MEMFD, (char *)&First, sizeof(First));

        Mapped = MapFindRevMapping(First.PageFrameNumber << PAGE_SHIFT);
        unix_lseek(MEMFD, First.PageFrameNumber << PAGE_SHIFT, 0);

        if (!First.Valid) continue;

        Printf(" %08x -> %08x\n", i << 22, First);

        for (j = 0; j < 1024; j++)
        {
            unix_read(MEMFD, (char *)&Second, sizeof(Second));
            memset(&SecondHard, 0, sizeof(SecondHard));
            while (((i << 22) | (j << 12)) > mapping.start &&
                   MapRead(map, &mapping));
            if (mapping.start <= ((i << 22) | (j << 12)) &&
                mapping.end > ((i << 22) | (j << 12)))
            {
                SecondHard.PageFrameNumber = mapping.offset >> 12;
                SecondHard.Valid = 1;
            }
            memset(&SecondMapped, 0, sizeof(SecondMapped));
#if 0
            if (Mapped)
                SecondMapped = Mapped[j];
#endif
            if (Second.Valid || SecondMapped.Valid || SecondHard.Valid)
                Printf
                    ("  %08x: F%08x M%08x H%08x\n", 
                     (i << 22) | (j << 12),
                     Second, 
                     SecondMapped,
                     SecondHard);
        }
    }

    Printf("`---- Complete\n");
    MapClose(map);
}
