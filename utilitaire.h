void test_surface(SDL_Surface* surface, int ref_err);
void test_police(TTF_Font* police, int ref_err);
void init_pos(SDL_Rect* position, int x, int y);
void init_vaisseau(_vaisseau *vaisseau, int intelligence, int poid, float vitesse, float acceleration, int v_max, int bouclier, int vie, int arme, int position_x, int position_y, int v_rotation, int angle);

SDL_Rect eff_vie(SDL_Surface *ecran, _vaisseau *vaisseau, SDL_Surface *save_screen);
SDL_Rect eff_bouclier(SDL_Surface *ecran, _vaisseau *vaisseau, SDL_Surface *save_screen);
SDL_Rect eff_vaisseau(SDL_Surface *ecran, _vaisseau *vaisseau, SDL_Surface *save_screen);

SDL_Rect aff_bouclier(SDL_Surface *ecran, _vaisseau *vaisseau);
SDL_Rect aff_vie(SDL_Surface *ecran, _vaisseau *vaisseau);
SDL_Rect aff_vaisseau(SDL_Surface *ecran, _vaisseau *vaisseau, SDL_Surface *save_screen);

void charge_sprite_bouclier(_vaisseau *v_ia);
void charge_sprite_vie(_vaisseau *v_ia);
void decharge_sprite_vie(_vaisseau *v_ia);
void decharge_sprite_bouclier(_vaisseau *v_ia);

void pause();
void degrade(_degrade prop_deg, SDL_Surface *ecran, SDL_Rect pos_degrade);
SDL_Rect aff_console (SDL_Surface *ecran, _vaisseau vaisseau,  SDL_Surface* save_screen, TTF_Font *police_texte);
