#ifndef DEF_INCLUDE
    #define DEF_INCLUDE
    #include <stdlib.h>
    #include <stdio.h>
    #include <time.h>
    #include <math.h>

    #include <SDL/SDL.h>
    #include <SDL/SDL_image.h>
    #include <SDL/SDL_ttf.h>
    #include <SDL/SDL_rotozoom.h>

    #include "constantes.h"
    #include "structure.h"

    #include "moteur_ia.h"
    #include "utilitaire.h"
    #include "mode_combat.h"
    #include "mode_campagne.h"
    #include "moteur_joueur.h"
    #include "moteur_affichage.h"
#endif

_comportement recherche_ia(_vaisseau *v_ia, _vaisseau *v_joueur){ ///detecte le joueur à partir d'une certaine distance si non se balade aléatoirement.
    int distance_detection_horizontale = 300; /// En pixels.
    int distance_detection_verticale = 300; /// En pixels.
    if((v_ia->position.x - distance_detection_horizontale) < v_joueur->position.x && v_joueur->position.x < (v_ia->position.x + distance_detection_horizontale)){
        if((v_ia->position.y - distance_detection_verticale) < v_joueur->position.y && v_joueur->position.y < (v_ia->position.y + distance_detection_verticale))
            return ATTAQUE; /// Le joueur est dans le carré de detection.
    }
    return CHERCHE;
}

void ia_cherche(_vaisseau *v_ia, _vaisseau *v_joueur){/// Le vaisseau "cherche": il avance en permanence et tourne de manière aléatoire.
    int action = 0, direction = 0;
    action = aleatoire(1, 5);
    if(action >= 4){ /// permet de ne pas trop le faire tourner souvent.
        direction = aleatoire(1, 20); /// random entre 1 et 40. Permet qu'il ne change pas trop souvent de sens de rotation.
        if(direction <= 10)
            mouvement_ia (TOURNE, POSITIF, v_ia, v_joueur);
        else
            mouvement_ia (TOURNE, NEGATIF, v_ia, v_joueur);
    }else{/// il avance mes pas trop vite car il ne fait que "chercher".
        if(v_ia->vitesse <= (v_ia->vitesse_max/4))
            mouvement_ia (AVANCE, DROIT, v_ia, v_joueur);
        else
            mouvement_ia (RECUL, DROIT, v_ia, v_joueur);
    }
}
void ia_attaque(_vaisseau *v_ia, _vaisseau *v_joueur){/// Le vaisseau "attaque": il suit le joueur et tir dès qu'il n'a plus à tourner.
    int pos_relative = 0, sens_de_rotation = 0;

    pos_relative = compare_position(v_ia, v_joueur);
    switch (pos_relative){
        case BAS_DROITE:
            v_ia->angle_de_decalage = (180/PI)*(atan(fabs(v_joueur->position.x - v_ia->position.x) / fabs(v_ia->position.y - v_joueur->position.y)));
            v_ia->angle_de_decalage += 0; /// décalage de l'arctan.
            break;
        case HAUT_DROITE:
            v_ia->angle_de_decalage = (180/PI)*(atan(fabs(v_ia->position.y - v_joueur->position.y) / fabs(v_joueur->position.x - v_ia->position.x)));
            v_ia->angle_de_decalage += 90; /// décalage de l'arctan.
            break;
        case HAUT_GAUCHE:
            v_ia->angle_de_decalage = (180/PI)*(atan(fabs(v_joueur->position.x - v_ia->position.x) / fabs(v_ia->position.y - v_joueur->position.y)));
            v_ia->angle_de_decalage += 180; /// décalage de l'arctan.
            break;
        case BAS_GAUCHE:
            v_ia->angle_de_decalage = (180/PI)*(atan(fabs(v_ia->position.y - v_joueur->position.y) / fabs(v_joueur->position.x - v_ia->position.x)));
            v_ia->angle_de_decalage += 270; /// décalage de l'arctan.
            break;
    }

    if(v_ia->vitesse <= v_joueur->vitesse){/// atteint sa vitesse et essaye de la dépasser.
        mouvement_ia(AVANCE, DROIT, v_ia, v_joueur);
    }else{ /// a trop dépaasé sa vitsse
        mouvement_ia(RECUL, DROIT, v_ia, v_joueur);
    }
    if(v_ia->angle != v_ia->angle_de_decalage){
        sens_de_rotation = choix_sens_de_rotation(v_ia, pos_relative);
        mouvement_ia(TOURNE, sens_de_rotation, v_ia, v_joueur);
    }else{
        if(v_ia->tir.etat != 1){
            tir_ia(v_ia);
        }
    }
}
void ia_fuit(_vaisseau *v_ia, _vaisseau *v_joueur){/// Le vaisseau "fuit": il va se caher derrière les obstacles de la carte.
    exit(00000); // Pas encore fait, il faudrait déjà qu'elle est un quelque part où fuire (obstacle).
}

void tour_ia(_vaisseau *v_ia, _vaisseau *v_joueur, SDL_Surface *ecran){
    if(v_ia->angle >= 360) /// un tour complet a été fait.
        v_ia->angle -= 360;
    else if(v_ia->angle < 0)
        v_ia->angle += 359;

    switch(v_ia->comportement){
        case CHERCHE:
            ia_cherche(v_ia, v_joueur);
            v_ia->comportement = recherche_ia(v_ia, v_joueur);
            if(v_ia->comportement != CHERCHE){/// l'ia a trouvé et choisi de d'engager le combat ou non.
                if(v_ia->vie.charge == BAS && v_ia->bouclier.charge == VIDE)
                    v_ia->comportement = FUIT;
                else
                    v_ia->comportement = ATTAQUE;
            }
            break;
        case ATTAQUE:
            ia_attaque(v_ia, v_joueur);
            break;
        case FUIT:
            ia_fuit(v_ia, v_joueur);
            break;
        default:
            exit(666);
            break;
    }
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
int choix_sens_de_rotation(_vaisseau *v_ia, int pos_relative){ /// Choix du sens de rotation: positif = sens trigo.
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
            if(sens == POSITIF){
                if(v_ia->angle <= v_ia->vitesse_rotation)/// fix du bug du changement de sens de rotation quand le v_joueur passe au dessus
                    v_ia->angle = v_ia->angle_de_decalage;
                else
                    v_ia->angle += v_ia->vitesse_rotation;
            }
            if(sens == NEGATIF){
                if(v_ia->angle >= 360-v_ia->vitesse_rotation)/// fix du bug du changement de sens de rotation quand le v_joueur passe au dessus.
                    v_ia->angle = v_ia->angle_de_decalage;
                else
                    v_ia->angle -= v_ia->vitesse_rotation;
            }
            break;
        case RIEN:
            /// L'ia choisi de ne pas bouger.
            break;
    }
}

void tir_ia(_vaisseau *v_ia){
    v_ia->tir.etat = 1;
    init_tir (v_ia);
}
