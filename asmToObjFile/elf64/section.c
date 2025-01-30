#include "section.h"
#include "stdlib.h"

// Already filled
Elf64_Shdr shdrNull = {.sh_name = 0,.sh_type = SHT_NULL,.sh_flags = 0,.sh_addr = 0,.sh_offset = 0,.sh_size = 0,.sh_link = 0,.sh_info = 0,.sh_addralign = 0,.sh_entsize = 0};

// Need sh_name / sh_offset / sh_size
Elf64_Shdr shdrText = {.sh_type = SHT_PROGBITS,.sh_flags = SHF_ALLOC | SHF_EXECINSTR,.sh_addr = 0,.sh_link = 0,.sh_info = 0,.sh_addralign = 16,.sh_entsize = 0};
Elf64_Shdr shdrShstrtab = {.sh_type = SHT_STRTAB,.sh_flags = 0,.sh_addr = 0,.sh_link = 0,.sh_info = 0,.sh_addralign = 1,.sh_entsize = 0};
Elf64_Shdr shdrRodata = {.sh_type = SHT_PROGBITS,.sh_flags = SHF_ALLOC,.sh_addr = 0,.sh_link = 0,.sh_info = 0,.sh_addralign = 4,.sh_entsize = 0};
Elf64_Shdr shdrStrtab = {.sh_type = SHT_STRTAB,.sh_flags = 0,.sh_addr = 0,.sh_link = 0,.sh_info = 0,.sh_addralign = 1,.sh_entsize = 0};

// Need sh_name / sh_offset / sh_size / sh_link / sh_info
Elf64_Shdr shdrSymtab = {.sh_type = SHT_SYMTAB,.sh_flags = 0,.sh_addr = 0,.sh_addralign = 8,.sh_entsize = sizeof(Elf64_Sym)};
Elf64_Shdr shdrRela = {.sh_type = SHT_RELA,.sh_flags = 0,.sh_addr = 0,.sh_addralign = 8,.sh_entsize = sizeof(Elf64_Rela)};

// Create a new Section
Error sectionInit(Section section, SectionHeaderType sectionType){
    Error err = 0;

    section->sectionType = sectionType;

    switch (sectionType)
    {
    case SECTION_NULL:
        section->header = shdrNull;
        break;
    case SECTION_TEXT:
        section->header = shdrText;
        break;
    case SECTION_SHSTRTAB:
        section->header = shdrShstrtab;
        break;
    case SECTION_RODATA:
        section->header = shdrRodata;
        break;
    case SECTION_STRTAB:
        section->header = shdrStrtab;
        break;
    case SECTION_SYMTAB:
        section->header = shdrSymtab;
        break;
    case SECTION_RELA:
        section->header = shdrRela;
        break;
    default:
        section->header = shdrNull;
        err = 1;
        break;
    }
    
    section->contentSize = 0;
    section->storageSize = 0;
    section->data = NULL;
    return err;
}


Error sectionAppendData(Section section, char * data, uint64_t dataSize){
    // TODO
    return 0;
}