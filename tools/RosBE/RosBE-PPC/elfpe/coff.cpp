#include "coff.h"

PECoffExecutable::PECoffExecutable
(const ElfObjectFile &elf, const std::string &file, uint32_t filealign)
    : elf(elf), tempFile(file), filealign(filealign)
{
    int i;
    FILE *f = fopen(tempFile.c_str(), "wb");
    if (!f) return;

    // Write PE Header
    const ElfObjectFile::Section &section = *elf.getNamedSection(".peheader");
    fwrite(section.getSectionData(), 1, section.logicalSize(), f);
    
    // Write out sections
    uint32_t physSize;
    uint32_t bytes = 0;

    for (i = 1; i < elf.getNumSections(); i++)
    {
        const ElfObjectFile::Section &section = elf.getSection(i);
        if (section.getName() == ".peheader") continue;
	if (section.getType() != SHT_NOBITS && section.getFlags() & SHF_ALLOC)
	{
	    physSize = roundup(section.logicalSize(), filealign);
#if 0
	    printf("P %08x:%08x %s\n", 
		   (int)ftell(f),
		   section.logicalSize(), 
		   section.getName().c_str());
#endif
	    bytes = fwrite(section.getSectionData(), 1, section.logicalSize(), f);
	    while(bytes++ < physSize) fputc(0, f);
	}
    }
    fclose(f);
}

void PECoffExecutable::Write(const std::string &resultFile)
{
    rename(tempFile.c_str(), resultFile.c_str());
}
