#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_ttf.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
using namespace std;

SDL_Event event;

const int space = 2;
const int block_size = 25;
const int Row = 14;
const int Col = 10;
const int Width = 500;
const int Height = 500;

int i , j , i_1 , j_1 , i_2 , j_2 , i_3 , j_3;

struct Block {
public:
	int x1;
	int y1;
	int x2;
	int y2;
	
	int status = 0;
	int R;
	int G;
	int B;
};

Block block [Row][Col];

void apply_surface (int x , int y , SDL_Surface *source , SDL_Surface *dest);
void print_num (SDL_Surface *screen , int x , int y , int z);
void initialize (SDL_Surface *screen);
void randomise (SDL_Surface *screen , bool *runp , int shape);
void show_block (SDL_Surface *screen , int x1 , int y1 , int x2 , int y2 , int R , int G , int B);
void print (SDL_Surface *screen , int score , int level , int nextshape);
void block_set (SDL_Surface *screen);
void block_color_set (SDL_Surface *screen);
void block_reset (SDL_Surface *screen);
void block_color_reset (SDL_Surface *screen , char a);
void move_down (SDL_Surface *screen , bool *movedown);
void move_left (SDL_Surface *screen);
void move_right (SDL_Surface *screen);
void orbit_move (SDL_Surface *screen);
void del_line (SDL_Surface *screen , int *scorePtr);
void freeze_block (SDL_Surface *screen);
void level_up (SDL_Surface *screen , int score , int *levelPtr);


int main (int argc , char *args[]) {

	srand (time (0));
	SDL_Surface *screen = SDL_SetVideoMode (Width , Height , 32 , 0);
	SDL_Surface *scoretext = NULL;
	SDL_Color textcolor = { 255 , 0 , 0 };
	SDL_Init (SDL_INIT_EVERYTHING);
	
	initialize (screen);

	int shape = rand () % 7;
	int nextshape = rand () % 7;
	bool run = true;
	bool *runp = &run;
	bool movedown = true;
	bool *movedownp = &movedown;
	int score = 0;
	int *scorePtr = &score;
	int level = 1;
	int *levelPtr = &level;

	while (event.type != SDL_QUIT) {

		movedown = true;		
		SDL_FillRect (screen , NULL , 0x000);
		randomise (screen , runp , shape);
		if (run == false)
			break;

		print (screen , score , level , nextshape);
		SDL_Delay (1200 - (level - 1)*100 );
		while (movedown) {

			while (SDL_PollEvent (&event)){

				if (event.type == SDL_KEYDOWN) {

					switch (event.key.keysym.sym) {

						case SDLK_LEFT: move_left (screen);
						print (screen , score , level , nextshape); break;
						case SDLK_RIGHT: move_right (screen);
						print (screen , score , level , nextshape); break;
						case SDLK_DOWN: move_down (screen , movedownp);
						print (screen ,score ,level ,nextshape);break;
						case SDLK_SPACE: orbit_move (screen);
						print (screen , score , level , nextshape); break;
					}
				}
			}

			move_down (screen , movedownp);
			print (screen , score , level , nextshape);

			SDL_Delay (1200 - (level - 1)*100 );
		}
	
		score++;
		freeze_block (screen);
		del_line (screen , scorePtr);
		print (screen , score , level , nextshape);
		level_up (screen , score , levelPtr);
		shape = nextshape;
		nextshape = rand () % 7;
	}

	return 0;
}

void apply_surface (int x , int y , SDL_Surface *source , SDL_Surface *dest){
	
	SDL_Rect offset;

	offset.x = x;
	offset.y = y;

	SDL_BlitSurface (source , NULL , dest , &offset);
}

void print_num (SDL_Surface *screen , int x , int y , int z){

	TTF_Init ();
	TTF_Font *font = TTF_OpenFont ("ARIAL.ttf" , 20);
	
	SDL_Surface *num;
	SDL_Color text_color = { 255 , 0 , 0 };
	
	stringstream z_2;
	z_2 << z;

	num = TTF_RenderText_Solid (font , z_2.str().c_str() , text_color);
	apply_surface (x , y , num , screen);
}

void initialize (SDL_Surface *screen) {

	for (int i= 0 ; i < Row ; i++)
		for (int j=0 ; j < Col ; j++){

			block[i][j].x1 = ( ((j+1)*space) + (j*block_size) );
			block[i][j].y1 = ( ((i+2)*space) + ((i+1)*block_size) );
			block[i][j].x2 = block[i][j].x1 + block_size;
			block[i][j].y2 = block[i][j].y1 + block_size;
		}
}

void show_block (SDL_Surface *screen , int x1 , int y1 , int x2 , int y2 , int R , int G , int B){
	
	boxRGBA (screen , x1 , y1 , x2 , y2 , R , G , B , 255);
}

void randomise (SDL_Surface *screen , bool *runp , int shape){
	
	int R , G  , B;
	i = 2;
	j = 5;
	switch (shape) {

		case 0: i_1 = 2 ; i_2 = 2 ; i_3 = 2 ; j_1 = 3 ; j_2 = 4 ; j_3 = 6 ; R = 0 ; G = 255 ; B = 255 ; break;
		case 1: i_1 = 1 ; i_2 = 1 ; i_3 = 2 ; j_1 = 4 ; j_2 = 5 ; j_3 = 4 ; R = 255 ; G = 255 ; B = 0 ; break;
		case 2: i_1 = 1 ; i_2 = 2 ; i_3 = 2 ; j_1 = 5 ; j_2 = 4 ; j_3 = 6 ; R = 153 ; G = 0 ; B = 153 ; break;
		case 3: i_1 = 1 ; i_2 = 2 ; i_3 = 2 ; j_1 = 6 ; j_2 = 4 ; j_3 = 6 ; R = 255 ; G = 128 ; B = 0 ; break;
		case 4: i_1 = 1 ; i_2 = 2 ; i_3 = 2 ; j_1 = 4 ; j_2 = 4 ; j_3 = 6 ; R = 0 ; G = 0 ; B = 255 ; break;
		case 5: i_1 = 1 ; i_2 = 1 ; i_3 = 2 ; j_1 = 4 ; j_2 = 5 ; j_3 = 6 ; R = 255 ; G = 0 ; B = 0 ; break;
		case 6: i_1 = 1 ; i_2 = 1 ; i_3 = 2 ; j_1 = 5 ; j_2 = 6 ; j_3 = 4 ; R = 0 ; G = 255 ; B = 0 ; break;
	}
	if (block[i][j].status == 1)
		*runp = false;
	if (block[i_1][j_1].status == 1)
		*runp = false;
	if (block[i_1][j_1].status == 1)
		*runp = false;
	if (block[i_1][j_1].status == 1)
		*runp = false;
	if (*runp == false)
		return;
	block[i][j].status = 2;
	block[i_1][j_1].status = 2;
	block[i_2][j_2].status = 2;
	block[i_3][j_3].status = 2;
	
	block[i][j].R = R;
	block[i_1][j_1].R = R;
	block[i_2][j_2].R = R;
	block[i_3][j_3].R = R;

	block[i][j].G = G;
	block[i_1][j_1].G = G;
	block[i_2][j_2].G = G;
	block[i_3][j_3].G = G;

	block[i][j].B = B;
	block[i_1][j_1].B = B;
	block[i_2][j_2].B = B;
	block[i_3][j_3].B = B;
}

void print (SDL_Surface *screen , int score , int level , int nextshape){

	SDL_FillRect (screen , NULL , 0x000);	
	for (int i=0 ; i<Row ; i++){
		
		for (int j=0 ; j<Col ; j++){
			
			if (block[i][j].status != 0){
				
				show_block (screen , block[i][j].x1 , block[i][j].y1 , block[i][j].x2 , block[i][j].y2 , block[i][j].R , block[i][j].G , block[i][j].B);
			}
		}
	}

	print_num (screen , 520 , 10 , score);
	print_num (screen , 530 , 10 , level);

	SDL_Flip (screen);
}

void block_reset (SDL_Surface *screen){
	
	block[i][j].status = 0;
	block[i_1][j_1].status = 0;
	block[i_2][j_2].status = 0;
	block[i_3][j_3].status = 0;
}

void block_set (SDL_Surface *screen){
	
	block[i][j].status = 2;
	block[i_1][j_1].status = 2;
	block[i_2][j_2].status = 2;
	block[i_3][j_3].status = 2;
}

void block_color_set (SDL_Surface *screen , char a){

	if (a=='d'){

		block[i][j].R = block[i-1][j].R;
		block[i_1][j_1].R = block[i_1-1][j_1].R;
		block[i_2][j_2].R = block[i_2-1][j_2].R;
		block[i_3][j_3].R = block[i_3-1][j_3].R;

		block[i][j].G = block[i-1][j].G;
		block[i_1][j_1].G = block[i_1-1][j_1].G;
		block[i_2][j_2].G = block[i_2-1][j_2].G;
		block[i_3][j_3].G = block[i_3-1][j_3].G;

		block[i][j].B = block[i-1][j].B;
		block[i_1][j_1].B = block[i_1-1][j_1].B;
		block[i_2][j_2].B = block[i_2-1][j_2].B;
		block[i_3][j_3].B = block[i_3-1][j_3].B;
	}
	
	else	if (a=='l'){

			block[i][j].R = block[i][j+1].R;
			block[i_1][j_1].R = block[i_1][j_1+1].R;
			block[i_2][j_2].R = block[i_2][j_2+1].R;
			block[i_3][j_3].R = block[i_3][j_3+1].R;

			block[i][j].G = block[i][j+1].G;
			block[i_1][j_1].G = block[i_1][j_1+1].G;
			block[i_2][j_2].G = block[i_2][j_2+1].G;
			block[i_3][j_3].G = block[i_3][j_3+1].G;

			block[i][j].B = block[i][j+1].B;
			block[i_1][j_1].B = block[i_1][j_1+1].B;
			block[i_2][j_2].B = block[i_2][j_2+1].B;
			block[i_3][j_3].B = block[i_3][j_3+1].B;
		}
		
	else	if (a=='r'){

			block[i][j].R = block[i][j-1].R;
			block[i_1][j_1].R = block[i_1][j_1-1].R;
			block[i_2][j_2].R = block[i_2][j_2-1].R;
			block[i_3][j_3].R = block[i_3][j_3-1].R;

			block[i][j].G = block[i][j-1].G;
			block[i_1][j_1].G = block[i_1][j_1-1].G;
			block[i_2][j_2].G = block[i_2][j_2-1].G;
			block[i_3][j_3].G = block[i_3][j_3-1].G;

			block[i][j].B = block[i][j-1].B;
			block[i_1][j_1].B = block[i_1][j_1-1].B;
			block[i_2][j_2].B = block[i_2][j_2-1].B;
			block[i_3][j_3].B = block[i_3][j_3-1].B;
		}
}

void move_down (SDL_Surface *screen , bool *movedownp) {
	
	if (i == Row - 1 || i_1 == Row - 1 || i_2 == Row - 1 || i_3 == Row - 1){
		*movedownp = false;
		return;
	}

	bool k = true;
	if (block[i+1][j].status == 1)
		k = false;
	if (block[i_1+1][j_1].status == 1)
		k = false;
	if (block[i_2+1][j_2].status == 1)
		k = false;
	if (block[i_3+1][j_3].status == 1)
		k = false;

	if (k){

		block_reset (screen);
		
		i++; i_1++; i_2++; i_3++;
		
		block_set (screen);
		block_color_set (screen , 'd');
	}
	
	else
		*movedownp = false;
}

void move_left (SDL_Surface *screen) {
	
	if (j == 0 || j_1 == 0 || j_2 == 0 || j_3 == 0)
		return;

	bool k = true;
	if (block[i][j-1].status == 1)
		k = false;
	if (block[i_1][j_1-1].status == 1)
		k = false;
	if (block[i_2][j_2-1].status == 1)
		k = false;
	if (block[i_3][j_3-1].status == 1)
		k = false;
	if (k){

		block_reset (screen);
		
		j--; j_1--; j_2--; j_3--;
		
		block_set (screen);
		block_color_set (screen , 'l');
	}
}

void move_right (SDL_Surface *screen) {
	
	if (j == Col - 1 || j_1 == Col - 1 || j_2 == Col - 1 || j_3 == Col - 1)
		return;

	bool k = true;
	if (block[i][j+1].status == 1)
		k = false;
	if (block[i_1][j_1+1].status == 1)
		k = false;
	if (block[i_2][j_2+1].status == 1)
		k = false;
	if (block[i_3][j_3+1].status == 1)
		k = false;
	if (k){

		block_reset (screen);
		
		j++; j_1++; j_2++; j_3++;
		
		block_set (screen);
		block_color_set (screen , 'r');
	}
}

void freeze_block (SDL_Surface *screen) {
	
	block[i][j].status = 1;
	block[i_1][j_1].status = 1;
	block[i_2][j_2].status = 1;
	block[i_3][j_3].status = 1;
}

void orbit_move (SDL_Surface *screen) {

	int x_1 = i_1 , x_2 = i_2 , x_3 = i_3;
	int y_1 = j_1 , y_2 = j_2 , y_3 = j_3;

	if (x_1==i){

		x_1 += (j - y_1); y_1 = j;
		if (x_1 < 0 || y_1 < 0 || x_1 >= Row || y_1 >= Col || block[x_1][y_1].status == 1)
			return;
	}

	else	if (y_1==j){

			y_1 += (x_1 - i); x_1 = i;
			if (x_1 < 0 || y_1 < 0 || x_1 >= Row || y_1 >= Col || block[x_1][y_1].status == 1)
				return;
		}

		else	if (x_1!=i && y_1!=j){

				if (x_1 < i && y_1 > j)
					y_1 -= 2;
				else	if (x_1 < i && y_1 < j)
						x_1 += 2;
					else	if (x_1 > i && y_1 < j)
							y_1 += 2;
						else	if (x_1 > i && y_1 > j)
								x_1 -= 2;
				if (x_1 < 0 || y_1 < 0 || x_1 >= Row || y_1 >= Col || block[x_1][y_1].status == 1)
					return;
			}
	if (x_2==i){

		x_2 += (j - y_2); y_2 = j;
		if (x_2 < 0 || y_2 < 0 || x_2 >= Row || y_2 >= Col || block[x_2][y_2].status == 1)
			return; 
	}

	else	if (y_2==j){

			y_2 += (x_2 - i); x_2 = i;
			if (x_2 < 0 || y_2 < 0 || x_2 >= Row || y_2 >= Col || block[x_2][y_2].status == 1)
				return;
		}

		else	if (x_2!=i && y_2!=j){

				if (x_2 < i && y_2 > j)
					y_2 -= 2;
				else	if (x_2 < i && y_2 < j)
						x_2 += 2;
					else	if (x_2 > i && y_2 < j)
							y_2 += 2;
						else	if (x_2 > i && y_2 > j)
								x_2 -= 2;
				if (x_2 < 0 || y_2 < 0 || x_2 >= Row || y_2 >= Col || block[x_2][y_2].status == 1)
					return;
			}
	if (x_3==i){
	
		x_3 += (j - y_3); y_3 = j;
		if (x_3 < 0 || y_3 < 0 || x_3 >= Row || y_3 >= Col || block[x_3][y_3].status == 1)
			return;
	}

	else	if (y_3==j){

			y_3 += (x_3 - i); x_3 = i;
			if (x_3 < 0 || y_3 < 0 || x_3 >= Row || y_3 >= Col || block[x_3][y_3].status == 1)
				return;
		}

		else	if (x_3!=i && y_3!=j){

				if (x_3 < i && y_3 > j)
					y_3 -= 2;
				else	if (x_3 < i && y_3 < j)
						x_3 += 2;
					else	if (x_3 > i && y_3 < j)
							y_3 += 2;
						else	if (x_3 > i && y_3 > j)
								x_3 -= 2;
				if (x_3 < 0 || y_3 < 0 || x_3 >= Row || y_3 >= Col || block[x_3][y_3].status == 1)
					return;
			}

	block_reset (screen);
	block[x_1][y_1].R = block[i_1][j_1].R;
	block[x_1][y_1].G = block[i_1][j_1].G;
	block[x_1][y_1].B = block[i_1][j_1].B;

	block[x_2][y_2].R = block[i_2][j_2].R;
	block[x_2][y_2].G = block[i_2][j_2].G;
	block[x_2][y_2].B = block[i_2][j_2].B;

	block[x_3][y_3].R = block[i_3][j_3].R;
	block[x_3][y_3].G = block[i_3][j_3].G;
	block[x_3][y_3].B = block[i_3][j_3].B;
	
	i_1 = x_1 ; i_2 = x_2 ; i_3 = x_3;
	j_1 = y_1 ; j_2 = y_2 ; j_3 = y_3;

	block_set (screen);
}

void del_line (SDL_Surface *screen , int *scorePtr){

	for (int i = Row - 1 ; i >= 0 ; i--){
		
		bool line = true;
		for (int j=0 ; j < Col ; j++){

			if (block[i][j].status != 1){

				line = false;
				break;
			}
		}
		
		if (line){

			*scorePtr += 10;
			for (int j = 0 ; j < Col ; j++){
				
				block[i][j].status = 0;
				block[i][j].R = 0;
				block[i][j].G = 0;
				block[i][j].B = 0;
			}
			for (int x = i-1 ; x >= 0 ; x--){

				for (int j = 0 ; j < Col ; j++){

					block[x+1][j].status = block[x][j].status;
					block[x+1][j].R = block[x][j].R;
					block[x+1][j].G = block[x][j].G;
					block[x+1][j].B = block[x][j].B;
				}
			}
		}
	}
}

void level_up (SDL_Surface *screen , int score , int *levelPtr){
	
	if (score % 100 == 0)
		*levelPtr++;
}

void print_shape (SDL_Surface *screen , int nextshape){

	int R , G , B;
	switch (nextshape) {

		case 0: R = 0 ; G = 255 ; B = 255 ;break;
		case 1: R = 255 ; G = 255 ; B = 0 ;break;
		case 2: R = 153 ; G = 0 ; B = 153 ;break;
		case 3: R = 255 ; G = 128 ; B = 0 ;break;
		case 4: R = 0 ; G = 0 ; B = 255 ;break;
		case 5: R = 255 ; G = 0 ; B = 0 ;break;
		case 6: R = 0 ; G = 255 ; B = 0 ;break;
	}

}






















