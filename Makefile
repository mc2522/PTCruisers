#
# Created by gmakemake (Ubuntu Jul 25 2014) on Tue Apr 23 02:14:25 2019
#

#
# Definitions
#

.SUFFIXES:
.SUFFIXES:	.a .o .c .C .cpp .s .S
.c.o:
		$(COMPILE.c) $<
.C.o:
		$(COMPILE.cc) $<
.cpp.o:
		$(COMPILE.cc) $<
.S.s:
		$(CPP) -o $*.s $<
.s.o:
		$(COMPILE.cc) $<
.c.a:
		$(COMPILE.c) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%
.C.a:
		$(COMPILE.cc) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%
.cpp.a:
		$(COMPILE.cc) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%

CC =		gcc
CXX =		g++

RM = rm -f
AR = ar
LINK.c = $(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)
LINK.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS)
COMPILE.c = $(CC) $(CFLAGS) $(CPPFLAGS) -c
COMPILE.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) -c
CPP = $(CPP) $(CPPFLAGS)
########## Flags from header.mak


CFLAGS =	-ggdb -Wall -Wextra -pedantic -std=c99 -pthread
CLIBFLAGS =	-lpthread -lrt


########## End of flags from header.mak


CPP_FILES =	
C_FILES =	display.c pt-cruisers.c racer.c test.c
PS_FILES =	
S_FILES =	
H_FILES =	display.h racer.h
SOURCEFILES =	$(H_FILES) $(CPP_FILES) $(C_FILES) $(S_FILES)
.PRECIOUS:	$(SOURCEFILES)
OBJFILES =	display.o racer.o 

#
# Main targets
#

all:	pt-cruisers test 

pt-cruisers:	pt-cruisers.o $(OBJFILES)
	$(CC) $(CFLAGS) -o pt-cruisers pt-cruisers.o $(OBJFILES) $(CLIBFLAGS)

test:	test.o $(OBJFILES)
	$(CC) $(CFLAGS) -o test test.o $(OBJFILES) $(CLIBFLAGS)

#
# Dependencies
#

display.o:	display.h
pt-cruisers.o:	display.h racer.h
racer.o:	display.h racer.h
test.o:	display.h racer.h

#
# Housekeeping
#

Archive:	archive.tgz

archive.tgz:	$(SOURCEFILES) Makefile
	tar cf - $(SOURCEFILES) Makefile | gzip > archive.tgz

clean:
	-/bin/rm -f $(OBJFILES) pt-cruisers.o test.o core

realclean:        clean
	-/bin/rm -f pt-cruisers test 
