#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include "globals.h"
#include "package.h"
#include "config.h"

#define INDENT indent += 4
#define UNINDENT indent -= 4
static int indent = -4;

package *PackageInit( name )
  char *name;
{
	int i;

	package *p = malloc(sizeof(package));
        p->name = strdup(name);

	for(i=0; i<MAX_DEPS; i++)	
        	p->depends[i] = NULL;

	p->state = InitS;
	p->version = NULL;
	p->depcount = 0;
}

void PackageSetVersion( p,v )
	package *p;
	char *v;
{
	p->version = strdup(v);
}

int PackageDownloadConfig( p ) 
	package *p;
{
	if(p->version == NULL) {
		fprintf(stderr,"ERROR (%s): Package version has not been set. \
						Cannot continue.",p->name);
		return 1;
	}

	char *destdir = getenv("MPKG_CONF_DIR");
	if(destdir == NULL) {
		fprintf(stderr,"ERROR (%s): Package configuration directory ",
				p->name);
		fprintf(stderr,"has not been set. Cannot continue.\n");
		return 1;
	}

	char buffer[MAX_LINE];
	char *cmd = malloc(sizeof(char)*MAX_LINE);
	char *output = "-q"; 
	#ifdef DEBUG
	output = "";
	#endif

	char filename[MAX_LINE];
	char *path = getenv("MPKG_REPO_FILE");
	sprintf(filename,"%s",path);

	// Open repository config file 
	FILE *fp = fopen(filename,"r");
	if( !fp ) {
		fprintf(stderr,"Cound not open repository configuration file: %s\n",filename);
		return 1;
	}

	#ifdef DEBUG
	fprintf(stderr,"DEBUG: Getting repositories from %s\n",filename);
	#endif

	while( fgets(buffer,sizeof(char)*MAX_LINE,fp) != NULL ) {
		if(buffer[strlen(buffer) - 1] == '\n') {
			buffer[strlen(buffer) - 1] = '\0';
		}
		sprintf(cmd,"wget %s -N %s/%s-%s.mpkg\n",output,buffer,p->name,p->version);	
		int result = 1;
		if(buffer[0] == '#') result = 1;
		else result = system(cmd);
		#ifdef DEBUG
		fprintf(stderr,"DEBUG: Download cmd: '%s', download: %d\n",
			cmd,result);
		#endif
		if(result == 0) {
			sprintf(cmd,"mv %s-%s.mpkg %s/\n",p->name,p->version, destdir);
			#ifdef DEBUG
			fprintf(stderr,"DEBUG: %s\n",cmd);
			#endif
			system(cmd); //no need to test, tested in later funcs
			free(cmd);
			return 0;
		}
	}

	free(cmd);
	return 1;
}

int PackageLoadConfig( p )
	package *p;
{
	if(p->version == NULL) {
		fprintf(stderr,"ERROR (%s): Package version has not been set. \
						Cannot continue.",p->name);
		return 1;
	}

	if(PackageDownloadConfig(p) != 0) {
		fprintf(stderr,"ERROR (%s): Package not available on ",
			p->name);
		fprintf(stderr,"repositories. Cannot continue.\n");
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
		fprintf(stderr,"Cound not open package configuration file: %s\n",filename);
		return 1;
	}

	#ifdef DEBUG
	fprintf(stderr,"DEBUG: Setting package variables from %s\n",filename);
	#endif
	
	while( fgets(buffer,sizeof(char)*MAX_LINE,fp) != NULL ) {
		#ifdef DEBUG
		fprintf(stderr,"DEBUG: Config line: %s --> ",buffer);
		fprintf(stderr,"Parsed Name: %s; ",ConfigParseAssignName(buffer));
		fprintf(stderr,"Parsed Value: %s\n",ConfigParseAssignVal(buffer));
		#endif
		PackageSetVar(p,
				ConfigParseAssignName(buffer),
				ConfigParseAssignVal(buffer));	
	}

	return 0;
}

int PackageGetSource( p )
	package *p;
{
	char *cmd = malloc(sizeof(char)*MAX_LINE);
	char *output = "-q"; 
	#ifdef DEBUG
	output = "";
	#endif
	sprintf(cmd,"wget %s -N %s\n",
			output,p->src);	
	#ifdef DEBUG
	fprintf(stderr,"DEBUG: Downloading package %s source with command:\n",
		p->name);
	fprintf(stderr," '%s'\n",cmd);
	#endif
	int status = system(cmd);
	free(cmd);
	return(status);
}

int PackageSetVar(p,name,val)
	package *p; 
	char *name;
	char *val;
{
	//clean name and val
	// trailing newline
	if(name[strlen(name) - 1] == '\n') name[strlen(name) - 1] = '\0'; 
	if(val[strlen(val) - 1] == '\n') val[strlen(val) - 1] = '\0';	

	//variables
	if(strcmp(name,"PKNAME") == 0) {
		p->name = strdup(val);
		#ifdef DEBUG
		fprintf(stderr,"DEBUG: Setting %s: %s\n",name,p->name);
		#endif
		return 0;
	}
	if(strcmp(name,"PKVERSION") == 0) {
		p->version = strdup(val);
		#ifdef DEBUG
		fprintf(stderr,"DEBUG: Setting %s: %s\n",name,p->version);
		#endif
		return 0;
	}
	if(strcmp(name,"PKGROUP") == 0) {
		p->group = strdup(val);
		#ifdef DEBUG
		fprintf(stderr,"DEBUG: Setting %s: %s\n",name,p->group);
		#endif
		return 0;
	}
	if(strcmp(name,"PKSRC") == 0) {
		p->src = strdup(val);
		#ifdef DEBUG
		fprintf(stderr,"DEBUG: Setting %s: %s\n",name,p->src);
		#endif
		return 0;
	}
	if(strcmp(name,"DEPS") == 0) {
		char *dep;
		dep = strtok(val," ");
		while(dep != NULL) { //for each dep name/version string
			//seperate name and version
			char *dname, *dversion;
			dname = strdup(dep);
			int i = 0;
			while(dname[i] != '\0') {
				if(dname[i] == '/') {
				  dname[i] = '\0';
				  i++;
                        	  dversion = &dep[i];//beginning of version
				} else i++;
			} //end seperate name and version
			//create and add dependancy package
			package *pdep = PackageInit(dname);
			PackageSetVersion(pdep,dversion);
			PackageAddDepends(p,pdep);
			//end create and add dependancy package	
			#ifdef DEBUG
			fprintf(stderr,"DEBUG: Adding dependency %s\n",dep);
			fprintf(stderr," DEBUG: Dependency name: %s\n",pdep->name);
			fprintf(stderr," DEBUG: Dependency version: %s\n",pdep->version);
			#endif
			free(dname); //frees dversion as well
			dep = strtok(NULL," "); //set dep to next token
		} //end while dep != NULL
		free(dep);
		return 0;
	} //end DEPS

	#ifdef DEBUG
	fprintf(stderr,"DEBUG: package var not matched: %s\n",name);
	#endif
	return 1;
}

void PackageAddDepends( p,d )
	package *p;
	package *d;
{
	package *tmp = p;
	p->depends[p->depcount] = d;
	p->depcount++;
	#ifdef DEBUG
	fprintf(stderr,"DEBUG: Adding %s to %s - %d\n",d->name,p->name,p->depcount); 
	#endif
}

void PackageDestroy( p )
	package *p;
{
	int i;
	if( p == NULL ) return;
	
	for(i=0; i<p->depcount; i++) {
		if(p->depends[i] != NULL ) {
			PackageDestroy(p->depends[i]);
			p->depends[i] = NULL;
		}
	}
	
	#ifdef DEBUG
	fprintf(stderr,"DEBUG: Releasing %s\n",p->name); 
	#endif
	
	if( p ) {
		if( p->version )
			free(p->version);
		if( p->name )
			free(p->name);
		free(p);
	}
	
	p = NULL;
}

void PackagePrint( p )
	package *p;
{
	int i;
	if( p == NULL ) return;

	INDENT;
	PackagePrintIndent();
	printf("%s", p->name);
	if( p->version != NULL )
		printf(" (%s)\n",p->version);
	else
		printf("\n");
	
	for(i=0; i<p->depcount; i++) {
		PackagePrint(p->depends[i]);
	}
	UNINDENT;
}

static void PackagePrintIndent( void ) {
  int i;
  for( i=0; i<indent; i++ )
    printf(" ");
}
