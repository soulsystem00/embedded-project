#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h> /* for ioctl */
#include <sys/mman.h> /* for mmap */
#include <sys/types.h>
#include <linux/fb.h> /* for fb_var_screeninfo, FBIOGET_VSCREENINFO */
#include <linux/input.h>

#define FBDEVFILE "/dev/fb2"

float a, b, c, d, e, f, k;
unsigned short *pfbdata;
int rpixel;
int offset;
int fd, fbfd;
struct fb_var_screeninfo fbvar;
struct input_event ie;
typedef unsigned char ubyte;

unsigned short makepixel(ubyte r, ubyte g, ubyte b) {
        return (unsigned short)(((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3));
}
typedef struct _Point {
        int x;
        int y;
}Point;

void setCalibration() {
        int i, j, tt, pressure;
        int x[3], y[3], xd[3] = { 50, 150, 300 }, yd[3] = { 100, 50, 200 };
        unsigned short red = makepixel(255, 0, 0);

	        for (tt = 0; tt < 3; tt++) {
        	   for (i = -5; i < 5; i++) {
                        offset = (yd[tt] + i) * fbvar.xres + xd[tt];
                        *(pfbdata + offset) = red;
                        offset = yd[tt] * fbvar.xres + xd[tt] + i;
                        *(pfbdata + offset) = red;
                }
                while (1) {
                        read(fd, &ie, sizeof(struct input_event));
                        if (ie.type == 3) {
                                if (ie.code == 0) x[tt] = ie.value;
                                else if (ie.code == 1) y[tt] = ie.value;
                                else if (ie.code == 24) pressure = ie.value;
                                if (pressure == 0)break;
                        }
                }
                //printf("%d %d\n%d %d\n", xd[tt], yd[tt], x[tt], y[tt]);
                pressure = -1;
        }

        k = ((x[0] - x[2])*(y[1] - y[2])) - ((x[1] - x[2])*(y[0] - y[2]));
        a = ((xd[0] - xd[2])*(y[1] - y[2])) - ((xd[1] - xd[2])*(y[0] - y[2]));
        b = ((x[0] - x[2])*(xd[1] - xd[2])) - ((xd[0] - xd[2])*(x[1] - x[2]));
        c = (y[0] * ((x[2] * xd[1]) - (x[1] * xd[2]))) + (y[1] * ((x[0] * xd[2])- (x[2] * xd[0]))) + (y[2] * ((x[1] * xd[0]) - (x[0] * xd[1])));
        d = ((yd[0] - yd[2])*(y[1] - y[2])) - ((yd[1] - yd[2])*(y[0] - y[2]));
        e = ((x[0] - x[2])*(yd[1] - yd[2])) - ((yd[0] - yd[2])*(x[1] - x[2]));
        f = (y[0] * ((x[2] * yd[1]) - (x[1] * yd[2]))) + (y[1] * ((x[0] * yd[2])- (x[2] * yd[0]))) + (y[2] * ((x[1] * yd[0]) - (x[0] * yd[1])));
        a = a / k; b = b / k; c = c / k;
        d = d / k; e = e / k; f = f / k;
}

void clearLcd() {
        int i, j;
        for (i = 0; i < 240; i++) {
                for (j = 0; j < 320; j++) {
                        offset = i * 320 + j;
                        *(pfbdata + offset) = 0;
                }
        }
}

void makeLineOval(unsigned short color){
        int i, j, tmp;
	int cnt;
 
	int x, y, a_2, b_2, d1, a1, b1, mx, my, x_move, y_move;
	int chk = 0;
	int pressure = -1;
        Point get, start, end;

	while (chk != 0)
	{
		read(fd, &ie, sizeof(struct input_event));
		if (ie.type == 3) {
			if (ie.code == 0) { get.x = ie.value; }
			else if (ie.code == 1) {get.y = ie.value; }
			else if (ie.code == 24)
			{
				pressure = ie.value;
				start.x = a * get.x + b * get.y + c;
				start.y = d * get.x + e * get.y + f;
				chk = 1;
			}
		} // end of setting start
	} // end of start

	while(pressure != 0)
	{
		read(fd, &ie, sizeof(struct input_event));
		if (ie.type == 3)
		{
			if (ie.code == 0) { get.x = ie.value; }
			else if (ie.code == 1) { get.y = ie.value; }
			else if (ie.code == 24)
			{
				end.x = a * get.x + b * get.y + c;
				end.y = d * get.x + e * get.y + f;
				pressure = ie.value;
			 // end of setting end
			
			if(start.x > end.x)
			{
				tmp = start.x;
				start.x = end.x;
				end.x = tmp;
			} // swap of x

			if(start.y > end.y)
			{
				tmp = start.y;
				start.y = end.y;
				end.y = tmp;
			} // swap of y

			x_move = (start.x + end.x)/2;
			y_move = (start.y + end.y)/2;
			
			a1 = end.x - x_move;
			b1 = end.y - y_move;
			x = 0;
			y = b1;
			a_2 = a1 * a1;
			b_2 = b1 * b1;
			d1 = (4 * b_2 + (a_2 * (1 - (4 * b1)))) / 4;

			offset = (y + y_move) * 320 + (x + x_move);  
			*(pfbdata + offset) = color;
			offset = (y + y_move) * 320 + ((-1)*x + x_move);  
			*(pfbdata + offset) = color;
			offset = ((-1)*y + y_move) * 320 + (x + x_move);  
			*(pfbdata + offset) = color;
			offset = ((-1)*y + y_move) * 320 + ((-1)*x + x_move);  
			*(pfbdata + offset) = color; // draw starting point x

			while(b_2 * x < a_2 * y)
			{
				++x;
				if(d1 < 0)
				{
					d1 += b_2 *((2 * x) + 1);	
 				}
				else
				{
					--y;
					d1 += b_2 * ((2 * x) + 1) - (2 * a_2 * y);
				}
					
				offset = (y + y_move) * 320 + (x + x_move);
                        	*(pfbdata + offset) = color;
                        	offset = (y + y_move) * 320 + ((-1)*x + x_move);
                        	*(pfbdata + offset) = color;
                        	offset = ((-1)*y + y_move) * 320 + (x + x_move);
                        	*(pfbdata + offset) = color;
                       		offset = ((-1)*y + y_move) * 320 + ((-1)*x + x_move);
                        	*(pfbdata + offset) = color;
			} // end of x draw */

			x = a1;
			y = 0;
			d1 = ((4 * a_2) + b_2 * (1 - 4 * a1)) / 4;

			offset = (y + y_move) * 320 + (x + x_move);
                        *(pfbdata + offset) = color;
                        offset = (y + y_move) * 320 + ((-1)*x + x_move);
                        *(pfbdata + offset) = color;
                        offset = ((-1)*y + y_move) * 320 + (x + x_move);
                        *(pfbdata + offset) = color;
                        offset = ((-1)*y + y_move) * 320 + ((-1)*x + x_move);
                        *(pfbdata + offset) = color;

			while(b_2 * x > a_2 * y)
			{
				++y;
				if (d1 < 0)
				{
					d1 += a_2 * ((2 * y) + 1);
				}
				else
				{
					--x;
					d1 += ((-2) * b_2 * x) + (a_2*(2 * y + 1));
				}
					
				offset = (y + y_move) * 320 + (x + x_move);
                        	*(pfbdata + offset) = color;
                        	offset = (y + y_move) * 320 + ((-1)*x + x_move);
                        	*(pfbdata + offset) = color;
                        	offset = ((-1)*y + y_move) * 320 + (x + x_move);
                        	*(pfbdata + offset) = color;
                        	offset = ((-1)*y + y_move) * 320 + ((-1)*x + x_move);
                        	*(pfbdata + offset) = color;
				break;
			} // end of y draw
		}
		} // end of if
	} // end of while	
} // end of Oval

int main(void) {
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
        
		unsigned short red = makepixel(255, 0, 0);
			makeLineOval(red);
		
		
		close(fd);
        close(fbfd);
        return 0;

} // end of main

