SDL_Rect* init_niveau_general(int *niveau, int *nbr_ia, int *nbr_pos_to_up_tir_ia);
_vaisseau* init_niveau_ia(int *niveau, int nbr_ia);
_vaisseau init_niveau_joueur(int *niveau);

_explosion init_explosions_joueur(int nbr_ia);
_explosion* init_explosions_ia(int nbr_ia);

void init_pos(SDL_Rect* position, int x, int y);

void init_vaisseau(_vaisseau *vaisseau, int tab_init_val[NBR_VALEURS_TRANSMISES]);

void init_tir(_vaisseau *vaisseau);

void init_hitbox(_hitbox* h, int xCercle, int yCercle, int rCercle, int nb_points, int xAABB, int yAABB, int wAABB, int hAABB);
