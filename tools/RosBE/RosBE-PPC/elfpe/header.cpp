#include <time.h>
#include "util.h"
#include "header.h"

ElfPeHeader::ElfPeHeader
	(uint32_t imagebase,
	 uint32_t filealign,
	 uint32_t sectionalign,
	 const ElfObjectFile::Symbol *entry,
	 uint32_t stackreserve,
	 uint32_t stackcommit,
	 uint32_t heapreserve,
	 uint32_t heapcommit,
	 int      subsysid,
	 bool     dll,
	 ElfObjectFile *eof) :
	imagebase(imagebase),
	sectionalign(sectionalign),
        filealign(filealign),
	stackreserve(stackreserve),
	stackcommit(stackcommit),
	heapreserve(heapreserve),
	heapcommit(heapcommit),
	subsysid(subsysid),
	entry(entry),
	eof(eof)
{
    data.resize(computeSize());
}

int ElfPeHeader::computeSize() const
{
    return roundup(0x80 + 0x200, filealign); /* We'll compute it for real later */
}

void ElfPeHeader::createHeaderSection(const std::vector<section_mapping_t> &sectionRvaSet, uint32_t imageSize)
{
    data[0] = 'M'; data[1] = 'Z';
    uint8_t *dataptr = &data[0x3c];
    uint32_t coffHeaderSize, optHeaderSizeMember;
    le32write_postinc(dataptr, 0x80);
    dataptr = &data[0x80];
    le32write_postinc(dataptr, 0x4550);
    le16write_postinc(dataptr, getPeArch());
    le16write_postinc(dataptr, sectionRvaSet.size());
    le32write_postinc(dataptr, time(NULL));
    le32write_postinc(dataptr, 0);
    le32write_postinc(dataptr, 0);
    optHeaderSizeMember = dataptr - &data[0];
    le16write_postinc(dataptr, 0); // Will fixup sizeof opt header
    le16write_postinc(dataptr, getExeFlags());
    coffHeaderSize = dataptr - &data[0];
    le16write_postinc(dataptr, 0);
    le16write_postinc(dataptr, 0x100);
    le32write_postinc(dataptr, 0);
    le32write_postinc(dataptr, 0);
    le32write_postinc(dataptr, 0);
    le32write_postinc(dataptr, getEntryPoint(sectionRvaSet, entry));
    le32write_postinc(dataptr, 0);
    le32write_postinc(dataptr, 0);
    le32write_postinc(dataptr, imagebase);
    le32write_postinc(dataptr, sectionalign);
    le32write_postinc(dataptr, filealign);
    le16write_postinc(dataptr, 4);
    le16write_postinc(dataptr, 0);
    le16write_postinc(dataptr, 1);
    le16write_postinc(dataptr, 0);
    le16write_postinc(dataptr, 4);
    le16write_postinc(dataptr, 0);
    le32write_postinc(dataptr, 0);
    le32write_postinc(dataptr, imageSize);
    le32write_postinc(dataptr, computeSize());
    le32write_postinc(dataptr, 0); // No checksum yet
    le16write_postinc(dataptr, subsysid);
    le16write_postinc(dataptr, getDllFlags());
    le32write_postinc(dataptr, stackreserve);
    le32write_postinc(dataptr, stackcommit);
    le32write_postinc(dataptr, heapreserve);
    le32write_postinc(dataptr, heapcommit);
    le32write_postinc(dataptr, 0);
    le32write_postinc(dataptr, 10); // # Directories
    // "Directories"
    le32pwrite_postinc(dataptr, getExportInfo(sectionRvaSet));
    le32pwrite_postinc(dataptr, getImportInfo(sectionRvaSet));
    le32pwrite_postinc(dataptr, getResourceInfo(sectionRvaSet));
    le32pwrite_postinc(dataptr, getExceptionInfo());
    le32pwrite_postinc(dataptr, getSecurityInfo());
    le32pwrite_postinc(dataptr, getRelocInfo(sectionRvaSet));
    le32pwrite_postinc(dataptr, getDebugInfo());
    le32pwrite_postinc(dataptr, getDescrInfo());
    le32pwrite_postinc(dataptr, getMachInfo());
    le32pwrite_postinc(dataptr, getTlsInfo());
    // Fixup size of optional header
    le16write
	(&data[0] + optHeaderSizeMember,
	 (dataptr - &data[0]) - coffHeaderSize);
    // Here, we store references to the sections, filling in the RVA and
    // size, but leaving out the other info.  We write the section name
    // truncated into the name field and leave the section id in the
    // physical address bit

    uint32_t paddr = computeSize();
    for (int i = 0; i < sectionRvaSet.size(); i++)
    {
	section_mapping_t mapping = sectionRvaSet[i];
	const ElfObjectFile::Section *section = mapping.section;
	std::string name = section->getName();
	uint32_t size = section->logicalSize();
        uint32_t psize = 
	    section->getType() == SHT_NOBITS ? 0 : roundup(size, filealign);
	uint32_t rva = mapping.rva;
	for (int j = 0; j < 8; j++)
	{
	    *dataptr++ = j < name.size() ? name[j] : '\000';
	}

#if 0
	printf("V %08x:%08x P %08x:%08x %s\n",
	       rva, size, paddr, psize, name.c_str());
#endif

	le32write_postinc(dataptr, size);
	le32write_postinc(dataptr, rva);
	le32write_postinc(dataptr, psize);
	le32write_postinc(dataptr, paddr);
	le32write_postinc(dataptr, 0);
	le32write_postinc(dataptr, 0);
	le32write_postinc(dataptr, 0);
        // XXX Figure out the real flags
	le32write_postinc(dataptr, IMAGE_SCN_CNT_CODE);
        paddr += psize;
    }
}

const std::vector<uint8_t> &ElfPeHeader::getData() const { return data; }

uint32_t ElfPeHeader::getSectionRvas(std::vector<section_mapping_t> &rvas) const
{
    uint32_t start = computeSize();
    uint32_t limit = start;

    rvas.clear();

    for(int i = 1; i < eof->getNumSections(); i++) {
	{
	    const ElfObjectFile::Section &sect = eof->getSection(i);
	    if(sect.getFlags() & SHF_ALLOC) {
		limit = roundup(start + sect.logicalSize(), sectionalign);
#if 0
		fprintf(stderr, "rva[%02d:-%20s] = (%08x %08x %08x %d)\n",
			rvas.size(), sect.getName().c_str(), start, sect.getStartRva(), sect.logicalSize(), i);
#endif
		rvas.push_back(section_mapping_t(&sect, start, i));
	    }
	}
	start = limit;
    }

    return limit;
}

uint16_t ElfPeHeader::getPeArch() const
{
    return IMAGE_FILE_MACHINE_POWERPCBE; /* for now */
}

u32pair_t getNamedSectionInfo(ElfObjectFile *eof, const std::vector<section_mapping_t> &mapping, const std::string &name)
{
    const ElfObjectFile::Section *sect = eof->getNamedSection(name);
    uint32_t sectaddr;
    int i;

    if(sect)
    {
	for(i = 0; i < mapping.size(); i++)
	    if(mapping[i].index == sect->getNumber())
	    {
		return std::make_pair
		    (mapping[i].rva, sect->logicalSize());
	    }
    }
    return std::make_pair(0,0);
}

u32pair_t ElfPeHeader::getExportInfo(const std::vector<section_mapping_t> &mapping) const
{
    return getNamedSectionInfo(eof, mapping, ".edata");
}

u32pair_t ElfPeHeader::getImportInfo(const std::vector<section_mapping_t> &mapping) const
{
    return getNamedSectionInfo(eof, mapping, ".idata");
}

u32pair_t ElfPeHeader::getResourceInfo(const std::vector<section_mapping_t> &mapping) const
{
    return getNamedSectionInfo(eof, mapping, ".rsrc");
}

u32pair_t ElfPeHeader::getExceptionInfo() const
{
    return std::make_pair(0,0);
}

u32pair_t ElfPeHeader::getSecurityInfo() const
{
    return std::make_pair(0,0);
}

u32pair_t ElfPeHeader::getRelocInfo(const std::vector<section_mapping_t> &mapping) const
{
    return getNamedSectionInfo(eof, mapping, ".reloc");
}

u32pair_t ElfPeHeader::getDebugInfo() const
{
    return std::make_pair(0,0);
}

u32pair_t ElfPeHeader::getDescrInfo() const
{
    return std::make_pair(0,0);
}

u32pair_t ElfPeHeader::getTlsInfo() const
{
    return std::make_pair(0,0);
}

u32pair_t ElfPeHeader::getMachInfo() const
{
    return std::make_pair(0,0);
}

uint32_t ElfPeHeader::getEntryPoint
(const std::vector<section_mapping_t> &secmap,
 const ElfObjectFile::Symbol *entry) const
{
    if(entry == NULL) return computeSize();
    for(int i = 0; i < secmap.size(); i++) {
	if(secmap[i].index == entry->section)
	    return secmap[i].rva + entry->offset;
    }
    return computeSize();
}
