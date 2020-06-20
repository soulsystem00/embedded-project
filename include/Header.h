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

unsigned short white;
unsigned short orange;
unsigned short red;
unsigned short green;
unsigned short yellow;
unsigned short darkblue;
unsigned short blue;
unsigned short black;

//int _Line[1584];
//int _Rectangle[1584];
typedef struct _Point {
        int x;
        int y;
}Point;


void setCalibration();

void clearLcd();

void makeLineBox(Point start, Point end, unsigned short color);

void PrintScreen(int Screen[], int size);

void FillScreen(unsigned short color); //대충은 구현, 그림판에 해당 color로 전부 칠하는 함수 (clearLcd() 기능과 비슷합니다.)

void SetColor(unsigned short color, int PointX, int PointY); //미완성, 현재 그림판의 color를 설정하는 함수

void Line(unsigned short color); //미완성, 직선을 긋는 함수

void FreeDraw(unsigned short color); //자유롭게 그림을 그리는 함수

