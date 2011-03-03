#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include "globals.h"
#include "package.h"

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

int PackageLoadConfig( p )
	package *p;
{
	if(p->version == NULL) {
		fprintf(stderr,"ERROR (%s): Package version has not been set. \
						Cannot continue.",p->name);
		return 1;
	}
	
	char filename[MAX_LINE];
	char *path = getenv("MPKG_CONF_DIR");
	sprintf(filename,"%s/%s-%s.mpkg",path,p->name,p->version);
	#ifdef DEBUG
	fprintf(stderr,"DEBUG: Loading package file: %s\n",filename);
	#endif
	return 0;
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
