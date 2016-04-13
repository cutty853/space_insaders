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

void test_surface(SDL_Surface* surface, int ref_err) {
    if (surface==NULL) {
        fprintf(stderr, "[ - ] Erreur n°%d\n Veuillez vous réferer au tableau des erreurs s'il vous plait.\n", ref_err);
        exit(EXIT_FAILURE);
    }
}

void init_pos(SDL_Rect* position, int x, int y) {
    position->x=x;
    position->y=y;
}

void pause() {
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

void init_vaisseau(_vaisseau *vaisseau, int intelligence, int poid, int vitesse, int acceleration, int v_max, int bouclier, int vie, int arme, int position_x, int position_y, int v_rotation, int angle){
    vaisseau->intelligence = intelligence;
    vaisseau->poid = poid;
    vaisseau->vitesse = vitesse;
    vaisseau->acceleration = acceleration;
    vaisseau->vitesse_max = v_max;
    vaisseau->bouclier = bouclier;
    vaisseau->vie = vie;
    vaisseau->arme = arme;
    //vaisseau.capacite=CAPA1;
    vaisseau->vitesse_rotation = v_rotation;
    vaisseau->angle = angle;
    if(intelligence == IA)
        vaisseau->sprite = IMG_Load("images/vaisseau_ia.png");
    else
        vaisseau->sprite = IMG_Load("images/joueur_ship.png");
    test_surface(vaisseau->sprite, 106);
    vaisseau->position.x = position_x;
    vaisseau->position.y = position_y;
}

SDL_Rect* aff_vaisseau(SDL_Surface *ecran, _vaisseau *vaisseau, SDL_Surface* save_screen) {
    SDL_Rect *pos_to_update, pre_pos_vaisseau;
    pos_to_update = malloc(sizeof(SDL_Rect)*2);

    // Effacement de l'ancien joueur remplace charge_niveau
    SDL_BlitSurface(save_screen, &(vaisseau->position), ecran, &(vaisseau->position));
    pos_to_update[0] = vaisseau->position;
    // Sauvegarde des anciennes positions
    pre_pos_vaisseau.w = vaisseau->position.w;
    pre_pos_vaisseau.h = vaisseau->position.h;
    // Calcul des positions
    vaisseau->position.x += (vaisseau->vitesse)*cos(RADIANATION(vaisseau->angle));
    vaisseau->position.y += (vaisseau->vitesse)*(-sin(RADIANATION(vaisseau->angle)));
    // Affichage du joueur

    vaisseau->sprite = rotozoomSurface(vaisseau->sprite, vaisseau->angle, 1.0, 1);
    SDL_BlitSurface(vaisseau->sprite, NULL, ecran, &(vaisseau->position));
    SDL_BlitSurface(save_screen, &(vaisseau->position), ecran, &(vaisseau->position));
    if (vaisseau->etat_rotation == 1) {
        vaisseau->position.x -= ((vaisseau->position.w - TAILLE_JOUEUR)-(pre_pos_vaisseau.w - TAILLE_JOUEUR))/2;
        vaisseau->position.y -= ((vaisseau->position.h - TAILLE_JOUEUR)-(pre_pos_vaisseau.h - TAILLE_JOUEUR))/2;
        vaisseau->etat_rotation = 0;
    } // Cette condition permet le décalage du joueur lors de sa angle, afin que la angle se fasse réellement par rapport au centre du sprite
    SDL_BlitSurface(vaisseau->sprite, NULL, ecran, &(vaisseau->position));
    pos_to_update[1] = vaisseau->position;
    SDL_FreeSurface(vaisseau->sprite); // Le rotozoom crée une seconde surface, cependant comme vaisseau->sprite n'est pas passé en pointeur de pointeur il n'est alors pas modifié
    return pos_to_update; // La fonction retourne un tableau de 2 positions qui servira a update une région spécifique de la carte (se tableau a été malloc il est donc à free)
}

void degrade(_degrade prop_deg, SDL_Surface *ecran, SDL_Rect pos_degrade) {
    int i, taux, taille, couleur;
    SDL_Surface **rectangle_degrade;
    switch (prop_deg.sens) {
        case HORIZONTAL:
            taille = prop_deg.taille_x;
            break;
        case VERTICAL:
            taille = prop_deg.taille_y;
            break;
    }
    if (prop_deg.nuance>256)
        prop_deg.nuance=256;
    if (prop_deg.nuance>taille)
        prop_deg.nuance=taille;
    couleur=256/prop_deg.nuance;
    taux = taille/prop_deg.nuance;
    rectangle_degrade = malloc(sizeof(SDL_Surface*)*(prop_deg.nuance));
    switch (prop_deg.sens) {
        case HORIZONTAL:
            prop_deg.taille_x=taux;
            break;
        case VERTICAL:
            prop_deg.taille_y=taux;
            break;
    }

    for (i=0; i<prop_deg.nuance; i++) {
        rectangle_degrade[i]=SDL_CreateRGBSurface(SDL_HWSURFACE, prop_deg.taille_x, prop_deg.taille_y, 32, 0, 0, 0, 0);
        if (taille>=prop_deg.nuance) {
            SDL_FillRect(rectangle_degrade[i], NULL, SDL_MapRGB(rectangle_degrade[i]->format, i*couleur, i*couleur, i*couleur));
            SDL_BlitSurface(rectangle_degrade[i], NULL, ecran, &pos_degrade);
            switch (prop_deg.sens) {
                case HORIZONTAL:
                    pos_degrade.x+=taux;
                    break;
                case VERTICAL:
                    pos_degrade.y+=taux;
                    break;
            }
        }
    }

    SDL_Flip(ecran);
    for (i = 0 ; i < prop_deg.nuance ; i++)
        SDL_FreeSurface(rectangle_degrade[i]);
}
