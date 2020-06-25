float a, b, c, d, e, f, k;
unsigned short *pfbdata;
int rpixel;
int offset;
int fd, fbfd;
struct fb_var_screeninfo fbvar;
struct input_event ie;

unsigned short white;
unsigned short orange;
unsigned short red;
unsigned short green;
unsigned short yellow;
unsigned short darkblue;
unsigned short blue;
unsigned short black;
unsigned short purple;
unsigned short CurrentColor;
unsigned short tmpColor;
unsigned short NULLcolor;

int ShapeMod;
int ColorMod;
int DrawMod;

unsigned short DrawArea[194][233];

int pressure;
int ret, t, i, j;
Point get, start, end;
