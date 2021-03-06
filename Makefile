ifndef PREFIX
PREFIX=i386-mingw32-
endif

CC=$(PREFIX)gcc
CXX=$(PREFIX)g++
STRIP=$(PREFIX)strip
LD=$(CC)

CFLAGS:=-DWIN32 -DTELNET -DGAPING_SECURITY_HOLE -c
LDFLAGS=-lws2_32 -lshlwapi

all: tinybox_m.exe

tinybox_m.exe: getopt.o doexec.o netcat.o socks.o ps.o scan.o reg.o exch.o \
		scquery.o tinybox.o meterpreter.o
	$(LD) $^ $(LDFLAGS) -o $@ 
	$(STRIP) $@

clean:
	-rm *.o tinybox_m.exe
