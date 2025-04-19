define newline


endef

ifdef fortify
FTFYSRC = fortify.c
FTFYHDRS = fortify.h ufortify.h
FTFYFLAG ="-DFORTIFY "
FTFYLINTEXCL = --exclude=fortify.*
endif

SOURCES = src/supplemental.c src/bar.c src/options.c src/tiling.c src/tiling_support.c src/windows.c src/dintwm.c src/commodity.c $(FTFYSRC)
OBJECTS = $(SOURCES:.c=.o)
CFLAGSSTRICT =-pedantic -std=c11 -ggdb3 $(FTFYFLAG)-O2 -Wall -Wextra -Wformat=2 -Wmissing-include-dirs -Winit-self -Wswitch-default -Wswitch-enum -Wunused-parameter -Wfloat-equal -Wundef -Wshadow -Wlarger-than-1000 -Wunsafe-loop-optimizations -Wbad-function-cast -Wcast-align -Wconversion -Wlogical-op -Waggregate-return -Wstrict-prototypes -Wold-style-definition -Wmissing-prototypes -Wmissing-declarations -Wpacked -Wpadded -Wredundant-decls -Wnested-externs -Wunreachable-code -Winline -Winvalid-pch -Wvolatile-register-var -Wstrict-aliasing=2 -Wstrict-overflow=2 -Wtraditional-conversion -Wwrite-strings -noixemul -save-temps=obj
CC = m68k-amigaos-gcc
CFLAGS =-std=c11 -Ofast -Wall -Wextra -s -fomit-frame-pointer -noixemul
MAINHEADER = include/tiling_support.h include/windows.h include/dintwm.h include/dintwm_shared.h
CONFHEADER = include/shared_defs.h include/commodity_types.h include/rawkey_combos.h include/commodity.h include/key_txts.h include/key_defs.h include/opt_defs.h
EXTHEADERS = include/wbar_config.h $(FTFYHDRS)
HEADERS = $(MAINHEADER) $(CONFHEADER) $(EXTHEADERS) 
CPPLINTCMD = cpplint
CPPLINTOPTS = --linelength 150 --filter=-build/include_what_you_use,-whitespace/tab,-whitespace/comments,-whitespace/braces,-whitespace/indent,-readability/casting,-runtime/int,-build/header_guard $(FTFYLINTEXCL)
CPPCHECKCMD = cppcheck
CPPCHECKOPTS = --check-level=exhaustive --enable=warning,style,performance,portability,unusedFunction -DFORTIFY
FLAWCMD = flawfinder
FLAWOPTS = -F 
SPLINTCMD = splint
SPLINTARGS = -I $(HOME)/opt/amiga/m68k-amigaos/ndk-include/
PANDOC = awk -v RS='\\[/*.:.*pancut.*panend)' -v ORS= '1;NR==1{printf "Check demos on: https://github.com/RasmusEdgar/dintwm"}' README.md | pandoc -f markdown-smart -t plain --wrap=none | sed 's/~~/|DONE|/g' > readme.txt
TARGET = dintwm
GITVER = $(shell git describe --tags)
VERCMD = sed -i "s/VERGIT/${GITVER}/g" include/dintwm.h
CLRVER = $(shell gawk -i inplace -v RS='//VERCUT.*VERCUT' -v ORS= '1;NR==1{printf "//VERCUT\n#define DINTWM_VERSION \"VERGIT\"\n//VERCUT"}' include/dintwm.h)


ifdef strict
CFLAGS = $(CFLAGSSTRICT)
TEMPS = $(SOURCES:.c=.i) $(SOURCES:.c=.s)
PEXCL = --exclude-path */opt/amiga/*
endif

.PHONY: setver
setver:
	$(VERCMD)

all : $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)
	$(PANDOC)
ifdef strict
	$(CPPLINTCMD) $(CPPLINTOPTS) $(SOURCES) $(filter-out $(EXTHEADERS),$(HEADERS))
	$(CPPCHECKCMD) $(CPPCHECKOPTS) $(filter-out $(FTFYSRC),$(SOURCES))
	$(FLAWCMD) $(FLAWOPTS) $(HEADERS) $(filter-out $(FTFYSRC),$(SOURCES))
	$(SPLINTCMD) $(SOURCES) $(SPLINTARGS)
endif


dintwm.o : $(MAINHEADER) $(EXTHEADERS)
commodity.o : $(MAINHEADER) $(CONFHEADER)

.PHONY : clean
clean :
	-rm dintwm $(OBJECTS) $(TEMPS) $(LOGS)
	$(CLRVER)


