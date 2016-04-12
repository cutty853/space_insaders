void test_surface(SDL_Surface* surface, int ref_err);
void pause();
SDL_Rect* aff_vaisseau(SDL_Surface *ecran, SDL_Surface *surface_vaisseau, _vaisseau *vaisseau, SDL_Surface* save_screen, int *etat_rotation);
void degrade(_degrade prop_deg, SDL_Surface *ecran, SDL_Rect pos_degrade);
