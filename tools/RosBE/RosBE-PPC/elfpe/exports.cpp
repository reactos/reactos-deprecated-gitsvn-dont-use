#include "exports.h"
#include "objectfile.h"
#include "section.h"

void ExportFixup
(ElfObjectFile &eof,
 const std::vector<section_mapping_t> &mapping)
{
    const ElfObjectFile::Section *exportSect = eof.getNamedSection(".edata");
    if (!exportSect) return;
    uint8_t *exportTable = exportSect->getSectionData();
    // Fixup the words of the export directory
    uint8_t *exportTarget = exportTable;
    uint32_t exportRva = FindRVA(mapping, exportSect->getNumber());

    uint32_t nameRva, ordinalBase, numberOfAddress, numberOfNames, addressRva,
        namePtrRva, ordinalTableRva;
    int i;

    le32write_postinc(exportTarget, be32read(exportTarget));
    le32write_postinc(exportTarget, be32read(exportTarget));
    le16write_postinc(exportTarget, be16read(exportTarget));
    le16write_postinc(exportTarget, be16read(exportTarget));
    le32write_postinc(exportTarget, (nameRva = be32read(exportTarget)));
    le32write_postinc(exportTarget, (ordinalBase = be32read(exportTarget)));
    le32write_postinc(exportTarget, (numberOfAddress = be32read(exportTarget)));
    le32write_postinc(exportTarget, (numberOfNames = be32read(exportTarget)));
    le32write_postinc(exportTarget, (addressRva = be32read(exportTarget)));
    le32write_postinc(exportTarget, (namePtrRva = be32read(exportTarget)));
    le32write_postinc(exportTarget, (ordinalTableRva = be32read(exportTarget)));
    
    // Address Table
    exportTarget = exportTable + addressRva - exportRva;
    for (i = 0; i < numberOfAddress; i++)
        le32write_postinc(exportTarget, be32read(exportTarget));
    
    // Name table
    exportTarget = exportTable + namePtrRva - exportRva;
    for (i = 0; i < numberOfNames; i++)
        le32write_postinc(exportTarget, be32read(exportTarget));

    // Ordinal table
    exportTarget = exportTable + ordinalTableRva - exportRva;
    for (i = 0; i < numberOfAddress; i++)
        le16write_postinc(exportTarget, be16read(exportTarget));
}
