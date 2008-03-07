#ifndef COMPDVR_SECTION_H
#define COMPDVR_SECTION_H

typedef struct section_mapping_t {
    const ElfObjectFile::Section *section;
    uint32_t rva;
    int index;
    
    section_mapping_t
    (const ElfObjectFile::Section *sect, uint32_t rva, int index) :
	section(sect), rva(rva), index(index) { }
    section_mapping_t(const section_mapping_t &other) :
	section(other.section), rva(other.rva), index(other.index) { }
} section_mapping_t;

#endif//COMPDVR_SECTION_H
