///Déclaration des fonctions de l'ia:
int compare_position(_vaisseau *v_ia, _vaisseau *v_joueur);
int choix_sens_de_rotation(_vaisseau *v_ia, int nouv_pos_relative);
void mouvement_ia(int action, int sens, _vaisseau *v_ia, _vaisseau *v_joueur);
int trouve_max(int tourne, int accelere, int ralenti);
void tir_ia(_vaisseau *v_ia); //Prototype
