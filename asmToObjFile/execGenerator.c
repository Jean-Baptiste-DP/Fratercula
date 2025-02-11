#include "elf.h"
#include <stdio.h>
#include <string.h>

// https://godbolt.org/z/vh8aEe
const unsigned char asmCode[] = {
    0xb8, 0x01, 0x00, 0x00, 0x00, // mov rax, 1 (syscall: write)
    0xbf, 0x01, 0x00, 0x00, 0x00, // mov rdi, 1 (stdout)
    0x48, 0xbe, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, // movabs rsi, [rel helloStr]
    0xba, 0x0e, 0x00, 0x00, 0x00, // mov rex, sizeof(helloStr)
    0x0f, 0x05, // syscall
    0xb8, 0x3c, 0x00, 0x00, 0x00, // mov eax, 3c
    0x48, 0x31, 0xff, // xor rdi, rdi
    0x0f, 0x05 // syscall
};
const char helloStr[] = "Hello World!\n\0";


int main(){
    const char sectionNames[] = "\0.text\0.shstrtab\0.rodata\0.symtab\0.strtab\0.rela.text\0";
    const char strtab[] = "\0.text\0_start\0.rodata\0msg\0";
    const Elf64_Half sectionHeaderNumber = 7;
    const Elf64_Half symboleTableNumber = 5;
    const Elf64_Half relatextNumber = 1;

    Elf64_Off headerSize = sizeof(Elf64_Ehdr);
    Elf64_Off sectionHeaderSize = sectionHeaderNumber*sizeof(Elf64_Shdr);
    Elf64_Off textSize = sizeof(asmCode);
    Elf64_Off shstrtabSize = sizeof(sectionNames);
    Elf64_Off rodataSize = sizeof(helloStr);
    Elf64_Off symtabSize = symboleTableNumber*sizeof(Elf64_Sym);
    Elf64_Off strtabSize = sizeof(strtab);
    Elf64_Off relatextSize = relatextNumber*sizeof(Elf64_Rela);

    Elf64_Off sectionHeaderOffset = headerSize;
    Elf64_Half sectionNamesOffset = headerSize+sectionHeaderSize;
    Elf64_Half entryOffset = sectionNamesOffset;

    const Elf64_Ehdr header = {
        .e_ident = {ELFMAG0, 
            ELFMAG1, 
            ELFMAG2, 
            ELFMAG3, 
            ELFCLASS64, 
            ELFDATA2LSB, 
            1, // ELF Version
            ELFOSABI_SYSV, 
            0, // EI ABI Version
            0, 0, 0, 0, 0, 0, 0},
        .e_type = ET_REL,
        .e_machine = EM_X86_64,
        .e_version = 1, // ELF Version
        .e_entry = 0,
        .e_phoff = 0,
        .e_shoff = sectionHeaderOffset,
        .e_flags = 0,
        .e_ehsize = headerSize,
        .e_phentsize = 0,
        .e_phnum = 0,
        .e_shentsize = sizeof(Elf64_Shdr),
        .e_shnum = sectionHeaderNumber,
        .e_shstrndx = 1
    };

    Elf64_Shdr sectionHeader[] = {
        {// NULL
            .sh_name = 0,
            .sh_type = SHT_NULL,
            .sh_flags = 0,
            .sh_addr = 0,
            .sh_offset = 0,
            .sh_size = 0,
            .sh_link = 0,
            .sh_info = 0,
            .sh_addralign = 0,
            .sh_entsize = 0
        },{// .shstrtab
            .sh_name = 7,
            .sh_type = SHT_STRTAB,
            .sh_flags = 0,
            .sh_addr = 0,
            .sh_offset = entryOffset,
            .sh_size = shstrtabSize,
            .sh_link = 0,
            .sh_info = 0,
            .sh_addralign = 1,
            .sh_entsize = 0
        },{// .text
            .sh_name = 1,
            .sh_type = SHT_PROGBITS,
            .sh_flags = SHF_ALLOC | SHF_EXECINSTR,
            .sh_addr = 0,
            .sh_offset = entryOffset + shstrtabSize,
            .sh_size = textSize,
            .sh_link = 0,
            .sh_info = 0,
            .sh_addralign = 16,
            .sh_entsize = 0
        },{// .rodata
            .sh_name = 17,
            .sh_type = SHT_PROGBITS,
            .sh_flags = SHF_ALLOC,
            .sh_addr = 0,
            .sh_offset = entryOffset + shstrtabSize + textSize,
            .sh_size = rodataSize,
            .sh_link = 0,
            .sh_info = 0,
            .sh_addralign = 4,
            .sh_entsize = 0
        },{// .symtab
            .sh_name = 25,
            .sh_type = SHT_SYMTAB,
            .sh_flags = 0,
            .sh_addr = 0,
            .sh_offset = entryOffset + textSize + shstrtabSize + rodataSize,
            .sh_size = symtabSize,
            .sh_link = 5,// Index of strtab
            .sh_info = 4,// Index of the last local symbol
            .sh_addralign = 8,
            .sh_entsize = sizeof(Elf64_Sym)
        },{// .strtab
            .sh_name = 33,
            .sh_type = SHT_STRTAB,
            .sh_flags = 0,
            .sh_addr = 0,
            .sh_offset = entryOffset + textSize + shstrtabSize + rodataSize + symtabSize,
            .sh_size = strtabSize,
            .sh_link = 0,
            .sh_info = 0,
            .sh_addralign = 1,
            .sh_entsize = 0
        },{// .rela.text
            .sh_name = 41,
            .sh_type = SHT_RELA,
            .sh_flags = 0,
            .sh_addr = 0,
            .sh_offset = entryOffset + textSize + shstrtabSize + rodataSize + symtabSize + strtabSize,
            .sh_size = relatextSize,
            .sh_link = 4,
            .sh_info = 2,
            .sh_addralign = 8,
            .sh_entsize = sizeof(Elf64_Rela)
        }
    };

    Elf64_Sym symboleTable[] = {
        {// NULL
            .st_name = 0,
            .st_info = 0,
            .st_other = 0,
            .st_shndx = 0,
            .st_value = 0,
            .st_size = 0
        },{// .text
            .st_name = 1,
            .st_info = ELF64_ST_INFO(STB_LOCAL, STT_SECTION),
            .st_other = ELF64_ST_VISIBILITY(STV_DEFAULT),
            .st_shndx = 2,
            .st_value = 0,
            .st_size = 0
        },{// .rodata
            .st_name = 14,
            .st_info = ELF64_ST_INFO(STB_LOCAL, STT_SECTION),
            .st_other = ELF64_ST_VISIBILITY(STV_DEFAULT),
            .st_shndx = 3,
            .st_value = 0,
            .st_size = 0
        },{// msg
            .st_name = 22,
            .st_info = ELF64_ST_INFO(STB_LOCAL, STT_NOTYPE),
            .st_other = ELF64_ST_VISIBILITY(STV_DEFAULT),
            .st_shndx = 3,
            .st_value = 0,
            .st_size = 0
        },{// _start
            .st_name = 7,
            .st_info = ELF64_ST_INFO(STB_GLOBAL, STT_NOTYPE),
            .st_other = ELF64_ST_VISIBILITY(STV_DEFAULT),
            .st_shndx = 2,
            .st_value = 0,
            .st_size = 0
        }
    };

    Elf64_Rela relatext[] = {
        {// msg
            .r_offset = 0xc,
            .r_info = ELF64_R_INFO(3,R_X86_64_64),
            .r_addend = 0
        }
    };

    FILE* file = fopen("executable.o", "w");

    fwrite(&header, headerSize, 1, file);
    fwrite(&sectionHeader, sectionHeaderSize, 1, file);

    fwrite(sectionNames, shstrtabSize , 1, file);
    fwrite(asmCode, 1, textSize, file);
    fwrite(helloStr, 1, rodataSize, file);
    fwrite(&symboleTable, symtabSize, 1, file);
    fwrite(strtab, strtabSize, 1, file);
    fwrite(&relatext, relatextSize, 1, file);

    fclose(file);
}