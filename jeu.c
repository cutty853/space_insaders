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

void cadre(SDL_Surface *ecran, int pos_x, int pos_y) {
    SDL_Surface *cadre[12]={NULL};
    SDL_Rect pos_cadre;
    int largeur, hauteur, i;

    largeur = 300;
    hauteur = 50;
    pos_cadre.x=pos_x;
    pos_cadre.y=pos_y;

    cadre[0] = SDL_CreateRGBSurface(SDL_HWSURFACE, largeur, hauteur, 32, 0, 0, 0, 0);
    SDL_FillRect(cadre[0], NULL, SDL_MapRGB(cadre[0]->format, 0, 0, 0));
    SDL_BlitSurface(cadre[0], NULL, ecran, &pos_cadre);
    largeur-=10;
    hauteur-=10;
    cadre[1] = SDL_CreateRGBSurface(SDL_HWSURFACE, largeur, hauteur, 32, 0, 0, 0, 0);
    SDL_FillRect(cadre[1], NULL, SDL_MapRGB(cadre[1]->format, 33, 33, 33));
    pos_cadre.x+=5;
    pos_cadre.y+=5;
    SDL_BlitSurface(cadre[1], NULL, ecran, &pos_cadre);
    for (i=0 ; i<10 ; i++) {
        cadre[i+2] = SDL_CreateRGBSurface(SDL_HWSURFACE, largeur-(i+1)*2, hauteur-(i+1)*2, 32, 0, 0, 0, 0);
        SDL_FillRect(cadre[i+2], NULL, SDL_MapRGB(cadre[i+2]->format, 33+((i+1)*3), 33+((i+1)*3),33+((i+1)*3)));
        pos_cadre.x++;
        pos_cadre.y++;
        SDL_BlitSurface(cadre[i+2], NULL, ecran, &pos_cadre);
    }
    SDL_Flip(ecran);
    for (i=0; i<12 ; i++)
        SDL_FreeSurface(cadre[i]);
}

int menu(SDL_Surface *ecran) {
    SDL_Surface *fond = NULL, *titre = NULL, *option[4]= {NULL};
    SDL_Rect pos_fond, pos_titre, pos_option[4];
    SDL_Color couleur_titre = {240, 170, 23}, couleur_option = {242, 242, 242};
    SDL_Event choix;
    TTF_Font *police_titre=NULL, *police_option=NULL;
    char texte_option[4][16]={"Jouer", "Sauvegarder...", "Charger...", "Quitter"};
    int i, continuer=1;

    pos_fond.x=0;
    pos_fond.y=0;

    //Placement d'un image de fond PS: La taille de l'image ne s'ajuste pas a l'écran (pour l'instant)
    fond = IMG_Load("images/background.jpg");
    SDL_BlitSurface(fond, NULL, ecran, &pos_fond);
    SDL_Flip(ecran);
    SDL_FreeSurface(fond);

    //Affichage des boutons
    for (i=0; i<4 ; i++) {
        pos_option[i].x = CENTRER(ecran->w, 300);
        pos_option[i].y = CENTRER(ecran->h,(50*4+30*3))+i*80;
        cadre(ecran, (int)pos_option[i].x, (int)pos_option[i].y);
    }

    //Ajouter les textes à cet endroit
        // TITRE
    police_titre = TTF_OpenFont("policescoalition.ttf", 52);
    titre = TTF_RenderText_Blended(police_titre, "SPACE INSADERS", couleur_titre);
    pos_titre.x = (ecran->w/2) - (titre->w/2);
    pos_titre.y = 150;
    SDL_BlitSurface(titre, NULL, ecran, &pos_titre);
    SDL_FreeSurface(titre);
    TTF_CloseFont(police_titre);
        // OPTIONS DU MENU
    police_option = TTF_OpenFont("polices/geo_sans_light.ttf", 26);
    for (i=0; i<4 ; i++) {
        option[i] = TTF_RenderText_Blended(police_option, texte_option[i], couleur_option);
        pos_option[i].x += 150 - (option[i]->w/2);
        pos_option[i].y += 10;
        SDL_BlitSurface(option[i], NULL, ecran, &pos_option[i]);
        SDL_FreeSurface(option[i]);
    }
    TTF_CloseFont(police_option);
    SDL_Flip(ecran);

    // Ajouter/Faire le code de gestion d'evenements pour les différentes options
    while (continuer) {
        SDL_WaitEvent(&choix);
        switch (choix.type) {
            case SDL_QUIT:
                continuer=0;
                break;
            case SDL_KEYDOWN:
                switch (choix.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        continuer=0;
                        break;
                    default:
                        break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                switch (choix.button.button) {
                    case SDL_BUTTON_LEFT:
                        if ((choix.button.x > CENTRER(ecran->w, 300)) && (choix.button.x < CENTRER(ecran->w, 300)+300)) {
                            if ((choix.button.y > CENTRER(ecran->h, 290)) && (choix.button.y < CENTRER(ecran->h, 290)+50)) {
                                return JOUER;
                            }
                            if ((choix.button.y > CENTRER(ecran->h, 290)+80) && (choix.button.y < CENTRER(ecran->h, 290)+130)) {
                                return SAUVEGARDER;
                            }
                            if ((choix.button.y > CENTRER(ecran->h, 290)+160) && (choix.button.y < CENTRER(ecran->h, 290)+210)) {
                                return CHARGER;
                            }
                            if ((choix.button.y > CENTRER(ecran->h, 290)+240) && (choix.button.y < CENTRER(ecran->h, 290)+290)) {
                                return QUITTER;
                            }
                        }
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }

    }
    return -1;
}

void charge_niveau (SDL_Surface *ecran) {
    SDL_Surface *fond_combat=NULL;
    SDL_Rect pos_fond;
    _vaisseau v_joueur;
    pos_fond.x=0;
    pos_fond.y=0;

    v_joueur.vie=HAUT;
    v_joueur.bouclier=BAS;

    // Affichage du fond de combat
    fond_combat = IMG_Load("images/map_fond_combat.jpg");
    SDL_BlitSurface(fond_combat, NULL, ecran, &pos_fond);
    SDL_Flip(ecran);
    SDL_FreeSurface(fond_combat);

    //Barre de vie
    barre_vie(ecran, v_joueur);
    barre_bouclier(ecran, v_joueur);
}

void barre_vie(SDL_Surface *ecran, _vaisseau v_joueur) {
    /** CETTE FONCTION AFFICHE DES BARRES DE VIE*/

    SDL_Surface *barre_vie=NULL;
    SDL_Rect pos_barre_vie;
    pos_barre_vie.x=5;
    pos_barre_vie.y=TAILLE_ECRAN_Y-25;


    // Affichage de la barre de vie
    barre_vie = SDL_CreateRGBSurface(SDL_HWSURFACE, 150, 20, 32, 0, 0, 0, 0);
    switch (v_joueur.vie) {
        case BAS:
            SDL_FillRect(barre_vie, NULL, SDL_MapRGB(barre_vie->format, 255, 0, 0));
            break;
        case MOYEN:
            SDL_FillRect(barre_vie, NULL, SDL_MapRGB(barre_vie->format, 255, 165, 0));
            break;
        case HAUT:
            SDL_FillRect(barre_vie, NULL, SDL_MapRGB(barre_vie->format, 0, 255, 0));
            break;
    }
    SDL_BlitSurface(barre_vie, NULL, ecran, &pos_barre_vie);
    SDL_Flip(ecran);
    SDL_FreeSurface(barre_vie);
}

void barre_bouclier(SDL_Surface *ecran, _vaisseau v_joueur) {
    SDL_Surface *barre_bouclier=NULL;
    SDL_Rect pos_barre_bouclier;
    pos_barre_bouclier.x=5;
    pos_barre_bouclier.y=TAILLE_ECRAN_Y-50;

    // Affichage de la barre du bouclier
    switch (v_joueur.bouclier) {
        case BAS:
            barre_bouclier = IMG_Load("images/bouclier_BAS.jpg");
            break;
        case MOYEN:
            barre_bouclier = IMG_Load("images/bouclier_MOYEN.jpg");
            break;
        case HAUT:
            barre_bouclier = IMG_Load("images/bouclier_HAUT.jpg");
            break;
    }
    SDL_BlitSurface(barre_bouclier, NULL, ecran, &pos_barre_bouclier);
    SDL_Flip(ecran);
    SDL_FreeSurface(barre_bouclier);
}







