#include <stdio.h>
#include <string.h>
void help();
int ps_main(int argc, char* argv[]);
extern "C" int nc_main(int argc, char* argv[]);
int socks_main(int argc, char* argv[]);
int kill_main(int argc, char* argv[]);
int whoami_main(int argc, char* argv[]);

typedef struct _applet_desc {
	const char* name;
	int (*entry)(int argc, char* argv[]);
	const char* desc;
} applet_desc;

applet_desc applets[] = {
	{"ps", ps_main, NULL}, 
	{"nc", nc_main, NULL}, 
	{"socks", socks_main, NULL},
	{"kill", kill_main, NULL},
	{"whoami", whoami_main, NULL},
};

int main(int argc, char* argv[])
{
	if (argc <= 1) {
		help();
		return -1;
	}
	
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

