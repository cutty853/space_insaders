#ifndef DEF_INCLUDE
    #define DEF_INCLUDE
    #include <stdlib.h>
    #include <stdio.h>
    #include <SDL/SDL.h>
    #include <SDL/SDL_image.h>
    #include <SDL/SDL_ttf.h>
    #include <SDL/SDL_rotozoom.h>
    #include "structure.h"
    #include "utilitaire.h"
    #include "jeu.h"
    #include "ia.h"
    #include "player.h"


    #include "constantes.h"
#endif


void barre_bouclier_ia(SDL_Surface *ecran, _vaisseau v_ia) {
    SDL_Surface *barre_bouclier=NULL;
    SDL_Rect pos_barre_bouclier;
    pos_barre_bouclier.x = v_ia.position.x;
    pos_barre_bouclier.y = (v_ia.position.y)+60;

    /// Affichage de la barre du bouclier
    switch (v_ia.bouclier) {
        case BAS:
            barre_bouclier = IMG_Load("images/bouclier_BAS.jpg");
            test_surface(barre_bouclier, 100); //Verif chargement.
            break;
        case MOYEN:
            barre_bouclier = IMG_Load("images/bouclier_MOYEN.jpg");
            test_surface(barre_bouclier, 100); //Verif chargement.
            break;
        case HAUT:
            barre_bouclier = IMG_Load("images/bouclier_HAUT.jpg");
            test_surface(barre_bouclier, 100); //Verif chargement.
            break;
    }
    SDL_BlitSurface(barre_bouclier, NULL, ecran, &pos_barre_bouclier);
    SDL_Flip(ecran);
    SDL_FreeSurface(barre_bouclier);
}
void barre_vie_ia(SDL_Surface *ecran, _vaisseau v_ia) {
    SDL_Surface *barre_vie=NULL;
    SDL_Rect pos_barre_vie;
    pos_barre_vie.x = v_ia.position.x;
    pos_barre_vie.y = (v_ia.position.y)+20+61;

    /// Affichage de la barre de vie
    barre_vie = SDL_CreateRGBSurface(SDL_HWSURFACE, 40, 5, 32, 0, 0, 0, 0);
    switch (v_ia.vie) {
        case BAS:
            SDL_FillRect(barre_vie, NULL, SDL_MapRGB(barre_vie->format, 255, 0, 0)); ///Rouge
            break;
        case MOYEN:
            SDL_FillRect(barre_vie, NULL, SDL_MapRGB(barre_vie->format, 255, 165, 0)); ///Orange
            break;
        case HAUT:
            SDL_FillRect(barre_vie, NULL, SDL_MapRGB(barre_vie->format, 0, 255, 0)); ///Bleu
            break;
    }
    SDL_BlitSurface(barre_vie, NULL, ecran, &pos_barre_vie);
    SDL_Flip(ecran);
    SDL_FreeSurface(barre_vie);
}

void tour_ia(_vaisseau *v_ia, _vaisseau *v_joueur, SDL_Surface *ecran){
    int position_relative = compare_position(v_ia, v_joueur);
    int sens_de_rotation = choix_sens_de_rotation(v_ia, position_relative);

    switch (position_relative){
        case EN_HAUT:
            mouvement_ia(AVANCE, DROIT, v_ia);
            break;
        case EN_BAS:
            mouvement_ia(AVANCE, DROIT, v_ia);
            break;
        case DROITE:
            mouvement_ia(AVANCE, DROIT, v_ia);
            break;
        case GAUCHE:
            mouvement_ia(AVANCE, DROIT, v_ia);
            break;
        case BAS_DROITE:
            mouvement_ia(TOURNE, sens_de_rotation, v_ia);
            break;
        case BAS_GAUCHE:
            mouvement_ia(TOURNE, sens_de_rotation, v_ia);
            break;
        case HAUT_DROITE:
            mouvement_ia(TOURNE, sens_de_rotation, v_ia);
            break;
        case HAUT_GAUCHE:
            mouvement_ia(TOURNE, sens_de_rotation, v_ia);
            break ;
    }
}

int compare_position(_vaisseau *v_ia, _vaisseau *v_joueur){
    int pos_x_relative = v_ia->position.x - v_joueur->position.x;
    int pos_y_relative = v_ia->position.y - v_joueur->position.y;
    if(pos_x_relative > 0 && pos_y_relative > 0)
        return BAS_DROITE;
    else if(pos_x_relative > 0 && pos_y_relative < 0)
        return HAUT_DROITE;
    else if(pos_x_relative < 0 && pos_y_relative > 0)
        return BAS_GAUCHE;
    else if(pos_x_relative < 0 && pos_y_relative < 0)
        return HAUT_GAUCHE;
    else if(pos_x_relative == 0 && pos_y_relative > 0)
        return EN_BAS;
    else if(pos_x_relative == 0 && pos_y_relative < 0)
        return EN_HAUT;
    else if(pos_x_relative > 0 && pos_y_relative == 0)
        return DROITE;
    else if(pos_x_relative < 0 && pos_y_relative == 0)
        return GAUCHE;
    else
        return 666;
}
int choix_sens_de_rotation(_vaisseau *v_ia, int compare_position){
    if(compare_position == BAS_DROITE && v_ia->angle > 180)
        return POSITIF;
    else if(compare_position == BAS_DROITE && v_ia->angle > 180)
        return NEGATIF;
    else if(compare_position == BAS_GAUCHE && v_ia->angle < 180)
        return NEGATIF;
    else if(compare_position == BAS_GAUCHE && v_ia->angle > 180)
        return POSITIF;
    else if(compare_position == HAUT_DROITE && v_ia->angle > 180)
        return NEGATIF;
    else if(compare_position == HAUT_DROITE && v_ia->angle > 180)
        return POSITIF;
    else if(compare_position == HAUT_GAUCHE && v_ia->angle < 180)
        return POSITIF;
    else if(compare_position == HAUT_GAUCHE && v_ia->angle > 180)
        return NEGATIF;
    else
        return 666;
}

void mouvement_ia (int action, int sens, _vaisseau *v_ia){
    switch (action){
        case AVANCE:
            /// Ne fait rien car maintient de vitesse et pas de rotation.
            break;
        case RECUL:
            v_ia->vitesse -= v_ia->acceleration;
            break;
        case TOURNE:
            if(sens == POSITIF)
                v_ia->angle += v_ia->vitesse_rotation;
            else
                v_ia->angle -= v_ia->vitesse_rotation;
            break;
    }
}


