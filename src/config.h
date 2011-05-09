#ifndef _CONFIG_H
#define _CONFIG_H

char *ConfigParseVars(char*);
int ConfigParseEnvironment(char*);
char *ConfigParseAssignName(char *);
char *ConfigParseAssignVal(char *);

#endif
