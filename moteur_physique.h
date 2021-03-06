int col_point_boite (_point *x, SDL_Rect *boite);
int col_point_cercle (int x, int y, _cercle* c);
int col_boite_boite (SDL_Rect *repere, SDL_Rect *boite);
int col_cercle_cercle (_cercle *c1, _cercle *c2);
int col_polygone_cercle (_polygone *p, _cercle* c);
int col_polygone_point (_polygone *p, _point* x);
int col_aabb_cercle (SDL_Rect* aabb, _cercle *c);

void supprime_hitbox (_hitbox* h);

void calcul_pos_hitbox_vaisseau(_vaisseau *v);
void calcul_pos_hitbox_tir(_tir* t);

void transform_aabb_polygone(SDL_Rect* aabb, _polygone *p);

SDL_Rect eff_hitbox_aabb(SDL_Rect *aabb, SDL_Surface *save_screen, SDL_Surface *ecran);

SDL_Rect aff_hitbox_aabb (SDL_Rect *aabb, SDL_Surface *ecran);
SDL_Rect aff_hitbox_cercle (_cercle *c, SDL_Surface* ecran, int x, int y);
