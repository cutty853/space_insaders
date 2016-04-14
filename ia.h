///Déclaration des fonctions de l'ia:
void barre_bouclier_ia(SDL_Surface* ecran, _vaisseau v_ia);
void barre_vie_ia(SDL_Surface* ecran, _vaisseau v_ia);
int compare_position(_vaisseau *v_ia, _vaisseau *v_joueur);
int choix_sens_de_rotation(_vaisseau *v_ia, int ancienne_pos_relative, int nouv_pos_relative);
void mouvement_ia(int action, int sens, _vaisseau *v_ia, _vaisseau *v_joueur);
