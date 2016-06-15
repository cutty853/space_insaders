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
#endif


void test_surface(SDL_Surface* surface, int ref_err){
    if (surface==NULL) {
        fprintf(stderr, "[ images ] Erreur n°%d\n Veuillez vous réferer au tableau des erreurs s'il vous plait.\n", ref_err);
        exit(EXIT_FAILURE);
    }
}
void test_police(TTF_Font* police, int ref_err){
    if (police==NULL) {
        fprintf(stderr, "[ polices ] Erreur n°%d\n Veuillez vous réferer au tableau des erreurs s'il vous plait.\n", ref_err);
        exit(EXIT_FAILURE);
    }
}

void pause(){
    int continuer=1;
    SDL_Event choix;

    while (continuer) {
        SDL_WaitEvent(&choix);
        switch(choix.type) {
            case SDL_QUIT:
                continuer=0;
                break;
            case SDL_KEYDOWN:
                switch(choix.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        continuer=0;
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }
}
int aleatoire(int mini, int maxi){/// random entre le minimum demande et le maximum demande (inclus).
    return ( (rand()%maxi)+mini );
}

void init_vaisseau(_vaisseau *vaisseau, int tab_init_val[NBR_VALEURS_TRANSMISES]){
    vaisseau->position.x = tab_init_val[0];
    vaisseau->position.y = tab_init_val[1];
    vaisseau->angle = tab_init_val[2];
    vaisseau->poid = tab_init_val[3];
    vaisseau->vitesse_max = tab_init_val[4];
    vaisseau->vitesse_rotation = tab_init_val[5];
    vaisseau->arme = tab_init_val[6];
    vaisseau->bouclier.charge = tab_init_val[7];
    vaisseau->vie.charge = tab_init_val[8];
    vaisseau->intelligence = tab_init_val[9];

    vaisseau->vitesse_min = -(vaisseau->vitesse_max/3);
    vaisseau->vitesse = 0.0;
    vaisseau->acceleration = ((vaisseau->poid)*(vaisseau->vitesse_max))/1000.0; ///accélération dépendante du poid.
    vaisseau->tir.etat = 0; /// à l'initialisation, aucun des vaisseaux ne tir.
    vaisseau->etat_rotation = 0; /// à l'initialisation, aucun des vaisseaux ne tourne.


    if(vaisseau->intelligence == IA){
        vaisseau->sprite = IMG_Load("images/vaisseau_ia.png");
        test_surface(vaisseau->sprite, 104);
        vaisseau->vaisseau_ia.seuil_intelligence = tab_init_val[10];
        vaisseau->vaisseau_ia.comportement = CHERCHE;
        vaisseau->vaisseau_ia.angle_de_decalage = 0;/// n'a pas encore choisi de cible.
        switch(vaisseau->vaisseau_ia.seuil_intelligence){
            case IA_NOVICE:
                vaisseau->vaisseau_ia.val_seuil_intelligence = 25;
                break;
            case IA_NORMAL:
                vaisseau->vaisseau_ia.val_seuil_intelligence = 50;
                break;
            case IA_EXPERTE:
                vaisseau->vaisseau_ia.val_seuil_intelligence = 75;
                break;
            default:
                exit(636);
        }
    }else if(vaisseau->intelligence == JOUEUR){/// il s'agit du joueur.
        vaisseau->sprite = IMG_Load("images/joueur_ship.png");
        test_surface(vaisseau->sprite, 105);
    }else{
        exit(656);
    }
}
void init_pos(SDL_Rect* position, int x, int y){
    position->x=x;
    position->y=y;
}
void init_tir (_vaisseau *vaisseau){/// Initialisation de tous les parametres du tir
    vaisseau->tir.etat = 1;
    init_pos(&(vaisseau->tir.position), vaisseau->position.x+(TAILLE_JOUEUR/2.0), vaisseau->position.y+(TAILLE_JOUEUR/2.0));
    vaisseau->tir.type = vaisseau->arme;
    vaisseau->tir.angle = vaisseau->angle;
    vaisseau->tir.distance_max = 25;
    vaisseau->tir.distance_parcourue = 0;
}

void mouvement_vaisseau (int action, int sens, _vaisseau *vaisseau){
    switch (action){
        case AVANCE:
            if (vaisseau->vitesse < vaisseau->vitesse_max)
                vaisseau->vitesse += vaisseau->acceleration;
            break;
        case RECUL:
            if (vaisseau->vitesse > vaisseau->vitesse_min)
                vaisseau->vitesse -= vaisseau->acceleration;
            break;
        case TOURNE:
            vaisseau->etat_rotation = 1;
            if(sens == POSITIF){
                if(vaisseau->angle <= vaisseau->vitesse_rotation && vaisseau->vaisseau_ia.comportement == ATTAQUE)/// fix du bug du changement de sens de rotation quand le v_joueur passe au dessus
                    vaisseau->angle = vaisseau->vaisseau_ia.angle_de_decalage;
                else
                    vaisseau->angle += vaisseau->vitesse_rotation;
            }
            if(sens == NEGATIF){
                if(vaisseau->angle >= 360-vaisseau->vitesse_rotation && vaisseau->vaisseau_ia.comportement == ATTAQUE)/// fix du bug du changement de sens de rotation quand le v_joueur passe au dessus.
                    vaisseau->angle = vaisseau->vaisseau_ia.angle_de_decalage;
                else
                    vaisseau->angle -= vaisseau->vitesse_rotation;
            }
            break;
        case RIEN:
            /// L'ia choisi de ne rien changer à sa trajectoire.
            break;
    }
}

void deplace_curseur(FILE* fichier){
    char deux_points = ':';
    do{/// Boucle de lecture des caractères un à un
        deux_points = fgetc(fichier); /// On lit le caractère
    } while (deux_points != ':'); /// On continue tant que fgetc n'a pas retourné EOF (fin de fichier)
}
int recup_int(FILE* fichier){
    int val_int = 0;
    fscanf(fichier, "%i", &val_int);
    return(val_int);
}
int recup_string(FILE* fichier){
    char val_string[4];
    fgets(val_string, 4, fichier);
    if(strcmp(val_string, "TIR") == 0){
        return(TIR_LASER);
    }else if(strcmp(val_string, "OBU") == 0){
        return(OBUS);
    }else if(strcmp(val_string, "RAY") == 0){
        return(RAYON_LASER);
    }else if(strcmp(val_string, "VID") == 0){
        return(VIDE);
    }else if(strcmp(val_string, "BAS") == 0){
        return(BAS);
    }else if(strcmp(val_string, "MOY") == 0){
        return(MOYEN);
    }else if(strcmp(val_string, "HAU") == 0){
        return(HAUT);
    }else if(strcmp(val_string, "JOU") == 0){
        return(JOUEUR);
    }else if(strcmp(val_string, "ART") == 0){
        return(IA);
    }else if(strcmp(val_string, "NOV") == 0){
        return(IA_NOVICE);
    }else if(strcmp(val_string, "NOR") == 0){
        return(IA_NORMAL);
    }else if(strcmp(val_string, "EXP") == 0){
        return(IA_EXPERTE);
    }else{
        //printf("%s", val_string);
        //pause();
        exit(4673);
    }
}

void degat_tir(_vaisseau *vaisseau){
    if(vaisseau->bouclier.charge == VIDE){
        vaisseau->vie.charge --;
    } else{
        vaisseau->bouclier.charge --;
    }
}
void degat_collisions(_vaisseau *vaisseau){
    if(vaisseau->bouclier.charge > VIDE)
        vaisseau->bouclier.charge --;
    vaisseau->vitesse = -1.0*(  vaisseau->vitesse_max+( vaisseau->vitesse_max/(2*(vaisseau->vitesse_max-(vaisseau->vitesse-1))) )  );
}

void gestion_distance_tir(_vaisseau *vaisseau){
    if (vaisseau->tir.etat == 1){
        vaisseau->tir.distance_parcourue ++;
        if(vaisseau->tir.distance_parcourue > vaisseau->tir.distance_max)
            vaisseau->tir.etat = 0;
    }
}




