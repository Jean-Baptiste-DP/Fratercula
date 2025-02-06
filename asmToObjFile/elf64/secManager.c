#include <stdlib.h>
#include <string.h>

#include "secManager.h"

// Local definition
static Section * _sectionHeaderNewSection(SectionHeader *, SectionType);

SectionHeader * sectionHeaderInit(){
    SectionHeader * secHead = malloc(sizeof(SectionHeader));
    secHead->size = 0;
    secHead->allSections = NULL;
    return secHead;
};

void sectionHeaderFree(SectionHeader * secHeader){
    if(secHeader!=NULL){
        if(secHeader->allSections!=NULL){
            for(uint16_t i=0; i<secHeader->size && i<UINT16_MAX; i++){
                sectionFree(secHeader->allSections[i]);
            }
            free(secHeader->allSections);
        }
        free(secHeader);
    }
};

Section * sectionHeaderGetByType(SectionHeader * secHead, SectionType sectionType){
    // TODO how to deal with multiple SECTION_RELA sections
    for(uint16_t i = 0; i<secHead->size && i<UINT16_MAX; i++){
        if(secHead->allSections[i]->sectionType == sectionType){
            return secHead->allSections[i];
        }
    }
    return _sectionHeaderNewSection(secHead,sectionType);
}

static Section * _sectionHeaderNewSection(SectionHeader * secHead, SectionType sectionType){
    Section * sec;

    if((sec = _sectionHeaderGetByType(secHead, sectionType))!=NULL){
        return sec;
    }

    Section ** newSectionArray = malloc((secHead->size + 1) * sizeof(Section *));

    if(secHead->allSections!=NULL){
        memcpy(newSectionArray, secHead->allSections, secHead->size * sizeof(Section *));
        free(secHead->allSections);
    }

    Section * sec = sectionNew(sectionType, secHead);
    newSectionArray[secHead->size] = sec;

    secHead->allSections = newSectionArray;
    secHead->size += 1;

    return sec;
}

Error sectionHeaderAddDataToSection(SectionHeader *secHead, SectionType sectionType, char * data, uint64_t dataSize){

};

