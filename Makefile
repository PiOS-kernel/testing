LDFILE=src/linker.ld
PIOS_KERNEL=getchar
PIOS_PATH=-Lsrc -l$(PIOS_KERNEL)
SRCFILES_C=$(subst .c,.o,$(wildcard src/*.c))
DEBFILES_C=$(subst src,debug,$(SRCFILES_C))
SRCFILES_S=$(subst .s,.o,$(wildcard src/*.s))
DEBFILES_S=$(subst src,debug,$(SRCFILES_S))

all:debug/final.elf
	@arm-none-eabi-objdump -D debug/final.elf > debug/final.list
# arm-none-eabi-objcopy -O binary debug/final.elf debug/final.bin
	@echo build success

debug/final.elf: $(DEBFILES_C) $(DEBFILES_S)
	@arm-none-eabi-ld -nostdlib -T $(LDFILE) $^ $(PIOS_PATH) -o $@

debug/%.o: src/%.s
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