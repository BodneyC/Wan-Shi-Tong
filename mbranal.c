#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "header.h"
#define CharSize 1

//------------------------------------------------------------------------------------

int mbranalysis (FILE* pmbr, FILE* pdd)
{
	int offset = 0x1BE;
	int partcount = 1;
	int bootpart = 1;
	char *ordinal;
	unsigned char ch; //for fgetc
	int a=0;
	unsigned char mbrstring[512]; //array of mbr values
	int RSTcount = 0;
	int totalpartcount = 0;
	
	memset(RSTdec, 0, (sizeof(long)*2)*127);
	
	for(a=0; a<=511; a++)
	{
		ch = fgetc(pdd);
		mbrstring[a] = ch;
	}		

//first two bytes for OS type
	if ((mbrstring[0] == 0xFA) && (mbrstring[1] == 0x33))
	{
		fprintf(pmbr, "\nOS Versions DOS 3.1 through Windows 95a");
	} else if ((mbrstring[0] == 0x33) && (mbrstring[1] == 0xC0))
	{
		fprintf(pmbr, "\nOS Versions Windows 95b Onwards");
	} else if ((mbrstring[0] == 0xFA) && (mbrstring[1] == 0xEB))
	{
		fprintf(pmbr, "\nOS using the Linux Bootloader (LILO)");
	} else if ((mbrstring[0] == 0xEB) && (mbrstring[1] == 0x3C))
	{
		fprintf(pmbr, "\nBoot record for Windows Floppy Disk");
	}

//disk signature
	fprintf(pmbr, "\nDisk Signature: %02x %02x %02x %02x (%02x %02x)", mbrstring[0x1b8], mbrstring[0x1b9], mbrstring[0x1ba], mbrstring[0x1bb], mbrstring[0x1bc], mbrstring[0x1bd]);

//finding bootable partition
	fprintf(pmbr, "\n\nPartition Analysis:\n");
	while((mbrstring[offset] != 0x80) && (offset <= 0x1EE))
	{
		bootpart++;
		offset = offset+0x10;
	}

// ordinal
	if(bootpart==1)
	{
		ordinal="first";
	}else if (bootpart==2)
	{
		ordinal="second";
	}else if (bootpart==3)
	{
		ordinal="third";
 	}else if (bootpart==4)
	{
		ordinal="fourth";
	}

//print bootable drive
	if((offset > 0x1EE) && (mbrstring[offset] != 0x80))
	{
		fprintf(pmbr, "There is no bootable drive, likely corrupt or storage drive");
	} else
	{
		fprintf(pmbr, "\nThe %s partition is bootable\n", ordinal);
	}

//populating partition strings
	offset = 0x1be;
	
//calling partitionanalysis in order
	partcount = 1;
	for (a=0;a<4;a++)
	{
		if (mbrstring[offset] == 0x00 && mbrstring[offset+4] == 0x00)
		{
		} else {
			totalpartcount++;
		}
		RSTdec[partcount-1][0] = partitionanalysis(mbrstring, partcount, pmbr, pdd);
		RSTdec[partcount-1][1] = mbrstring[offset+4];
		offset = offset+0x10;
		partcount++;
	}
	
//ms_ext
	if (mbrstring[offset+0x34] == 0x05 || mbrstring[offset+0x34] == 0x0f)
	{
		fseek(pdd, ((RSTdec[3][0]*512)+0x1be), SEEK_SET);
		for(a=0; a<=511; a++)
		{
			ch = fgetc(pdd);
			mbrstring[a] = ch;
		}
		for (a=0;a<4;a++)
		{
			if (mbrstring[offset] == 0x00 && mbrstring[offset+4] == 0x00)
			{
			} else {
				totalpartcount++;
			}
			RSTdec[partcount-1][0] = partitionanalysis(mbrstring, partcount, pmbr, pdd);
			RSTdec[partcount-1][1] = mbrstring[offset+4];
			offset = offset+0x10;
			partcount++;
		}
		
	} else if (mbrstring[offset+0x4] == 0xEF || mbrstring[offset+0x14] == 0xEF || mbrstring[offset+0x24] == 0xEF || mbrstring[offset+0x34] == 0xEF) //GPT_Switch
	{
		return 129;
	} else {
		fprintf(pmbr, "\nNo additional partitions (MBR extended)\n");
	}	
	return totalpartcount;

}


//------------------------------------------------------------------------------------


long int partitionanalysis(unsigned char mbrstring[], int partcount, FILE* pmbr, FILE *pdd)
{
	unsigned char RSThex[4] = {0};
	unsigned char TShex[4] = {0};
	int offset2 = 0;
	int a = 0;
	long RST = 0;
	long TSdec = 0;
	int offset = (0x1be + ((partcount-1) * 0x10));

	
//ignore empty partitions
	if((mbrstring[offset] == 0x00) && (mbrstring[offset+4] == 0x00))
	{
		fprintf(pmbr, "\nPartition %i is not in use (empty)\n", partcount);
	} else
	{		
		if (mbrstring[offset] == 0x80)
		{
			fprintf(pmbr, "Partition %i, Bootable Partition Analysis:\n", partcount);
		} else if(mbrstring[offset+4] == 0x05){
			fprintf(pmbr, "\nMicrosoft extended partition analysis:\n");
		} else{
			fprintf(pmbr, "\nPartition %i Analysis:\n", partcount);
		}

// partition analysis - physical
		fprintf(pmbr, "Physical Disk Location (CHS)\n");
		fprintf(pmbr, " Start cylinder: %i\n", mbrstring[offset+0x3]);
		fprintf(pmbr, " Start head: %i\n", mbrstring[offset+0x2]);
		fprintf(pmbr, " Start sector: %i\n", mbrstring[offset+0x1]);
		fprintf(pmbr, " Ending cylinder: %i\n", mbrstring[offset+0x7]);
		fprintf(pmbr, " Ending head: %i\n", mbrstring[offset+0x6]);
		fprintf(pmbr, " Ending sector: %i\n", mbrstring[offset+0x5]);
		fprintf(pmbr, "Partition type ");
		switch(mbrstring[offset+0x4])
		{
				case 0x00:
					fprintf(pmbr, "%02x, Empty", mbrstring[offset+0x4]);
					break;
				case 0x01:
					fprintf(pmbr, "%02x, FAT12", mbrstring[offset+0x4]);
					break;
				case 0x04:
				case 0x06:
				case 0x0E:
					fprintf(pmbr, "%02x, FAT16", mbrstring[offset+0x4]);
					break;
				case 0x05:
					fprintf(pmbr, "%02x, MS Extended", mbrstring[offset+0x4]);
					break;
				case 0x0F:
					fprintf(pmbr, "%02x, MS Extended", mbrstring[offset+0x4]);
					break;
				case 0x07:
					fprintf(pmbr, "%02x, NTFS", mbrstring[offset+0x4]);
					break;
				case 0x0B:
				case 0x0C:
					fprintf(pmbr, "%02x, FAT32", mbrstring[offset+0x4]);
					break;
				case 0x11:
					fprintf(pmbr, "%02x, Hidden FAT12", mbrstring[offset+0x4]);
					break;
				case 0x14:
				case 0x16:
				case 0x1E:
					fprintf(pmbr, "%02x, Hidden FAT16", mbrstring[offset+0x4]);
					break;
				case 0x1B:
				case 0x1C:
					fprintf(pmbr, "%02x, Hidden FAT32", mbrstring[offset+0x4]);
					break;
				case 0x27:
					fprintf(pmbr, "%02x, Windows Recovery (Hidden)", mbrstring[offset+0x4]);
					break;
				case 0x42:
					fprintf(pmbr, "%02x, Dynamic MBR", mbrstring[offset+0x4]);
					break;
				case 0x82:
					fprintf(pmbr, "%02x, Linux swap", mbrstring[offset+0x4]);
					break;
				case 0x83:
					fprintf(pmbr, "%02x, Linuc", mbrstring[offset+0x4]);
					break;
				case 0x84:
					fprintf(pmbr, "%02x, Hibernation", mbrstring[offset+0x4]);
					break;
				case 0x85:
					fprintf(pmbr, "%02x, Linux Extended", mbrstring[offset+0x4]);
					break;
				case 0xA5:
					fprintf(pmbr, "%02x, FreeBSD", mbrstring[offset+0x4]);
					break;
				case 0xA6:
					fprintf(pmbr, "%02x, OpenBSD", mbrstring[offset+0x4]);
					break;
				case 0xA8:
					fprintf(pmbr, "%02x, MacOSX", mbrstring[offset+0x4]);
					break;
				case 0xA9:
					fprintf(pmbr, "%02x, NetBSD", mbrstring[offset+0x4]);
					break;
				case 0xEE:
				case 0xEF:
					fprintf(pmbr, "%02x, U/EFI GPT Disk", mbrstring[offset+0x4]);
					break;
				case 0xFB:
					fprintf(pmbr, "%02x, VMWare File System", mbrstring[offset+0x4]);
					break;
				case 0xFC:
					fprintf(pmbr, "%02x, VMWare Swap", mbrstring[offset+0x4]);
					break;
				default:
					fprintf(pmbr, "unknown to this software");
			}
			
		offset2 = 8;
		for(a=0; a<=4; a++)
		{
			RSThex[a] = mbrstring[offset+offset2];
			TShex[a] = mbrstring[offset+(offset2+4)];
			offset++;
		}
		RST = hextodec(RSThex, 4);
		TSdec = hextodec(TShex, 4);



		if (mbrstring[offset+4] != 0x05)
		{
			fprintf(pmbr, "\nRelative start sector: %i", RST);
		} else {
			fprintf(pmbr, "\nStarting sector of extended partition table: %i", RST);
		}
		fprintf(pmbr, "\nTotal Sectors: %i\n", TSdec);
		
		
	}
//return 0 if ntfs
	offset = (0x1be + ((partcount-1) * 0x10));
	if ((mbrstring[offset+4] == 0x05) || (mbrstring[offset+4] == 0x0F) || (mbrstring[offset+4] == 0x07) || (mbrstring[offset+4] == 0x27))
	{
		return RST;
	} else {
		return 0;
	}
}
