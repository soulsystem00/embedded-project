#include "../include/Header.h"
extern int Screen[76800];


int main(void) {
	white = makepixel(255,255,255);
	orange = makepixel(255,128,64);
	red = makepixel(255,0,0);
	green = makepixel(0,255,0);
	yellow = makepixel(255,255,0);
	darkblue = makepixel(0,0,128);
	blue = makepixel(0,0,255);
	black = makepixel(0,0,0);


        pressure = -1;
	ShapeMod = 0;
	ColorMod = 0;
	DrawMod = 0;
        int x1, x2, y1, y2;
        fbfd = open(FBDEVFILE, O_RDWR);

        if (fbfd < 0) {
                perror("fbdev open");
                exit(1);
        }
        ret = ioctl(fbfd, FBIOGET_VSCREENINFO, &fbvar);
        if (ret < 0) {
                perror("fbdev ioctl");
                exit(1);
        }
        if (fbvar.bits_per_pixel != 16) {
                fprintf(stderr, "bpp is not 16\n");
                exit(1);
        }

        pfbdata = (unsigned short *)mmap(0, fbvar.xres*fbvar.yres * 16 / 8, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
        if ((unsigned)pfbdata == (unsigned)-1) {
                perror("fbdev mmap");
                exit(1);
        }

        fd = open("/dev/input/event4", O_RDONLY);
        if (fd < 0) return -1;

        printf("start\n");
        clearLcd();
        setCalibration();
        clearLcd();
	PrintScreen(Screen,sizeof(Screen)/sizeof(int));
 	while(1)
	{
		FreeDraw(black);
        }
        


        close(fd);
        close(fbfd);
        return 0;

} // end of main
