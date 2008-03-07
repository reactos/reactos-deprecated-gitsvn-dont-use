#ifndef COMPDVR_EXPORTS_H
#define COMPDVR_EXPORTS_H

#include <vector>
#include <utility>
#include "pedef.h"
#include "util.h"
#include "objectfile.h"
#include "section.h"

void ExportFixup
(ElfObjectFile &eof, const std::vector<section_mapping_t> &mapping);

#endif//COMPDVR_EXPORTS_H
