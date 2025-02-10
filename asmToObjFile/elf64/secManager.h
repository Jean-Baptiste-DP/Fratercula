#include "error.h"
#include "section.h"

typedef struct
{
    uint32_t size;
    Section ** allSections;
} SectionHeader;

SectionHeader * sectionHeaderInit();
void sectionHeaderFree(SectionHeader *);

Section * sectionHeaderGetByType(SectionHeader *, SectionType);
uint32_t sectionHeaderGetIndex(SectionHeader *, SectionType);
Error sectionHeaderAddDataToSection(SectionHeader *, SectionType, char *, uint64_t, uint64_t *);

