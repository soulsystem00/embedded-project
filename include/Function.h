typedef struct _Point {
        int x;
        int y;
}Point;

typedef unsigned char ubyte;

unsigned short makepixel(ubyte r, ubyte g, ubyte b);

void setCalibration();

void clearLcd();

void makeLineBox(Point start, Point end, unsigned short color);
void makeLineBox2(int sx,int sy, int ex,int ey, unsigned short color);

void PrintScreen(int Screen[], int size);
void FillinitColor();
int setMod(Point touch);

void Line(unsigned short CurrentColor);
void Rectangle(unsigned short CurrentColor);
void Oval(unsigned short CurrentColor);
void FreeDraw(unsigned short color);
void Selete();
void Erase();
void clearDraw();

void Fill(unsigned short CurrentColor);
void FillFunction(int x, int y,unsigned short CurrentColor);

void PrintAry(unsigned short Screen[194][233]);
void SaveAry(unsigned short Screen[194][233]);
void PrintDrawArea();
