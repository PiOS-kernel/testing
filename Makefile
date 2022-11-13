LDFILE=src/linker.ld
SRCFILES=$(subst .c,.o,$(wildcard src/*.c))
DEBFILES=$(subst src,debug,$(SRCFILES))

all:debug/final.elf
	@arm-none-eabi-objdump -D debug/final.elf > debug/final.list
# arm-none-eabi-objcopy -O binary debug/final.elf debug/final.bin
	@echo build success

debug/final.elf:debug/_init.o $(DEBFILES)
	@arm-none-eabi-ld -nostdlib -T $(LDFILE) $^ -Lsrc -lgetchar -o $@

debug/_init.o:src/_init.s
	@arm-none-eabi-as --warn --fatal-warnings -mcpu=cortex-m4 -ggdb $^ -o $@

debug/%.o: src/%.c
	@arm-none-eabi-gcc -Wall -O0 -mcpu=cortex-m4 -mthumb -nostartfiles -ggdb -c $^ -o $@

clean:
	rm -r debug/*.o debug/*.elf debug/*.list
# debug/*.bin

gdb:
	qemu-system-arm -cpu cortex-m4 -machine lm3s6965evb -nographic -gdb tcp::3333 -S -kernel debug/final.elf

run:
	qemu-system-arm -cpu cortex-m4 -machine lm3s6965evb -nographic -kernel debug/final.elf
#qemu-system-arm -cpu cortex-m4 -machine lm3s6965evb -nographic -semihosting-config enable=on,target=native -gdb tcp::3333 -kernel main