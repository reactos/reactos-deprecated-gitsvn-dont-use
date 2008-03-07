#ifndef _ELFPE_RELOC_H
#define _ELFPE_RELOC_H

#include <vector>
#include "section.h"
#include "objectfile.h"

void SingleRelocSection
(const ElfObjectFile &obf,
 const ElfObjectFile::Section &section,
 const std::vector<section_mapping_t> &rvas,
 std::vector<uint8_t> &relocData,
 uint32_t imageBase,
 uint32_t &relocAddr);

void AddReloc
(std::vector<uint8_t> &reloc, 
 uint32_t imageBase,
 uint32_t &relocAddrOff,
 uint32_t newReloc, int type);

#endif//_ELFPE_RELOC_H
