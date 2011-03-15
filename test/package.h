#ifndef _TESTPACKAGE_H
#define _TESTPACKAGE_H

#include "../src/globals.h"
#include "../src/package.h"

package *p1;

void testPackageInit (void);
void testPackageSetVersion (void);
void testPackageLoadConfig (void);
void testPackageAddDepends (void);
void testPackageDestroy (void);
void testPackagePrint (void);

#endif
