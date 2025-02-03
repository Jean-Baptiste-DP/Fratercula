#include "error.h"
#include "section.h"

typedef struct
{
    uint16_t number;
    Section ** allSections;
} SectionHeader;

SectionHeader * sectionHeaderInit();
void * sectionHeaderFree(SectionHeader *);

Error sectionHeaderAddDataToSection(SectionHeader *, SectionType, char *, uint64_t);
