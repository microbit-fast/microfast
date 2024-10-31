CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy

SRCS = hardware.s main.cpp

all: clean main.hex flash

noflash: clean main.hex

flash:
	openocd -f interface/cmsis-dap.cfg -f target/nrf52.cfg -c "program main.hex reset exit" 2> /dev/null

clean:
	rm -f *.o *.elf *.hex *.map

# main.elf: $(SRCS)
# 	$(CC)                                              \
# 	-L/usr/arm-none-eabi/lib/arm/v5te/hard             \
# 	-L/usr/lib/gcc/arm-none-eabi/14.1.0/arm/v5te/hard  \
# 	-Wall -Wextra -Wno-deprecated-copy                 \
# 	-ffreestanding                                     \
# 	-ffunction-sections -fdata-sections                \
# 	-mfloat-abi=hard -mfpu=fpv4-sp-d16                 \
# 	-Os                                                \
# 	-nostartfiles -nostdlib                            \
# 	-mcpu=cortex-m4 -mthumb                            \
# 	$^                                                 \
# 	-o main.elf                                        \
# 	-T linker.ld                                       \
# 	-lstdc++ -lm -lc -lgcc -lg -lnosys                 \
# 	--specs=nano.specs --specs=nosys.specs             \
# 	-Wl,--gc-sections -Wl,-Map=output.map

main.elf: $(SRCS)
	$(CC)                                                    \
	-L/usr/arm-none-eabi/lib/thumb/v7e-m+dp/hard             \
	-L/usr/lib/gcc/arm-none-eabi/14.1.0/thumb/v7e-m+dp/hard  \
	-Wall -Wextra -Wno-deprecated-copy                       \
	-ffreestanding                                           \
	-ffunction-sections -fdata-sections                      \
	-mfloat-abi=hard -mfpu=fpv5-d16                          \
	-Os                                                      \
	-nostartfiles -nostdlib                                  \
	-mcpu=cortex-m4 -mthumb                                  \
	$^                                                       \
	-I. -Ilibs                                               \
	-o main.elf                                              \
	-T linker.ld                                             \
	-lstdc++ -lm -lc -lgcc -lg -lnosys                       \
	--specs=nano.specs --specs=nosys.specs                   \
	-Wl,--gc-sections -Wl,-Map=output.map

main.hex: main.elf
	$(OBJCOPY) main.elf main.hex -O ihex
