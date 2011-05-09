#ifndef _PACKAGE_H
#define _PACKAGE_H

package *PackageInit( char* );
void PackageSetVersion( package*,char* );
int PackageDownloadConfig( package* );
int PackageLoadConfig( package* );
void PackageAddDepends( package*,package* );
int PackageAddDirectiveName( char* );
int PackageAddDirectiveVal( char* );
int PackageSetVar(package *p, char *, char *);
void PackageDestroy( package* );
void PackagePrint( package* );
static void PackagePrintIndent( void );
int PackageProcess( package *);

#endif
