OBJECTS =dintwm.o commodity.o timer.o
CFLAGSSTRICT =-pedantic -std=c99 -ggdb3 -O0 -Wall -Wextra -Wformat=2 -Wmissing-include-dirs -Winit-self -Wswitch-default -Wswitch-enum -Wunused-parameter -Wfloat-equal -Wundef -Wshadow -Wlarger-than-1000 -Wunsafe-loop-optimizations -Wbad-function-cast -Wcast-align -Wconversion -Wlogical-op -Waggregate-return -Wstrict-prototypes -Wold-style-definition -Wmissing-prototypes -Wmissing-declarations -Wpacked -Wpadded -Wredundant-decls -Wnested-externs -Wunreachable-code -Winline -Winvalid-pch -Wvolatile-register-var -Wstrict-aliasing=2 -Wstrict-overflow=2 -Wtraditional-conversion -Wwrite-strings -noixemul -save-temps
CC = m68k-amigaos-gcc
CFLAGS =-std=c99 -O2 -noixemul

ifdef strict
CFLAGS = $(CFLAGSSTRICT)
TEMPS = dintwm.i dintwm.s commodity.i commodity.s timer.i timer.s
RUNCHECK = true
endif

TARGET = dintwm

all : $(OBJECTS)
	 $(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

dintwm.o : dintwm.h ketopt.h
commodity.o : dintwm.h config.h
timer.o : dintwm.h

.PHONY : clean
clean :
	-rm dintwm $(OBJECTS) $(TEMPS)
