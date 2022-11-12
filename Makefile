LDFILE=src/linker.ld
FILES=$(subst .c,.o,$(wildcard src/*.c))

all:debug/final.elf
	@arm-none-eabi-objdump -D debug/final.elf > debug/final.list
	@echo build success
	
# arm-none-eabi-objdump -D notmain.elf > notmain.list
# arm-none-eabi-objcopy -O binary notmain.elf notmain.bin


debug/final.elf:debug/flash.o debug/main.o debug/startup.o debug/systick.o debug/tools.o
	@arm-none-eabi-ld -nostdlib -T $(LDFILE) $^ -Lsrc -lgetchar -o $@

debug/flash.o:src/flash.s
	@arm-none-eabi-as --warn --fatal-warnings -mcpu=cortex-m4 -ggdb $^ -o $@

debug/main.o:src/main.c
	@arm-none-eabi-gcc -Wall -O0 -mcpu=cortex-m4 -mthumb -nostartfiles -ggdb -c $^ -o $@

debug/startup.o:src/startup.c
	@arm-none-eabi-gcc -Wall -O0 -mcpu=cortex-m4 -mthumb -nostartfiles -ggdb -c $^ -o $@

debug/tools.o:src/tools.c
	@arm-none-eabi-gcc -Wall -O0 -mcpu=cortex-m4 -mthumb -nostartfiles -ggdb -c $^ -o $@

debug/systick.o:src/systick.c
	@arm-none-eabi-gcc -Wall -O0 -mcpu=cortex-m4 -mthumb -nostartfiles -ggdb -c $^ -o $@

clean:
	rm -r debug/*.o debug/*.elf debug/*.list
# debug/*.list debug/*.bin

gdb:
	qemu-system-arm -cpu cortex-m4 -machine lm3s6965evb -nographic -gdb tcp::3333 -S -kernel debug/final.elf

run:
	qemu-system-arm -cpu cortex-m4 -machine lm3s6965evb -nographic -kernel debug/final.elf
#qemu-system-arm -cpu cortex-m4 -machine lm3s6965evb -nographic -semihosting-config enable=on,target=native -gdb tcp::3333 -kernel main