#ifndef COMPDVR_IMPORTS_H
#define COMPDVR_IMPORTS_H

#include <vector>
#include <utility>
#include "pedef.h"
#include "util.h"
#include "objectfile.h"
#include "section.h"

void ImportFixup
(ElfObjectFile &eof, const std::vector<section_mapping_t> &mapping);

#endif//COMPDVR_IMPORTS_H
