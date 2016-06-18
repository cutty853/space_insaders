void test_surface(SDL_Surface* surface, int ref_err);
void test_police(TTF_Font* police, int ref_err);

void init_pos(SDL_Rect* position, int x, int y);
void init_vaisseau(_vaisseau *vaisseau, int tab_init_val[NBR_VALEURS_TRANSMISES]);
void init_tir (_vaisseau *vaisseau);

void pause();
int aleatoire(int mini, int maxi);
void mouvement_vaisseau(int action, int sens, _vaisseau *vaisseau);

void deplace_curseur(FILE* fichier);
void trouve_titre(FILE *fichier, int taille_string, char titre_voulu[]);
int recup_int(FILE* fichier);
int recup_string(FILE* fichier);

void degat_tir(_vaisseau *vaisseau_touche, _vaisseau *vaisseau_tireur);
void degat_collisions(_vaisseau *vaisseau);
void gestion_distance_tir(_vaisseau *vaisseau);

void calcul_pos_bouclier(_vaisseau *vaisseau);
void calcul_pos_vie(_vaisseau *vaisseau);
void calcul_pos_vaisseau(_vaisseau *vaisseau, SDL_Surface *ecran);
void calcul_pos_tir(_vaisseau *vaisseau);

void supprime_vaisseau (_vaisseau *vaisseau);
