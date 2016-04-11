void test_surface(SDL_Surface* surface, int ref_err);
void init_pos(SDL_Rect* position, int x, int y);
void pause();
void degrade(_degrade prop_deg, SDL_Surface *ecran, SDL_Rect pos_degrade);
void init_vaisseau(_vaisseau *vaisseau, int poid, int vitesse, int acceleration, int v_max, int bouclier, int vie, int arme, int position_x, int position_y, int v_rotation, int angle);
