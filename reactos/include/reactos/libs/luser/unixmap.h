#ifndef _NTOSKRNL_LUSER_MAPS_H
#define _NTOSKRNL_LUSER_MAPS_H

typedef struct _unix_map_t {
    ULONG_PTR start, end;
    ULONG_PTR offset;
    int isram;
    int prot;
} unix_map_t;

int MapOpen();
void MapClose(int mapfd);
int MapRead(int mapfd, unix_map_t *map);
ULONG_PTR MapGetTableAddr();
HARDWARE_PTE MapGetFirstLevelPTE(ULONG_PTR Addr);
HARDWARE_PTE MapGetFirstLevelPTEWithCR(ULONG_PTR CR, ULONG_PTR Addr);
HARDWARE_PTE MapGetSecondLevelPTE(HARDWARE_PTE FirstLvlPte, ULONG_PTR Addr);
HARDWARE_PTE MapGetPTEWithRefresh(ULONG_PTR Addr);

#endif/*_NTOSKRNL_LUSER_MAPS_H*/
