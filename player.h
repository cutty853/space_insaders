void vitesse_joueur(_vaisseau *v_joueur, int sens);
void charge_sprite_explosion (_explosion *boom);
SDL_Rect eff_vaisseau(SDL_Surface *ecran, _vaisseau *vaisseau, SDL_Surface *save_screen);
SDL_Rect aff_explosion (SDL_Surface *ecran, _vaisseau *vaisseau);
void decharge_sprite_explosion (_explosion *boom);
void charge_sprite_laser(_tir *tir,_vaisseau vaisseau);
void charge_tir (_tir *tir, _vaisseau vaisseau);
SDL_Rect eff_tir (SDL_Surface *ecran, _tir *tir, SDL_Surface *save_screen);
SDL_Rect aff_tir (SDL_Surface *ecran, _tir *tir);
