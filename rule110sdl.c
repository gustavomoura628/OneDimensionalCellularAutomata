#include <SDL2/SDL.h> 
//#include <SDL2/SDL_image.h> 
#include <SDL2/SDL_timer.h> 
#include <SDL2/SDL_ttf.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
int bitToInt(int a, int b, int c)
{
	return c+b*2+a*4;
}
int loadRule(int * rule, int a)
{
	int i;
	for(i = 0 ; i < 8 ; i++)
	{
		rule[i] = a % 2;
		a/=2;
	}
}
int rule110(int a, int b, int c)
{
	return (b || c) && !(a && b && c);
	//return ( (b || c) && (!b || !c) ) || (!a && c);
	//return !(((b && c) || (!b && !c)) && (a || !b));
}
int iterate(int * rule, int regsize, char * reg0, char * reg1)
{
	int A, B, C;
	int pointer;

	int wrapAround()
	{
		if(pointer >= regsize) pointer = pointer % regsize;
		else if (pointer < 0) pointer = pointer % regsize + regsize;
	}

	pointer = regsize-1;
	do
	{
		//printf("A = reg0[%d] == %d, ", pointer, reg0[pointer]);
		A = reg0[pointer];
		pointer++;
		wrapAround();
		//printf("Pointer = %d\n",pointer);
		//printf("B = reg0[%d] == %d, ", pointer, reg0[pointer]);
		B = reg0[pointer];
		pointer++;
		wrapAround();
		//printf("Pointer = %d\n",pointer);
		//printf("C = reg0[%d] == %d, ", pointer, reg0[pointer]);
		C = reg0[pointer];
		pointer--;
		wrapAround();
		//printf("Pointer = %d\n",pointer);
		//reg1[pointer] = rule110(A,B,C);
		reg1[pointer] = rule[bitToInt(A,B,C)];
		//printf("reg1[%d] == %d\n", pointer, reg1[pointer]);
		//printf("A = %d , B = %d, C = %d, R = %d\n", A ,B, C, reg1[pointer]);
		//getchar();
	}while(pointer < regsize-1);
}
int printreg(int * reg, int regsize)
{
	int i;
	for(i = 0; i < regsize ; i++)
	{
		if(reg[i] == 1)printf("#");
		else printf(".");
	}
	//printf("\n");
}



const int SCREEN_WIDTH = 1366;
const int SCREEN_HEIGHT = 768;
const int FPS = 60;
int main(int argc, char ** argv) 
{ 

	// retutns zero on success else non-zero 
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) { 
		printf("error initializing SDL: %s\n", SDL_GetError()); 
	} 
	SDL_Window* win = SDL_CreateWindow("GAME", // creates a window 
									SDL_WINDOWPOS_CENTERED, 
									SDL_WINDOWPOS_CENTERED, 
									SCREEN_WIDTH, SCREEN_HEIGHT, 0); 

	// triggers the program that controls 
	// your graphics hardware and sets flags 
	Uint32 render_flags = SDL_RENDERER_ACCELERATED; 

	// creates a renderer to render our images 
	SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags); 



	TTF_Init();

	
	
	int textDisplay(char * textInput, int textSize, int x, int y, int r, int g, int b, int outline_size)
	{
		TTF_Font *Buran = TTF_OpenFont("Buran.ttf", textSize); //this opens a font style and sets a size

		TTF_Font *font_outline = TTF_OpenFont("Buran.ttf", textSize); 
		TTF_SetFontOutline(font_outline, outline_size); 
    		
		
		SDL_Color textColor = {r, g, b, 0};
		SDL_Color inverseTextColor = {255 - r, 255 - g, 255 - b, 0};

		SDL_Surface* surfaceMessage = TTF_RenderText_Blended(Buran, textInput, textColor); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
		SDL_Surface* surfaceOutline = TTF_RenderText_Blended(font_outline, textInput, inverseTextColor); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first

		TTF_CloseFont(Buran);
		TTF_CloseFont(font_outline);


		SDL_Rect rect = { outline_size, outline_size, surfaceMessage->w, surfaceMessage->h}; 

		SDL_SetSurfaceBlendMode(surfaceMessage, SDL_BLENDMODE_BLEND);
		SDL_BlitSurface(surfaceMessage, NULL, surfaceOutline, &rect);

		//SDL_Texture* Message = SDL_CreateTextureFromSurface(rend, surfaceMessage); //now you can convert it into a texture
		SDL_Texture* Outline = SDL_CreateTextureFromSurface(rend, surfaceOutline); //now you can convert it into a texture
		// clears main-memory 
		SDL_FreeSurface(surfaceMessage); 
		SDL_FreeSurface(surfaceOutline); 
	
		//SDL_Rect Message_rect; //create a rect
		SDL_Rect Outline_rect; //create a rect
		//Message_rect.w = 100; // controls the width of the rect
		//Message_rect.h = 100; // controls the height of the rect
		
		//SDL_QueryTexture(Message, NULL, NULL, &Message_rect.w, &Message_rect.h); 
		SDL_QueryTexture(Outline, NULL, NULL, &Outline_rect.w, &Outline_rect.h); 
		
		//Message_rect.x = x-(Message_rect.w/2);  //controls the rect's x coordinate 
		//Message_rect.y = y-(Message_rect.h/2); // controls the rect's y coordinte
		
		Outline_rect.x = x-(Outline_rect.w/2);  //controls the rect's x coordinate 
		Outline_rect.y = y-(Outline_rect.h/2); // controls the rect's y coordinte
	
		//SDL_SetRenderDrawColor( rend, 0, 0, 0, 255 );
		//SDL_RenderFillRect(rend, &Message_rect);
		
		SDL_RenderCopy(rend, Outline, NULL, &Outline_rect); 
	//	SDL_RenderCopy(rend, Message, NULL, &Message_rect); 
	
		// destroy texture 
		//SDL_DestroyTexture(Message); 	
		SDL_DestroyTexture(Outline); 	
	}

	// let us control our image position 
	// so that we can move it with our keyboard. 
	SDL_Rect dest; 

	// adjust height and width of our image box. 
	dest.w = 10; 
	dest.h = 10; 

	// sets initial x-position of object 
	dest.x = (SCREEN_WIDTH - dest.w) / 2; 

	// sets initial y-position of object 
	dest.y = (SCREEN_HEIGHT - dest.h) / 2; 

	// controls annimation loop 
	int close = 0; 

	// speed of box 
	int speed = 300; 

	int width = SCREEN_WIDTH;
	int height = SCREEN_HEIGHT;
	int running = 1;
	int i,j;

	int mousex;
	int mousey;

	//int gridx = 1000;
	//int gridy = 600;//gridx*((1.0*height)/width);
	//int gridoffsetx=0;
	//int gridoffsety=0;
	//int gridwall=0;
	//int gridwidth=width-gridoffsetx;
	//int gridheight=height-gridoffsety;
	//int gridsquarewidth=gridwidth/gridx;
	//int gridsquareheight=gridsquarewidth;//height/gridy;
	
	
	int gridx;
	sscanf(argv[1], "%d", &gridx);
	//int gridy = 600;//gridx*((1.0*height)/width);
	int gridoffsetx=0;
	int gridoffsety=0;
	int gridwall=0;
	int gridwidth=width-gridoffsetx;
	int gridheight=height-gridoffsety;
	double gridsquarewidth=((1.0)*gridwidth)/gridx;
	double gridsquareheight=gridsquarewidth;//height/gridy;
	int gridy = round((1.0*gridx*height)/width);


	int getpixelpositionx(int x){
		return x*gridsquarewidth+gridoffsetx;
	}
	int getpixelpositiony(int y){
		return y*gridsquareheight+gridoffsety;
	}

	char **grid = malloc(sizeof grid* gridx);
	for(i=0;i<gridx;i++){
		grid[i] = malloc(sizeof grid[i] * gridy);
		for(j=0;j<gridy;j++){
			//if(j!=0)grid[i][j]=(i+j)%2;
			//else grid[i][j] = 0;
			grid[i][j] = 0;
		}
	}
	
	void drawgrid(int line){
		int i, j;
		int l;
		int d;
		int value;

		SDL_Rect rect;
		rect.w = gridsquarewidth+1;
		rect.h = gridsquareheight+1;
		//printf("gridx = %d, gridy = %d, width = %d, height = %d, w = %f, h = %f\n", gridx, gridy, width, height, rect.w, rect.h);
		for(d=gridy-1,i=0,l=line+gridy-1;l>0;l--,d--){
			i=l%gridy;
;
			for(j=0;j<gridx;j++){	
				value = (!grid[j][i])*255;
				rect.x = getpixelpositionx(j);
				rect.y = getpixelpositiony(d);
				SDL_SetRenderDrawColor( rend, value, value, value, 255 );
				SDL_RenderFillRect(rend, &rect);
				//al_draw_filled_rectangle(getpixelpositionx(j), getpixelpositiony(d), getpixelpositionx(j+1), getpixelpositiony(d+1), al_map_rgb(100, 100, 100));
				//al_draw_filled_rectangle(0,10,10,20,al_map_rgb(255,255,255));
				
				//if(grid[j][i])al_draw_filled_rectangle(j*gridsquarewidth, gridsquareheight*d, gridsquarewidth*(j+1), gridsquareheight*(d+1), al_map_rgb(255, 255, 255));
				//if(grid[j][i])al_draw_line(j*gridsquarewidth, gridsquareheight*d, gridsquarewidth*(j+1), gridsquareheight*(d+1), al_map_rgb(255, 255, 255),1);
				//if(grid[j][i])al_draw_filled_circle((.5+j)*gridsquarewidth, gridsquareheight*(d+.5),gridsquarewidth/2.0, al_map_rgb(255, 255, 255));
				
				//else if(!grid[i][j])al_draw_filled_rectangle(getpixelpositionx(j)+gridwall, getpixelpositiony(d)+gridwall, getpixelpositionx(j+1)-gridwall, getpixelpositiony(d+1)-gridwall, al_map_rgb(0, 0, 0));
			}
		}
	}
	
	srand(time(0));
	int rule[8] = { 0, 1, 1, 1, 0, 1, 1, 0};
	int ruleNumber = rand() % 256;
	if(argc >= 3) sscanf(argv[2], "%d", &ruleNumber);
	printf("Displaying Rule %d\n", ruleNumber);
	loadRule(rule, ruleNumber);
	int regsize;
	//sscanf(argv[1], "%d", &regsize);
	regsize = gridx;
	char * reg0 = malloc(sizeof reg0 * regsize);
	char * reg1 = malloc(sizeof reg1 * regsize);


	int loadStructure(char * reg0, int regsize, char * structure, int position)
	{
		int size = strlen(structure);
		int i;
		for(i=0; i<size; i++)
		{
			if(structure[i] == '1')
			{
				reg0[(position+i)%regsize] = 1;
			}
			else
			{
				reg0[(position+i)%regsize] = 0;
			}
		}
		return size;
	}
	
	int useSinglePoint = 0;
	int loadpointer=0;
	int loadcounter=0;
	int isLoading = 1;
	int isRandomLoad = 0;
	int isTimeRandomLoad = 0;
	for(i=0;i<regsize;i++)
	{
		
		if(!isLoading)
		{
			reg0[i] = rand() % 2;
			if(useSinglePoint)
			{
				reg0[i] = 0;
				if(i == regsize/2 ) reg0[i] = 1;
			}
		}
		else if(isLoading)
		{
			if(loadpointer == i)
			{
				if(loadcounter == 0) loadStructure(reg0, regsize, "1111111101101111101011010011100110000011101110011", i);
				if(loadcounter == 0) loadpointer += loadStructure(reg0, regsize, "0001110111", i);
				else if(loadcounter == 16) loadpointer += loadStructure(reg0, regsize, "1001111", i);
				else if(loadcounter == 20) loadpointer += loadStructure(reg0, regsize, "111", i);
				else loadpointer += loadStructure(reg0, regsize, "00010011011111", i);
				
				loadcounter++;
			}
		}
	}
	int counter = 0;
	int line = 0;
	int linehistory=0;
	float generalspeed=gridy/100+!(gridy/100);
	int scrollspeed;
	int updatespeed=1;
	// annimation loop 
	int keyboard[1000];
	int keyboardprev[1000];
	int keyboardonce[1000];
	int keyboarddown[1000];
	int keyboardup[1000];

	int lastTime = SDL_GetTicks();
	int randomCount = 0;
	int randomCounter = 0;
	memset(keyboard,0,sizeof keyboard);
	while (!close) {
		if(isRandomLoad || (isTimeRandomLoad && SDL_GetTicks() - lastTime > 3000) || (randomCount && randomCounter >= gridy/3.0))
		{
			lastTime = SDL_GetTicks();
			ruleNumber = rand() % 256;
			printf("Displaying Rule %d\n", ruleNumber);
			loadRule(rule, ruleNumber);
			for(i=0;i<regsize;i++)
			{
				reg0[i] = rand() % 2;
				if(useSinglePoint)
				{
					reg0[i] = 0;
					if(i == regsize/2 ) reg0[i] = 1;
				}
			}
			isRandomLoad = 0;
			randomCounter = 0;
		}
		SDL_Event event; 
		// Events mangement 
		while (SDL_PollEvent(&event)) {
			switch (event.type) { 

			case SDL_QUIT: 
				// handling of close button 
				close = 1; 
				break; 
			case SDL_KEYDOWN:
				keyboard[event.key.keysym.scancode] = 1;
				keyboarddown[event.key.keysym.scancode] = 1;
				break;
			case SDL_KEYUP:
				keyboard[event.key.keysym.scancode] = 0;
				keyboardup[event.key.keysym.scancode] = 1;
				break;

			/*
			case SDL_KEYDOWN: 

				// keyboard API for key pressed 
				switch (event.key.keysym.scancode) { 
				case SDL_SCANCODE_W: 
				case SDL_SCANCODE_UP: 
					dest.y -= speed / 30; 
					break; 
				case SDL_SCANCODE_A: 
				case SDL_SCANCODE_LEFT: 
					dest.x -= speed / 30; 
					break; 
				case SDL_SCANCODE_S: 
				case SDL_SCANCODE_DOWN: 
					dest.y += speed / 30; 
					break; 
				case SDL_SCANCODE_D: 
				case SDL_SCANCODE_RIGHT: 
					dest.x += speed / 30; 
				case SDL_SCANCODE_SPACE:
					Key_Space = 1;
					break; 
				}
				*/
			}
		} 
		int i;
		for(i=0;i<1000;i++)
		{
			keyboardonce[i] = 0;
			if(!keyboardprev[i] && keyboard[i])keyboardonce[i] = 1;
			keyboardprev[i] = keyboard[i];
		}
		if(keyboardonce[SDL_SCANCODE_RETURN])
		{
			isRandomLoad = 1;
		}

		if(keyboardonce[SDL_SCANCODE_K])
		{
			for(i=0;i<regsize;i++)
			{
				reg0[i] = rand() % 2;
			}

		}

		if(keyboardonce[SDL_SCANCODE_L])
		{
			for(i=0;i<regsize;i++)
			{
				if(i == 0 ) reg0[i] = 1;
				else reg0[i] = 0;
			}

		}
		if(keyboard[SDL_SCANCODE_DOWN])
		{
			generalspeed-=gridy/1000.0;
		}
		if(keyboard[SDL_SCANCODE_UP])
		{
			generalspeed+=gridy/1000.0;
		}

		if(generalspeed<0){
			scrollspeed = 1;
			updatespeed = -round(generalspeed);
			if(scrollspeed == 0)scrollspeed = 1;
			if(updatespeed == 0)updatespeed = 1;
		}
		else
		{
			scrollspeed = round(generalspeed);
			updatespeed = 1;
			if(scrollspeed == 0)scrollspeed = 1;
			if(updatespeed == 0)updatespeed = 1;
		}
		//printf("GeneralSpeed = %f, ScrollSpeed = %d, UpdateSpeed = %d\n", generalspeed, scrollspeed, updatespeed);
		
		SDL_SetRenderDrawColor( rend, 0, 0, 0, 255 );
		SDL_RenderClear(rend); 
		drawgrid(line);
		// clears the screen 
		if(counter % updatespeed == 0)
		{
			//SDL_RenderCopy(rend, tex, NULL, &dest); 
			
			int j;
			if(!keyboard[SDL_SCANCODE_SPACE]){
				for(j=0;j<scrollspeed;j++)
				{
					randomCounter++;
					
					iterate(rule, regsize, reg0, reg1);
					memcpy(reg0, reg1, sizeof reg0 * regsize);
					for(i=0;i<gridx;i++)
					{
						grid[i][line] = reg1[i];
						}
					line = (line + 1) % gridy;
					linehistory++;
				}
			}
		}
		char buffer0[1000] = "Rule ";
		char buffer1[1000];
		sprintf(buffer1, "%d", ruleNumber);
		strcat(buffer0,buffer1);

		textDisplay(buffer0, SCREEN_WIDTH/30, SCREEN_WIDTH/2, SCREEN_WIDTH/25, 255, 255, 255, 3);
		// Change color to blue!
		//SDL_SetRenderDrawColor( rend, 0, 0, 255, 255 );
 
		// Render our SDL_Rect
		// The rectangle will now be blue
		//SDL_RenderFillRect( rend, &dest );
		
		// triggers the double buffers 
		// for multiple rendering 
		SDL_RenderPresent(rend); 

		// calculates to 60 fps 
		counter++;
		SDL_Delay(1000 / FPS);
	} 

	// destroy renderer 
	SDL_DestroyRenderer(rend); 

	// destroy window 
	SDL_DestroyWindow(win); 
	return 0; 
} 

