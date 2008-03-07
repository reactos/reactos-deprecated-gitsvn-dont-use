#ifndef COMPDVR_COFF_H
#define COMPDVR_COFF_H

#include <vector>
#include <utility>
#include "pedef.h"
#include "util.h"
#include "objectfile.h"
#include "section.h"

class PECoffExecutable {
public:
    PECoffExecutable(const ElfObjectFile &elf, const std::string &tmpFile, uint32_t filealign);
    void Write(const std::string &resultFile);

private:
    const ElfObjectFile &elf;
    std::string tempFile;
    uint32_t filealign;
};

#endif//COMPDVR_COFF_H
