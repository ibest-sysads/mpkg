#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include "globals.h"
#include "package.h" 
#include "control.h"

#include "buildConfig.h" //gets mpkg.conf location 

#define CONFIGPATH_LENGTH 2048

//not sure where to put these...
char *main_pkg_name, *main_pkg_version;

static int opt_debug;
char opt_config[CONFIGPATH_LENGTH];
static struct option long_options[] = {
	{"config",	1, 0, 	'c'},
	{"install",	1, 0, 	'i'},
	{"version",	1, 0, 	'v'},
	{"pretend",	0, 0,	'p'},
	{"help",	0, 0, 	'h'},
	{"debug",	0, 0,	0},
	{0,0,0,0}
};

void ShowUsage() {
	printf("Usage: mpkg [options]\n");
	printf("Options:\n");
	printf(" -c FILE, config FILE\n");
	printf("		Environment configuration file\n");
	printf(" -i <name>, install <name> \n");
	printf("		Package install\n");
	printf(" -v <version>, version <version> \n");
	printf("		Package version\n");
	printf(" -p, pretend <version> \n");
	printf("		pretend install\n");
	printf("Report bugs to /dev/null\n");
	exit(0);
}

static int GetOptions(argc,argv)
	int argc;
	char *argv[];
{
	int c;
	int option_index;

	// Set up defaults
	opt_debug = 0;
	//strncpy(opt_config,"/etc/mpkg/mpkg.conf",CONFIGPATH_LENGTH);
	strncpy(opt_config,MPKG_CONF_FILE,CONFIGPATH_LENGTH);
	
	while(1) {
		c = getopt_long(argc,argv,"c:i:v:phd",long_options,&option_index);
		if(c == -1) break;
		switch(c) {
			case 'h':
				ShowUsage();
				break;
			case 'c':
				strncpy(opt_config,optarg,CONFIGPATH_LENGTH);
				break;
			case 'i':
				main_pkg_name = (char*)malloc(sizeof(char*) * \
							strlen(optarg));
				main_pkg_name = strdup(optarg);
				break;
			case 'v':
				main_pkg_version = (char*)malloc(sizeof(char*)\
							* strlen(optarg));
				main_pkg_version = strdup(optarg);
				break;
			case 'd':
				opt_debug = 1;
				break;
			default:
				ShowUsage();
				break;
		}
	}
	return optind;
}

int main(argc,argv)
	int argc;
	char **argv;
{
	int optlen;
	optlen = GetOptions(argc,argv);
	argv += optlen;
	argc -= optlen;

	#ifdef _GLOBALCONF
	char *globalconf = strdup("etc/mpkg.conf");
	#else
	char *globalconf = strdup("/etc/mpkg/mpkg.conf");
	#endif
	
	if( ConfigParseEnvironment(opt_config) > 0 ) exit(1);

	package *mainp = PackageInit(main_pkg_name);
	PackageSetVersion(mainp,main_pkg_version);
	PackageLoadConfig(mainp); //loads deps
	printf("%s dependency tree:\n",mainp->name);
	ControlPrintDeps(mainp);
	ControlBuild(mainp);

	PackageDestroy(mainp);

	//cleanup
	free(globalconf);
}
