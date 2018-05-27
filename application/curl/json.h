#ifndef H_JSON__
#define H_JSON__
typedef struct{
	int tmp;
    char shightemp[20];
    char slowtemp[20];
}weather;
weather dofile(char *filename);/* Read a file, parse, render back, etc. */
#endif
