void cadre(SDL_Surface *ecran, int pos_x, int pos_y);
int menu(SDL_Surface *ecran);
void charge_niveau (SDL_Surface *ecran);

///Déclaration des fonctions du joueurs:
void barre_vie_joueur(SDL_Surface* ecran, _vaisseau v_joueur);
void barre_bouclier_joueur(SDL_Surface* ecran, _vaisseau v_joueur);
void play(SDL_Surface *ecran);

///Déclaration des fonctions de l'ia:
void tour_ia(_vaisseau *v_ia, _vaisseau *v_joueur, SDL_Surface *ecran);
