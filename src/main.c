#include "../include/Header.h"
extern int Screen[76800];


int main(void) {
	white = makepixel(255,255,255);
	orange = makepixel(255,128,64);
	red = makepixel(255,0,0);
	green = makepixel(0,255,0);
	yellow = makepixel(255,255,0);
	darkblue = makepixel(0,0,128);
	blue = makepixel(0,0,255);
	black = makepixel(0,0,0);
	for (i = 0; i <320; i++)
	{
		for (j = 0; j < 240; j++)
		{
			R_DrawArea[j][i] = white;
		}
	}
	Point touch;

        pressure = -1;
	ShapeMod = 0;
	CurrentColor = black;
	DrawMod = 0;
        int x1, x2, y1, y2;
        fbfd = open(FBDEVFILE, O_RDWR);

	for(i = 0;i<233;i++)
	{
		for(j=0;j<194;j++)
		{
			DrawArea[j][i] = white;
		}
	} // clear draw area
        if (fbfd < 0) {
                perror("fbdev open");
                exit(1);
        }
        ret = ioctl(fbfd, FBIOGET_VSCREENINFO, &fbvar);
        if (ret < 0) {
                perror("fbdev ioctl");
                exit(1);
        }
        if (fbvar.bits_per_pixel != 16) {
                fprintf(stderr, "bpp is not 16\n");
                exit(1);
        }

        pfbdata = (unsigned short *)mmap(0, fbvar.xres*fbvar.yres * 16 / 8, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
        if ((unsigned)pfbdata == (unsigned)-1) {
                perror("fbdev mmap");
                exit(1);
        }

        fd = open("/dev/input/event4", O_RDONLY);
        if (fd < 0) return -1;

        printf("start\n");
        clearLcd();
        setCalibration();
        clearLcd();
	PrintScreen(Screen,sizeof(Screen)/sizeof(int));
	FillinitColor();
	PrintDrawArea();
	start:
	printf("start touch\n");
 	while(1)
	{
		read(fd, &ie, sizeof(struct input_event));
		if(ie.type == 3)
		{

			if(ie.code == 0) get.x = ie.value;
			if(ie.code == 1) get.y = ie.value;
			if(ie.code == 24)
			{
				pressure = ie.value;
				if(pressure == 0)
				{
					touch.x = a*get.x+b*get.y+c;
					touch.y = d*get.x+e*get.y+f;
					if(setMod(touch))
						break;
				}
				
			}
		}
	}
	printf("start draw\n");
	while(1)
	{
		if(DrawMod == 0)
		{
			if(ShapeMod == 0)
			{
				printf("start Line\n");
				//TODO("implement Line Function")

				Line(CurrentColor);
			}
			else if(ShapeMod == 1)
			{
				printf("start Rectangle\n");
				//TODO("implement Rectangle Function")
			
				Rectangle(CurrentColor);
				goto start;
			}
			else if(ShapeMod == 2)
			{
				printf("start Oval\n");
				//TODO("implement Oval Function")

				//Oval(CurrentColor);
				goto start;
			}
			else if(ShapeMod == 3)
			{
				printf("start FreeDraw\n");
				FreeDraw(CurrentColor);
			}
			else if(ShapeMod == 4)
			{
				printf("start Select\n");
				//TODO("implement Select Function")

				//Selete();
				goto start;
			}	
			else if(ShapeMod == 5)
			{
				printf("start Erase\n");
				//TODO("implement Erase Function")

				Erase();
			}
			else if(ShapeMod == 6)
			{
				printf("start Clear\n");
				clearDraw();
				goto start;	
			}
		}
		else if(DrawMod == 1)
		{
			printf("start Fill\n");
			Fill(CurrentColor);
		}
		if(!(start.x>=79 && start.x<=272) && (start.y>=3 && start.y<=235))
		{ 
			start.x = 79; start.y = 3; makeLineBox2(79,3,274,237,black); goto start; 
		}
		
	}   


        close(fd);
        close(fbfd);
        return 0;

} // end of main
