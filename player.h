void vitesse_joueur(_vaisseau *v_joueur, int sens);
void aff_player(SDL_Surface *ecran, SDL_Surface *surface_player, _vaisseau *v_player);

void charge_sprite_explosion (_explosion *boom);
SDL_Rect aff_explosion (SDL_Surface *ecran, _explosion *boom, _vaisseau vaisseau);
void decharge_sprite_explosion (_explosion *boom);
