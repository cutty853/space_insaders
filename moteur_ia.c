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

_comportement ia_cherche(_vaisseau *v_ia, _vaisseau *v_joueur){ ///detecte le joueur à partir d'une certaine distance si non se balade aléatoirement.
    int distance_detection_horizontale = 250; /// En pixels.
    int distance_detection_verticale = 250; /// En pixels.
    if((v_ia->position.x - distance_detection_horizontale) < v_joueur->position.x && v_joueur->position.x < (v_ia->position.x + distance_detection_horizontale)){
        if((v_ia->position.y - distance_detection_verticale) < v_joueur->position.y && v_joueur->position.y < (v_ia->position.y + distance_detection_verticale)){
            return ATTAQUE; /// Le joueur est dans le carré de detection.
        }
    }
    else{/// Le vaisseau "cherche": avance en permanence et tourne de manière aléatoire.
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
    return CHERCHE;
}

_comportement ia_attaque(_vaisseau *v_ia, _vaisseau *v_joueur){
    int nouv_pos_relative = 0, sens_de_rotation = 0;
    int tourne = 0, accelere = 0, ralenti = 0;

    if(v_ia->angle >= 360) /// un tour complet a été fait.
        v_ia->angle -= 360;
    else if(v_ia->angle < 0)
        v_ia->angle += 359;

    nouv_pos_relative = compare_position(v_ia, v_joueur);
    switch (nouv_pos_relative){
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

    if((v_ia->angle+2) < v_ia->angle_de_decalage || (v_ia->angle-2) > v_ia->angle_de_decalage){ /// +2 et -2 = imprecision, OBLIGATOIRE dans le cas où v_ratation != 1.
        tourne ++;
        accelere --;
        ralenti --;
    }
    if(v_ia->vitesse < v_joueur->vitesse){
        accelere ++;
        ralenti --;
    }
    if(v_ia->vitesse > v_joueur->vitesse){
        ralenti ++;
        accelere --;
    }

    int max = trouve_max(tourne, accelere, ralenti);/// Trouve la plus haute valeur (importance) de mouvement.
    if(max == TOURNE){
        sens_de_rotation = choix_sens_de_rotation(v_ia, nouv_pos_relative);
        mouvement_ia(TOURNE, sens_de_rotation, v_ia, v_joueur);
    }else if(max == AVANCE){
        mouvement_ia(AVANCE, DROIT, v_ia, v_joueur);
        tir_ia(v_ia);
    }else if(max == RECUL){
        mouvement_ia(RECUL, DROIT, v_ia, v_joueur);
        tir_ia(v_ia);
    }else{
        mouvement_ia(RIEN, DROIT, v_ia, v_joueur);
    }
    return(ATTAQUE);
}

void tour_ia(_vaisseau *v_ia, _vaisseau *v_joueur, SDL_Surface *ecran){
    switch(v_ia->comportement){
        case CHERCHE:
            v_ia->comportement = ia_cherche(v_ia, v_joueur);
            break;
        case ATTAQUE:
            v_ia->comportement = ia_attaque(v_ia, v_joueur);
            break;
        case FUIT:
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
int choix_sens_de_rotation(_vaisseau *v_ia, int nouv_pos_relative){ /// Choix du sens de rotation: positif = sens trigo.
    if(v_ia->angle < v_ia->angle_de_decalage)
        return POSITIF;
    else
        return NEGATIF;
}
int trouve_max(int tourne, int accelere, int ralenti){
    if(tourne > accelere && tourne > ralenti)
        return TOURNE;
    else if(accelere > tourne && accelere > ralenti)
        return AVANCE;
    else if(ralenti > tourne && ralenti > accelere)
        return RECUL;
    else if(tourne == accelere || tourne == ralenti){
        srand(time(NULL)); /// initialisation de rand
        int direction = (rand()%2)+1; /// random entre 1 et 2.
        switch (direction){
            case 1:
                return TOURNE;
                break;
            case 2:
                if(tourne == accelere)
                    return AVANCE;
                else
                    return RECUL;
                break;
            default:
                exit(666);
                break;
        }
    }
    else
        return RIEN;
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
            if (v_ia->vitesse > (v_ia->vitesse_max/2)) /// utilié 0, ia plus dur à contourner
                v_ia->vitesse -= v_ia->acceleration;
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
    // Comment on fait ? est ce que on fairait pas 3 fonctions de tir communes ia/joueur pour les 3 modes de tir différend ?
    // Si oui, l'orientation du tir se calcul dns chacune des fonctions ou on en fait une autre commune ?
    // Moi, j'imagine (faudra y réfléchir plus profondemment) le truc comme ça:

    switch (v_ia->arme){// les fonctions calcul l'orientation elles mêmes:
        case TIR_LASER:
            // appelle de la fonction commune de tir en mode tir_laser.
            break;
        case OBUS:
            // appelle de la fonction communne de tir en mode obus.
            break;
        case RAYON_LASER:
            // appelle de la fonction commune de tir en mode rayon_laser.
            break;
        default:
            exit(666); /// le chiffre du mal pour une erreur impossible, ça te va ? x)
            break;
    }
}
