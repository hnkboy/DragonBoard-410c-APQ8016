#ifndef H_JSON__
#define H_JSON__
typedef struct{
	int status;
	int tmp;
    char shightemp[20];
    char slowtemp[20];
	char stype[20];
	char snotice[40];
}weather;
int dofile(char *filename,weather *pweather);/* Read a file, parse, render back, etc. */
#endif
