#ifndef DEF_INCLUDE
    #define DEF_INCLUDE
    #include <stdlib.h>
    #include <stdio.h>
    #include <SDL/SDL.h>
    #include <SDL/SDL_image.h>
    #include <SDL/SDL_ttf.h>
    #include "structure.h"
    #include "utilitaire.h"
    #include "jeu.h"

    #define TAUX_SECONDE 20
    #define TAILLE_ECRAN_Y 768
    #define TAILLE_ECRAN_X 1366
    enum {JOUER, SAUVEGARDER, CHARGER, QUITTER};

    #define CENTRER(cadre, bloc) (cadre/2) - (bloc/2)
#endif

void init_vaisseau(_vaisseau *v_ia){
    v_ia->poid = 100;
    v_ia->acceleration = 20;
    v_ia->vitesse_max = 200;
    v_ia->bouclier = HAUT;
    v_ia->vie = HAUT;
    v_ia->arme = TIR_LASER;
    //v_ia.capacite=CAPA1;

    v_ia->position_x = 1000;
    v_ia->position_y = 400;
}

void barre_bouclier_ia(SDL_Surface *ecran, _vaisseau v_ia) {
    SDL_Surface *barre_bouclier=NULL;
    SDL_Rect pos_barre_bouclier;
    pos_barre_bouclier.x = v_ia.position_x;
    pos_barre_bouclier.y = (v_ia.position_y)+1;

    /// Affichage de la barre du bouclier
    switch (v_ia.bouclier) {
        case BAS:
            barre_bouclier = IMG_Load("images/bouclier_BAS.jpg");
            test_surface(barre_bouclier); //Verif chargement.
            break;
        case MOYEN:
            barre_bouclier = IMG_Load("images/bouclier_MOYEN.jpg");
            test_surface(barre_bouclier); //Verif chargement.
            break;
        case HAUT:
            barre_bouclier = IMG_Load("images/bouclier_HAUT.jpg");
            test_surface(barre_bouclier); //Verif chargement.
            break;
    }
    SDL_BlitSurface(barre_bouclier, NULL, ecran, &pos_barre_bouclier);
    SDL_Flip(ecran);
    SDL_FreeSurface(barre_bouclier);
}
void barre_vie_ia(SDL_Surface *ecran, _vaisseau v_ia) {
    SDL_Surface *barre_vie=NULL;
    SDL_Rect pos_barre_vie;
    pos_barre_vie.x = v_ia.position_x;
    pos_barre_vie.y = (v_ia.position_y)+21;

    /// Affichage de la barre de vie
    barre_vie = SDL_CreateRGBSurface(SDL_HWSURFACE, 150, 20, 32, 0, 0, 0, 0);
    switch (v_ia.vie) {
        case BAS:
            SDL_FillRect(barre_vie, NULL, SDL_MapRGB(barre_vie->format, 255, 0, 0)); //Rouge
            break;
        case MOYEN:
            SDL_FillRect(barre_vie, NULL, SDL_MapRGB(barre_vie->format, 255, 165, 0)); //Orange
            break;
        case HAUT:
            SDL_FillRect(barre_vie, NULL, SDL_MapRGB(barre_vie->format, 0, 255, 0)); //Bleu
            break;
    }
    SDL_BlitSurface(barre_vie, NULL, ecran, &pos_barre_vie);
    SDL_Flip(ecran);
    SDL_FreeSurface(barre_vie);
}

