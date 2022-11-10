LDFILE=src/flash.ld

all:debug/main.elf
	@echo build success
	
# arm-none-eabi-objdump -D notmain.elf > notmain.list
# arm-none-eabi-objcopy -O binary notmain.elf notmain.bin


debug/main.elf:debug/flash.o debug/main.o
	@arm-none-eabi-ld -nostdlib -T $(LDFILE) $^ -Lsrc -lgetchar -o $@

debug/flash.o:src/flash.s
	@arm-none-eabi-as --warn --fatal-warnings -mcpu=cortex-m4 -ggdb $^ -o $@

debug/main.o:src/main.c
	@arm-none-eabi-gcc -Wall -O2 -mcpu=cortex-m4 -mthumb -nostartfiles -ggdb -c $^ -o $@

clean:
	rm -r debug/*.o debug/*.elf
# debug/*.list debug/*.bin

load:
	qemu-system-arm -cpu cortex-m4 -machine lm3s6965evb -nographic -gdb tcp::3333 -S -kernel debug/main.elf
#qemu-system-arm -cpu cortex-m4 -machine lm3s6965evb -nographic -semihosting-config enable=on,target=native -gdb tcp::3333 -kernel main