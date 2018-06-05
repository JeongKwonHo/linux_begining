#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <linux/fb.h>

#include "bmpFile.h"

#define FBDEVFILE "/*********/"

void read_bmp(char *filename, char ** pDib, char **data, int *cols, int *rows)
{
	BITMAPFILEHEADER bmpHeader;
	BITMAPINFOHEADER bmpInfoHeader;
	unsigned int size;
	unsigned char ID[2];
	int nread;
	FILE *fp;
	
	printf("File Name : %s\n", filename);
	fp = fopen(filename,"rb");
	if(fp==NULL){
		printf("ERROR\n");
		return;
	}
 
	////////////////////////// BITMAPFILEHEADER
	fread(&bmpHeader.bfType, sizeof(unsigned short int), 1, fp);
	fread(&bmpHeader.bfSize, sizeof(unsigned int), 1, fp);
	fread(&bmpHeader.bfReserved1, sizeof(unsigned short int), 1, fp);
	fread(&bmpHeader.bfReserved2, sizeof(unsigned short int), 1, fp);
	fread(&bmpHeader.bfOffBits, sizeof(unsigned int), 1, fp);

      	/////////////////////////// BITMAPINFOHEADER
	fread(&bmpInfoHeader.biSize, sizeof(unsigned int), 1, fp);
	fread(&bmpInfoHeader.biWidth, sizeof(unsigned int), 1, fp);
	fread(&bmpInfoHeader.biHeight, sizeof(unsigned int), 1, fp);
	fread(&bmpInfoHeader.biPlanes, sizeof(unsigned short int), 1, fp);
	fread(&bmpInfoHeader.biBitCount, sizeof(unsigned short int), 1, fp);
	fread(&bmpInfoHeader.biCompression, sizeof(unsigned int), 1, fp);
	fread(&bmpInfoHeader.SizeImage, sizeof(unsigned int), 1, fp);
	fread(&bmpInfoHeader.biXpelsPerMeter, sizeof(unsigned int), 1, fp);
	fread(&bmpInfoHeader.biYpelsPerMeter, sizeof(unsigned int), 1, fp);
	fread(&bmpInfoHeader.biClrUsed, sizeof(unsigned int), 1, fp);
	fread(&bmpInfoHeader.biClrImportant, sizeof(unsigned int), 1, fp);

	if(24 != bmpInfoHeader.biBitCount)
	{
		printf("It supports only 24bit bmp!\n");
		fclose(fp);
		return;
	}

	*cols = bmpInfoHeader.biWidth;
	*rows = bmpInfoHeader.biHeight;

	printf("Width : %d\n", bmpInfoHeader.biWidth);
	printf("Height : %d\n", bmpInfoHeader.biHeight);
	printf("Bit Count : %d\n", bmpInfoHeader.biBitCount);

	size = bmpHeader.bfSize - (sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER));
	*pDib = (unsigned char *)malloc(size);
	fread(*pDib, 1, size, fp);

//	bmpInfoHeader = (BITMAPINFOHEADER*)*pDib;
	
	*data = (char*)(*pDib + bmpHeader.bfOffBits - \
			(sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)) -1);

	fclose(fp);
}

int main(int argc, char *argv[])
{
	int cols, rows;
	char *pData, *data;
	char r,g,b;
	unsigned short bmpdata1[1024*768];
	unsigned short pixel;
	unsigned short *pfbmap;
	struct fb_var_screeninfo fbvar;
	int fbfd;
	int i,j,k,t;

	if(argc!=2){
		printf("\nUsage: /drawbmp  xxx.bmp\n");
		return 0;
	}

	fbfd = open(FBDEVFILE, O_RDWR);
	
	if(fbfd<0){
		perror("fbdev open");
		exit(1);
	}

	pfbmap=(unsigned short *)
		mmap(0, COLS*ROWS*2, PROT_READ|PROT_WRITE, MAP_SHARED, fbfd, 0);

	if((unsigned)pfbmap ==(unsigned)-1)
	{
		perror("fbdev mmap");
		exit(1);
	}

	read_bmp(argv[1], &pData, &data, &cols, &rows);

	for(j=0; j<rows;j++) {
		k=j*cols*3;
		t=(rows-1-j)*cols;
		for(i=0; i<cols;i++) {
			b=*(data + (k + i*3));
			g=*(data + (k + i*3+1));
			r=*(data + (k + i*3+2));
			pixel = ((r>>3)<<11)|((g>>2)<<5)|(b>>3);
			bmpdata1[t+i] = pixel;
		}
	}

	memcpy(pfbmap, bmpdata1, COLS*ROWS*2);
	munmap(pfbmap, COLS*ROWS*2);

	free(pData);
	close(fbfd);
	return 0;
}
	
