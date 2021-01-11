define newline


endef

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
TEMPS = $(SOURCES:.c=.i) $(SOURCES:.c=.s)
LOGS = $(SOURCES:.c=.log)
endif

TARGET = dintwm

all : $(OBJECTS)
	 $(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)
ifdef strict
	 cpplint $(CPPLINTOPTS) $(SOURCES) $(HEADERS)
	 cppcheck --enable=all $(HEADERS) $(SOURCES)
	 flawfinder -F $(HEADERS) $(SOURCES)
	 $(foreach elem,$(SOURCES),pvs-studio --cfg PVS-Studio.cfg --source-file $(elem) --i-file $(elem:.c=.i) --output-file $(elem:.c=.log)${newline})
	 $(foreach elem,$(SOURCES),plog-converter -t csv $(elem:.c=.log)${newline})
endif


dintwm.o : dintwm.h ketopt.h
commodity.o : dintwm.h config.h
timer.o : dintwm.h

.PHONY : clean
clean :
	-rm dintwm $(OBJECTS) $(TEMPS) $(LOGS)


