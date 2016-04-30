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
int aleatoire(int mini, int maxi){
    srand(time(NULL)); /// initialisation de rand
    return ( (rand()%maxi)+mini ); /// random entre le minimum demande et le maximum demande.
}

void init_vaisseau(_vaisseau *vaisseau, _intelligence intelligence, _comportement comportement, int poid, float vitesse, float acceleration, int v_max, int bouclier, int vie, _arme arme, int position_x, int position_y, int v_rotation, int angle){
    vaisseau->intelligence = intelligence;
    vaisseau->comportement = comportement;
    vaisseau->poid = poid;
    vaisseau->vitesse = vitesse;
    vaisseau->acceleration = acceleration;
    vaisseau->vitesse_max = v_max;
    vaisseau->vitesse_min = -v_max/3;
    vaisseau->bouclier.charge = bouclier;
    vaisseau->vie.charge = vie;
    vaisseau->arme = arme;
    vaisseau->tir.etat = 0; /// à l'initialisation, aucun des vaisseaux ne tir.
    //vaisseau.capacite=CAPA1;
    vaisseau->vitesse_rotation = v_rotation;
    vaisseau->angle = angle;
    if(intelligence == IA) {
        vaisseau->sprite = IMG_Load("images/vaisseau_ia.png");
        test_surface(vaisseau->sprite, 104);
    }
    else {
        vaisseau->sprite = IMG_Load("images/joueur_ship.png");
        test_surface(vaisseau->sprite, 105);
    }
    vaisseau->position.x = position_x;
    vaisseau->position.y = position_y;
}
void init_pos(SDL_Rect* position, int x, int y){
    position->x=x;
    position->y=y;
}
void init_tir (_vaisseau *vaisseau){/// Initialisation de tous les parametres du tir
    init_pos(&(vaisseau->tir.position), vaisseau->position.x+(TAILLE_JOUEUR/2.0), vaisseau->position.y+(TAILLE_JOUEUR/2.0));
    vaisseau->tir.type = vaisseau->arme;
    vaisseau->tir.angle = vaisseau->angle;
    vaisseau->tir.sprite = rotozoomSurface(vaisseau->tir.sprite, vaisseau->tir.angle, 1.0, 1);
}


