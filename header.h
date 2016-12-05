#ifndef header  
#define header
//functions
int mbranalysis (FILE *pmbr, FILE *pdd);
long vbranalysis (FILE* pvbr, FILE *pdd);
long partitionanalysis (unsigned char partx[], int partcount, FILE *pmbr, FILE *pdd);
long hextodec(unsigned char hex[], int size);
void hexoutput (FILE *pdd, FILE *pxbr, int bytecount);
int bytetobit(int x);
long cfr(unsigned char bin[], int size);
int sanitizepartselect (int totalpartcount);
char printtoscreen();
void shiftzero(long RSTdec[], int b);
//gvars
long RSTdec[128];

#endif
