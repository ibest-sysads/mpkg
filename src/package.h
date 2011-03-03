#ifndef _PACKAGE_H
#define _PACKAGE_H

package *PackageInit( char* );
void PackageSetVersion( package*,char* );
int PackageLoadConfig( package* );
void PackageAddDepends( package*,package* );
void PackageDestroy( package* );
void PackagePrint( package* );
static void PackagePrintIndent( void );

#endif
