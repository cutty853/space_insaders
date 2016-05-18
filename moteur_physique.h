int col_point_boite (int x, int y, SDL_Rect *boite);
int col_boite_boite (SDL_Rect *repere, SDL_Rect *boite);
int col_point_cercle (int x, int y, _cercle* c);
void col_tir_objet(SDL_Surface *ecran);
Uint32 obtenirPixel(SDL_Surface *surface, _vaisseau *vaisseau);
