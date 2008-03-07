#include "objectfile.h"
#include "reloc.h"
#include "section.h"
#include "util.h"
#include <assert.h>

void AddReloc
(std::vector<uint8_t> &reloc, 
 uint32_t imageBase,
 uint32_t &relocAddrOff,
 uint32_t newReloc, int type)
{
    size_t oldsize = reloc.size(), oddsize = oldsize & sizeof(uint16_t);
    uint8_t *relptr;
    if (relocAddrOff == (uint32_t)-1)
    {
	reloc.resize(sizeof(uint32_t) * 2 + sizeof(uint16_t));
	relocAddrOff = 0;
	*GetRelocTarget(reloc, relocAddrOff) = newReloc & ~0xfff;
	relptr = &reloc[2 * sizeof(uint32_t)];
    }
    else if (type != -1 && ((newReloc & ~0xfff) != *GetRelocTarget(reloc, relocAddrOff)))
    {
        reloc.resize(reloc.size() + 2 * sizeof(uint32_t) + sizeof(uint16_t) + oddsize);
        relptr = &reloc[oldsize + oddsize];
	uint8_t *oldptr = (uint8_t*)GetRelocTarget(reloc, relocAddrOff);
	le32write_postinc(oldptr, *GetRelocTarget(reloc, relocAddrOff) - imageBase);
	le32write_postinc(oldptr, relptr - &reloc[0] - relocAddrOff);
	relocAddrOff = relptr - &reloc[0];
        *GetRelocTarget(reloc, relocAddrOff) = newReloc & ~0xfff;
        relptr += sizeof(uint32_t);
	le32write_postinc(relptr, 0);
    }
    else
    {
        reloc.resize(reloc.size() + sizeof(uint16_t));
        relptr = &reloc[oldsize];
    }

    le16write(relptr, type << 12 | newReloc & 0xfff);
}

#define ADDR24_MASK 0xfc000003

void SingleReloc
(const ElfObjectFile &eof,
 uint8_t *relptr, int relocsize, 
 const std::vector<section_mapping_t> &rvas,
 const ElfObjectFile::Section &symbols,
 const ElfObjectFile::Section &target,
 std::vector<uint8_t> &relocSect,
 uint32_t imageBase,
 uint32_t &relocAddr)
{
    int j;
    Elf32_Rela reloc = { 0 };
    Elf32_Sym symbol;
    uint8_t *symptr;
    uint32_t S,A,P;
    
    /* Get the reloc */
    memcpy(&reloc, relptr, relocsize);
#if 0
    printf("RELOC: offset %08x info %08x addend %08x [%02x %06x]\n", 
           reloc.r_offset, reloc.r_info, reloc.r_addend,
           ELF32_R_TYPE(reloc.r_info), ELF32_R_SYM(reloc.r_info));
#endif
    
    /* Get the symbol */
    symptr = &symbols.getSectionData()
        [ELF32_R_SYM(reloc.r_info) * sizeof(symbol)];
    memcpy(&symbol, symptr, sizeof(symbol));

    /* Compute addends */
    S = symbol.st_value - 
        eof.getSection(symbol.st_shndx).getStartRva() + 
        FindRVA(rvas, symbol.st_shndx) + 
        imageBase;
    A = reloc.r_addend;
    P = reloc.r_offset + FindRVA(rvas, target.getNumber()) - target.getStartRva();
    //printf("start of target elf section %08x\n", target.getStartRva());

#if 0
    printf("SYMBOL: value %08x size %08x info %02x other %02x shndx %08x total %08x\n",
           symbol.st_value,
           symbol.st_size,
           symbol.st_info,
           symbol.st_other,
           symbol.st_shndx,
           S);
#endif

    uint8_t *Target = TargetPtr(rvas, target, P);
    uint8_t *tword = TargetPtr(rvas, target, P & ~3);
    uint8_t oldBytes[sizeof(uint32_t)];
    memcpy(oldBytes, tword, sizeof(oldBytes));
    
    P += imageBase;

    switch (ELF32_R_TYPE(reloc.r_info))
    {
    case R_PPC_NONE:
        break;
    case R_PPC_ADDR32:
        //printf("ADDR32 S %08x A %08x P %08x\n", S, A, P);
        be32write(Target, S + A);
        AddReloc(relocSect, imageBase, relocAddr, P, 3);
        break;
    case R_PPC_REL32:
        //printf("REL32 S %08x A %08x P %08x\n", S, A, P);
        be32write(Target, S + A - P);
        break;
    case R_PPC_UADDR32: /* Special: Treat as RVA */
        //printf("UADDR32 S %08x A %08x P %08x\n", S, A, P);
        be32write(Target, S + A - imageBase);
        break;
    case R_PPC_REL24:
        //printf("REL24 S %08x A %08x P %08x\n", S, A, P);
        //printf("New Offset: %08x to Addr %08x from %08x\n", S+A-P, S+A, P);
        be32write(Target, ((S+A-P) & ~ADDR24_MASK) | (be32read(Target) & ADDR24_MASK));
        break;
    case R_PPC_ADDR16_LO:
        //printf("ADDR16_LO S %08x A %08x P %08x\n", S, A, P);
        be16write(Target, S + A);
        AddReloc(relocSect, imageBase, relocAddr, P, 2);
        break;
    case R_PPC_ADDR16_HA:
        //printf("ADDR16_HA S %08x A %08x P %08x\n", S, A, P);
        be16write(Target, (S + A + 0x8000) >> 16);
        AddReloc(relocSect, imageBase, relocAddr, P, 4);
	AddReloc(relocSect, imageBase, relocAddr, S + A, -1);
        break;
    default:
        break;
    }

    uint8_t newBytes[sizeof(uint32_t)];
    memcpy(newBytes, tword, sizeof(newBytes));
#if 0
    printf("Reloc changed %08x [%02x %02x %02x %02x] --> [%02x %02x %02x %02x]\n",
           P & ~3,
           oldBytes[0],
           oldBytes[1],
           oldBytes[2],
           oldBytes[3],
           newBytes[0],
           newBytes[1],
           newBytes[2],
           newBytes[3]);
#endif
}

void SingleRelocSection
(const ElfObjectFile &obf, 
 const ElfObjectFile::Section &section,
 const std::vector<section_mapping_t> &rvas,
 std::vector<uint8_t> &relocData,
 uint32_t imageBase,
 uint32_t &relocAddr)
{
    Elf32_Rela reloc = { };
    uint8_t *Target;
    int numreloc, relstart, relsize, j;
    uint8_t *sectionData = section.getSectionData();
    
    relsize = section.getType() == SHT_RELA ? 12 : 8;
    numreloc = section.logicalSize() / relsize;
    const ElfObjectFile::Section &targetSection = obf.getSection(section.getInfo());

    /* Don't relocate non-program section */
    if (!(targetSection.getFlags() & SHF_ALLOC))
	return;

    targetSection.setDirty();

    /* Get the symbol section */
    const ElfObjectFile::Section &symbolSection = obf.getSection(section.getLink());

    for(j = 0; j < numreloc; j++)
    {
        SingleReloc
            (obf,
             sectionData + j * relsize, 
             relsize,
	     rvas,
             symbolSection,
             targetSection,
             relocData,
             imageBase,
	     relocAddr);
    }
}
