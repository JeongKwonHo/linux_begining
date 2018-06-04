#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>

#define FBDEVICE	"/dev/fb0"

int main(int argc, char** argv)
{
	int fbfd = 0;

	struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;
	
	fbfd = open(FBDEVICE, O_RDWR);
	
	printf("The framebuffer device was opened successfully.\n");

	ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo);

	ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo);
	
	vinfo.bits_per_pixel = 16;

	printf("Resolution : %dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);
	printf("Virtual Resolution : %dx%d\n", vinfo.xres_virtual, vinfo.yres_virtual);
	printf("Length of frame buffer memory : %d\n", finfo.smem_len);

	close(fbfd);
	



	return 0;
}
