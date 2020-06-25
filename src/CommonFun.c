#include "../include/Header.h"
#include "../include/Function.h"
#include "../include/Var.h"

unsigned short makepixel(ubyte r, ubyte g, ubyte b) {
	return (unsigned short)(((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3));
}

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
	k = ((x[0] - x[2]) * (y[1] - y[2])) - ((x[1] - x[2]) * (y[0] - y[2]));
	a = ((xd[0] - xd[2]) * (y[1] - y[2])) - ((xd[1] - xd[2]) * (y[0] - y[2]));
	b = ((x[0] - x[2]) * (xd[1] - xd[2])) - ((xd[0] - xd[2]) * (x[1] - x[2]));
	c = (y[0] * ((x[2] * xd[1]) - (x[1] * xd[2]))) + (y[1] * ((x[0] * xd[2]) - (x[2] * xd[0]))) + (y[2] * ((x[1] * xd[0]) - (x[0] * xd[1])));
	d = ((yd[0] - yd[2]) * (y[1] - y[2])) - ((yd[1] - yd[2]) * (y[0] - y[2]));
	e = ((x[0] - x[2]) * (yd[1] - yd[2])) - ((yd[0] - yd[2]) * (x[1] - x[2]));
	f = (y[0] * ((x[2] * yd[1]) - (x[1] * yd[2]))) + (y[1] * ((x[0] * yd[2]) - (x[2] * yd[0]))) + (y[2] * ((x[1] * yd[0]) - (x[0] * yd[1])));
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



void PrintScreen(int Screen[], int size)
{
	int i, j;
	int count = 0;
	unsigned short red = makepixel(255, 255, 255);
	for (i = 0; i < 240; i++)
	{
		for (j = 0; j < 320; j++)
		{
			if (Screen[count] == 0)
			{
				offset = i * 320 + j;
				*(pfbdata + offset) = red;
			}
			count++;
		}
	}
}

void FillinitColor() {
	int i, j;
	int x = 278;
	int y = 27;
	for (i = 0; i <= 16; i++) {
		for (j = 0; j <= 16; j++) {
			offset = (y + i) * 320 + (x + j);
			*(pfbdata + offset) = white;
		}
	}
	y = y + 21;
	for (i = 0; i <= 16; i++) {
		for (j = 0; j <= 16; j++) {
			offset = (y + i) * 320 + (x + j);
			*(pfbdata + offset) = red;
		}
	}
	y = y + 20;
	for (i = 0; i <= 16; i++) {
		for (j = 0; j <= 16; j++) {
			offset = (y + i) * 320 + (x + j);
			*(pfbdata + offset) = yellow;
		}
	}
	y = y + 20;
	for (i = 0; i <= 16; i++) {
		for (j = 0; j <= 16; j++) {
			offset = (y + i) * 320 + (x + j);
			*(pfbdata + offset) = blue;
		}
	}
	x = 298;
	y = 27;
	for (i = 0; i <= 16; i++) {
		for (j = 0; j <= 16; j++) {
			offset = (y + i) * 320 + (x + j);
			*(pfbdata + offset) = orange;
		}
	}
	y = y + 21;
	for (i = 0; i <= 16; i++) {
		for (j = 0; j <= 16; j++) {
			offset = (y + i) * 320 + (x + j);
			*(pfbdata + offset) = green;
		}
	}
	y = y + 20;
	for (i = 0; i <= 16; i++) {
		for (j = 0; j <= 16; j++) {
			offset = (y + i) * 320 + (x + j);
			*(pfbdata + offset) = darkblue;
		}
	}
	y = y + 20;
	for (i = 0; i <= 16; i++) {
		for (j = 0; j <= 16; j++) {
			offset = (y + i) * 320 + (x + j);
			*(pfbdata + offset) = black;
		}
	}
}
int setMod(Point touch)
{
	Point ShapeStart;
	Point ShapeEnd;

	Point ColorStart;
	Point ColorEnd;

	Point DrawStart;
	Point DrawEnd;

	if ((touch.x >= 4 && touch.x <= 75) && (touch.y >= 34 && touch.y <= 55)) { printf("sets mod Line\n");	 ShapeMod = 0; DrawMod = 0; }
	else if ((touch.x >= 4 && touch.x <= 75) && (touch.y >= 58 && touch.y <= 79)) { printf("sets mod Rectangle\n");  ShapeMod = 1; DrawMod = 0; }
	else if ((touch.x >= 4 && touch.x <= 75) && (touch.y >= 82 && touch.y <= 103)) { printf("sets mod Oval\n");	 ShapeMod = 2; DrawMod = 0; }
	else if ((touch.x >= 4 && touch.x <= 75) && (touch.y >= 106 && touch.y <= 127)) { printf("sets mod FreeDraw\n");   ShapeMod = 3; DrawMod = 0; }
	else if ((touch.x >= 4 && touch.x <= 75) && (touch.y >= 130 && touch.y <= 151)) { printf("sets mod Selete\n");	 ShapeMod = 4; DrawMod = 0; }
	else if ((touch.x >= 4 && touch.x <= 75) && (touch.y >= 154 && touch.y <= 175)) { printf("sets mod Erase\n");	 ShapeMod = 5; DrawMod = 0; }
	else if ((touch.x >= 4 && touch.x <= 75) && (touch.y >= 178 && touch.y <= 199)) { printf("sets mod Clear\n");	 ShapeMod = 6; DrawMod = 0; }

	else if ((touch.x >= 277 && touch.x <= 295) && (touch.y >= 26 && touch.y <= 44)) { printf("sets color white\n");   CurrentColor = white; }
	else if ((touch.x >= 297 && touch.x <= 315) && (touch.y >= 26 && touch.y <= 44)) { printf("sets color orange\n");  CurrentColor = orange; }
	else if ((touch.x >= 277 && touch.x <= 295) && (touch.y >= 47 && touch.y <= 65)) { printf("sets color red\n");     CurrentColor = red; }
	else if ((touch.x >= 297 && touch.x <= 315) && (touch.y >= 47 && touch.y <= 65)) { printf("sets color green\n");   CurrentColor = green; }
	else if ((touch.x >= 277 && touch.x <= 295) && (touch.y >= 67 && touch.y <= 85)) { printf("sets color yellow\n");  CurrentColor = yellow; }
	else if ((touch.x >= 297 && touch.x <= 315) && (touch.y >= 67 && touch.y <= 85)) { printf("sets color darkblue\n"); CurrentColor = darkblue; }
	else if ((touch.x >= 277 && touch.x <= 295) && (touch.y >= 87 && touch.y <= 105)) { printf("sets color blue\n");	  CurrentColor = blue; }
	else if ((touch.x >= 297 && touch.x <= 315) && (touch.y >= 87 && touch.y <= 105)) { printf("sets color black\n");   CurrentColor = black; }

	else if ((touch.x >= 280 && touch.x <= 314) && (touch.y >= 154 && touch.y <= 175)) { printf("sets draw mod PEN\n"); DrawMod = 0; }
	else if ((touch.x >= 280 && touch.x <= 314) && (touch.y >= 178 && touch.y <= 199)) { printf("sets draw mod Fill\n"); DrawMod = 1; }

	else if ((touch.x >= 80 && touch.x <= 273) && (touch.y >= 4 && touch.y <= 236)) { printf("selected Draw Area\n"); makeLineBox2(79, 3, 274, 237, red); return 1; }

	if (ShapeMod == 0)
	{
		ShapeStart.x = 4; ShapeStart.y = 34; ShapeEnd.x = 75; ShapeEnd.y = 55;
	}
	else if (ShapeMod == 1)
	{
		ShapeStart.x = 4; ShapeStart.y = 58; ShapeEnd.x = 75; ShapeEnd.y = 79;
	}
	else if (ShapeMod == 2)
	{
		ShapeStart.x = 4; ShapeStart.y = 82; ShapeEnd.x = 75; ShapeEnd.y = 103;
	}
	else if (ShapeMod == 3)
	{
		ShapeStart.x = 4; ShapeStart.y = 106; ShapeEnd.x = 75; ShapeEnd.y = 127;
	}
	else if (ShapeMod == 4)
	{
		ShapeStart.x = 4; ShapeStart.y = 130; ShapeEnd.x = 75; ShapeEnd.y = 151;
	}
	else if (ShapeMod == 5)
	{
		ShapeStart.x = 4; ShapeStart.y = 154; ShapeEnd.x = 75; ShapeEnd.y = 175;
	}
	else if (ShapeMod == 6)
	{
		ShapeStart.x = 4; ShapeStart.y = 178; ShapeEnd.x = 75; ShapeEnd.y = 199;
	}

	if (CurrentColor == white)
	{
		ColorStart.x = 277; ColorStart.y = 26; ColorEnd.x = 295; ColorEnd.y = 44;
	}
	else if (CurrentColor == orange)
	{
		ColorStart.x = 297; ColorStart.y = 26; ColorEnd.x = 315; ColorEnd.y = 44;
	}
	else if (CurrentColor == red)
	{
		ColorStart.x = 277; ColorStart.y = 47; ColorEnd.x = 295; ColorEnd.y = 65;
	}
	else if (CurrentColor == green)
	{
		ColorStart.x = 297; ColorStart.y = 47; ColorEnd.x = 315; ColorEnd.y = 65;
	}
	else if (CurrentColor == yellow)
	{
		ColorStart.x = 277; ColorStart.y = 67; ColorEnd.x = 295; ColorEnd.y = 85;
	}
	else if (CurrentColor == darkblue)
	{
		ColorStart.x = 297; ColorStart.y = 67; ColorEnd.x = 315; ColorEnd.y = 85;
	}
	else if (CurrentColor == blue)
	{
		ColorStart.x = 277; ColorStart.y = 87; ColorEnd.x = 295; ColorEnd.y = 105;
	}
	else if (CurrentColor == black)
	{
		ColorStart.x = 297; ColorStart.y = 87; ColorEnd.x = 315; ColorEnd.y = 105;
	}

	if (DrawMod == 0)
	{
		DrawStart.x = 280; DrawStart.y = 154; DrawEnd.x = 314; DrawEnd.y = 174;
	}
	else if (DrawMod == 1)
	{
		DrawStart.x = 280; DrawStart.y = 178; DrawEnd.x = 314; DrawEnd.y = 199;
	}

	makeLineBox2(4, 34, 75, 55, black);
	makeLineBox2(4, 58, 75, 79, black);
	makeLineBox2(4, 82, 75, 103, black);
	makeLineBox2(4, 106, 75, 127, black);
	makeLineBox2(4, 130, 75, 151, black);
	makeLineBox2(4, 154, 75, 175, black);
	makeLineBox2(4, 178, 75, 199, black);

	makeLineBox2(277, 26, 295, 44, black);
	makeLineBox2(297, 26, 315, 44, black);
	makeLineBox2(277, 47, 295, 65, black);
	makeLineBox2(297, 47, 315, 65, black);
	makeLineBox2(277, 67, 295, 85, black);
	makeLineBox2(297, 67, 315, 85, black);
	makeLineBox2(277, 87, 295, 105, black);
	makeLineBox2(297, 87, 315, 105, black);

	makeLineBox2(280, 154, 314, 174, black);
	makeLineBox2(280, 178, 314, 199, black);

	makeLineBox(ShapeStart, ShapeEnd, red);
	makeLineBox(ColorStart, ColorEnd, red);
	makeLineBox(DrawStart, DrawEnd, red);

	return 0;
}

void Fill(unsigned short CurrentColor)
{
	printf("start Fill\n");
	read(fd, &ie, sizeof(struct input_event));

	if (ie.type == 3)
	{
		if (ie.code == 0) get.x = ie.value;
		if (ie.code == 1) get.y = ie.value;
		if (ie.code == 24)
		{
			pressure = ie.value;
			if ((pressure == 0))
			{
				start.x = a * get.x + b * get.y + c;
				start.y = d * get.x + e * get.y + f;
				printf("%d %d\n", start.x, start.y);
				if ((start.x >= 80 && start.x <= 273) && (start.y >= 4 && start.y <= 236))
				{
					offset = start.y * 320 + start.x;
					tmpColor = *(pfbdata + offset);
					printf("%u\n", tmpColor);
					printf("Fill Function start\n");
					FillFunction(start.x, start.y, CurrentColor);
					printf("Fill Function inininin\n");
					PrintDrawArea();
				}
				else
				{
					return;
				}
			}

		}
	}
}

void FillFunction(int x, int y, unsigned short CurrentColor)
{

	if (((x >= 80 && x <= 273) && (y >= 4 && y <= 236)) && (tmpColor != CurrentColor))
	{
		if (DrawArea[x - 80][y - 4] == tmpColor)
		{

			DrawArea[x - 80][y - 4] = CurrentColor;
			FillFunction(x, y + 1, CurrentColor);
			FillFunction(x, y - 1, CurrentColor);
			FillFunction(x + 1, y, CurrentColor);
			FillFunction(x - 1, y, CurrentColor);
		}
		else return;
	}
	else return;


}









void PrintAry(unsigned short Screen[194][233])
{
	int i, j;
	int count = 0;

	for (i = 0; i < 233; i++)
	{
		for (j = 0; j < 194; j++)
		{
			offset = (i + 4) * 320 + (j + 80);
			*(pfbdata + offset) = white;
		}
	} // clear draw area

	for (i = 0; i < 233; i++)
	{
		for (j = 0; j < 194; j++)
		{
			offset = (i + 4) * 320 + (j + 80);
			*(pfbdata + offset) = DrawArea[j][i];
		}
	} // Draw Saved Pixel

	for (i = 0; i < 233; i++)
	{
		for (j = 0; j < 194; j++)
		{
			offset = (i + 4) * 320 + (j + 80);
			if (Screen[j][i] != NULLcolor)
				*(pfbdata + offset) = Screen[j][i];
		}
	} // Draw tmp pixel
}

void SaveAry(unsigned short Screen[194][233])
{
	int i, j;

	for (i = 0; i < 233; i++)
	{
		for (j = 0; j < 194; j++)
		{
			if (DrawArea[j][i] != Screen[j][i] && Screen[j][i] != NULLcolor)
				DrawArea[j][i] = Screen[j][i];
			/*else if(DrawArea[j][i] != Screen[j][i] && Screen[j][i] == NULL)
				DrawArea[j][i] = Screen[j][i];*/

		}
	}
}

void PrintDrawArea()
{
	int i, j;
	for (i = 0; i < 233; i++)
	{
		for (j = 0; j < 194; j++)
		{
			offset = (i + 4) * 320 + (j + 80);
			*(pfbdata + offset) = DrawArea[j][i];
		}
	} // Draw Saved Pixel
}

