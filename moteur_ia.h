void tour_ia(_vaisseau *v_ia, _vaisseau *v_joueur, SDL_Surface *ecran);

_comportement ia_cherche(_vaisseau *v_ia, _vaisseau *v_joueur);
_comportement ia_attaque(_vaisseau *v_ia, _vaisseau *v_joueur);

int compare_position(_vaisseau *v_ia, _vaisseau *v_joueur);
int choix_sens_de_rotation(_vaisseau *v_ia, int nouv_pos_relative);
int trouve_max(int tourne, int accelere, int ralenti);
void mouvement_ia (int action, int sens, _vaisseau *v_ia, _vaisseau *v_joueur);

void tir_ia(_vaisseau *v_ia);
