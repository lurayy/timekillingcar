#include <stdio.h>
#include <time.h>
#include <SDL.h>
#include <SDL_image.h>

//structures
typedef struct
{
        float x;
        float y;
        float h;
        float w;
} pos;
typedef struct
{
        pos hero;
        pos gunda[3];
        pos collide;
        SDL_Texture *road;
        SDL_Texture *herodaikocar;
        SDL_Window *window ;
        SDL_Renderer *render;
        SDL_Texture *gundakocar;
} gamestate;

//functions
int texture(gamestate *game);
int eventprocess (gamestate *game) ;
int renderer(gamestate *game);
int collision(gamestate *game);
int collisionmove(gamestate *game);
int loadgame (gamestate *game);
int menu (gamestate *game);

 //main
int main (int argc, char *argv[])
{
        //initalization
        int i;
        gamestate game;
        game.hero.h=38;
        game.hero.w=47;
        game.hero.y=100;
        game.hero.x=100;
        game.collide.x=0;

        for(i=0;i<3;i++)
        {
                game.gunda[i].h=0;
                game.gunda[i].w=0;
                game.gunda[i].y=0;
                game.gunda[i].x=0;

        }

        //window and renderer creation and initialization
        SDL_Window *win;
        SDL_Renderer *ren;
        win =  SDL_CreateWindow("Test",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,499,586,0);
        ren = SDL_CreateRenderer(win,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        game.window = win;
        game.render = ren;

        texture(&game);

        //event loop
        int done=0 ;
        while (!done )
        {
                        collision(&game);
                        done = eventprocess(&game);
                        collisionmove(&game);
                        renderer(&game);
        }
        SDL_DestroyWindow(win);
        SDL_DestroyRenderer(ren);

        SDL_DestroyTexture(game.gundakocar);
        SDL_DestroyTexture(game.road);
        SDL_Quit();
        return 0;

}



//load texture
int texture (gamestate *game )
{
        //load textures
        SDL_Surface *surface= NULL;
       	 surface= IMG_Load ( "png/car.png");
        if(surface == NULL )
           {
               printf("Error");
               SDL_Quit();
            }
        game->herodaikocar = SDL_CreateTextureFromSurface(game->render , surface);
		SDL_FreeSurface (surface);

        SDL_Surface *background= NULL;
        background = IMG_Load ("png/road.png");
        if(background == NULL )
        {
        	printf("\n Error Cant find background");
        	SDL_Quit();
        }
		game->road = SDL_CreateTextureFromSurface(game->render, background);
		SDL_FreeSurface (background);
		return 0;
}


//eventprocess
        int eventprocess(gamestate *game)
        {
                int done =0;
                SDL_Event movecar;
                while (SDL_PollEvent(&movecar))
                        {
                                switch(movecar.type)
                                {
                                case SDL_WINDOWEVENT_CLOSE:
                                        {
                                                if(game->window)
                                                {
                                                        SDL_DestroyWindow(game->window);
                                                        game->window=NULL;
                                                        done=1;
                                                }
                                        }
                                        break;
                                        case SDL_KEYDOWN:
                                        {
                                                switch(movecar.key.keysym.sym)
                                                {
                                                        case SDLK_ESCAPE:
                                                                done=1;
                                                        break;
                                                }
                                        }
                                        break;
                                        case SDL_QUIT:
                                        done=1;
                                        break;
                                }
                        }
                        const  Uint8 *state = SDL_GetKeyboardState(NULL);
                        if (state[SDL_SCANCODE_RIGHT])
                        {
                                game->hero.x+=5;
                        }
                                if (state[SDL_SCANCODE_LEFT])
                        {
                                        game->hero.x-=5;
                        }
                            if (state[SDL_SCANCODE_UP])
                        {
                                game->hero.y-=5;
                        }
                                if (state[SDL_SCANCODE_DOWN])
                        {
                                        game->hero.y+=5;
                        }
                        return(done);
                }




        //draw /render graphics
int renderer(gamestate *game)
{
        //initialization and screen clearing
                SDL_SetRenderDrawColor(game->render , 0,0,0,255);
                SDL_RenderClear(game->render);

        //road ko drawing
                SDL_Rect bg = {0,0,499,586};
                SDL_RenderCopy(game->render, game->road,NULL ,&bg);

        //hero dai ko car ko drawing
                SDL_Rect herodai  = {game->hero.x,game->hero.y,game->hero.h,game->hero.w};
                SDL_RenderCopy(game->render, game->herodaikocar , NULL , &herodai );

                //tests
                             SDL_SetRenderDrawColor(game->render,255,0,0,255) ;
                                SDL_RenderDrawLine(game->render,54,0,54,500);
                SDL_RenderPresent(game->render);
                SDL_Delay(10 );
                return 0;

}

//collision
int collision( gamestate *game)
{
        int i ;
       float  mx=game->hero.x;
        float my = game->hero.y;
      float  bl=59;
       float br=422;
        for(i=0;i<586;i++)
        {
                        if(bl==mx && my==i  )
                        {
                                game->hero.x=game->hero.x-5;
                                        printf("collide");
                        }
        }
  return  0;
}


//collision move
int collisionmove(gamestate *game)
{
        return 0;
}













/*
int  collision(gamestate *game)
{
	float mw=32 , mh=32;
	float mx=game->hero.x, my=game->hero.y;
	int i;
	float bw=200 , bh=69;
	for(i=0;i<10;i++)
	{
		float bx=game->brickpos[i].x , by=game->brickpos[i].y;
                //colide with right edge
		if (my+mh >by && my<  by+bh)
		{
		        if (mx < bx+bw && mx +mw > bx+bw )
                        {
                        game->hero.x=bx+bw;
			mx= bx + bw;
                        }
                                //colide with left edge
                        else if( mx+mw > bx && mx <bx )
                        {
                        game->hero.x = bx - mw ;
                        mx= bx-mw;
                        }
                }
	   if (mx+mw >bx && mx <bx+bw )
           {
                   //bumping head
                   if (my <by+bh && my>by)
                   {
                           game->hero.y= by+bh;
                           game->hero.dy= 0;
                   }
                   else if (my+mh > by && my<by )
                   {
                           game->hero.y= by-mh;
                   }
           }

	}
return 0;
         */
