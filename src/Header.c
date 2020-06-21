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
        int i, j, w, h, tmp, weight, height, x_move, y_move;
 
	int x, y, a_2, b_2, d1, a1, b1, mx, my;
	int chk = 0;
	int pressure = -1;
        int ret, t;
        Point get, start, end;

        int x1, x2, y1, y2;

	 while (chk == 0){
         read(fd, &ie, sizeof(struct input_event));
         if (ie.type == 3){
          if (ie.code == 0){
           get.x = ie.value;
          } 
          else if (ie.code == 1){
           get.y = ie.value;
          } 
          else if (ie.code == 24){
           start.x = a * get.x + b * get.y + c;
           start.y = d * get.x + e * get.y + f;
           pressure = ie.value;
	   chk = 1;
          } 
         } 
	} 
	
	while (1){ 
          read(fd, &ie, sizeof(struct input_event));
          if (ie.type == 3){

         /*   for (i = start.x; i < end.x; i++){ // x^2/a^2 + y^2/b^2 == 1을 이용하여 좌표를 찍을 때 사용하던 원래 픽셀을 지우는 반복문
             int m = i - x_move;
             for (j = start.y; j < end.y; j++){
              int b = j - y_move;
              if((w != 0) && (h != 0)){
               if((((m * m)/(w * w)) + ((b * b)/(h * h))) == 1){
                offset = j * 320 + i;
                *(pfbdata + offset) = 0;
               }
              }
             }
            }*/
	  

           if (ie.code == 0){
            get.x = ie.value;
           } 
           else if (ie.code == 1){
            get.y = ie.value;
           } 
           else if (ie.code == 24){
            end.x = a*get.x+b*get.y+c;
            end.y = d*get.x+e*get.y+f;
            pressure = ie.value;
	   
	    printf("1\n");	
	    
		//아직 타원 알고리즘을 이해중입니다. 주석 내용이 정확하지 않을 수 있으니 오른쪽 사이트에서 알고리즘 확인 부탁드립니다.-> https://kipl.tistory.com/20

	    a1 = end.x; // 현재 그리는 타원의 x축 끝부분의 좌표
	    b1 = end.y; // 현재 그리는 타원의 y축 끝부분의 좌표
	    x = (start.x + end.x) / 2; //현재 그리는 타원의 중심 부분의 좌표, 중심부분 에서 x가 바뀌면서 픽셀이 그려짐
	    y = end.y; // 현재 그리는 
	    a_2 = a1 * a1; // 판별식 d1에서 쓸 a1^2
	    b_2 = b1 * b1; // 판별식 d1에서 쓸 b1^2
	    d1 = (4 * b_2 + (a_2 * (1 - 4 * b1))) / 4;
		// 판별식 d1,  f(x,y) = b^2 * x^2 + a^2 * y^2 -a^2 * b^2에 f(x+1, y-1/2)를 적용시킨 것으로
		// 두 점중 어느 좌표에 점을 찍을지 정하는 판별식
	    
	    printf("2\n");
	    printf("%d\n", start.x);
	    printf("%d\n", end.x);
	    printf("%d\n", x);
	    printf("%d\n", y);
	    printf("%d\n", offset);
	    mx = (start.x + end.x) - x; // 현재 x의 좌표를 타원 중앙의 좌표를 기준으로 반대로 넘긴 값
	    my = (start.y + end.y) - y; // 현재 y의 좌표를 타원 중앙의 좌표를 기준으로 반대로 넘긴 값
	    offset = y * 320 + x;  // (x,y), (mx,y), (x,my), (mx,my)에 각각 픽셀을 설정함, 대칭성을 이용하여 한 좌표를 설정하면 그것을 이동시켜서 픽셀 설정
	    *(pfbdata + offset) = color;
	    offset = y * 320 + mx;  
	    *(pfbdata + offset) = color;
	    offset = my * 320 + x;  
	    *(pfbdata + offset) = color;
	    offset = my * 320 + mx;  
	    *(pfbdata + offset) = color;

	    while(b_2 * x < a_2 * y){ // x 독립변수 구간, 타원의 접하는 직선의 기울기가 0에서 -1인 지점 까지, 타원식을 미분한 결과값을 이용해 반복 횟수 설정
		++x; // x+1을 설정하고
		if(d1 < 0){ // d1이 0보다 작은 경우 
		 d1 += b_2 *(2 * x + 1); // d1_new = F(x+2,y) = d_old +  b^2 *(2 * x + 1)로 설정해줌
 		}
		else{ // d1이 0보다 클 경우
		 --y; // 
		 d1 += b_2 * (2 * x + 1) - 2 * a_2 * y; // d_new = F(x+2,y-3/2) = d_old + b^2 * (2 * x + 1) - 2 * a^2 * y
		}
		mx = (start.x + end.x) - x; // x,y값이 새로 설정되었으니 위와 마찬가지로 대칭성을 이용하여 픽셀 설정 
	    my = (start.y + end.y) - y; 
	    offset = y * 320 + x;  
	    *(pfbdata + offset) = color;
	    offset = y * 320 + mx;  
	    *(pfbdata + offset) = color;
	    offset = my * 320 + x;  
	    *(pfbdata + offset) = color;
	    offset = my * 320 + mx;  
	    *(pfbdata + offset) = color;
	    }

	    x = end.x; 
	    y = (end.y + start.y) / 2;
	    d1 = (4 * a_2 + b_2 * (1 - 4 * a1)) / 4; // y 독립변수 구간에서 사용할 판별식,f(x-1/2,y+1)

	    mx = (start.x + end.x) - x;
	    my = (start.y + end.y) - y; 
	    offset = y * 320 + x;  
	    *(pfbdata + offset) = color;
	    offset = y * 320 + mx;  
	    *(pfbdata + offset) = color;
	    offset = my * 320 + x;  
	    *(pfbdata + offset) = color;
	    offset = my * 320 + mx;  
	    *(pfbdata + offset) = color;

	    while(b_2 * x > a_2 * y){ //y 독립변수 구간
		++y;
		if(d1 < 0){
		 d1 += a_2 * (2 * y + 1);
		}
		else{
		 --x;
		 d1 += (-2) * b_2 * x + a_2*(2 * y + 1);
		}
		mx = (start.x + end.x) - x;
	    my = (start.y + end.y) - y; 
	    offset = y * 320 + x;  
	    *(pfbdata + offset) = color;
	    offset = y * 320 + mx;  
	    *(pfbdata + offset) = color;
	    offset = my * 320 + x;  
	    *(pfbdata + offset) = color;
	    offset = my * 320 + mx;  
	    *(pfbdata + offset) = color;
	    }

	    pressure -1;
	   }
	  }
	  
    }
}
