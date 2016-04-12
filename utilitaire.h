void test_surface(SDL_Surface* surface, int ref_err);
void init_pos(SDL_Rect* position, int x, int y);
void pause();
SDL_Rect* aff_vaisseau(SDL_Surface *ecran, SDL_Surface *surface_vaisseau, _vaisseau *vaisseau, SDL_Surface* save_screen, int *etat_rotation);
void degrade(_degrade prop_deg, SDL_Surface *ecran, SDL_Rect pos_degrade);
void init_vaisseau(_vaisseau *vaisseau, int poid, int vitesse, int acceleration, int v_max, int bouclier, int vie, int arme, int position_x, int position_y, int v_rotation, int angle);
SDL_Rect aff_console (SDL_Surface *ecran, _vaisseau vaisseau,  SDL_Surface* save_screen, TTF_Font *police_texte);
