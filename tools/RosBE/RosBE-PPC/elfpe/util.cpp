#include "util.h"
#include <assert.h>

uint32_t roundup(uint32_t value, int round)
{
    round--;
    return (value + round) & ~round;
}

void le16write(uint8_t *dataptr, uint16_t value)
{
    dataptr[0] = value;
    dataptr[1] = value >> 8;
}

void le16write_postinc(uint8_t *&dataptr, uint16_t value)
{
    le16write(dataptr, value); dataptr += 2;
}

void le32write(uint8_t *dataptr, uint32_t value)
{
    le16write(dataptr, value);
    le16write(dataptr + 2, value >> 16);
}

void le32write_postinc(uint8_t *&dataptr, uint32_t value)
{
    le32write(dataptr, value); dataptr += 4;
}

void le32pwrite_postinc(uint8_t *&dataptr, const u32pair_t &value)
{
    le32write_postinc(dataptr, value.first);
    le32write_postinc(dataptr, value.second);
}

void be16write_postinc(uint8_t *&dataptr, uint16_t value)
{
    *dataptr++ = value >> 8;
    *dataptr++ = value;
}

void be16write(uint8_t *dataptr, uint16_t value)
{
    be16write_postinc(dataptr, value);
}

void be32write_postinc(uint8_t *&dataptr, uint32_t value)
{
    be16write_postinc(dataptr, value >> 16);
    be16write_postinc(dataptr, value);
}

void be32write(uint8_t *dataptr, uint32_t value)
{
    be32write_postinc(dataptr, value);
}

uint16_t be16read(uint8_t *dataptr) 
{
    return dataptr[0] << 8 | dataptr[1];
}

uint16_t be16read_postinc(uint8_t *&dataptr)
{
    uint16_t res = be16read(dataptr);
    dataptr += 2;
    return res;
}

uint32_t be32read(uint8_t *dataptr)
{
    return (be16read(dataptr) << 16) | be16read(dataptr+2);
}

uint32_t be32read_postinc(uint8_t *&dataptr)
{
    uint32_t res = be32read(dataptr);
    dataptr += 4;
    return res;
}

uint32_t FindRVA(const std::vector<section_mapping_t> &mapping, int secnum)
{
    int i;
    for (i = 0; i < mapping.size(); i++)
	if (mapping[i].section->getNumber() == secnum) return mapping[i].rva;
    return 0;
}

uint8_t *TargetPtr
(const std::vector<section_mapping_t> &mapping, 
 const ElfObjectFile::Section &target, uint32_t va)
{
    uint32_t srva = FindRVA(mapping, target.getNumber());
    uint32_t off = va - srva;
#if 0
    printf
        ("Relocating against VA %08x in section (%08x-%08x) %s\n", 
         va, 
         srva, 
         srva + target.logicalSize(), 
         target.getName().c_str());
#endif
    assert(off < target.logicalSize());
    return target.getSectionData() + off;
}

uint32_t *GetRelocTarget(const std::vector<uint8_t> &reloc, uint32_t off)
{
    return (uint32_t *)(&reloc[0] + off);
}

