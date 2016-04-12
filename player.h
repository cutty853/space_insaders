void vitesse_player(_vaisseau *v_player, int sens);
SDL_Rect* aff_player(SDL_Surface *ecran, SDL_Surface *surface_player, _vaisseau *v_player, SDL_Surface *save_screen, int *etat_rotation);
SDL_Rect aff_console (SDL_Surface *ecran, _vaisseau vaisseau, SDL_Surface* save_screen, TTF_Font *police_texte);
