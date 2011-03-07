#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include "globals.h"
#include "package.h" 
#include "control.h"

#define CONFIGPATH_LENGTH 2048

static int opt_debug;
char opt_config[CONFIGPATH_LENGTH];
static struct option long_options[] = {
	{"config",	1, 0, 	'c'},
	{"help",	0, 0, 	'h'},
	{"debug",	0, 0,	0},
	{0,0,0,0}
};

void ShowUsage() {
	printf("Showing usage\n");
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
	strncpy(opt_config,"/etc/mpkg/mpkg.conf",CONFIGPATH_LENGTH);
	
	while(1) {
		c = getopt_long(argc,argv,"c:hd",long_options,&option_index);
		if(c == -1) break;
		switch(c) {
			case 'h':
				ShowUsage();
				break;
			case 'c':
				strncpy(opt_config,optarg,CONFIGPATH_LENGTH);
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

	package *p1,*p2,*p3,*p4,*p5,*p6;

	p1 = PackageInit("test-one");
	p2 = PackageInit("test-two");
	p3 = PackageInit("test-three");
	p4 = PackageInit("test-four");
	p5 = PackageInit("test-five");
	p6 = PackageInit("test-six");
	
	PackageSetVersion(p1,"1.1");
	PackageSetVersion(p2,"1.2");
	PackageSetVersion(p3,"1.3");
	PackageSetVersion(p4,"1.4");
	PackageSetVersion(p5,"1.5");
	PackageSetVersion(p6,"1.6");
	
	PackageLoadConfig(p1);
	PackageLoadConfig(p2);
	PackageLoadConfig(p3);
	PackageLoadConfig(p4);
	PackageLoadConfig(p5);
	PackageLoadConfig(p6);

	PackageAddDepends(p1,p2);
	PackageAddDepends(p1,p3);
	PackageAddDepends(p3,p4);
	PackageAddDepends(p4,p5);
	PackageAddDepends(p4,p6);
	PackageAddDepends(p1,p6);
	
	PackagePrint(p1); 
	ControlBuild(p1);
	PackageDestroy(p1);
	
	free(globalconf);
}
