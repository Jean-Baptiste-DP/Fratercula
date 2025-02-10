#ifndef SECTIONHEADER_H
#define SECTIONHEADER_H

#include "elf.h"
#include "error.h"

/* Legal values for sectionType.  */
typedef uint8_t SectionType;

#define SECTION_NULL        (0)
#define SECTION_TEXT        (1)	
#define SECTION_SHSTRTAB    (2)	
#define SECTION_RODATA      (3)	
#define SECTION_SYMTAB      (4)	
#define SECTION_STRTAB      (5)	
#define SECTION_DATA        (6)	
#define SECTION_BSS         (7)
#define SECTION_DYNAMIC     (8)	
#define SECTION_DYNSTR      (9)	
#define SECTION_DYNSYM      (10)
// It's possible to combine SECTION_RELA with other one (like SECTION_TEXT => SECTION_TEXT + SECTION_RELA : .rela.text)
#define SECTION_RELA        (1U << 6)
#define SECTION_REL         (2U << 6)

typedef struct{
    SectionType sectionType;
    Elf64_Shdr header;
    uint64_t storageSize;
    uint64_t contentSize;
    char * data;
}Section;

Section * sectionNew(SectionType, SectionHeader *);
void sectionFree(Section *);
Error sectionAppendData(Section *, char *, uint64_t);

#endif //SECTIONHEADER_H