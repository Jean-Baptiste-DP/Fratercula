# avoid confusion with *.c.o and *.asm.o files
.SUFFIXES: .c .asm .c.o .asm.o .c.dump .asm.dump

# compilation parameters
asm_compiler := nasm
asm_flags := -f elf64
c_compiler := gcc
ld_function := ld
dynamic_interpreter := $(shell ldconfig -p | grep ld-linux | grep x86-64 | awk '{print $$4}' | head -n 1)
ld_flags := -lc --dynamic-linker=$(dynamic_interpreter)
# ld_flags := -static

# project folders
working_directory := $(shell pwd)
src_folder := $(working_directory)/src
obj_folder := $(working_directory)/obj
target_folder := $(working_directory)/bin
dump_folder := $(working_directory)/dump

# compile target name
target_name := executable
target_file := $(target_folder)/$(target_name)

# project files
asm_files := $(wildcard $(src_folder)/*.asm)
c_files := $(wildcard $(src_folder)/*.c)
obj_files := $(asm_files:$(src_folder)/%.asm=$(obj_folder)/%.asm.o) $(c_files:$(src_folder)/%.c=$(obj_folder)/%.c.o)
dump_files := $(asm_files:$(src_folder)/%.asm=$(dump_folder)/%.asm.dump) $(c_files:$(src_folder)/%.c=$(dump_folder)/%.c.dump) $(target_file:$(target_folder)/%=$(dump_folder)/%.dump)

# compile and dump
.PHONY: compile
compile: $(target_file) dump

# link object to binary
$(target_file): $(obj_files)
	mkdir -p $(target_folder)
	$(ld_function) -o $@ $^ $(ld_flags)

# compile assembly
$(obj_folder)/%.asm.o: $(src_folder)/%.asm
	mkdir -p $(obj_folder)
	$(asm_compiler) $(asm_flags) -o $@ $<

# compile c
$(obj_folder)/%.c.o: $(src_folder)/%.c
	mkdir -p $(obj_folder)
	$(c_compiler) -o $@ -c $<

# dump binaries
$(dump_folder)/%.asm.dump: $(obj_folder)/%.asm.o
	mkdir -p $(dump_folder)
	readelf -r $< > $@
	objdump -t -d -s $< >> $@
$(dump_folder)/%.c.dump: $(obj_folder)/%.c.o
	mkdir -p $(dump_folder)
	readelf -r $< > $@
	objdump -t -d -s $< >> $@
$(dump_folder)/%.dump: $(target_file)
	mkdir -p $(dump_folder)
	objdump -t -d -s $< > $@

.PHONY: dump
dump: $(dump_files)

# clean
.PHONY: clean
clean:
	rm -rf $(obj_folder)
	rm -rf $(dump_folder)

# execute binary
.PHONY: run
run: $(target_file)
	$(target_file)
