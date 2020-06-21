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
unsigned short CurrentColor;
unsigned short tmpColor;
int ShapeMod;
int ColorMod;
int DrawMod;

//int _Line[1584];
//int _Rectangle[1584];
typedef struct _Point {
        int x;
        int y;
}Point;

int pressure;
int ret, t, i, j;
Point get, start, end;

void setCalibration();

void clearLcd();

void makeLineBox(Point start, Point end, unsigned short color);
void makeLineBox2(int sx,int sy, int ex,int ey, unsigned short color);

void PrintScreen(int Screen[], int size);
void FreeDraw(unsigned short color);
void clearDraw();
int setMod(Point touch);
void FillinitColor();
void Line(unsigned short CurrentColor);
void Rectangle(unsigned short CurrentColor);
void Oval(unsigned short CurrentColor);
void Selete();
void Erase();

void Fill(unsigned short CurrentColor);
void FillFunction(int x, int y,unsigned short CurrentColor);
