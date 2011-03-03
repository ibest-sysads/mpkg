#include <stdio.h>
#include <stdlib.h>
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

void ControlBuild( p )
	package *p;
{
	int i;
	for(i=0; i<p->depcount; i++) {
		ControlBuild(p->depends[i]);
	}
	if( p->state == BuiltS ) {
		printf("Skipping: %s - already installed\n",p->name);
	} else {
		
		p->state = BuiltS;
		printf("Building: %s\n",p->name);
	}
}

