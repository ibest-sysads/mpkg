#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "CUnit/Basic.h"

#include "test-control.h"
#include "control.h"

void testPackageInit (void)
{
	init_suite();
	
	CU_ASSERT(0 == 0);

	clean_suite();
}
