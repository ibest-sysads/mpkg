#ifndef _GLOBALS_H
#define _GLOBALS_H

#define DEBUG 1
#define MAX_DEPS 20
#define MAX_LINE 1024
#define MAX_WORD 100

typedef enum { InitS,LoadedS,PreBuildS,BuiltS,InstallS,CleanS } State;

typedef struct _package {
	char *name;
	char *version;
	char *group;
	char *src;
	int depcount; //dependency number
	State state;
	struct _package *depends[MAX_DEPS]; //dependency list
} package;

#endif
