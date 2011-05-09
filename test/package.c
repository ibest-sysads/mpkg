#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "CUnit/Basic.h"

#include "test-package.h"
#include "package.h"
#include "../src/config.h"

void testPackageInit (void)
{
	init_suite();
	
	CU_ASSERT(strcmp(p1->name,"test") == 0);
	CU_ASSERT(p1->state == InitS);
	CU_ASSERT(p1->version == NULL);
	CU_ASSERT(p1->depcount == 0);

	clean_suite();
}

void testPackageSetVersion (void) {
	init_suite();

	PackageSetVersion(p1,"1.0");
	CU_ASSERT(strcmp(p1->version,"1.0") == 0);

	clean_suite();
}

void testPackageLoadConfig (void) {
	init_suite();

	//unitialized test
	CU_ASSERT(PackageLoadConfig(p1) == 1);	
		
	PackageSetVersion(p1,"1.0");
	CU_ASSERT(PackageLoadConfig(p1) == 0);
	
	clean_suite();
}


void testPackageAddDepends (void) {
	init_suite();

	CU_ASSERT(p1->depcount == 0);

	package *d;
	d = PackageInit("test2");
	PackageAddDepends(p1,d);
	CU_ASSERT(p1->depcount == 1);
	CU_ASSERT(strcmp(p1->depends[p1->depcount - 1]->name, "test2") == 0);
		

	PackageDestroy(d);
	clean_suite();
}

void testPackageDestroy (void) {
	init_suite();

	PackageDestroy(p1);
	CU_ASSERT(p1 == NULL);

	clean_suite();
}

void testPackagePrint (void) {
	init_suite();

	//nothing to assert here, just can't die horribly.
	PackagePrint(p1);
	package *d;
	d = PackageInit("test2");
	PackageAddDepends(p1,d);
	PackagePrint(p1);

	PackageDestroy(d);
	clean_suite();
}

