#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "header.h"
#define CharSize 1

int main()
{
	char file[50]; //string of filename
	FILE* pdd; //pointer to dd
	FILE* pmbr; //pointer to mbranalysis.txt
	FILE* pvbr; //pointer to vbranalysis.txt
	FILE* out = stdout; //pointer to stdout
	int a = 0;
	int b = 0;
	int partitionselection = 0;
	int totalpartcount = 0;
	char vbrfilename[100];
	char partin[4];
	char PTS;
	long MFTdec[128] = {0};
	/*to be deleted*/ char pathtoimage[] = "D:\\Users\\BenJC\\Documents\\1.Current\\DMU\\FYP\\C\\images\\01.001";
	

//open file pointers
	printf("Image path for analysis: \n");
	/* D:\Users\BenJC\Desktop\FYP\c\images\01.001 */
	// ***uncomment when done:	gets(file);
	/* ***should be 'file' not 'x'; */	pdd = fopen(pathtoimage,"rb"); 
	
	while( pdd == NULL )
	{
		fclose(pdd);
		memset(&file[0], 0, sizeof(file));
		printf("Cannot open file\nImage path for MBR analysis: \n");
		gets(file);
		pdd = fopen(file,"rb");
	}
	
	pmbr = fopen("01. MBR_analysis.txt", "wt");
	while (pmbr == NULL)
	{
		fclose (pmbr);
		pmbr = fopen("01. MBR_analysis.txt", "w");
	}
	
	

// MBR_Analysis
	fseek(pdd, 0, SEEK_SET);
	hexoutput (pdd, pmbr, 512);	
	fseek(pdd, 0, SEEK_SET);
	totalpartcount = mbranalysis (pmbr, pdd);
	
	printf("\nFile output successful\n");
	PTS = printtoscreen();	
	if (PTS == 'y')
	{
		fseek(pdd, 0, SEEK_SET);
		hexoutput (pdd, out, 512);
		fseek(pdd, 0, SEEK_SET);
		mbranalysis (out, pdd);
	} else if (PTS == 'n')
	{
		printf("\nOutputted to file alone");
	}
	
	
	if (totalpartcount == 129)
	{
		rename("01. MBR_analysis.txt", "01. Legacy_MBR_analysis.txt");

		//GPT shit
	} else {
		b = sizeof(RSTdec) / sizeof(RSTdec[0]);
		shiftzero(RSTdec, b);
		b=0;
	}

	
	a=0;
	while (RSTdec[a] != 0)
	{
		
		snprintf(vbrfilename, sizeof(vbrfilename), "02. VBR Analysis for Partition %i.txt", a+1);
		pvbr = fopen(vbrfilename, "wt");
		while (pvbr == NULL)
		{
			fclose (pvbr);
			pmbr = fopen(vbrfilename, "w");
		}
		
		fseek(pdd, (RSTdec[a] * 0x200), SEEK_SET);
		hexoutput (pdd, pvbr, 512);
		fseek(pdd, -512, SEEK_CUR);
		MFTdec[a] = vbranalysis (pvbr, pdd);
		
		MFTdec[a] += RSTdec[a];
		
		printf("%d ", MFTdec[a]);
		
		
		a++;
		fclose(pvbr);
	}
	
	printf("\nThere are %i total NTFS partitions available\n\nWhich partition to analyse?\n", totalpartcount);
	partitionselection=(sanitizepartselect (totalpartcount) - 1 );
	PTS = printtoscreen();
	if (PTS == 'y')
	{
		fseek(pdd, (RSTdec[partitionselection] * 0x200), SEEK_SET);
		hexoutput (pdd, out, 512);
		fseek(pdd, -512, SEEK_CUR);
		vbranalysis (out, pdd);
	} else if (PTS == 'n')
	{
		printf("\nOutputted to file alone");
	}
	
	
	
	
//VBR_analysis	
/*
	
	MFTdec = lMFTdec + RSTdec;
	*/
	
	
	
	
//close image file	
	fclose(pdd);
	printf("\nComplete");
	return 0;
}

int sanitizepartselect (int totalpartcount)
{
	int a = 1;
	char partin[4];
	
	fgets(partin, sizeof(partin), stdin);	
	if  (partin != NULL)
	{
		a = atoi(partin);
		while (a > totalpartcount || a <=0 )
		{
			printf("\nOut of bounds, maximum: %i:\n", totalpartcount);
			fgets(partin, sizeof(partin), stdin);	
			if  (partin != NULL)
			{
				a = atoi(partin);
			}
		}			
	} else {
		printf("\nNo partition selected, please enter partition for analysis:\n");
		fgets(partin, sizeof(partin), stdin);	
		if  (partin != NULL)
		{
			a = atoi(partin);
			while (a > totalpartcount || a <=0 )
			{
				printf("\nOut of bounds, maximum: %i:\n", totalpartcount);
				fgets(partin, sizeof(partin), stdin);	
				if  (partin != NULL)
				{
					a = atoi(partin);
				}
			}		
		}
		
	}
	
	return a;
}

char printtoscreen()
{
	char PTS;
	
	printf("\nPrint analysis to screen? (y or n)\n");
	PTS = getchar();
	while(getchar() != '\n');
	
	while (PTS != 'y' && PTS != 'n')
	{
		printf("Incorrect input (y or n)\n");
		PTS = getchar();
		while(getchar() != '\n');
	}
	
	return PTS;
}