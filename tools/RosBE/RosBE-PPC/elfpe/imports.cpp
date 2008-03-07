#include "imports.h"
#include "objectfile.h"
#include "section.h"

void ImportFixup
(ElfObjectFile &eof,
 const std::vector<section_mapping_t> &mapping)
{
    const ElfObjectFile::Section *importSect = eof.getNamedSection(".idata");
    if (!importSect) return;
    uint8_t *importTable = importSect->getSectionData();
    uint8_t *importTarget = importTable, *tableAddr, *hintName;
    uint32_t importRva = FindRVA(mapping, importSect->getNumber());
    uint32_t tableRva, iatRva, hintNameEntry;

    do
    {
        le32write_postinc(importTarget, (tableRva = be32read(importTarget)));
        le32write_postinc(importTarget, be32read(importTarget));
        le32write_postinc(importTarget, be32read(importTarget));
        le32write_postinc(importTarget, be32read(importTarget));
        le32write_postinc(importTarget, (iatRva = be32read(importTarget)));

        if (!tableRva) return;

        // Rewrite the import lookup table
        tableAddr = importTable + tableRva - importRva;
        while (hintNameEntry = be32read(tableAddr))
        {
            le32write_postinc(tableAddr, hintNameEntry);
            // Rewrite the hint/name element
            //hintName = importTable + hintNameEntry - importRva;
            //le16write(hintName, be16read(hintName));
        }

        // Do the second address table
        tableAddr = importTable + iatRva - importRva;
        while (hintNameEntry = be32read(tableAddr))
        {
            le32write_postinc(tableAddr, hintNameEntry);
        }
    } while(1);
}
