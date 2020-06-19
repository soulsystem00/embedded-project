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


void PrintScreen(int Screen[], int size)
{
	int i,j;
	int count = 0;
	unsigned short red = makepixel(255,255,255);
	for(i = 0;i<240;i++)
	{
		for(j=0;j<320;j++)
		{
			if(Screen[count] == 0)
			{
				offset = i * 320 + j;
				*(pfbdata + offset) = red;
			}	
			count++;	
		}
	}
}

void FillScreen(unsigned short color)
{
	int i,j;
	for(j = 4 ; j < 237 ; j++)
		for(i = 80 ; i < 274 ; i++)
			*(pfbdata + i + j * fbvar.xres) = color; //Draw 공간을 해당 color로 초기화
}

void SetColor(unsigned short color, int PointX, int PointY);
{
	//PointX와 PointY 위치 + main.c에서 color 속성을 지정하는 변수를 인자로 받음
	// color button 값들 좌표 비교해서 해당 좌표에 있다면 그 색으로 변경.
	// 구상중..
}


void Line(unsigned short color)
{
	while(pressure == -1){ //화면에 펜을 딱 갖다댔을 때 처음 좌표를 읽습니다
		read(fd, &ie, sizeof(struct input_event));
        	if (ie.type == 3) {
                	if (ie.code == 0) {
                        	get.x = ie.value;
                	}
                	else if (ie.code == 1) {
                        	get.y = ie.value;
                	}
                	else if (ie.code == 24) {
				start.x = a*get.x+b*get.y+c; //선을 그을 때, 원점이 되는 좌표입니다.
                                start.y = d*get.x+e*get.y+f;
                        	pressure = ie.value;
                        }
                }
        }
	printf("%d %d\n", start.x, start.y); //확인용
	while(1){
		read(fd, &ie, sizeof(struct input_event)); //여기부터는, 펜을 계속 움직이면서 선을 그립니다.
        	if (ie.type == 3) {
                	if (ie.code == 0) {
                        	get.x = ie.value;
                	}
                	else if (ie.code == 1) {
                        	get.y = ie.value;
                	}
                	else if (ie.code == 24) {
				end.x = a*get.x+b*get.y+c;
                                end.y = d*get.x+e*get.y+f;
                        	pressure = ie.value;
				if(start.x == end.x){ //직선의 방정식을 사용했는데, 두 x값이 같으면 분모가 0이 되므로 따로 예외처리.
					for(i=start.y;i<=end.y;i++)
						*(pfbdata + i*fbvar.xres) = color;
				}
				else{
					m = (end.y - start.y)/(end.x - start.x); //직선의 기울기값
					if(end.x > start.x){
						for(i=start.x;i<=end.x;i++)
							*(pfbdata + i + i*m*320) = color;
					}
					else{
						for(i=start.x;i>=end.x;i--)
							*(pfbdata + i + i*m*fbvar.xres) = color;
					}
				} //제대로 찍히는 경우 : 원점의 x, y좌표보다 높은 곳으로 갈때(기울기가 1에 가까운 형태일 때만)
				// 구현해야 할 점 : 펜을 움직일 때 마다 그려졌던 선 지우는 것
				// + 좌표 계산에 대한 예외처리, 기울기가 변할때 등등.. 상당히 어렵습니다.
					

                        }
                }
		pressure = -1;
        }
}
