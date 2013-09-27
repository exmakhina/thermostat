# AVR Makefile

# what do you want to call your binary
PROG=thermostat

# please uncomment the CPU that is used on your arduino
CPU=atmega328p
#CPU=atmega168

# please uncomment the speed that corresponds to your arduino (normally 16MHz)
F_CPU=16000000
#F_CPU=8000000

#############################################################################
# no additional config needed below (except perpapse the 'make install')
#############################################################################

CFLAGS= -g -Os -Wall -Wstrict-prototypes -Wa,-ahlms=$(PROG).lst -mmcu=$(CPU) -DF_CPU=$(F_CPU)
CFLAGS += -gdwarf-2 -std=gnu99 -Wextra -pedantic -ffunction-sections -fdata-sections -fno-inline-small-functions -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF

LFLAGS= -Wl,-Map=$(PROG).map,--cref -mmcu=$(CPU)
# use LFLAGS below if you need to printf floating point numbers
#LFLAGS= -Wl,-u,vfprintf,-Map=$(PROG).map,--cref -mmcu=$(CPU) -lprintf_min -lm
INCL = -I. -I../../lib/libarduino
LIBPATH = -L /home/tgrimault/workspace/arduino/lib/libarduino
LIB = -lm -larduino
SRC = main.c serial_if.c
OBJ = $(SRC:.c=.o)

# default target when "make" is run w/o arguments
all: $(PROG).rom

# compile .c into .o
%.o: %.c
	avr-gcc -c $(CFLAGS) $(INCL) $*.c
	
# link up *.o into relays.elf
$(PROG).elf: $(OBJ)
	avr-gcc $(OBJ) $(LIBPATH) $(LIB) $(LFLAGS) -o $(PROG).elf

# copy ROM (FLASH) object out of relays.elf into relays.rom
$(PROG).rom: $(PROG).elf
	avr-objcopy -O srec $(PROG).elf $(PROG).rom
	./checksize $(PROG).elf

# command to program chip (optional) (invoked by running "make install")
install:
	# SparkFun pocket ISP programmer
	# avrdude -p atmega168 -c usbtiny -U flash:w:$(PROG).rom 
	# atmega328 based Arduino Uno
	avrdude -F -v -p m328p -c stk500v1 -P /dev/serial/by-id/usb-Arduino__www.arduino.cc__0043_741333535373519132D0-if00 -U flash:w:$(PROG).rom

# command to clean up junk (no source files) (invoked by "make clean")
clean:
	rm -f *.o *.rom *.elf *.map *~ *.lst

