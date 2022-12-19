LDFILE=src/linker.ld
# Files in ./src directory
SRCFILES_C=$(subst .c,.o,$(wildcard src/*.c))
SRCFILES_S=$(subst .s,.o,$(wildcard src/*.s))
# Files in ./src/tests directory
TESTFILES_C=$(subst .c,.o,$(wildcard testing/*.c))
TESTFILES_S=$(subst .s,.o,$(wildcard testing/*.s))
# Kernel files
KERNELFILES_C=$(subst .c,.o,$(wildcard pios-kernel/kernel/*.c))
KERNELFILES_S=$(subst .s,.o,$(wildcard pios-kernel/kernel/*.s))
HEAPFILES_C=$(subst .c,.o,$(wildcard pios-kernel/kernel/heap/*.c))
HEAPFILES_S=$(subst .s,.o,$(wildcard pios-kernel/kernel/heap/*.s))
# SYSCALLSFILES_C=$(subst .c,.o,$(wildcard pios-kernel/kernel/syscalls/*.c))
# SYSCALLSFILES_S=$(subst .s,.o,$(wildcard pios-kernel/kernel/syscalls/*.s))
# TASKFILES_C=$(subst .c,.o,$(wildcard pios-kernel/kernel/task/*.c))
# TASKFILES_S=$(subst .s,.o,$(wildcard pios-kernel/kernel/task/*.s))

BUILDSRCFILES_C=$(subst src,build,$(SRCFILES_C))
BUILDSRCFILES_S=$(subst src,build,$(SRCFILES_S))
BUILDTESTFILES_C=$(subst testing,build,$(TESTFILES_C))
BUILDTESTFILES_S=$(subst testing,build,$(TESTFILES_S))
BUILDKERNELFILES_C=$(subst pios-kernel/kernel,build,$(KERNELFILES_C))
BUILDKERNELFILES_S=$(subst pios-kernel/kernel,build,$(KERNELFILES_S))
BUILDHEAPFILES_C=$(subst pios-kernel/kernel/heap,build,$(HEAPFILES_C))
BUILDHEAPFILES_S=$(subst pios-kernel/kernel/heap,build,$(HEAPFILES_S))
BUILDFILES_C=$(BUILDSRCFILES_C) $(BUILDTESTFILES_C) $(BUILDKERNELFILES_C) $(BUILDHEAPFILES_C)
BUILDFILES_S=$(BUILDSRCFILES_S) $(BUILDTESTFILES_S) $(BUILDKERNELFILES_S) $(BUILDHEAPFILES_S)

all:build/final.elf
	@arm-none-eabi-objdump -D build/final.elf > build/final.list
## arm-none-eabi-objcopy -O binary build/final.elf build/final.bin
	@echo build succeded

# Rule for linking the final executable

build/final.elf: $(BUILDFILES_C) $(BUILDFILES_S)
	@echo linking...
	@[ -d build ] || mkdir build
	@arm-none-eabi-ld -nostdlib -T $(LDFILE) $^ $(PIOS_PATH) -o $@

# Rules for assembling S files

build/%.o: src/%.s
	@echo assembling $^
	@[ -d build ] || mkdir build
	@arm-none-eabi-as --warn --fatal-warnings -mcpu=cortex-m4 -ggdb $^ -o $@

build/%.o: pios-kernel/kernel/heap/%.s
	@echo assembling $^
	@[ -d build ] || mkdir build
	@arm-none-eabi-as --warn --fatal-warnings -mcpu=cortex-m4 -ggdb $^ -o $@

build/%.o: pios-kernel/kernel/%.s
	@echo assembling $^
	@[ -d build ] || mkdir build
	@arm-none-eabi-as --warn --fatal-warnings -mcpu=cortex-m4 -ggdb $^ -o $@

build/%.o: testing/%.s
	@echo assembling $^
	@[ -d build ] || mkdir build
	@arm-none-eabi-as --warn --fatal-warnings -mcpu=cortex-m4 -ggdb $^ -o $@

# Rules for compiling C files

build/%.o: src/%.c
	@echo compiling $^
	@[ -d build ] || mkdir build
	@arm-none-eabi-gcc -Wall -O0 -mcpu=cortex-m4 -mthumb -nostartfiles -ggdb -c $^ -o $@

build/%.o: pios-kernel/kernel/heap/%.c
	@echo compiling $^
	@[ -d build ] || mkdir build
	@arm-none-eabi-gcc -Wall -O0 -mcpu=cortex-m4 -mthumb -nostartfiles -ggdb -c $^ -o $@

build/%.o: pios-kernel/kernel/%.c
	@echo compiling $^
	@[ -d build ] || mkdir build
	@arm-none-eabi-gcc -Wall -O0 -mcpu=cortex-m4 -mthumb -nostartfiles -ggdb -c $^ -o $@

build/%.o: testing/%.c
	@echo compiling $^
	@[ -d build ] || mkdir build
	@arm-none-eabi-gcc -Wall -O0 -mcpu=cortex-m4 -mthumb -nostartfiles -ggdb -c $^ -o $@

# Rule for cleaning the build directory

clean:
	rm -r build/*.o build/*.elf build/*.list
## build/*.bin

# Rule for running the executable in qemu with gdb

gdb:
	qemu-system-arm -cpu cortex-m4 -machine lm3s6965evb -nographic -gdb tcp::3333 -S -kernel build/final.elf

# Rule for running the executable in qemu without gdb

test:
	qemu-system-arm -cpu cortex-m4 -machine lm3s6965evb -nographic -kernel build/final.elf
##qemu-system-arm -cpu cortex-m4 -machine lm3s6965evb -nographic -semihosting-config enable=on,target=native -gdb tcp::3333 -kernel main