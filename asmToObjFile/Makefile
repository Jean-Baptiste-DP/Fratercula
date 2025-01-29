# compilation parameters
c_compiler := gcc
ld_function := ld
ld_flags := -static

# project files
c_files := $(wildcard *.c)
c_target := execGenerator

# compile targets
target_file := executable
target_objects := $(target_file).o

# link generated output
$(target_file): $(target_objects)
	$(ld_function) -o $@ $^ $(ld_flags)

# generate executable object
$(target_objects): $(c_target)
	./$(c_target)

# compile generator
$(c_target): $(c_files)
	gcc -o $@ $^


# run
.PHONY: run
run:
	./$(target_file)

# clean
.PHONY: clean
clean:
	rm $(c_target)
	rm $(target_file)
	rm $(target_objects)