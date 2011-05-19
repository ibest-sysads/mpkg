#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "globals.h"
#include "control.h"

int DEPTH = 0;

int ControlSetEnv( p )
	package *p;
{
	int status = (
		setenv("PKNAME",p->name,1)
		|| setenv("PKVERSION",p->version,1)
		|| setenv("PKGROUP",p->group,1)
		|| setenv("PKSRC",p->src,1)
		);
	return status;
}

int ControlUnsetEnv( p )
	package *p;
{
	return 1;	
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
	if(ControlSetEnv(p)) {
		fprintf(stderr,"Could not set package %s environment.\n",
			p->name);
		fprintf(stderr,"Cannot continue.\n");
		return(1);
	}

	if(PackageGetSource(p)) {
		fprintf(stderr,"Could not open package source: %s.\n",p->src);
		fprintf(stderr,"Cannot continue.\n");
		return 1;
	}

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
                fprintf(stderr,"Could not open package configuration file: %s\n",filename);
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
	if(ControlUnsetEnv(p)) {
		fprintf(stderr,"Could not unset package %s environment.\n",
			p->name);
		fprintf(stderr,"Cannot continue.\n");
		return 1;
	}

	return 0;
}

int ControlExecDirect(p,val)
        package* p;
        char* val;
{
	char *path;
	path = getenv("MPKG_BUILD_DIR"); //default location
	char *cmd = NULL; //directive command

        //clean name and val
        // trailing newline
        if(val[strlen(val) - 1] == '\n') val[strlen(val) - 1] = '\0';

        //ignore package properties
        if (	(strstr(val,"PKNAME") != NULL
		|| strstr(val,"PKVERSION") != NULL
		|| strstr(val,"PKGROUP") != NULL
		|| strstr(val,"PKSRC") != NULL
		|| strstr(val,"DEPS") != NULL
		)
		&& strstr(val,"$") == NULL
	) {
		return 0;
	}
        else if (strcmp(val,"#~prebuild") == 0) {
                p->state = PreBuildS;
		path = getenv("MPKG_BUILD_DIR");
                #ifdef DEBUG
                fprintf(stderr,"DEBUG: Setting state to PreBuildS\n");
                #endif
        }
        else if (strcmp(val,"#~install") == 0) {
                p->state = InstallS;
		path = getenv("MPKG_BUILD_DIR");
		sprintf(path,"%s/%s-%s",path,p->name,p->version);
                #ifdef DEBUG
                fprintf(stderr,"DEBUG: Setting state to InstallS\n");
                #endif
        }
        else if (strcmp(val,"#~clean") == 0) {
                p->state = CleanS;
		path = getenv("MPKG_BUILD_DIR");
                #ifdef DEBUG
                fprintf(stderr,"DEBUG: Setting state to CleanS\n");
                #endif
        }        
	else if (strcmp(val,"#~module") == 0) {
                p->state = BuiltS;
		path = getenv("MPKG_BUILD_DIR");
                #ifdef DEBUG
                fprintf(stderr,"DEBUG: Setting state to BuiltS\n");
                #endif
 	} else {
		cmd = val;
	        #ifdef DEBUG
	        fprintf(stderr,"DEBUG: Setting execute command to '%s'\n",cmd);
	        #endif
	}
	
        #ifdef DEBUG
	fprintf(stderr,"DEBUG: changing to directory '%s'\n", path);
	#endif
	if(chdir(path)) {
			fprintf(stderr,"ERROR: Could not change directory ");
			fprintf(stderr,"to '%s', error: '%s'.\n", 
				path,strerror(errno));
			char buffer[MAX_WORD];
			getcwd(buffer, MAX_WORD);
			fprintf(stderr," (Current directory is '%s'.)\n",
				buffer);
			fprintf(stderr,"Cannot continue.\n");
		return 1;
	}

	//if cmd set, execute directive mode
	if(cmd != NULL) return(system(cmd));

	return 0;
}

void ControlPrintDeps( p )
	package *p;
{	
        if(p == NULL) return;

	#ifdef DEBUG
	printf("%*s:%d\n",DEPTH + (int)strlen(p->name),p->name,DEPTH);
	#else
	printf("%*s\n",DEPTH + (int)strlen(p->name),p->name);
	#endif        

	int i;
        for(i = 0; i < p->depcount; i++) {
                PackageLoadConfig(p->depends[i]);
		if(DEPTH > MAX_DEPTH) {
			fprintf(stderr, "ERROR: MAX_DEPTH exceeded for ");
			fprintf(stderr,"dependencies. Cannot continue");
			return;
		}
		DEPTH++;
		ControlPrintDeps(p->depends[i]);
		DEPTH--;
        }
}
