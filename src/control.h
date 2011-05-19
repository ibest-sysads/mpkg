#include "globals.h"

#ifndef _CONTROL_H
#define _CONTROL_H

#define MAX_DEPTH 15

int ControlSetEnv(package*);
int ControlUnsetEnv(package*);
int ControlBuild(package*);
int ControlExecDirect(package*,char*);
void ControlPrintDeps(package*);

#endif

