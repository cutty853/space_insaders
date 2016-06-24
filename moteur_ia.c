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

    #include "structure.h"
    #include "constantes.h"

    #include "utilitaire.h"
    #include "mode_combat.h"
    #include "mode_campagne.h"
    #include "moteur_ia.h"
    #include "moteur_joueur.h"
    #include "moteur_affichage.h"
    #include "moteur_physique.h"
    #include "moteur_initialisation.h"
#endif

/// Creation de variables globales:
int direction = 0;
int duree = 0;

_comportement detection_ia(_vaisseau *v_ia, _vaisseau *v_joueur){ ///detecte le joueur à partir d'une certaine distance si non se balade aléatoirement.
    int distance_detection_horizontale = 10*v_ia->vaisseau_ia.val_seuil_intelligence;/// En pixels.
    int distance_detection_verticale = distance_detection_horizontale;/// Formation d'un carré.
    if((v_ia->position.x - distance_detection_horizontale) < v_joueur->position.x && v_joueur->position.x < (v_ia->position.x + distance_detection_horizontale)){
        if((v_ia->position.y - distance_detection_verticale) < v_joueur->position.y && v_joueur->position.y < (v_ia->position.y + distance_detection_verticale))
            return ATTAQUE; /// Le joueur est dans le carré de detection.
    }
    return CHERCHE;
}
int detecte_charge(_vaisseau *v_ia, _vaisseau *v_joueur){
    int distance_charge_horizontale = 150+v_ia->vaisseau_ia.val_seuil_intelligence;
    int distance_charge_verticale = distance_charge_horizontale; /// En pixels, formation d'un carré.
    if(v_joueur->bouclier.charge != VIDE){
        if(    ((v_ia->position.x - distance_charge_horizontale) < v_joueur->position.x && v_joueur->position.x < (v_ia->position.x + distance_charge_horizontale))  &&  ((v_ia->position.y - distance_charge_verticale) < v_joueur->position.y && v_joueur->position.y < (v_ia->position.y + distance_charge_verticale))    )
            return 1;///le vaisseau est dans le carré.
    }
    return 0;
}

void ia_cherche(_vaisseau *v_ia, _vaisseau *v_joueur){/// Le vaisseau "cherche": il avance en permanence et tourne de manière aléatoire.
    if(duree == 0){
        direction = aleatoire(1, 5);
        duree = aleatoire(30, 60);
    }
    switch(direction){
        case 1:
            mouvement_vaisseau(TOURNE, POSITIF, v_ia);
            duree --;
            break;
        case 2:
            mouvement_vaisseau(TOURNE, NEGATIF, v_ia);
            duree --;
            break;
        default:
            if(v_ia->vitesse <= (v_ia->vitesse_max/2.0))
                mouvement_vaisseau(AVANCE, DROIT, v_ia);
            else
                mouvement_vaisseau(RIEN, DROIT, v_ia);
            duree --;
            break;
    }
}
void ia_attaque(_vaisseau *v_ia, _vaisseau *v_joueur){/// Le vaisseau "attaque": il suit le joueur et tir dès qu'il n'a plus à tourner.
    int pos_relative = compare_position(v_ia, v_joueur), etat_charge = 0;
    switch (pos_relative){// Plus optimisé avec des puissances 2 plutot que des valeurs absolues.
        case BAS_DROITE:
            v_ia->vaisseau_ia.angle_de_decalage = (180/PI)*(atan(fabs(v_joueur->position.x - v_ia->position.x) / fabs(v_ia->position.y - v_joueur->position.y)));
            v_ia->vaisseau_ia.angle_de_decalage += 0; /// décalage de l'arctan.
            break;
        case HAUT_DROITE:
            v_ia->vaisseau_ia.angle_de_decalage = (180/PI)*(atan(fabs(v_ia->position.y - v_joueur->position.y) / fabs(v_joueur->position.x - v_ia->position.x)));
            v_ia->vaisseau_ia.angle_de_decalage += 90; /// décalage de l'arctan.
            break;
        case HAUT_GAUCHE:
            v_ia->vaisseau_ia.angle_de_decalage = (180/PI)*(atan(fabs(v_joueur->position.x - v_ia->position.x) / fabs(v_ia->position.y - v_joueur->position.y)));
            v_ia->vaisseau_ia.angle_de_decalage += 180; /// décalage de l'arctan.
            break;
        case BAS_GAUCHE:
            v_ia->vaisseau_ia.angle_de_decalage = (180/PI)*(atan(fabs(v_ia->position.y - v_joueur->position.y) / fabs(v_joueur->position.x - v_ia->position.x)));
            v_ia->vaisseau_ia.angle_de_decalage += 270; /// décalage de l'arctan.
            break;
    }
    etat_charge = detecte_charge(v_ia, v_joueur);
    if(etat_charge == 1){
        ia_attaque_charge(v_ia, v_joueur, pos_relative);
    }else{
        ia_attaque_tir(v_ia, v_joueur, pos_relative);
    }
}
void ia_attaque_charge (_vaisseau *v_ia, _vaisseau *v_joueur, int pos_relative){
    if(v_ia->angle != v_ia->vaisseau_ia.angle_de_decalage){/// tourne tant que pas bien orienter pour charger.
        int sens_de_rotation = choix_sens_de_rotation(v_ia, pos_relative);
        mouvement_vaisseau(TOURNE, sens_de_rotation, v_ia);
    }else
        mouvement_vaisseau(AVANCE, DROIT, v_ia);
}
void ia_attaque_tir(_vaisseau *v_ia, _vaisseau *v_joueur, int pos_relative){
    if(v_ia->angle <= v_ia->vaisseau_ia.angle_de_decalage-(100/v_ia->vaisseau_ia.val_seuil_intelligence) || v_ia->angle >= v_ia->vaisseau_ia.angle_de_decalage+(100/v_ia->vaisseau_ia.val_seuil_intelligence)){/// tourne tant que pas bien orienter pour tirer avec une certaine ''imprecision''.
        int sens_de_rotation = choix_sens_de_rotation(v_ia, pos_relative);
        mouvement_vaisseau(TOURNE, sens_de_rotation, v_ia);
    }
    int distance_charge_horizontale = (v_ia->vitesse*20.5)+80; /// En pixels, distance progressive en fonction de la vitesse (20,5 est calculé pour avoir le temps de s'arrêter à vitesse max avec l'obstacle fixe + 80 est quand vitesse nulle taille du sprite vaisseau avec distance de sécurité).
    int distance_charge_verticale = distance_charge_horizontale; /// En pixels, formation d'un carré.
    if((v_ia->position.x - distance_charge_horizontale) < v_joueur->position.x && v_joueur->position.x < (v_ia->position.x + distance_charge_horizontale)){
        if((v_ia->position.y - distance_charge_verticale) < v_joueur->position.y && v_joueur->position.y < (v_ia->position.y + distance_charge_verticale))/// L'ia est trop proche.
            mouvement_vaisseau(RECUL, DROIT, v_ia);
    }else
        mouvement_vaisseau(AVANCE, DROIT, v_ia);
    /// Conditions de tir:
    gestion_tir_ia(v_ia);
}
void ia_fuit(_vaisseau *v_ia, _vaisseau *v_joueur){/// Le vaisseau "fuit": il va se caher derrière les obstacles de la carte.
    exit(00000); // Pas encore fait, il faudrait déjà qu'elle est un quelque part où fuire (obstacle).
}

void tour_ia(_vaisseau *v_ia, _vaisseau *v_joueur, SDL_Surface *ecran){
    if(v_ia->angle >= 360) /// un tour complet a été fait.
        v_ia->angle -= 360;
    else if(v_ia->angle < 0)
        v_ia->angle += 359;

    switch(v_ia->vaisseau_ia.comportement){
        case CHERCHE:
            ia_cherche(v_ia, v_joueur);
            v_ia->vaisseau_ia.comportement = detection_ia(v_ia, v_joueur);
            if(v_ia->vaisseau_ia.comportement != CHERCHE){/// l'ia a trouvé et choisi de d'engager le combat ou non.
                if(v_ia->vie.charge == BAS && v_ia->bouclier.charge == VIDE)
                    v_ia->vaisseau_ia.comportement = FUIT;
                else
                    v_ia->vaisseau_ia.comportement = ATTAQUE;
            }
            break;
        case ATTAQUE:
            ia_attaque(v_ia, v_joueur);
            break;
        case FUIT:
            ia_fuit(v_ia, v_joueur);
            break;
        default:
            exit(616);
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
        exit(626);
}
int choix_sens_de_rotation(_vaisseau *v_ia, int pos_relative){ /// Choix du sens de rotation: positif = sens trigo.
    int premiere_angle = 0;
    int deuxieme_angle = 0;
    if(v_ia->angle < v_ia->vaisseau_ia.angle_de_decalage){
        premiere_angle = v_ia->vaisseau_ia.angle_de_decalage - v_ia->angle;
        deuxieme_angle = 360 - premiere_angle;
        if(premiere_angle > deuxieme_angle)
            return NEGATIF;
        else
            return POSITIF;
    }else{
        premiere_angle = v_ia->angle - v_ia->vaisseau_ia.angle_de_decalage;
        deuxieme_angle = 360 - premiere_angle;
        if(premiere_angle > deuxieme_angle)
            return POSITIF;
        else
            return NEGATIF;
    }
}

void gestion_tir_ia(_vaisseau *v_ia){
    if(v_ia->tir.etat != 1 && ( (v_ia->angle >= (v_ia->vaisseau_ia.angle_de_decalage)-(80-v_ia->vaisseau_ia.val_seuil_intelligence) ) && (v_ia->angle <= (v_ia->vaisseau_ia.angle_de_decalage)+(80-v_ia->vaisseau_ia.val_seuil_intelligence)) ) ){/// Si pas déjà entrain de tirer alors tir.
        int alea_tir = aleatoire(1, 100); /// Aléa pour empêcher le tir permanant.
        if(alea_tir > 100-v_ia->vaisseau_ia.val_seuil_intelligence){
            init_tir(v_ia);
            init_hitbox(&(v_ia->tir.hitbox), 0, 0, 0, 0, v_ia->tir.position.x, v_ia->tir.position.y, 6, 6);
        }
    }
}
