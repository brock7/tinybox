#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void help();
#ifdef WIN32
int ps_main(int argc, char* argv[]);
int kill_main(int argc, char* argv[]);
int whoami_main(int argc, char* argv[]);
extern "C" int reg_main(int argc, char* argv[]);
extern "C" int scquery_main(int argc, char* argv[]);

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

	srand((unsigned int)time(NULL));
	
	for (int i = 0; i < sizeof(applets) / sizeof(applets[0]); i ++) {
		if (strcmp(applets[i].name, argv[1]) == 0) {
			return applets[i].entry(argc - 1, &argv[1]);
		}
	}
	help();
	return -1;
}

void help()
{
	printf("help msg\n");
}

