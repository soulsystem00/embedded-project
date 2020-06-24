#include "../include/Header.h"

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

void makeLineBox(Point start, Point end, unsigned short color){
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
        for (j = start.x; j < end.x; j++) {
                offset = start.y * 320 + j;
                *(pfbdata + offset) = color;
                offset = end.y * 320 + j;
                *(pfbdata + offset) = color;
        }
        for (i = start.y; i < end.y; i++) {
                offset = i * 320 + start.x;
                *(pfbdata + offset) = color;
                offset = i * 320 + end.x;
                *(pfbdata + offset) = color;
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

			x_move = (start.x + end.x)/2; // x가 (0,0)에서부터 이동한 거리
			y_move = (start.y + end.y)/2; // y가 (0,0)에서부터 이동한 거리
						      // https://kipl.tistory.com/20 <- 알고리즘 출처
			a1 = end.x - x_move;
			b1 = end.y - y_move;
			x = 0;
			y = b1;
			a_2 = a1 * a1;
			b_2 = b1 * b1;
			d1 = (4 * b_2 + (a_2 * (1 - (4 * b1)))) / 4; // x 독립매개 변수일때 판별식

			offset = (y + y_move) * 320 + (x + x_move);  // 타원의 중심을 (0,0)으로 이동했다 생각하고 
			*(pfbdata + offset) = color;                 // 거기에서 판별식을 이용해 x,y 값을 결정하고 x,y값이 이동한 만큼
			offset = (y + y_move) * 320 + ((-1)*x + x_move);  // x,y의 좌표를 x_move, y_move만큼 이동시켜서 픽셀을 찍음
			*(pfbdata + offset) = color;
			offset = ((-1)*y + y_move) * 320 + (x + x_move);  
			*(pfbdata + offset) = color;
			offset = ((-1)*y + y_move) * 320 + ((-1)*x + x_move);  
			*(pfbdata + offset) = color; // draw starting point x

			while(b_2 * x < a_2 * y) // x - 독립매개 변수 draw
			{
				++x; 
				if(d1 < 0)
				{
					d1 += b_2 *((2 * x) + 1);	
 				}
				else // 
				{
					--y;
					d1 += b_2 * ((2 * x) + 1) - (2 * a_2 * y);
				} // 우선 x를 1 증가시키고 판별식에 따라 y를 결정하여 연속적으로 그림
					
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
			d1 = ((4 * a_2) + b_2 * (1 - 4 * a1)) / 4; // y 독립매개 변수일때의 판별식

			offset = (y + y_move) * 320 + (x + x_move);
                        *(pfbdata + offset) = color;
                        offset = (y + y_move) * 320 + ((-1)*x + x_move);
                        *(pfbdata + offset) = color;
                        offset = ((-1)*y + y_move) * 320 + (x + x_move);
                        *(pfbdata + offset) = color;
                        offset = ((-1)*y + y_move) * 320 + ((-1)*x + x_move);
                        *(pfbdata + offset) = color;

			while(b_2 * x > a_2 * y) // y - 독립매개 변수 draw
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
