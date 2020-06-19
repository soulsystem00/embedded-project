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
	
	unsigned short Color = red; //default color = red, 컬러 속성을 저장하는 값.


        int pressure = -1;
        int ret, t, i, j;
        Point get, start, end;

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
 while(1){
        read(fd, &ie, sizeof(struct input_event));
        if (ie.type == 3) {
                if (ie.code == 0) {
                        get.x = ie.value;
                }
                else if (ie.code == 1) {
                        get.y = ie.value;
                }
                else if (ie.code == 24) {
                        pressure = ie.value;
                        if (pressure==0) {
                                //printf("start\n");
                                start.x = a*get.x+b*get.y+c;
                                start.y = d*get.x+e*get.y+f;
                                break;
                        }
                }
        }
        }
while(1){
        read(fd, &ie, sizeof(struct input_event));
        if (ie.type == 3) {
                if (ie.code == 0) {
                        get.x = ie.value;
                }
                else if (ie.code == 1) {
                        get.y = ie.value;
                }
                else if (ie.code == 24) {
                        pressure = ie.value;
                        if (pressure==0) {
                                //printf("start\n");
                                end.x = a*get.x+b*get.y+c;
                                end.y = d*get.x+e*get.y+f;
                                break;
                        }
                }
        }
        }

        makeLineBox(start, end, red);

        close(fd);
        close(fbfd);
        return 0;

} // end of main
