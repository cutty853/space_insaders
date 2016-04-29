#ifndef DEF_INCLUDE5
#define DEF_INCLUDE5

#include "structure.h"

#endif // DEF_INCLUDE

void test_surface(SDL_Surface* surface, int ref_err);
void test_police(TTF_Font* police, int ref_err);

void init_pos(SDL_Rect* position, int x, int y);
void init_vaisseau(_vaisseau *vaisseau, int intelligence, int poid, float vitesse, float acceleration, int v_max, int bouclier, int vie, int arme, int position_x, int position_y, int v_rotation, int angle);
void init_tir (_tir *pew, _vaisseau vaisseau);

void pause();
