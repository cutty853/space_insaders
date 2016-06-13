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
    #include "moteur_physique.h"
#endif

/// Creation de variables globales:
int direction = 0;
int duree = 0;

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
            if(v_ia->vitesse <= (v_ia->vitesse_max/3.0))
                mouvement_vaisseau(AVANCE, DROIT, v_ia);
            else
                mouvement_vaisseau(RIEN, DROIT, v_ia);
            duree --;
            break;
    }
}
void ia_attaque(_vaisseau *v_ia, _vaisseau *v_joueur){/// Le vaisseau "attaque": il suit le joueur et tir dès qu'il n'a plus à tourner.
    int pos_relative = 0, sens_de_rotation = 0;

    pos_relative = compare_position(v_ia, v_joueur);
    switch (pos_relative){// Mieux optimisé avec des puissances 2 plutot que des valeurs absolues.
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

    int distance_securite_horizontale = (v_ia->vitesse*20.5)+80; /// En pixels, distance progressive en fonction de la vitesse (20,5 est calculé pour avoir le temps de s'arrêter à vitesse max avec l'obstacle fixe + 80 est quand vitesse nulle taille du sprite vaisseau avec distance de sécurité).
    int distance_securite_verticale = distance_securite_horizontale; /// En pixels, formation d'un carré.
    if((v_ia->position.x - distance_securite_horizontale) < v_joueur->position.x && v_joueur->position.x < (v_ia->position.x + distance_securite_horizontale)){
        if((v_ia->position.y - distance_securite_verticale) < v_joueur->position.y && v_joueur->position.y < (v_ia->position.y + distance_securite_verticale))
            mouvement_vaisseau(RECUL, DROIT, v_ia);; /// L'ia est trop proche est risque de déclencher une colision.
    }else if(v_ia->vitesse < v_joueur->vitesse){/// atteint la vitesse du joueur mais ne le rattrape pas par sécurité.
        mouvement_vaisseau(AVANCE, DROIT, v_ia);
    }
    if(v_ia->angle <= v_ia->angle_de_decalage-5 || v_ia->angle >= v_ia->angle_de_decalage+5){/// tourne tant que pas bien orienter pour tirer avec une certaine ''imprecision''.
        sens_de_rotation = choix_sens_de_rotation(v_ia, pos_relative);
        mouvement_vaisseau(TOURNE, sens_de_rotation, v_ia);
    }
    /// Conditions de tir:
    if(v_ia->tir.etat != 1 && ( (v_ia->angle >= (v_ia->angle_de_decalage)-(80-v_ia->seuil_intelligence) && v_ia->angle <= (v_ia->angle_de_decalage)+(80-v_ia->seuil_intelligence)) ) ){/// Si pas déjà entrain de tirer alors tir.
        int alea_tir = aleatoire(1, 100); /// Aléa pour empêcher le tir permanant.
        if(alea_tir > 100-v_ia->seuil_intelligence)
            tir_ia(v_ia);
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
        case INDEPENDENT:
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

void tir_ia(_vaisseau *v_ia){
    init_tir (v_ia);
    init_hitbox(&(v_ia->tir.hitbox), 0, 0, 0, 0, v_ia->tir.position.x, v_ia->tir.position.y, 6, 6);
}
