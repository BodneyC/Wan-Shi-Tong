#ifndef header
#define header
//functions
int mbranalysis (FILE *pmbr, FILE *pdd);
long vbranalysis (FILE* pvbr, FILE *pdd, int secpclus, int bps);
long partitionanalysis (unsigned char partx[], int partcount, FILE *pmbr, FILE *pdd);
long hextodec(unsigned char hex[], int size);
void hexoutput (FILE *pdd, FILE *pxbr, int bytecount);
int bytetobit(int x);
long cfr(unsigned char bin[], int size);
int sanitizepartselect (int totalpartcount);
char printtoscreen();
void shiftzero(int b);
void clearscreen(int x);
long bytestodec (int offset, unsigned char vbrstring[], int bytenum);
//gvars
long RSTdec[128][2];

#endif
