#ifndef DEF_INCLUDE
    #define DEF_INCLUDE
    #include <stdlib.h>
    #include <stdio.h>
    #include <math.h>
    #include <time.h>
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

void tour_ia(_vaisseau *v_ia, _vaisseau *v_joueur, SDL_Surface *ecran){
    int nouv_pos_relative = 0, sens_de_rotation = 0;
    int tourne = 0, accelere = 0, ralenti = 0;

    if(v_ia->angle >= 360) /// un tour complet a �t� fait.
        v_ia->angle -= 360;
    else if(v_ia->angle < 0)
        v_ia->angle += 359;

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

    if((v_ia->angle+2) < v_ia->angle_de_decalage || (v_ia->angle-2) > v_ia->angle_de_decalage){ /// +2 et -2 = imprecision, OBLIGATOIRE dans le cas o� v_ratation != 1.
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
        int direction = (rand()%2)+1; /// random entre 1 et 4.
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
            if (v_ia->vitesse > (v_ia->vitesse_max/2)) /// utili� 0, ia plus dur � contourner
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
    // Comment on fait ? est ce que on fairait pas 3 fonctions de tir communes ia/joueur pour les 3 modes de tir diff�rend ?
    // Si oui, l'orientation du tir se calcul dns chacune des fonctions ou on en fait une autre commune ?
    // Moi, j'imagine (faudra y r�fl�chir plus profondemment) le truc comme �a:

    switch (v_ia->arme){// les fonctions calcul l'orientation elles m�mes:
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
            exit(666); /// le chiffre du mal pour une erreur impossible, �a te va ? x)
            break;
    }
}
