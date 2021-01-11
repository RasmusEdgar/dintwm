OBJECTS =dintwm.o commodity.o timer.o
CFLAGSSTRICT =-pedantic -std=c11 -ggdb3 -O0 -Wall -Wextra -Wformat=2 -Wmissing-include-dirs -Winit-self -Wswitch-default -Wswitch-enum -Wunused-parameter -Wfloat-equal -Wundef -Wshadow -Wlarger-than-1000 -Wunsafe-loop-optimizations -Wbad-function-cast -Wcast-align -Wconversion -Wlogical-op -Waggregate-return -Wstrict-prototypes -Wold-style-definition -Wmissing-prototypes -Wmissing-declarations -Wpacked -Wpadded -Wredundant-decls -Wnested-externs -Wunreachable-code -Winline -Winvalid-pch -Wvolatile-register-var -Wstrict-aliasing=2 -Wstrict-overflow=2 -Wtraditional-conversion -Wwrite-strings -noixemul -save-temps
CC = m68k-amigaos-gcc
CFLAGS =-std=c11 -O2 -noixemul -Wall -s
SOURCES = dintwm.c commodity.c timer.c
HEADERS = dintwm.h config.h 
EXTHEADERS = ketopt.h
CPPLINTOPTS = --linelength 120 --filter=-whitespace/tab,-whitespace/comments,-whitespace/braces,-whitespace/indent,-readability/casting,-runtime/int,-build/header_guard

ifdef strict
CFLAGS = $(CFLAGSSTRICT)
#TEMPS = dintwm.i dintwm.s commodity.i commodity.s timer.i timer.s
TEMPS = $(SOURCES:.c=.i) $(SOURCES:.c=.s)
#LOGS = pvs-dintwm.log pvs-com.log pvs-timer.log
LOGS = $(SOURCES:.c=.log)
RUNCHECK = true
endif

TARGET = dintwm

all : $(OBJECTS)
	 $(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)
ifdef RUNCHECK
	 cpplint $(CPPLINTOPTS) $(SOURCES) $(HEADERS)
	 cppcheck --enable=all $(HEADERS) $(SOURCES)
	 flawfinder -F $(HEADERS) $(SOURCES)
	 pvs-studio --cfg PVS-Studio.cfg --source-file dintwm.c --i-file dintwm.i --output-file dintwm.log
	 pvs-studio --cfg PVS-Studio.cfg --source-file commodity.c --i-file commodity.i --output-file commodity.log
	 pvs-studio --cfg PVS-Studio.cfg --source-file timer.c --i-file timer.i --output-file timer.log
	 plog-converter -t csv dintwm.log
	 plog-converter -t csv commodity.log
	 plog-converter -t csv timer.log
endif


dintwm.o : dintwm.h ketopt.h
commodity.o : dintwm.h config.h
timer.o : dintwm.h

.PHONY : clean
clean :
	-rm dintwm $(OBJECTS) $(TEMPS) $(LOGS)


