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

unsigned short makepixel(ubyte r, ubyte g, ubyte b);

typedef struct _Point {
        int x;
        int y;
}Point;


void setCalibration();

void clearLcd();

void makeLineBox(Point start, Point end, unsigned short color);

void makeLineOval(Point start, Point end, unsigned short color){
        int i, j, tmp, x_move, y_move;
        
	if(start.x > end.x){ 
	 tmp = start.x;
	 start.y = end.y;
	 end.y = tmp;
	}

	if(start.y > end.y){
	 tmp = start.y;
	 start.y = end.y;
	 end.y = tmp;
	}

	x_move = (start.x + end.x) / 2; // 타원의 중심 x값
	y_move = (start.y + end.y) / 2; // 타원의 중심 y값

	/*현재 문제점 : 타원이 아니라 사각형이 구현됨
	                몇번 실행하다 보면 x,y축으로 하나 혹은 둘 다 긴 직선이 그려짐
					도형을 터치로 잡아당기면서 원래 있던 도형이 사라지는 것은 아직 구현 x*/

	for(i = start.x; i < end.x; i++){
	 int m = i - x_move; // 타원의 중심이 (0,0)에 있었다고 가정하고 현재 위치한 타원이 옮겨진 x의 값만큼 다시 감소시킴
	 int w = (end.x - start.x) / 2; // 타원의 세로길이의 1/2
	 for(j = start.y; j < end.y; j++){
	  int b = j - y_move; // 타원의 중심이 (0,0)에 있었다고 가정하고 현재 위치한 타원이 옮겨진 y의 값만큼 다시 감소시킴
	  int n = (end.y - start.y) / 2; //  타원의 가로길이의 1/2
	  if ((w != 0) && (h != 0)) { // w,h 둘 중 하나라도 0일 경우 floating point exception이 일어나므로 조건 추가
		  if ((((m * m) / (w * w)) + ((b * b) / (n * n))) == 1) { // 타원 방정식 (((x - m)^2)/(a^2)) + (((y - n)^2)/(b^2)) = 1을 이용해서
			  offset = j * fbvar.xres + i;                     // x,y 값이 위 방정식의 조건에 부합하면 픽셀을 그리도록 구현중입니다.
			  *(pfbdata + offset) = color;
		  }
	  }
	 } // j의 for 반복문 끝
	} // i의 for 반복문 끝
}
