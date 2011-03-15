#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "CUnit/Basic.h"

#include "test-config.h"
#include "config.h"

void testConfigParseVars (void)
{
	init_suite();

	char * line;
	line = (char*)malloc(sizeof(char) * MAX_LINE * 2);	
	setenv("TEST","test",0);
	line = "$TEST\n";
	CU_ASSERT(strcmp(ConfigParseVars(line), "test") == 0);

	//free(line);
	clean_suite();
}

void testConfigParseEnvironment (void)
{
	init_suite();

	char *fname;
	fname = (char*)malloc(sizeof(char) * MAX_LINE * 2);	
	fname = "/dev/dne";
	CU_ASSERT(ConfigParseEnvironment(fname) == 1);
	fname = "../samples/data/mpkg.conf";
	CU_ASSERT(ConfigParseEnvironment(fname) == 0);

	//free(fname);
	clean_suite();
}
