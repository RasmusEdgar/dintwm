define newline


endef

SOURCES = dintwm.c commodity.c
OBJECTS = $(SOURCES:.c=.o)
CFLAGSSTRICT =-pedantic -std=c11 -ggdb3 -O0 -Wall -Wextra -Wformat=2 -Wmissing-include-dirs -Winit-self -Wswitch-default -Wswitch-enum -Wunused-parameter -Wfloat-equal -Wundef -Wshadow -Wlarger-than-1000 -Wunsafe-loop-optimizations -Wbad-function-cast -Wcast-align -Wconversion -Wlogical-op -Waggregate-return -Wstrict-prototypes -Wold-style-definition -Wmissing-prototypes -Wmissing-declarations -Wpacked -Wpadded -Wredundant-decls -Wnested-externs -Wunreachable-code -Winline -Winvalid-pch -Wvolatile-register-var -Wstrict-aliasing=2 -Wstrict-overflow=2 -Wtraditional-conversion -Wwrite-strings -noixemul -save-temps
CC = m68k-amigaos-gcc
CFLAGS =-std=c11 -Ofast -noixemul -Wall -Wextra -s -fomit-frame-pointer -fbaserel
MAINHEADER = dintwm.h
CONFHEADER = cxm_config.h wbar_config.h
EXTHEADERS = ketopt.h
HEADERS = $(MAINHEADER) $(CONFHEADER) $(EXTHEADERS) 
CPPLINTCMD = cpplint
CPPLINTOPTS = --linelength 120 --filter=-whitespace/tab,-whitespace/comments,-whitespace/braces,-whitespace/indent,-readability/casting,-runtime/int,-build/header_guard
CPPCHECKCMD = cppcheck
CPPCHECKOPTS = --enable=all
FLAWCMD = flawfinder
FLAWOPTS = -F
SPLINTCMD = splint
SPLINTARGS = -I $(HOME)/opt/amiga/m68k-amigaos/ndk-include/
TARGET = dintwm

ifdef strict
CFLAGS = $(CFLAGSSTRICT)
TEMPS = $(SOURCES:.c=.i) $(SOURCES:.c=.s)
LOGS = $(SOURCES:.c=.log)
PEXCL = --exclude-path */opt/amiga/* --exclude-path ./ketopt.h
ifdef misra
MPLOGFLAG = -d V2511,V2516,V2510 -a MISRA:1,2
endif
endif

all : $(OBJECTS)
	 $(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)
ifdef strict
	 $(CPPLINTCMD) $(CPPLINTOPTS) $(SOURCES) $(filter-out $(EXTHEADERS),$(HEADERS))
	 $(CPPCHECKCMD) $(CPPCHECKOPTS) $(HEADERS) $(SOURCES)
	 $(FLAWCMD) $(FLAWOPTS) $(HEADERS) $(SOURCES)
	 $(SPLINTCMD) $(SOURCES) $(SPLINTARGS)
	 $(foreach elem,$(SOURCES),pvs-studio --cfg PVS-Studio.cfg $(PEXCL) --source-file $(elem) --i-file $(elem:.c=.i) --output-file $(elem:.c=.log)${newline})
	 $(foreach elem,$(SOURCES),plog-converter $(MPLOGFLAG) -t csv $(elem:.c=.log) | grep 'Filtered' ${newline})
ifdef misra
	 $(foreach elem,$(SOURCES),plog-converter -t csv $(elem:.c=.log) | grep 'Filtered' ${newline})
endif
endif


dintwm.o : $(MAINHEADER) $(EXTHEADERS)
commodity.o : $(MAINHEADER) $(CONFHEADER)

.PHONY : clean
clean :
	-rm dintwm $(OBJECTS) $(TEMPS) $(LOGS)


