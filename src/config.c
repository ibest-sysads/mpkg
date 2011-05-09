#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "config.h"

char *ConfigParseVars(line)
	char *line;
{
	char* value = malloc( sizeof(char)*MAX_LINE*2 );
	char* lptr = line;
	char* vptr = value;
	
	while( *lptr && *lptr != '\n' ) {
		if( *lptr == '$' ) {
			
			// Move past the '$' and if there is a '{' then move 
			// past that as well.
			lptr++;
			if( *lptr == '{' ) lptr++;
			
			// Pull out and expand the variable
			char buffer[MAX_WORD];
			char *bptr, *env;
			bptr = buffer;
			while( isalnum(*lptr) || *lptr == '_' ) {
				*bptr = *lptr;
				lptr++; bptr++;
			}
			*bptr = '\0';
			env = getenv(buffer);

			#ifdef DEBUG
			fprintf(stderr,"DEBUG: Parsed variable: %s\n",buffer);
			fprintf(stderr,"DEBUG: Parsed value: %s\n",env);
			#endif
			
			// Concatenate the env value to the parsed string
			while(*env != '\0') {
				*vptr = *env;
				env++; vptr++;
			}
			
			// Handle the increment that happens regardless of the conditional
			if( *lptr != '}' ) lptr--;
		} else {
			*vptr = *lptr;
			vptr++;
		}
		lptr++;
	}
	
	*vptr = '\0';
	return value;
}

int ConfigParseEnvironment(filename)
	char *filename;
{
	int i;
	char c;
	char *buffer = malloc(sizeof(char)*MAX_LINE);
	char *key = malloc(sizeof(char)*MAX_LINE);
	char *value = malloc(sizeof(char)*MAX_LINE);
		
  // Open global config file 
	FILE *fp = fopen(filename,"r");
	if( !fp ) {
		fprintf(stderr,"Cound not open global configuration file: %s\n",filename);
		return 1;
	}
	
	#ifdef DEBUG
	fprintf(stderr,"DEBUG: Setting environment variables from %s\n",filename); 
	#endif	
	
	while( fgets(buffer,sizeof(char)*MAX_LINE,fp) != NULL ) {
		#ifdef DEBUG
		fprintf(stderr,"DEBUG: Config line: %s",buffer);
		#endif
		
		int count = sscanf(buffer,"%s = %s\n",key,value);
		if( count == 2 ) {
			fprintf(stderr,"DEBUG: Config key: %s\n",key);
			fprintf(stderr,"DEBUG: Config value: %s\n",value);
			char *parsed = ConfigParseVars(value);
			int retval = setenv(key,parsed,1);
			if(retval) fprintf(stderr,"Could not set %s to %s\n",key,parsed);
			#ifdef DEBUG
			fprintf(stderr,"DEBUG: Setting %s to %s\n",key,parsed); 
			#endif
			free(parsed);
		} else {
			fprintf(stderr,"ERROR (%d): Could not parse line: %s",count,buffer);
			return 1;
		}
	}
	
	fclose(fp);
	free(buffer);
	free(key);
	free(value);
	return 0;
}

char *ConfigParseAssignName(line)
	char *line;
{
	if(strcmp(line,"") == 0) { //empty line 
		return(strdup(""));
	}

	char *valP; char *retVal;
	valP = (char*)malloc(sizeof(char) * strlen(line));
	*valP = *line;
	retVal = valP;
	int i = 0;
	while((valP[i] = line[i]) != '\0') {
		if(valP[i] == '=' || valP[i] == ' ') valP[i] = '\0';
		i++;
	}
	return(strdup(retVal));
}

char *ConfigParseAssignVal(line)
	char *line;
{
	if(strcmp(line,"") == 0) { //empty line 
		return(strdup(""));
	}

	char *retVal;
	int i = 0;
	while(line[i] != '\0') {
		if(line[i] == '=') {
			i++;
			retVal = &line[i]; //beginning of retVal
			while(line[i] == ' ') { //strip leading spaces
				i++;
				retVal = &line[i];
			}
		} else i++;
	}

	return(strdup(retVal));
}
