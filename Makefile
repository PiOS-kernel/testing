LDFILE=src/linker.ld
# Files in ./src directory
SRCFILES_C=$(subst .c,.o,$(wildcard src/*.c))
SRCFILES_S=$(subst .s,.o,$(wildcard src/*.s))
# Files in ./testing directory
TESTFILES_C=$(subst .c,.o,$(wildcard testing/*.c))
# Kernel files
KERNELFILES_C=$(subst .c,.o,$(wildcard pios-kernel/kernel/*/*.c pios-kernel/kernel/*.c))
KERNELFILES_S=$(subst .s,.o,$(wildcard pios-kernel/kernel/port/gcc/*.s))

# Build files
BUILDSRCFILES_C=$(subst src,build,$(SRCFILES_C))
BUILDSRCFILES_S=$(subst src,build,$(SRCFILES_S))
BUILDTESTFILES_C=$(subst testing,build,$(TESTFILES_C))
BUILDKERNELFILES_C=$(subst pios-kernel/kernel,build,$(KERNELFILES_C))
BUILDKERNELFILES_S=$(subst pios-kernel/kernel/port/gcc,build,$(KERNELFILES_S))
BUILDFILES_C=$(BUILDSRCFILES_C) $(BUILDTESTFILES_C) $(BUILDKERNELFILES_C)
BUILDFILES_S=$(BUILDSRCFILES_S) $(BUILDKERNELFILES_S)

all:build/final.elf
	@arm-none-eabi-objdump -D build/final.elf > build/final.list
## arm-none-eabi-objcopy -O binary build/final.elf build/final.bin
	@echo build succeded

# Rule for linking the final executable

build/final.elf: $(BUILDFILES_C) $(BUILDFILES_S)
	@arm-none-eabi-ld -nostdlib -T $(LDFILE) $(addprefix build/,$(notdir $^)) $(PIOS_PATH) -o $@

# Rules for assembling S files

build/%.o: src/%.s
	@[ -d build ] || mkdir build
	@arm-none-eabi-as --warn --fatal-warnings -mcpu=cortex-m4 -ggdb $^ -o $@

build/%.o: pios-kernel/kernel/port/gcc/%.s
	@[ -d build ] || mkdir build
	@arm-none-eabi-as --warn --fatal-warnings -mcpu=cortex-m4 -ggdb $^ -o $@

# Rules for compiling C files

build/%.o: src/%.c
	@[ -d build ] || mkdir build
	@arm-none-eabi-gcc -Wall -O0 -mcpu=cortex-m4 -mthumb --freestanding -nostartfiles -ggdb -c $^ -o $@

build/%.o: pios-kernel/kernel/%.c
	@[ -d build ] || mkdir build
	@arm-none-eabi-gcc -Wall -O0 -mcpu=cortex-m4 -mthumb --freestanding -nostartfiles -ggdb -c $^ -o build/$(notdir $@)

build/%.o: testing/%.c
	@[ -d build ] || mkdir build
	@arm-none-eabi-gcc -Wall -O0 -mcpu=cortex-m4 -mthumb --freestanding -nostartfiles -ggdb -c $^ -o $@

# Rule for cleaning the build directory

clean:
	@rm -r build/*.o build/*.elf build/*.list
## build/*.bin

# Rule for running the executable in qemu with gdb

gdb:
	@(make clean && make) || make
	@qemu-system-arm -cpu cortex-m4 -machine lm3s6965evb -nographic -gdb tcp::3333 -S -kernel build/final.elf

# Rule for running the executable in qemu without gdb

test:
	@(make clean && make) || make
	@qemu-system-arm -cpu cortex-m4 -machine lm3s6965evb -nographic -kernel build/final.elf
##qemu-system-arm -cpu cortex-m4 -machine lm3s6965evb -nographic -semihosting-config enable=on,target=native -gdb tcp::3333 -kernel main