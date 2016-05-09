void cadre(SDL_Surface *ecran, int pos_x, int pos_y);
int menu(SDL_Surface *ecran);
void charge_niveau (SDL_Surface *ecran);

SDL_Rect eff_vie(SDL_Surface *ecran, _vaisseau *vaisseau, SDL_Surface *save_screen);
SDL_Rect eff_bouclier(SDL_Surface *ecran, _vaisseau *vaisseau, SDL_Surface *save_screen);
SDL_Rect eff_vaisseau(SDL_Surface *ecran, _vaisseau *vaisseau, SDL_Surface *save_screen);
SDL_Rect eff_tir(SDL_Surface *ecran, SDL_Surface *save_screen, _vaisseau *vaisseau);

void calcul_pos_bouclier(_vaisseau *vaisseau);
void calcul_pos_vie(_vaisseau *vaisseau);
void calcul_pos_vaisseau(_vaisseau *vaisseau, SDL_Surface *ecran);
void calcul_pos_tir(_vaisseau *vaisseau);

SDL_Rect aff_bouclier(SDL_Surface *ecran, _vaisseau *vaisseau);
SDL_Rect aff_vie(SDL_Surface *ecran, _vaisseau *vaisseau);
SDL_Rect aff_vaisseau(SDL_Surface *ecran, _vaisseau *vaisseau, SDL_Surface *save_screen);
SDL_Rect aff_tir (SDL_Surface *ecran, _vaisseau *vaisseau);
SDL_Rect aff_explosion (SDL_Surface *ecran, _explosion *boom, _vaisseau vaisseau);

void charge_sprite_bouclier(_vaisseau *vaisseau);
void charge_sprite_vie(_vaisseau *vaisseau);
void charge_sprite_tir (_vaisseau *vaisseau);
void charge_sprite_explosion (_explosion *boom);
void decharge_sprite_tir (_vaisseau *vaisseau);

void decharge_sprite_vie(_vaisseau *vaisseau);
void decharge_sprite_bouclier(_vaisseau *vaisseau);
void decharge_sprite_explosion (_explosion *boom);

void degrade(_degrade prop_deg, SDL_Surface *ecran, SDL_Rect pos_degrade);

SDL_Rect aff_console (SDL_Surface *ecran, _vaisseau vaisseau,  SDL_Surface* save_screen, TTF_Font *police_texte);

void barre_vie_joueur(SDL_Surface *ecran, _vaisseau v_joueur);
void barre_bouclier_joueur(SDL_Surface *ecran, _vaisseau v_joueur);
