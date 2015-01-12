
cc=cl
link=link

cflags=/nologo /ML /W3 /GX /O2 /Zd /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "TELNET" /D "GAPING_SECURITY_HOLE" /YX /FD /c 
#lflags=kernel32.lib user32.lib wsock32.lib winmm.lib /nologo /subsystem:console /incremental:yes /machine:I386 /debug /out:tinybox.exe /PDB:tinybox.pdb
lflags=kernel32.lib user32.lib wsock32.lib winmm.lib /nologo /subsystem:console /incremental:yes /machine:I386 /out:tinybox.exe

all: tinybox.exe

getopt.obj: getopt.c
    $(cc) $(cflags) getopt.c

doexec.obj: doexec.c
    $(cc) $(cflags) doexec.c

netcat.obj: netcat.c
    $(cc) $(cflags) netcat.c

socks.obj: socks.cpp
	$(cc) $(cflags) socks.cpp

ps.obj: ps.cpp
	$(cc) $(cflags) ps.cpp

scan.obj: scan.c
	#(cc) $(cflags) scan.c

reg.obj: reg.c
	#(cc) $(cflags) reg.c

tinybox.obj: tinybox.cpp
	$(cc) $(cflags)	tinybox.cpp

tinybox.exe: getopt.obj doexec.obj netcat.obj socks.obj ps.obj scan.obj reg.obj tinybox.obj
    $(link) getopt.obj doexec.obj netcat.obj socks.obj ps.obj tinybox.obj $(lflags)

