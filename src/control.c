#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "globals.h"
#include "control.h"

void ControlUnsetEnv( p )
	package *p;
{
	return;
}

void ControlSetEnv( p )
	package *p;
{
	return;	
}

int ControlBuild( p )
	package *p;
{	
        if(p == NULL) return 1;

        //first process dependencies
        int i;
        for(i = 0; i < p->depcount; i++) {
                PackageLoadConfig(p->depends[i]);
                if(ControlBuild(p->depends[i])) {
			fprintf(stderr, "ERROR: Building package '%s'. ",
					p->depends[i]->name);
			fprintf(stderr,"Cannot continue.\n");
			return 1;
		}
        }

        //now process current package
	if( p->state == BuiltS ) {
		printf("Skipping: %s - already installed\n",p->name);
		return 1;
	}
	printf("Building: %s\n",p->name);

        // Open package config file
        char filename[MAX_LINE];
        char *buffer = malloc(sizeof(char)*MAX_LINE);
        char *path = getenv("MPKG_CONF_DIR");
        sprintf(filename,"%s/%s-%s.mpkg",path,p->name,p->version);
        #ifdef DEBUG
        fprintf(stderr,"DEBUG: Loading package file: %s\n",filename);
        #endif
        FILE *fp = fopen(filename,"r");
        if( !fp ) {
                fprintf(stderr,"Cound not open package configuration file: %s\n",filename);
                return 1;
        }
        #ifdef DEBUG
        fprintf(stderr,"DEBUG: Setting package variables from %s\n",filename);
        #endif
        while( fgets(buffer,sizeof(char)*MAX_LINE,fp) != NULL ) {
                #ifdef DEBUG
                fprintf(stderr,"DEBUG: Config line: %s --> ",buffer);
                #endif
                if(ControlExecDirect(p,buffer)) {
			fprintf(stderr, "ERROR: Executing directives. ");
			fprintf(stderr,"Cannot continue.\n");
			return 1;
		}
        }

	p->state = BuiltS;
	printf("Built: %s\n",p->name);

	return 0;
}

int ControlExecDirect(p,val)
        package* p;
        char* val;
{
	char *path;
	path = getenv("MPKG_BUILD_DIR"); //default location

        //clean name and val
        // trailing newline
        if(val[strlen(val) - 1] == '\n') val[strlen(val) - 1] = '\0';

        //directives
        if (strcmp(val,"#~prebuild") == 0) {
                p->state = PreBuildS;
		path = getenv("MPKG_BUILD_DIR");
                #ifdef DEBUG
                fprintf(stderr,"DEBUG: Setting state to PreBuildS\n");
                #endif
        }
        else if (strcmp(val,"#~install") == 0) {
                p->state = InstallS;
                #ifdef DEBUG
                fprintf(stderr,"DEBUG: Setting state to InstallS\n");
                #endif
        }
        else if (strcmp(val,"#~clean") == 0) {
                p->state = CleanS;
                #ifdef DEBUG
                fprintf(stderr,"DEBUG: Setting state to CleanS\n");
                #endif
        } else {
	        #ifdef DEBUG
	        fprintf(stderr,"DEBUG: executing command '%s'\n",val);
	        #endif
	}
	
        #ifdef DEBUG
	fprintf(stderr,"DEBUG: changing to directory '%s'\n", path);
	#endif
	if(chdir(getenv(path))) {
			fprintf(stderr,"ERROR: Could not change directory ");
			fprintf(stderr,"to '%s', error: '%s'. ", 
				path,strerror(errno));
			fprintf(stderr,"Cannot continue.\n");
		return 1;
	}
	return 0;
}

