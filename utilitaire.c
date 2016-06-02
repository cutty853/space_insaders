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
        fprintf(stderr, "[ images ] Erreur n�%d\n Veuillez vous r�ferer au tableau des erreurs s'il vous plait.\n", ref_err);
        exit(EXIT_FAILURE);
    }
}
void test_police(TTF_Font* police, int ref_err){
    if (police==NULL) {
        fprintf(stderr, "[ polices ] Erreur n�%d\n Veuillez vous r�ferer au tableau des erreurs s'il vous plait.\n", ref_err);
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
int aleatoire(int mini, int maxi){
    return ( (rand()%maxi)+mini ); /// random entre le minimum demande et le maximum demande.
}

void init_vaisseau(_vaisseau *vaisseau, _intelligence intelligence, _comportement comportement, int tab_init_val[9]){
    vaisseau->intelligence = intelligence;
    vaisseau->comportement = comportement;

    vaisseau->position.x = tab_init_val[0];
    vaisseau->position.y = tab_init_val[1];
    vaisseau->angle = tab_init_val[2];
    vaisseau->poid = tab_init_val[3];
    vaisseau->vitesse_max = tab_init_val[4];
    vaisseau->vitesse_rotation = tab_init_val[5];
    vaisseau->arme = tab_init_val[6];
    vaisseau->bouclier.charge = tab_init_val[7];
    vaisseau->vie.charge = tab_init_val[8];

    vaisseau->vitesse_min = 0;
    vaisseau->vitesse = 0.0;
    vaisseau->acceleration = ((vaisseau->poid)*(vaisseau->vitesse_max))/1000.0; ///acc�l�ration d�pendante du poid.
    vaisseau->tir.etat = 0; /// � l'initialisation, aucun des vaisseaux ne tir.
    vaisseau->tir.temps_passe = 0;

    if(intelligence == IA) {
        vaisseau->sprite = IMG_Load("images/vaisseau_ia.png");
        test_surface(vaisseau->sprite, 104);
    }
    else {
        vaisseau->sprite = IMG_Load("images/joueur_ship.png");
        test_surface(vaisseau->sprite, 105);
    }
}
void init_pos(SDL_Rect* position, int x, int y){
    position->x=x;
    position->y=y;
}
void init_tir (_vaisseau *vaisseau){/// Initialisation de tous les parametres du tir
    init_pos(&(vaisseau->tir.position), vaisseau->position.x+(TAILLE_JOUEUR/2.0), vaisseau->position.y+(TAILLE_JOUEUR/2.0));
    vaisseau->tir.type = vaisseau->arme;
    vaisseau->tir.angle = vaisseau->angle;
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
                if(vaisseau->angle <= vaisseau->vitesse_rotation && vaisseau->comportement == ATTAQUE)/// fix du bug du changement de sens de rotation quand le v_joueur passe au dessus
                    vaisseau->angle = vaisseau->angle_de_decalage;
                else
                    vaisseau->angle += vaisseau->vitesse_rotation;
            }
            if(sens == NEGATIF){
                if(vaisseau->angle >= 360-vaisseau->vitesse_rotation && vaisseau->comportement == ATTAQUE)/// fix du bug du changement de sens de rotation quand le v_joueur passe au dessus.
                    vaisseau->angle = vaisseau->angle_de_decalage;
                else
                    vaisseau->angle -= vaisseau->vitesse_rotation;
            }
            break;
        case RIEN:
            /// L'ia choisi de ne rien changer � sa trajectoire.
            break;
    }
}

void deplace_curseur(FILE* fichier){
    char deux_points = ':';
    do{/// Boucle de lecture des caract�res un � un
        deux_points = fgetc(fichier); /// On lit le caract�re
    } while (deux_points != ':'); /// On continue tant que fgetc n'a pas retourn� EOF (fin de fichier)
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
    }else
        exit(666);/// t�moin de chargement.
}





