#include <stdlib.h>
#include <string.h>

#include "section.h"
#include "secManager.h"

// Local function definition
static uint64_t _next_power_of_2(uint64_t);

// Already filled
Elf64_Shdr shdrNull = {.sh_name = 0,.sh_type = SHT_NULL,.sh_flags = 0,.sh_addr = 0,.sh_offset = 0,.sh_size = 0,.sh_link = 0,.sh_info = 0,.sh_addralign = 0,.sh_entsize = 0};

// Need sh_name / sh_offset / sh_size
Elf64_Shdr shdrText = {.sh_type = SHT_PROGBITS,.sh_flags = SHF_ALLOC + SHF_EXECINSTR,.sh_addr = 0,.sh_link = 0,.sh_info = 0,.sh_addralign = 16,.sh_entsize = 0};
Elf64_Shdr shdrShstrtab = {.sh_type = SHT_STRTAB,.sh_flags = 0,.sh_addr = 0,.sh_link = 0,.sh_info = 0,.sh_addralign = 1,.sh_entsize = 0};
Elf64_Shdr shdrRodata = {.sh_type = SHT_PROGBITS,.sh_flags = SHF_ALLOC,.sh_addr = 0,.sh_link = 0,.sh_info = 0,.sh_addralign = 4,.sh_entsize = 0};
Elf64_Shdr shdrStrtab = {.sh_type = SHT_STRTAB,.sh_flags = 0,.sh_addr = 0,.sh_link = 0,.sh_info = 0,.sh_addralign = 1,.sh_entsize = 0};

// Need sh_name / sh_offset / sh_size / sh_link / sh_info
Elf64_Shdr shdrSymtab = {.sh_type = SHT_SYMTAB,.sh_flags = 0,.sh_addr = 0,.sh_addralign = 8,.sh_entsize = sizeof(Elf64_Sym)};
Elf64_Shdr shdrRela = {.sh_type = SHT_RELA,.sh_flags = 0,.sh_addr = 0,.sh_addralign = 8,.sh_entsize = sizeof(Elf64_Rela)};

// Create a new Section
Section * sectionNew(SectionType sectionType, SectionHeader * secHead){
    // TODO error managing
    uint64_t headOfAddedData = 0;
    
    Section * section = malloc(sizeof(Section));
    
    section->sectionType = sectionType;
    section->contentSize = 0;
    section->storageSize = 0;
    section->data = NULL;


    // If not SECTION_RELA
    switch (sectionType)
    {
    case SECTION_NULL:
        section->header = shdrNull;
        break;
    case SECTION_TEXT:
        section->header = shdrText;
        sectionHeaderAddDataToSection(secHead, SECTION_SHSTRTAB, ".text", strlen(".text")+1, &headOfAddedData);
        break;
    case SECTION_SHSTRTAB:
        section->header = shdrShstrtab;
        sectionAppendData(section, ".shstrtab", strlen(".shstrtab")+1, &headOfAddedData);
        break;
    case SECTION_RODATA:
        section->header = shdrRodata;
        sectionHeaderAddDataToSection(secHead, SECTION_SHSTRTAB, ".rodata", strlen(".rodata")+1, &headOfAddedData);
        break;
    case SECTION_STRTAB:
        section->header = shdrStrtab;
        sectionHeaderAddDataToSection(secHead, SECTION_SHSTRTAB, ".strtab", strlen(".strtab")+1, &headOfAddedData);
        break;
    case SECTION_SYMTAB:
        section->header = shdrSymtab;
        sectionHeaderAddDataToSection(secHead, SECTION_SHSTRTAB, ".symtab", strlen(".symtab")+1, &headOfAddedData);
        section->header.sh_link = sectionHeaderGetIndex(secHead, SECTION_STRTAB);
        section->header.sh_info = 0;
        break;
    // TODO SECTION_RELA case
    // case SECTION_RELA:
    //     section->header = shdrRela;
    //     // TODO name depend on sh_link (.rela.text if linked on .text) 
    //     // TODO add sh_link / sh_info
    //     // TODO extract position of the name to fill sh_name
    //     sectionHeaderAddDataToSection(secHead, SECTION_SHSTRTAB, ".rela.text", strlen(".rela.text")+1);
    //     break;
    default:
        section->header = shdrNull;
        break;
    }

    section->header.sh_name = headOfAddedData;

    return sectionType;
}

// Free Section memory
void sectionFree(Section * section){
    if(section!=NULL){
        if(section->data!=NULL){
            free(section->data);
        }
        free(section);
    }
}

// Get the lowest power of 2 that is greater than n
static uint64_t _next_power_of_2(uint64_t n) {
    if (n == 0) return 1;

    n--; // Évite le cas où `n` est déjà une puissance de 2
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    n |= n >> 32;
    return n + 1;
}


Error sectionAppendData(Section * section, char * data, uint64_t dataSize, uint64_t * headOfData){
    // TODO need to update sh_info of SECTION_SYMTAB
    Error err = NO_ERROR;

    if(section->contentSize + dataSize <= section->storageSize){
        // It's possible to copy data in section storage
        memcpy(section->data + section->contentSize, data, dataSize);
        *headOfData = section->contentSize;
        section->contentSize += dataSize;
    }else{
        // Data isn't enough to store both previous and new data
        uint64_t newStorageSize = _next_power_of_2(section->contentSize + dataSize);

        char * newStorage = malloc(newStorageSize);
        if(newStorage==NULL){
            err = FAILED_TO_ALLOCATE;
            return err;
        }

        if(section->storageSize != 0 && section->contentSize != 0 && section->data != NULL){
            memcpy(newStorage, section->data, section->contentSize);
            free(section->data);
        }

        memcpy(newStorage, data, dataSize);
        section->storageSize = newStorageSize;
        *headOfData = section->contentSize;
        section->contentSize += dataSize;
        section->data = newStorage;
    }

    return err;
}
