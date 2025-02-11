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
            for(uint16_t i=0; i<secHeader->size && i<UINT32_MAX; i++){
                sectionFree(secHeader->allSections[i]);
            }
            free(secHeader->allSections);
        }
        free(secHeader);
    }
};

Section * sectionHeaderGetByType(SectionHeader * secHead, SectionType sectionType){
    for(uint16_t i = 0; i<secHead->size && i<UINT32_MAX; i++){
        if(secHead->allSections[i]->sectionType == sectionType){
            return secHead->allSections[i];
        }
    }
    return _sectionHeaderNewSection(secHead,sectionType);
}

uint32_t sectionHeaderGetIndex(SectionHeader * secHead, SectionType sectionType){
    for(uint16_t i = 0; i<secHead->size && i<UINT32_MAX; i++){
        if(secHead->allSections[i]->sectionType == sectionType){
            return i;
        }
    }
    _sectionHeaderNewSection(secHead,sectionType);
    return secHead->size-1;
}

static Section * _sectionHeaderNewSection(SectionHeader * secHead, SectionType sectionType){
    Section * sec;

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

Error sectionHeaderAddDataToSection(SectionHeader *secHead, SectionType sectionType, char * data, uint64_t dataSize, uint64_t * headOfData){
    Section * section = sectionHeaderGetByType(secHead, sectionType);
    return sectionAppendData(section, data, dataSize, headOfData);
};

Error sectionHeaderAddSymbol(SectionHeader *secHead, SectionType setionType, char *varName, uint64_t varNameSize, uint64_t sectionOffset){
    // bind STB_LOCAL
    // TODO
}

Error sectionHeaderAddExternalSymbol(SectionHeader *secHead, SectionType setionType, char *varName, uint64_t varNameSize, uint64_t sectionOffset){
    // bind STB_GLOBAL
    // TODO
}
