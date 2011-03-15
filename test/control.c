#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "CUnit/Basic.h"

#include "test-control.h"
#include "control.h"

void testControlUnsetEnv (void)
{
	init_suite();
	
	//nothing yet to test
	ControlUnsetEnv(p1);
	//CU_ASSERT();

	clean_suite();
}

void testControlSetEnv (void)
{
	init_suite();
	
	//nothing yet to test
	ControlSetEnv(p1);
	//CU_ASSERT();

	clean_suite();
}

void testControlBuild (void)
{
	init_suite();
	
	CU_ASSERT(p1->state != BuiltS);
	ControlBuild(p1);
	CU_ASSERT(p1->state == BuiltS);

	clean_suite();
}
