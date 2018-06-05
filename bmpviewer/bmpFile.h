#ifndef __BMP_FILE_H__
#define __BMP_FILE_H__

#define COLS /*********/
#define ROWS /*********/

typedef struct __attribute__((__packed__)){//tagBITMAPFILEHEADER{
	unsigned short bfType; /*********/
	unsigned int bfSize;
	unsigned short bfReserved1;
	unsigned short bfReserved2;
	unsigned int bfOffBits;
}BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER{
	unsigned int biSize;
	unsigned int biWidth;
	unsigned int biHeight;
	unsigned short biPlanes;
	unsigned short biBitCount;
	unsigned int biCompression;
	unsigned int SizeImage;
	unsigned int biXPelsPerMeter;
	unsigned int biYPelsPerMeter;
	unsigned int biClrUsed;
	unsigned int biClrImportant;
} BITMAPINFOHEADER;

//typedef struct tagBITMAPINFO{
//	BITMAPINFOHEADER bmiHeader;
//	RGBQUAD bmicolor[1];
//}BITMAPINFOHEADER;


typedef struct tagRGBQUAD{
	unsigned char rgbBlue;
	unsigned char rgbGreen;
	unsigned char rgbRed;
	unsigned char rgbReserved;
}RGBQUAD;

#endif			/* __BMP_FILE_H__ */
