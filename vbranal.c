#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "header.h"
#define CharSize 1


//------------------------------------------------------------------------------------


long vbranalysis (FILE* pvbr, FILE* pdd, int secpclus, int bps)
{


	long cMFTdec = 0;
	long lMFTdec = 0;
	unsigned char vbrstring[512]; //array of vbr values
	unsigned char ch; //for fgetc
	long fromconv = 0;
	int offset = 0x000;
	int a = 0;
	int cfr = 0;

	for(a=0; a<=511; a++)
	{
		ch = fgetc(pdd);
		vbrstring[a] = ch;
	}

	fprintf(pvbr, "\n\nFirst three bytes:\n 0x000: %02x %02x %02x\nJump instruction to 0x054 (Executable code)", vbrstring[0], vbrstring[1], vbrstring[2]);
	fprintf(pvbr, "\n\nOEM ID: %c%c%c%c\n 0x003: %02x %02x %02x %02x", vbrstring[3], vbrstring[4], vbrstring[5], vbrstring[6], vbrstring[3], vbrstring[4], vbrstring[5], vbrstring[6]);
	fprintf(pvbr, "\n\nBIOS Paramter Block Analysis:");

	offset = 0x00b;
	bps = bytestodec(offset, vbrstring, 2);
	offset = offset+2;
	fprintf(pvbr, "\n 0x00b: Bytes per sector:\t\t%i (%02x %02xh)", bps, vbrstring[0x00c], vbrstring[0x00b]);

	secpclus = bytestodec(offset, vbrstring, 1);
	offset = offset+1;
	fprintf(pvbr, "\n 0x00d: Sectors per cluster:\t\t%i (%02xh)", secpclus, vbrstring[0x00d]);
	fprintf(pvbr, "\n 0x00e: Reserved/Unused");
	fprintf(pvbr, "\n 0x00f: Reserved/Unused");
	fprintf(pvbr, "\n 0x010: No. of FATs (0):\t\t%i (%02xh)", vbrstring[0x010], vbrstring[0x010]);
	offset = 0x011;

	fromconv = bytestodec(offset, vbrstring, 2);
	offset = offset+2;

	fprintf(pvbr, "\n 0x011: Max route dir entries:\t\t%i (%02x %02xh)", fromconv, vbrstring[0x012], vbrstring[0x011]);

	fromconv = bytestodec(offset, vbrstring, 2);
	offset = offset+2;

	fprintf(pvbr, "\n 0x013: FAT small sectors count:\t%i (%02x %02xh)", fromconv, vbrstring[0x014], vbrstring[0x013]);
	fprintf(pvbr, "\n 0x015: Media descriptor ID:\t\t%02x: ", vbrstring[0x15]);
	switch (vbrstring[0x15])
	{
		case 0xfb:
		case 0xfc:
		case 0xfd:
		case 0xfe:
		case 0xff:
		case 0xf9:
		case 0xf0:
			fprintf(pvbr, "Floppy disk");
			break;
		case 0xfa:
			fprintf(pvbr, "RAMdisk");
			break;
		case 0xf8:
			fprintf(pvbr, "Hard disk");
			break;
		default:
			fprintf(pvbr, "Type unknown");
	}
	offset = 0x016;

	fromconv = bytestodec(offset, vbrstring, 2);
	offset = offset+2;
	fprintf(pvbr, "\n 0x016: Sectors per FAT:\t\t%i (%02x %02xh)", fromconv, vbrstring[0x017], vbrstring[0x016]);

	fromconv = bytestodec(offset, vbrstring, 2);
	offset = offset+2;
	fprintf(pvbr, "\n 0x018: Sectors per track:\t\t%i (%02x %02xh)", fromconv, vbrstring[0x019], vbrstring[0x018]);

	fromconv = bytestodec(offset, vbrstring, 2);
	offset = offset+2;
	fprintf(pvbr, "\n 0x01a: Number of heads:\t\t%i (%02x %02xh)", fromconv, vbrstring[0x01b], vbrstring[0x01a]);

	fromconv = bytestodec(offset, vbrstring, 4);
	offset = offset+4;
	fprintf(pvbr, "\n 0x01c: No. hidden sectors:\t\t%i (%02x %02x %02x %02xh)", fromconv, vbrstring[0x01f], vbrstring[0x01e], vbrstring[0x01d], vbrstring[0x01c]);

	fromconv = bytestodec(offset, vbrstring, 4);
	offset = offset+4;
	fprintf(pvbr, "\n 0x020: Total sector in FAT32:\t\t%i (%02x %02x %02x %02xh)", fromconv, vbrstring[0x023], vbrstring[0x022], vbrstring[0x021], vbrstring[0x020]);
	offset = offset + 4;
	fprintf(pvbr, "\n 0x024: Drive information:\t\t%02x %02x %02x %02xh", vbrstring[0x027], vbrstring[0x026], vbrstring[0x025], vbrstring[0x024]);

	fromconv = bytestodec(offset, vbrstring, 8);
	offset = offset+8;
	fprintf(pvbr, "\n 0x028: Total sectors in volume:\t%i (%02x %02x %02x %02x %02x %02x %02x %02xh)", fromconv, vbrstring[0x02f], vbrstring[0x02e], vbrstring[0x02d], vbrstring[0x02c], vbrstring[0x02b], vbrstring[0x02a], vbrstring[0x029], vbrstring[0x028]);

	cMFTdec = bytestodec(offset, vbrstring, 8);
	offset = offset+8;
	lMFTdec = cMFTdec * secpclus;
	fprintf(pvbr, "\n 0x030: $MFT start cluster:\t\t%i (%02x %02x %02x %02x %02x %02x %02x %02xh)", cMFTdec, vbrstring[0x037], vbrstring[0x036], vbrstring[0x035], vbrstring[0x034], vbrstring[0x033], vbrstring[0x032], vbrstring[0x031], vbrstring[0x030]);
	offset = 0x38;

	fromconv = bytestodec(offset, vbrstring, 8);
	offset = offset+8;
	fprintf(pvbr, "\n 0x030: $MFT start cluster mirror:\t%i (%02x %02x %02x %02x %02x %02x %02x %02xh)", fromconv, vbrstring[0x03f], vbrstring[0x03e], vbrstring[0x03d], vbrstring[0x03c], vbrstring[0x03b], vbrstring[0x03a], vbrstring[0x039], vbrstring[0x038]);
	offset = 0x40;

	cfr = bytestodec(offset, vbrstring, 1);
	offset = offset+1;
	if (vbrstring[40] <= 0x7f)
	{
		fprintf(pvbr, "\n 0x040: Clusters per MFT entry:\t\t%i", cfr);
	}else{
		cfr = bytetobit(cfr);
		fprintf(pvbr, "\n 0x040: Bytes per MFT entry:\t\t%i", cfr);
	}
	fprintf(pvbr, "\n 0x041: Reserved/Unused");
	fprintf(pvbr, "\n 0x048: NTFS volume serial number:\t");
	offset = 0x4f;

	fromconv = bytestodec(offset, vbrstring, 8);


	//do dis shit right here

	offset = offset+8;
	offset = 0x50;
	fprintf(pvbr, "\n 0x041: Reserved/Unused\n\n\n");


	return lMFTdec;
}
