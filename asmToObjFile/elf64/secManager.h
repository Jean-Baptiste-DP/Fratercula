#include "error.h"
#include "section.h"

typedef struct
{
    uint16_t size;
    Section ** allSections;
} SectionHeader;

SectionHeader * sectionHeaderInit();
void sectionHeaderFree(SectionHeader *);

Section * sectionHeaderGetByType(SectionHeader *, SectionType);
Error sectionHeaderAddDataToSection(SectionHeader *, SectionType, char *, uint64_t, uint64_t *);

