Wan-Shi-Tong
===

He who knows ten-thousand things

## History

This is in fact one of the first C programs I ever began to write, this was for an academic project for which the focus shifted part way through and as yet it is not wholly complete. I do intend to complete it, but perhaps when there is nothing I can find to do and I feel the need to return to C.

## Function

The overall purpose of WST was forensic analysis of drive images containing NTFS partitions almost like a terrible FTK rip off at the command line.

- It would take a path to an image file for a drive (i.e. not a volume)
- It would then analyse the MBR and print out some information including partition information and a hex output of the 512 bytes.
- A choice of which partition to follow would be given
- The VBR of the partition would then be analysed in a similar fashion as the MBR (the program would gather the starting point of the MFT at this time)
- The metafiles of the MFT would then be analysed and optionally printed
- INDX files would then be analysed and the files of the system indexed and printed, the MFT record of a chosen could then be viewed and the contents of the file written to a .bin

## Compilation

WST does currently compile despite its incomplete state, fairly simply:

```batch
git clone https://github.com/BodneyC/Wan-Shi-Tong.git
cd Wan-Shi-Tong
gcc -c *.c
gcc *.o -o WST.exe
```

And it does work as far as the INDX records, after this the program just finishes.
