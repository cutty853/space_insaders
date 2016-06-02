void test_surface(SDL_Surface* surface, int ref_err);
void test_police(TTF_Font* police, int ref_err);

void init_pos(SDL_Rect* position, int x, int y);
void init_vaisseau(_vaisseau *vaisseau, _intelligence intelligence, _comportement comportement, int tab_init_val[9]);
void init_tir (_vaisseau *vaisseau);

void pause();
int aleatoire(int mini, int maxi);
void mouvement_vaisseau(int action, int sens, _vaisseau *vaisseau);

void deplace_curseur(FILE* fichier);
int recup_int(FILE* fichier);
int recup_string(FILE* fichier);
