#ifndef SECTIONHEADER_H
#define SECTIONHEADER_H

#include "elf.h"
#include "error.h"

typedef enum
{
    SECTION_NULL,
    SECTION_TEXT,
    SECTION_SHSTRTAB,
    SECTION_RODATA,
    SECTION_SYMTAB,
    SECTION_STRTAB,
    SECTION_RELA,
    SECTION_DATA,
    SECTION_BSS,
    SECTION_DYNAMIC,
    SECTION_DYNSTR,
    SECTION_DYNSYM,
} SectionType;

typedef struct{
    SectionType sectionType;
    Elf64_Shdr header;
    uint64_t storageSize;
    uint64_t contentSize;
    char * data;
}Section;

Section * sectionNew(SectionType);
void sectionFree(Section *);
Error sectionAppendData(Section *, char *, uint64_t);

#endif //SECTIONHEADER_H