void tour_ia(_vaisseau *v_ia, _vaisseau *v_joueur, SDL_Surface *ecran);

_comportement recherche_ia(_vaisseau *v_ia, _vaisseau *v_joueur);

void ia_cherche(_vaisseau *v_ia, _vaisseau *v_joueur);
void ia_attaque(_vaisseau *v_ia, _vaisseau *v_joueur);
void ia_fuit(_vaisseau *v_ia, _vaisseau *v_joueur);

int compare_position(_vaisseau *v_ia, _vaisseau *v_joueur);
int choix_sens_de_rotation(_vaisseau *v_ia, int pos_relative);

void tir_ia(_vaisseau *v_ia);
