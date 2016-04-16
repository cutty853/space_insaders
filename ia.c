#ifndef DEF_INCLUDE
    #define DEF_INCLUDE
    #include <stdlib.h>
    #include <stdio.h>
    #include <math.h>
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

void barre_bouclier_ia(SDL_Surface *ecran, _vaisseau *v_ia) {
    v_ia->bouclier.position.x = v_ia->position.x;
    v_ia->bouclier.position.y = (v_ia->position.y)+70; /// +70 = taille verticale du vaisseau.

    switch (v_ia->bouclier.charge) {/// "Affichage" de la barre du bouclier
        case VIDE:
            ///plus de bouclier
            break;
        case BAS:
            v_ia->bouclier.sprite = IMG_Load("images/ia_bouclier_BAS.png");
            break;
        case MOYEN:
            v_ia->bouclier.sprite = IMG_Load("images/ia_bouclier_MOYEN.png");
            break;
        case HAUT:
            v_ia->bouclier.sprite = IMG_Load("images/ia_bouclier_HAUT.png");
            break;
    }
    test_surface(v_ia->bouclier.sprite, 100); ///Verif chargement.
}
void barre_vie_ia(SDL_Surface *ecran, _vaisseau *v_ia) {
    v_ia->vie.sprite = SDL_CreateRGBSurface(SDL_HWSURFACE, 40, 5, 32, 0, 0, 0, 0);

    v_ia->vie.position.x = v_ia->position.x;
    v_ia->vie.position.y = (v_ia->position.y)+70+5; /// +70 = taille verticale du vaisseau, +5 = epaisseur de la barre de bouclier.

    switch (v_ia->vie.charge) {/// "Affichage" de la barre de vie
        case VIDE:
            /// plus de vie
            break;
        case BAS:
            SDL_FillRect(v_ia->vie.sprite, NULL, SDL_MapRGB((v_ia->vie.sprite)->format, 255, 0, 0)); ///Rouge
            break;
        case MOYEN:
            SDL_FillRect(v_ia->vie.sprite, NULL, SDL_MapRGB((v_ia->vie.sprite)->format, 255, 165, 0)); ///Orange
            break;
        case HAUT:
            SDL_FillRect(v_ia->vie.sprite, NULL, SDL_MapRGB((v_ia->vie.sprite)->format, 0, 255, 0)); ///Bleu
            break;
    }
}


void tour_ia(_vaisseau *v_ia, _vaisseau *v_joueur, SDL_Surface *ecran){
    int ancienne_pos_relative = 0, nouv_pos_relative = 0, sens_de_rotation = 0;

    if(v_ia->angle >= 360) /// un tour complet a �t� fait.
        v_ia->angle -= 360;
    else if(v_ia->angle < 0)
        v_ia->angle += 360;

    ancienne_pos_relative = nouv_pos_relative;
    nouv_pos_relative = compare_position(v_ia, v_joueur);
    switch (nouv_pos_relative){
        case BAS_DROITE:
            v_ia->angle_de_decalage = (180/PI)*(atan(fabs(v_joueur->position.x - v_ia->position.x) / fabs(v_ia->position.y - v_joueur->position.y)));
            v_ia->angle_de_decalage += 0; /// d�calage de l'arctan.
            break;
        case HAUT_DROITE:
            v_ia->angle_de_decalage = (180/PI)*(atan(fabs(v_ia->position.y - v_joueur->position.y) / fabs(v_joueur->position.x - v_ia->position.x)));
            v_ia->angle_de_decalage += 90; /// d�calage de l'arctan.
            break;
        case HAUT_GAUCHE:
            v_ia->angle_de_decalage = (180/PI)*(atan(fabs(v_joueur->position.x - v_ia->position.x) / fabs(v_ia->position.y - v_joueur->position.y)));
            v_ia->angle_de_decalage += 180; /// d�calage de l'arctan.
            break;
        case BAS_GAUCHE:
            v_ia->angle_de_decalage = (180/PI)*(atan(fabs(v_ia->position.y - v_joueur->position.y) / fabs(v_joueur->position.x - v_ia->position.x)));
            v_ia->angle_de_decalage += 270; /// d�calage de l'arctan.
            break;
    }


    /*if(ancienne_pos_relative == BAS_GAUCHE && nouv_pos_relative == BAS_DROITE)
        sens_de_rotation = POSITIF;
    else if(ancienne_pos_relative == BAS_DROITE && nouv_pos_relative == BAS_GAUCHE)
        sens_de_rotation = NEGATIF;*/

    if((v_ia->angle+2) < v_ia->angle_de_decalage || (v_ia->angle-2) > v_ia->angle_de_decalage){ /// +2 et -2 = imprecision.
        sens_de_rotation = choix_sens_de_rotation(v_ia, ancienne_pos_relative, nouv_pos_relative);
        mouvement_ia(TOURNE, sens_de_rotation, v_ia, v_joueur);
    }else if(v_ia->vitesse < v_joueur->vitesse){
        mouvement_ia(AVANCE, DROIT, v_ia, v_joueur);
    }else if(v_ia->vitesse > v_joueur->vitesse){
        mouvement_ia(RECUL, DROIT, v_ia, v_joueur);
    }else{
        mouvement_ia(RIEN, DROIT, v_ia, v_joueur);
    }

    barre_bouclier_ia(ecran, v_ia);
    barre_vie_ia(ecran, v_ia);
}

int compare_position(_vaisseau *v_ia, _vaisseau *v_joueur){ /// Position du vaisseau ia par rapport au vaisseau joueur.
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
int choix_sens_de_rotation(_vaisseau *v_ia, int ancienne_pos_relative, int nouv_pos_relative){ /// Choix du sens de rotation: positif = sens trigo.
    if(v_ia->angle < v_ia->angle_de_decalage)
        return POSITIF;
    else
        return NEGATIF;
}

void mouvement_ia (int action, int sens, _vaisseau *v_ia, _vaisseau *v_joueur){
    switch (action){
        case AVANCE:
            if (v_ia->vitesse < v_ia->vitesse_max)
                v_ia->vitesse += v_ia->acceleration;
            break;
        case RECUL:
            if (v_ia->vitesse > v_ia->vitesse_min)
                v_ia->vitesse -= v_ia->acceleration;
            break;
        case TOURNE:
            v_ia->etat_rotation = 1;
            if(sens == POSITIF)
                v_ia->angle += v_ia->vitesse_rotation;
            else
            v_ia->angle -= v_ia->vitesse_rotation;
            break;
        case RIEN:
            /// L'ia choisi de ne pas bouger.
            break;
    }
}
