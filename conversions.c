#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "header.h"
#define CharSize 1

void hexoutput (FILE *pdd, FILE *pxbr, int bytecount)
{
	int i = 0;
	int k = 0;
	int j = 0;
	int a = 0;
	unsigned char ch;
	
	fprintf(pxbr, "Hexadecimal Output\n");
	fprintf(pxbr, "\n        0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f  : 0 1 2 3 4 5 6 7 8 9 a b c d e f\n");
	fprintf(pxbr, "       ------------------------------------------------ : -------------------------------\n0x%03x | ", j);
	j=16;
	while( i != bytecount )
	{
		if (k != 15)
		{
			ch = fgetc(pdd);
			fprintf(pxbr, "%02x ", ch);
			k++;
		} else if (k == 15)
		{
			if(j % 512 != 0)
			{
				ch = fgetc(pdd);
				fprintf(pxbr, "%02x : ", ch);
				
				fseek (pdd, -16, SEEK_CUR);
				for (a=0; a <= 15; a++)
				{
					ch = fgetc(pdd);
					if((ch <= 0x20) || (ch == 0x7F))
					{
						fprintf(pxbr, ". ");
					} else {
						fprintf(pxbr, "%c ", ch);
					}
				}
				fprintf(pxbr, "\n0x%03x | ", j);
				k = 0;
				j = j+16;
			} else {
				ch = fgetc(pdd);
				fprintf(pxbr, "%02x : ", ch);
				fseek (pdd, -16, SEEK_CUR);
				for (a=0; a <= 15; a++)
				{
					ch = fgetc(pdd);
					if((ch <= 0x20) || (ch == 0x7F))
					{
						fprintf(pxbr, ". ");
					} else {
						fprintf(pxbr, "%c ", ch);
					}
				}
				if(i != bytecount-1)
				{
					
					fprintf(pxbr, "\n       ------------------------------------------------ : -------------------------------");
					fprintf(pxbr, "\n0x%03x | ", j);
				}
				j=j+16;
				k = 0;
			}
	
		}
	i++;
	}
}

int bytetobit(int x)
{
    static char bits[9];
    bits[0] = '\0';
    int a;
	int b = 0;
	
    for (a = 128; a > 0; a >>= 1)
    {
        strcat(bits, ((x & a) == a) ? "1" : "0");
    }
	b = cfr (bits, 8);
	
    return b;
}

long cfr(unsigned char bin[], int size)
{
	long int x = 0;
	int a = 0;
	int b = 6;
	
	for(a=0; a<size; a++)
	{
		if (bin[a] == '1')
		{
			bin[a] = 0;
		} else if (bin[a] == '0')
		{
			bin[a] = 1;
		}
	}
	
	for (a=1; a < size; a++)
	{
		x = x + (bin[a] * (pow(2, b)));
		b--;
	}
	x=(pow(2, (x+1)));
	return x;
}

long int hextodec(unsigned char hex[], int size)
{
	long int x = 0;
	int a = 0;
	int b = 0;
	
	for (a=0; a < size; a++)
	{
		x = x + (hex[a] * (pow(16, b)));
		b=b+2;
	}
	return x;
}

void shiftzero(long RSTdec[], int b)
{
	int a=0;
	int c = 0;
	

	for (int a = 0; a < b; a++)
	{
        if (RSTdec[a] != 0)
		{
            RSTdec[c++] = RSTdec[a];
		}
	}
	while (c < b)
    {
		RSTdec[c++] = 0;
	}
}