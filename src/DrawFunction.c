#include "../include/Header.h"
#include "../include/Function.h"
#include "../include/Var.h"

void makeLineBox(Point start, Point end, unsigned short color) {
	int i, j, tmp;
	if (start.x > end.x) {
		tmp = start.x;
		start.x = end.x;
		end.x = tmp;
	}
	if (start.y > end.y) {
		tmp = start.y;
		start.y = end.y;
		end.y = tmp;
	}
	for (j = start.x; j <= end.x; j++) {
		offset = start.y * 320 + j;
		*(pfbdata + offset) = color;
		offset = end.y * 320 + j;
		*(pfbdata + offset) = color;
	}
	for (i = start.y; i <= end.y; i++) {
		offset = i * 320 + start.x;
		*(pfbdata + offset) = color;
		offset = i * 320 + end.x;
		*(pfbdata + offset) = color;
	}
}

void makeLineBox2(int sx, int sy, int ex, int ey, unsigned short color) {
	int i, j, tmp;
	if (sx > ex) {
		tmp = sx;
		sx = ex;
		ex = tmp;
	}
	if (sy > ey) {
		tmp = sy;
		sy = ey;
		ey = tmp;
	}
	for (j = sx; j <= ex; j++) {
		offset = sy * 320 + j;
		*(pfbdata + offset) = color;
		offset = ey * 320 + j;
		*(pfbdata + offset) = color;
	}
	for (i = sy; i <= ey; i++) {
		offset = i * 320 + sx;
		*(pfbdata + offset) = color;
		offset = i * 320 + ex;
		*(pfbdata + offset) = color;
	}
}

void makeLineBox3(int sx, int sy, int ex, int ey, int ary[194][233]) {
	int i, j, tmp;
	for (i = 0; i < 194; i++)
	{
		for (j = 0; j < 233; j++)
		{
			ary[i][j] = 0;
		}
	}
	if (sx > ex) {
		tmp = sx;
		sx = ex;
		ex = tmp;
	}
	if (sy > ey) {
		tmp = sy;
		sy = ey;
		ey = tmp;
	}
	for (j = sx; j <= ex; j++) {
		ary[j][sy] = 1;
		ary[j][ey] = 1;
		/*offset = sy * 320 + j;
		*(pfbdata + offset) = color;
		offset = ey * 320 + j;
		*(pfbdata + offset) = color;*/
	}
	for (i = sy; i <= ey; i++) {
		ary[sx][i] = 1;
		ary[ex][i] = 1;
		/*offset = i * 320 + sx;
		*(pfbdata + offset) = color;
		offset = i * 320 + ex;
		*(pfbdata + offset) = color;*/
	}
}

void Line(unsigned short CurrentColor) {
	int count = 0;
	int addx = 0;
	int addy = 0;
	int i, x1, y1;
	int chk = 0;
	int dx = 0; int dy = 0;

	unsigned short tmp[194][233];

	while (chk == 0) {
		read(fd, &ie, sizeof(struct input_event));
		if (ie.type == 3) {
			if (ie.code == 0) { get.x = ie.value; }
			else if (ie.code == 1) { get.y = ie.value; }
			else if (ie.code == 24) {
				pressure = ie.value;
				start.x = a * get.x + b * get.y + c;
				start.y = d * get.x + e * get.y + f;
				if ((start.x >= 80 && start.x <= 273) && (start.y >= 4 && start.y <= 236)) {
					chk = 1;
				}
				else return;
			}
		}
	}
	while (pressure != 0) {
		read(fd, &ie, sizeof(struct input_event));
		x1 = start.x; y1 = start.y;
		for (i = 0; i < 233; i++)
		{
			for (j = 0; j < 194; j++)
			{
				tmp[j][i] = NULLcolor;
			}
		} // clear draw area
		if (ie.type == 3) {
			if ((end.x >= 80 && end.x <= 273) && (end.y >= 4 && end.y <= 236))
			{
				if (dx >= dy) {
					for (i = 0; i < dx; i++) {
						x1 += addx;
						count += dy;
						if (count >= dx) {
							y1 += addy;
							count -= dx;
						}
						//*(pfbdata + x1 + y1 * 320) = white;
						tmp[x1 - 80][y1 - 4] = NULLcolor;

					}
				}//end of dx >= dy
				else {
					for (i = 0; i < dy; i++) {
						y1 += addy; count += dx;
						if (count >= dy) {
							x1 += addx; count -= dy;
						}
						//*(pfbdata + x1 + y1 * 320) = white;
						tmp[x1 - 80][y1 - 4] = NULLcolor;
					}
				}//end of else 
			}

			if (ie.code == 0) { get.x = ie.value; }
			else if (ie.code == 1) { get.y = ie.value; }
			else if (ie.code == 24)
			{

				end.x = a * get.x + b * get.y + c;
				end.y = d * get.x + e * get.y + f;
				if ((end.x >= 80 && end.x <= 273) && (end.y >= 4 && end.y <= 236)) {
					pressure = ie.value;
					dx = end.x - start.x; dy = end.y - start.y;
					x1 = start.x; y1 = start.y;
					if (dx < 0) { addx = -1; dx = -dx; }
					else { addx = 1; }
					if (dy < 0) { addy = -1;  dy = -dy; }
					else { addy = 1; }

					if (dx >= dy) {
						for (i = 0; i < dx; i++) {
							x1 += addx;
							count += dy;
							if (count >= dx) {
								y1 += addy;
								count -= dx;
							}
							//*(pfbdata + x1 + y1 * 320) = CurrentColor;
							tmp[x1 - 80][y1 - 4] = CurrentColor;
						}
					}//end of dx >= dy
					else {
						for (i = 0; i < dy; i++) {
							y1 += addy; count += dx;
							if (count >= dy) {
								x1 += addx; count -= dy;
							}
							//*(pfbdata + x1 + y1 * 320) = CurrentColor;
							tmp[x1 - 80][y1 - 4] = CurrentColor;

						}
					}//end of else
					PrintAry(tmp);
					if (pressure == 0)
					{
						x1 = start.x; y1 = start.y;
						if (dx >= dy) {
							for (i = 0; i < dx; i++) {
								x1 += addx;
								count += dy;
								if (count >= dx) {
									y1 += addy;
									count -= dx;
								}
								//*(pfbdata + x1 + y1 * 320) = CurrentColor;
								tmp[x1 - 80][y1 - 4] = CurrentColor;
							}
						}//end of dx >= dy
						else {
							for (i = 0; i < dy; i++) {
								y1 += addy; count += dx;
								if (count >= dy) {
									x1 += addx; count -= dy;
								}
								//*(pfbdata + x1 + y1 * 320) = CurrentColor;
								tmp[x1 - 80][y1 - 4] = CurrentColor;
							}
						}//end of else
						SaveAry(tmp);
						PrintDrawArea();
						break;
					}

				}
			}

		}//end of ie. event handler
	}// end of while
}//end of fun Line


void Rectangle(unsigned short CurrentColor) { //This function based on Line func.
	int addx = 0;
	int addy = 0;
	int i, x1, y1;
	int dx = 0; int dy = 0;
	int R_chk = 0;
	end.x = 0; end.y = 0;
	unsigned short tmp[194][233];
	while (R_chk == 0) {
		read(fd, &ie, sizeof(struct input_event));
		if (ie.type == 3) {
			if (ie.code == 0) { get.x = ie.value; }
			else if (ie.code == 1) { get.y = ie.value; }
			else if (ie.code == 24) {
				start.x = a * get.x + b * get.y + c;
				start.y = d * get.x + e * get.y + f;
				if ((start.x >= 80 && start.x <= 273) && (start.y >= 4 && start.y <= 236)) {
					R_chk = 1;
				}
				else return;
			}
		}
	}
	while (R_chk != 0) {
		for (i = 0; i < 233; i++)
		{
			for (j = 0; j < 194; j++)
			{
				tmp[j][i] = NULLcolor;
			}
		} // clear draw area

		read(fd, &ie, sizeof(struct input_event));
		x1 = start.x; y1 = start.y;

		if (ie.type == 3) {
			if ((end.x >= 80 && end.x <= 273) && (end.y >= 4 && end.y <= 236)) {
				for (i = 0; i < dx; i++) {
					x1 += addx;
					//*(pfbdata + x1 + start.y * 320) = white;
					//*(pfbdata + x1 + end.y * 320) = white;
					tmp[x1 - 80][start.y - 4] = NULLcolor;
					tmp[x1 - 80][end.y - 4] = NULLcolor;
				}
				for (i = 0; i < dy; i++) {
					y1 += addy;
					/**(pfbdata + start.x + y1 * 320) = white;
					*(pfbdata + end.x + y1 * 320) = white;*/
					tmp[start.x - 80][y1 - 4] = NULLcolor;
					tmp[end.x - 80][y1 - 4] = NULLcolor;
				}
			}
			if (ie.code == 0) { get.x = ie.value; }
			else if (ie.code == 1) { get.y = ie.value; }
			else if (ie.code == 24) {

				end.x = a * get.x + b * get.y + c;
				end.y = d * get.x + e * get.y + f;
				if ((end.x >= 80 && end.x <= 273) && (end.y >= 4 && end.y <= 236)) {

					pressure = ie.value;
					dx = end.x - start.x; dy = end.y - start.y;
					x1 = start.x; y1 = start.y;
					if (dx < 0) { addx = -1; dx = -dx; } // if dx, dy are minus, addx, addy minus too. 
					else { addx = 1; }
					if (dy < 0) { addy = -1;  dy = -dy; }
					else { addy = 1; }

					for (i = 0; i < dx; i++) { //dx is x's increase
						x1 += addx;
						/**(pfbdata + x1 + start.y * 320) = CurrentColor;
						*(pfbdata + x1 + end.y * 320) = CurrentColor;*/
						tmp[x1 - 80][start.y - 4] = CurrentColor;
						tmp[x1 - 80][end.y - 4] = CurrentColor;
					}
					for (i = 0; i < dy; i++) {//dy is y's increase
						y1 += addy;
						/**(pfbdata + start.x + y1 * 320) = CurrentColor;
						*(pfbdata + end.x + y1 * 320) = CurrentColor;*/
						tmp[start.x - 80][y1 - 4] = CurrentColor;
						tmp[end.x - 80][y1 - 4] = CurrentColor;
					}
					PrintAry(tmp);
					if (pressure == 0) {
						x1 = start.x; y1 = start.y;
						for (i = 0; i < dx; i++) {
							x1 += addx;
							/**(pfbdata + x1 + start.y * 320) = CurrentColor;
							*(pfbdata + x1 + end.y * 320) = CurrentColor;*/
							tmp[x1 - 80][start.y - 4] = CurrentColor;
							tmp[x1 - 80][end.y - 4] = CurrentColor;
						}
						for (i = 0; i < dy; i++) {
							y1 += addy;
							/**(pfbdata + start.x + y1 * 320) = CurrentColor;
							*(pfbdata + end.x + y1 * 320) = CurrentColor;*/
							tmp[start.x - 80][y1 - 4] = CurrentColor;
							tmp[end.x - 80][y1 - 4] = CurrentColor;
						}
						SaveAry(tmp);
						PrintDrawArea();
						break;
					}
				}
			}

		}//end of ie. event handler
	}// end of while
}//end of fun Rec

void Oval(unsigned short color)
{
	int i, j;
	int cnt;
	int x, y, a_2, b_2, d1, a1, b1, mx, my, x_move, y_move;
	int _a1,_b1;
	int chk = 0;
	int pressure = -1;
	printf("Oval start coordinate\n");
	unsigned short tmp[194][233];
	while (chk == 0)
	{
		read(fd, &ie, sizeof(struct input_event));
		if (ie.type == 3) {
			if (ie.code == 0) { get.x = ie.value; }
			else if (ie.code == 1) { get.y = ie.value; }
			else if (ie.code == 24)
			{
				pressure = ie.value;
				start.x = a * get.x + b * get.y + c;
				start.y = d * get.x + e * get.y + f;
				if ((start.x >= 80 && start.x <= 273) && (start.y >= 4 && start.y <= 236)) {
					chk = 1;
				}
				else {SaveAry(tmp);PrintDrawArea();return;}
			}
		} // end of setting start
	} // end of start
	printf("Oval END coordinate\n");
	while (pressure != 0)
	{
		for (i = 0; i < 233; i++)
		{
			for (j = 0; j < 194; j++)
			{
				tmp[j][i] = NULLcolor;
			}
		} // clear draw area
		read(fd, &ie, sizeof(struct input_event));
		if (ie.type == 3)
		{
			if (end.y > 0) {
				b1 = end.y - y_move;
				
				if(b1 < 0) b1 = b1*-1;
				x = 0;
				y = b1;
				d1 = (4 * b_2 + (a_2 * (1 - (4 * b1)))) / 4;

				/*offset = (y + y_move) * 320 + (x + x_move);
				*(pfbdata + offset) = white;
				offset = (y + y_move) * 320 + ((-1) * x + x_move);
				*(pfbdata + offset) = white;
				offset = ((-1) * y + y_move) * 320 + (x + x_move);
				*(pfbdata + offset) = white;
				offset = ((-1) * y + y_move) * 320 + ((-1) * x + x_move);
				*(pfbdata + offset) = white; // draw starting point x */
				
				tmp[x + x_move - 80][y + y_move - 4] = NULLcolor;
				tmp[((-1) * x + x_move) - 80][y + y_move - 4] = NULLcolor;
				tmp[x + x_move - 80][((-1) * y + y_move) - 4] = NULLcolor;
				tmp[((-1) * x + x_move) - 80][((-1) * y + y_move) - 4] = NULLcolor;

				while (b_2 * x < a_2 * y)
				{
					++x;
					if (d1 < 0)
					{
						d1 += b_2 * ((2 * x) + 1);
					}
					else
					{
						--y;
						d1 += b_2 * ((2 * x) + 1) - (2 * a_2 * y);
					}
					/*
					offset = (y + y_move) * 320 + (x + x_move);
					*(pfbdata + offset) = white;
					offset = (y + y_move) * 320 + ((-1) * x + x_move);
					*(pfbdata + offset) = white;
					offset = ((-1) * y + y_move) * 320 + (x + x_move);
					*(pfbdata + offset) = white;
					offset = ((-1) * y + y_move) * 320 + ((-1) * x + x_move);
					*(pfbdata + offset) = white;
					*/

					tmp[x + x_move - 80][y + y_move - 4] = NULLcolor;
					tmp[((-1) * x + x_move) - 80][y + y_move - 4] = NULLcolor;
					tmp[x + x_move - 80][((-1) * y + y_move) - 4] = NULLcolor;
					tmp[((-1) * x + x_move) - 80][((-1) * y + y_move) - 4] = NULLcolor;
				} // end of x draw */
				if(a1 < 0) a1 = a1*-1;
				x = a1;
				y = 0;
				d1 = ((4 * a_2) + b_2 * (1 - 4 * a1)) / 4;

				/*offset = (y + y_move) * 320 + (x + x_move);
				*(pfbdata + offset) = white;
				offset = (y + y_move) * 320 + ((-1) * x + x_move);
				*(pfbdata + offset) = white;
				offset = ((-1) * y + y_move) * 320 + (x + x_move);
				*(pfbdata + offset) = white;
				offset = ((-1) * y + y_move) * 320 + ((-1) * x + x_move);
				*(pfbdata + offset) = white;
				*/
				tmp[x + x_move - 80][y + y_move - 4] = NULLcolor;
				tmp[((-1) * x + x_move) - 80][y + y_move - 4] = NULLcolor;
				tmp[x + x_move - 80][((-1) * y + y_move) - 4] = NULLcolor;
				tmp[((-1) * x + x_move) - 80][((-1) * y + y_move) - 4] = NULLcolor;
				
				while (b_2 * x > a_2 * y)
				{
					++y;
					if (d1 < 0)
					{
						d1 += a_2 * ((2 * y) + 1);
					}
					else
					{
						--x;
						d1 += ((-2) * b_2 * x) + (a_2 * (2 * y + 1));
					}

					/*offset = (y + y_move) * 320 + (x + x_move);
					*(pfbdata + offset) = white;
					offset = (y + y_move) * 320 + ((-1) * x + x_move);
					*(pfbdata + offset) = white;
					offset = ((-1) * y + y_move) * 320 + (x + x_move);
					*(pfbdata + offset) = white;
					offset = ((-1) * y + y_move) * 320 + ((-1) * x + x_move);
					*(pfbdata + offset) = white;*/
					tmp[x + x_move - 80][y + y_move - 4] = NULLcolor;
					tmp[((-1) * x + x_move) - 80][y + y_move - 4] = NULLcolor;
					tmp[x + x_move - 80][((-1) * y + y_move) - 4] = NULLcolor;
					tmp[((-1) * x + x_move) - 80][((-1) * y + y_move) - 4] = NULLcolor;
				} // end of y draw
			}
			
			if (ie.code == 0) { get.x = ie.value; }
			else if (ie.code == 1) { get.y = ie.value; }
			else if (ie.code == 24)
			{
				pressure = ie.value;
				end.x = a * get.x + b * get.y + c;
				end.y = d * get.x + e * get.y + f;
				if ((end.x >= 80 && end.x <= 273) && (end.y >= 4 && end.y <= 236))
				{
					
					pressure = ie.value;
					// end of setting end

					/*if (start.x > end.x)
					{
						tmp = start.x;
						start.x = end.x;
						end.x = tmp;
					} // swap of x

					if (start.y > end.y)
					{
						tmp = start.y;
						start.y = end.y;
						end.y = tmp;
					} // swap of y*/

					x_move = (start.x + end.x) / 2;
					y_move = (start.y + end.y) / 2;

					a1 = end.x - x_move;
					b1 = end.y - y_move;
					printf("%d %d\n", a1,b1);
					if(a1 < 0) a1 = a1*-1;
					if(b1 < 0) b1 = b1*-1;
					x = 0;
					y = b1;
					a_2 = a1 * a1;
					b_2 = b1 * b1;
					d1 = (4 * b_2 + (a_2 * (1 - (4 * b1)))) / 4;

					/*offset = (y + y_move) * 320 + (x + x_move);
					*(pfbdata + offset) = color;
					offset = (y + y_move) * 320 + ((-1) * x + x_move);
					*(pfbdata + offset) = color;
					offset = ((-1) * y + y_move) * 320 + (x + x_move);
					*(pfbdata + offset) = color;
					offset = ((-1) * y + y_move) * 320 + ((-1) * x + x_move);
					*(pfbdata + offset) = color; // draw starting point x
					*/
					tmp[x + x_move - 80][y + y_move - 4] = color;
					tmp[((-1) * x + x_move) - 80][y + y_move - 4] = color;
					tmp[x + x_move - 80][((-1) * y + y_move) - 4] = color;
					tmp[((-1) * x + x_move) - 80][((-1) * y + y_move) - 4] = color;

					while (b_2 * x < a_2 * y)
					{
						++x;
						if (d1 < 0)
						{
							d1 += b_2 * ((2 * x) + 1);
						}
						else
						{
							--y;
							d1 += b_2 * ((2 * x) + 1) - (2 * a_2 * y);
						}

						/*offset = (y + y_move) * 320 + (x + x_move);
						*(pfbdata + offset) = color;
						offset = (y + y_move) * 320 + ((-1) * x + x_move);
						*(pfbdata + offset) = color;
						offset = ((-1) * y + y_move) * 320 + (x + x_move);
						*(pfbdata + offset) = color;
						offset = ((-1) * y + y_move) * 320 + ((-1) * x + x_move);
						*(pfbdata + offset) = color;
						*/
						tmp[x + x_move - 80][y + y_move - 4] = color;
						tmp[((-1) * x + x_move) - 80][y + y_move - 4] = color;
						tmp[x + x_move - 80][((-1) * y + y_move) - 4] = color;
						tmp[((-1) * x + x_move) - 80][((-1) * y + y_move) - 4] = color;
					} // end of x draw */

					x = a1;
					y = 0;
					d1 = ((4 * a_2) + b_2 * (1 - 4 * a1)) / 4;

					/*offset = (y + y_move) * 320 + (x + x_move);
					*(pfbdata + offset) = color;
					offset = (y + y_move) * 320 + ((-1) * x + x_move);
					*(pfbdata + offset) = color;
					offset = ((-1) * y + y_move) * 320 + (x + x_move);
					*(pfbdata + offset) = color;
					offset = ((-1) * y + y_move) * 320 + ((-1) * x + x_move);
					*(pfbdata + offset) = color;
					*/
					tmp[x + x_move - 80][y + y_move - 4] = color;
					tmp[((-1) * x + x_move) - 80][y + y_move - 4] = color;
					tmp[x + x_move - 80][((-1) * y + y_move) - 4] = color;
					tmp[((-1) * x + x_move) - 80][((-1) * y + y_move) - 4] = color;

					while (b_2 * x > a_2 * y)
					{
						++y;
						if (d1 < 0)
						{
							d1 += a_2 * ((2 * y) + 1);
						}
						else
						{
							--x;
							d1 += ((-2) * b_2 * x) + (a_2 * (2 * y + 1));
						}

						/*offset = (y + y_move) * 320 + (x + x_move);
						*(pfbdata + offset) = color;
						offset = (y + y_move) * 320 + ((-1) * x + x_move);
						*(pfbdata + offset) = color;
						offset = ((-1) * y + y_move) * 320 + (x + x_move);
						*(pfbdata + offset) = color;
						offset = ((-1) * y + y_move) * 320 + ((-1) * x + x_move);
						*(pfbdata + offset) = color;
						*/
						tmp[x + x_move - 80][y + y_move - 4] = color;
						tmp[((-1) * x + x_move) - 80][y + y_move - 4] = color;
						tmp[x + x_move - 80][((-1) * y + y_move) - 4] = color;
						tmp[((-1) * x + x_move) - 80][((-1) * y + y_move) - 4] = color;

					} // end of y draw
					PrintAry(tmp);
					if(pressure == 0){
						a1 = end.x - x_move;
						b1 = end.y - y_move;
						printf("%d %d\n", a1,b1);
						if(a1 < 0) a1 = a1*-1;
						if(b1 < 0) b1 = b1*-1;
						x = 0;
						y = b1;
						a_2 = a1 * a1;
						b_2 = b1 * b1;
						d1 = (4 * b_2 + (a_2 * (1 - (4 * b1)))) / 4;

						/*
						offset = (y + y_move) * 320 + (x + x_move);
						*(pfbdata + offset) = white;
						offset = (y + y_move) * 320 + ((-1) * x + x_move);
						*(pfbdata + offset) = white;
						offset = ((-1) * y + y_move) * 320 + (x + x_move);
						*(pfbdata + offset) = white;
						offset = ((-1) * y + y_move) * 320 + ((-1) * x + x_move);
						*(pfbdata + offset) = white;
						*/
						
						tmp[x + x_move - 80][y + y_move - 4] = color;
						tmp[((-1) * x + x_move) - 80][y + y_move - 4] = color;
						tmp[x + x_move - 80][((-1) * y + y_move) - 4] = color;
						tmp[((-1) * x + x_move) - 80][((-1) * y + y_move) - 4] = color;

						while (b_2 * x < a_2 * y)
						{
							++x;
							if (d1 < 0)
							{
								d1 += b_2 * ((2 * x) + 1);
							}
							else
							{
								--y;
								d1 += b_2 * ((2 * x) + 1) - (2 * a_2 * y);
							}

							/*
							offset = (y + y_move) * 320 + (x + x_move);
							*(pfbdata + offset) = white;
							offset = (y + y_move) * 320 + ((-1) * x + x_move);
							*(pfbdata + offset) = white;
							offset = ((-1) * y + y_move) * 320 + (x + x_move);
							*(pfbdata + offset) = white;
							offset = ((-1) * y + y_move) * 320 + ((-1) * x + x_move);
							*(pfbdata + offset) = white;
							*/
							
							tmp[x + x_move - 80][y + y_move - 4] = color;
							tmp[((-1) * x + x_move) - 80][y + y_move - 4] = color;
							tmp[x + x_move - 80][((-1) * y + y_move) - 4] = color;
							tmp[((-1) * x + x_move) - 80][((-1) * y + y_move) - 4] = color;
						} // end of x draw */

						x = a1;
						y = 0;
						d1 = ((4 * a_2) + b_2 * (1 - 4 * a1)) / 4;

						/*
						offset = (y + y_move) * 320 + (x + x_move);
						*(pfbdata + offset) = white;
						offset = (y + y_move) * 320 + ((-1) * x + x_move);
						*(pfbdata + offset) = white;
						offset = ((-1) * y + y_move) * 320 + (x + x_move);
						*(pfbdata + offset) = white;
						offset = ((-1) * y + y_move) * 320 + ((-1) * x + x_move);
						*(pfbdata + offset) = white;
						*/
						
						tmp[x + x_move - 80][y + y_move - 4] = color;
						tmp[((-1) * x + x_move) - 80][y + y_move - 4] = color;
						tmp[x + x_move - 80][((-1) * y + y_move) - 4] = color;
						tmp[((-1) * x + x_move) - 80][((-1) * y + y_move) - 4] = color;

						while (b_2 * x > a_2 * y)
						{
							++y;
							if (d1 < 0)
							{
								d1 += a_2 * ((2 * y) + 1);
							}
							else
							{
								--x;
								d1 += ((-2) * b_2 * x) + (a_2 * (2 * y + 1));
							}

							/*
							offset = (y + y_move) * 320 + (x + x_move);
							*(pfbdata + offset) = white;
							offset = (y + y_move) * 320 + ((-1) * x + x_move);
							*(pfbdata + offset) = white;
							offset = ((-1) * y + y_move) * 320 + (x + x_move);
							*(pfbdata + offset) = white;
							offset = ((-1) * y + y_move) * 320 + ((-1) * x + x_move);
							*(pfbdata + offset) = white;
							*/
							
							tmp[x + x_move - 80][y + y_move - 4] = color;
							tmp[((-1) * x + x_move) - 80][y + y_move - 4] = color;
							tmp[x + x_move - 80][((-1) * y + y_move) - 4] = color;
							tmp[((-1) * x + x_move) - 80][((-1) * y + y_move) - 4] = color;

						} // end of y draw						
						
						SaveAry(tmp);
						PrintDrawArea();
						break;
					}
				}
				printf("print tmp\n");
				PrintAry(tmp);
			}
		} // end of if

	} // end of while
	
}

void FreeDraw(unsigned short color)
{
	read(fd, &ie, sizeof(struct input_event));
	unsigned short tmp[194][233];
	if (ie.type == 3)
	{
		if (ie.code == 0) get.x = ie.value;
		if (ie.code == 1) get.y = ie.value;
		if (ie.code == 24)
		{
			start.x = a * get.x + b * get.y + c;
			start.y = d * get.x + e * get.y + f;
			if ((start.x >= 80 && start.x <= 273) && (start.y >= 4 && start.y <= 236))
			{
				DrawArea[start.x + 1 - 80][start.y + 1 - 4] = color;
				DrawArea[start.x - 80][start.y + 1 - 4] = color;
				DrawArea[start.x + 1 - 80][start.y - 4] = color;
				DrawArea[start.x - 80][start.y - 4] = color;
				DrawArea[start.x - 1 - 80][start.y - 1 - 4] = color;
				DrawArea[start.x - 80][start.y - 1 - 4] = color;
				DrawArea[start.x - 1 - 80][start.y - 4] = color;
				DrawArea[start.x + 1 - 80][start.y - 1 - 4] = color;
				DrawArea[start.x + 1 - 80][start.y + 1 - 4] = color;
				/*offset = (start.y + 1) * 320 + (start.x + 1);
				*(pfbdata + offset) = color;
				offset = (start.y +1) * 320 + start.x;
				*(pfbdata + offset) = color;
				offset = start.y * 320 + (start.x+1);
				*(pfbdata + offset) = color;
				offset = start.y * 320 + start.x;
				*(pfbdata + offset) = color;
				offset = (start.y -1) * 320 + (start.x -1);
				*(pfbdata + offset) = color;
				offset = (start.y -1) * 320 + start.x;
				*(pfbdata + offset) = color;
				offset = start.y * 320 + (start.x-1);
				*(pfbdata + offset) = color;
				offset = (start.y - 1) * 320 + (start.x + 1);
				*(pfbdata + offset) = color;
				offset = (start.y +1) * 320 + (start.x-1);
				*(pfbdata + offset) = color;*/
			}
		}
	}
	PrintDrawArea();
}

void Selete()
{
	unsigned short tmp[194][233]; //selected area rectangle
	unsigned short tmp2[194][233]; // all of selected area 
	unsigned int selected[194][233]; //all of selected area flag
	int addx = 0;
	int addy = 0;
	int i, x1, y1;
	int dx = 0; int dy = 0;
	int R_chk = 0;
	end.x = 0; end.y = 0;
	for (i = 0; i < 233; i++)
	{
		for (j = 0; j < 194; j++)
		{
			selected[j][i] = 0;
		}
	} // clear draw area
	while (R_chk == 0)
	{
		read(fd, &ie, sizeof(struct input_event));
		if (ie.type == 3) {
			if (ie.code == 0) { get.x = ie.value; }
			else if (ie.code == 1) { get.y = ie.value; }
			else if (ie.code == 24) {
				start.x = a * get.x + b * get.y + c;
				start.y = d * get.x + e * get.y + f;
				if ((start.x >= 80 && start.x <= 273) && (start.y >= 4 && start.y <= 236)) {
					R_chk = 1;
				}
				else return;
			}
		}
	}
	while (R_chk != 0)
	{
		for (i = 0; i < 233; i++)
		{
			for (j = 0; j < 194; j++)
			{
				tmp[j][i] = NULLcolor;
			}
		} // clear draw area

		read(fd, &ie, sizeof(struct input_event));
		x1 = start.x; y1 = start.y;

		if (ie.type == 3)
		{
			if ((end.x >= 80 && end.x <= 273) && (end.y >= 4 && end.y <= 236)) {
				for (i = 0; i < dx; i++)
				{
					x1 += addx;
					//*(pfbdata + x1 + start.y * 320) = white;
					//*(pfbdata + x1 + end.y * 320) = white;
					tmp[x1 - 80][start.y - 4] = NULLcolor;
					tmp[x1 - 80][end.y - 4] = NULLcolor;
				}
				for (i = 0; i < dy; i++)
				{
					y1 += addy;
					/**(pfbdata + start.x + y1 * 320) = white;
					*(pfbdata + end.x + y1 * 320) = white;*/
					tmp[start.x - 80][y1 - 4] = NULLcolor;
					tmp[end.x - 80][y1 - 4] = NULLcolor;
				}
			}
			if (ie.code == 0) { get.x = ie.value; }
			else if (ie.code == 1) { get.y = ie.value; }
			else if (ie.code == 24)
			{

				end.x = a * get.x + b * get.y + c;
				end.y = d * get.x + e * get.y + f;
				if ((end.x >= 80 && end.x <= 273) && (end.y >= 4 && end.y <= 236))
				{

					pressure = ie.value;
					dx = end.x - start.x; dy = end.y - start.y;
					x1 = start.x; y1 = start.y;
					if (dx < 0) { addx = -1; dx = -dx; } // if dx, dy are minus, addx, addy minus too. 
					else { addx = 1; }
					if (dy < 0) { addy = -1;  dy = -dy; }
					else { addy = 1; }

					for (i = 0; i < dx; i++)
					{ //dx is x's increase
						x1 += addx;
						/**(pfbdata + x1 + start.y * 320) = CurrentColor;
						*(pfbdata + x1 + end.y * 320) = CurrentColor;*/
						tmp[x1 - 80][start.y - 4] = purple;
						tmp[x1 - 80][end.y - 4] = purple;
					}
					for (i = 0; i < dy; i++)
					{//dy is y's increase
						y1 += addy;
						/**(pfbdata + start.x + y1 * 320) = CurrentColor;
						*(pfbdata + end.x + y1 * 320) = CurrentColor;*/
						tmp[start.x - 80][y1 - 4] = purple;
						tmp[end.x - 80][y1 - 4] = purple;
					}
					PrintAry(tmp);
					if (pressure == 0)
					{
						//x1 = start.x; y1 = start.y;
						if (start.x > end.x)
						{
							int tmp = end.x; end.x = start.x; start.x = tmp;
						}
						if (start.y > end.y)
						{
							int tmp = end.y; end.y = start.y; start.y = tmp;
						}
						for (i = start.x; i < end.x; i++)
						{
							for (j = start.y; j < end.y; j++)
							{
								selected[i - 80][j - 4] = 1;
							}
						}
						break;
					}
				}
			}
		}
	}
	//move start
	for (i = 0; i < 233; i++)
	{
		for (j = 0; j < 194; j++)
		{
			tmp2[j][i] = NULLcolor;
		}
	} // clear draw area
	R_chk = 0;
	while (R_chk == 0)
	{
		read(fd, &ie, sizeof(struct input_event));
		if (ie.type == 3) {
			if (ie.code == 0) { get.x = ie.value; }
			else if (ie.code == 1) { get.y = ie.value; }
			else if (ie.code == 24) {
				start.x = a * get.x + b * get.y + c;
				start.y = d * get.x + e * get.y + f;
				printf("%d %d\n", start.x, start.y);
				if ((start.x >= 80 && start.x <= 273) && (start.y >= 4 && start.y <= 236)) {
					R_chk = 1;
				}
				else return;
			}
		}
	}

	while (R_chk != 0)
	{
		read(fd, &ie, sizeof(struct input_event));
		x1 = start.x; y1 = start.y;

		if (ie.type == 3)
		{
			if (ie.code == 0) { get.x = ie.value; }
			else if (ie.code == 1) { get.y = ie.value; }
			else if (ie.code == 24)
			{
				pressure = ie.value;
				if (pressure == 0)
				{
					end.x = a * get.x + b * get.y + c;
					end.y = d * get.x + e * get.y + f;
					dx = end.x - start.x; dy = end.y - start.y;
					if ((end.x >= 80 && end.x <= 273) && (end.y >= 4 && end.y <= 236))
					{
						pressure = ie.value;
						dx = end.x - start.x;
						dy = end.y - start.y;
						printf("%d %d\n", end.x, end.y);
						if (pressure == 0)
						{
							printf("%d %d\n", dx, dy);
							for (i = 0; i < 233; i++)
							{
								for (j = 0; j < 194; j++)
								{
									if (selected[j][i] == 1)
									{
										if (j + dx < 233 && i + dy < 194)
										{
											tmp2[j + dx][i + dy] = DrawArea[j][i];
										}
									}
								}
							} // clear draw area
							for (i = 0; i < 233; i++)
							{
								for (j = 0; j < 194; j++)
								{
									if (selected[j][i] == 1)
									{
										DrawArea[j][i] = white;
									}
								}
							} // clear draw area
							for (i = 0; i < 233; i++)
							{
								for (j = 0; j < 194; j++)
								{
									if (tmp2[j][i] != NULLcolor)
									{
										DrawArea[j][i] = tmp2[j][i];
									}
								}
							} // clear draw area
							PrintDrawArea();
							break;
						}
					}
				}
			}
		}
	}
}

void Erase() {
	read(fd, &ie, sizeof(struct input_event));

	if (ie.type == 3)
	{
		if (ie.code == 0) get.x = ie.value;
		if (ie.code == 1) get.y = ie.value;
		if (ie.code == 24)
		{
			start.x = a * get.x + b * get.y + c;
			start.y = d * get.x + e * get.y + f;
			if ((start.x >= 80 && start.x <= 273) && (start.y >= 4 && start.y <= 236))
			{
				DrawArea[start.x + 1 - 80][start.y + 1 - 4] = white;
				DrawArea[start.x - 80][start.y + 1 - 4] = white;
				DrawArea[start.x + 1 - 80][start.y - 4] = white;
				DrawArea[start.x - 80][start.y - 4] = white;
				DrawArea[start.x - 1 - 80][start.y - 1 - 4] = white;
				DrawArea[start.x - 80][start.y - 1 - 4] = white;
				DrawArea[start.x - 1 - 80][start.y - 4] = white;
				DrawArea[start.x + 1 - 80][start.y - 1 - 4] = white;
				DrawArea[start.x + 1 - 80][start.y + 1 - 4] = white;
			}
		}
	}
	PrintDrawArea();
}

void clearDraw() {
	int i, j;
	for (i = 0; i < 233; i++) {
		for (j = 0; j < 194; j++) {
			DrawArea[j][i] = white;
		}
	}
	PrintDrawArea();
}



