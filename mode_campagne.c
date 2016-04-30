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

    #include "mode_campagne.h"
    #include "utilitaire.h"
    #include "mode_combat.h"
    #include "moteur_ia.h"
    #include "moteur_joueur.h"
    #include "moteur_affichage.h"
#endif


void charge_sprite_systeme (_planete **systeme, int nb_planetes)
{
    SDL_Surface *tmp_planetes = NULL;
    SDL_Rect case_courante;
    int i;

    tmp_planetes = SDL_DisplayFormat(IMG_Load("images/planetes2.png"));
    test_surface(tmp_planetes, 110);
    SDL_SetAlpha(tmp_planetes, SDL_SRCALPHA, 255);
    init_pos(&case_courante, 0, 0);
    case_courante.w = 75;
    case_courante.h = 75;
    for (i=0 ; i<nb_planetes ; i++) {
        // Décalage de la lecture de l'image
        printf("%d zbre\n", (int)&(*systeme+i)->sprite);
        if ((i%3==0) && (i!=0)) {
            case_courante.x = 0;
            case_courante.y += 75;
        } else if (i==0){
            init_pos(&case_courante, 0, 0);
        } else {
            case_courante.x += 75;
        }
        // Collage de chaque planetes dans sa case
        (*systeme+i)->sprite = SDL_CreateRGBSurface(SDL_HWSURFACE, 100, 100, 32, 0, 0, 0, 0);
        SDL_BlitSurface(tmp_planetes, &case_courante, (*systeme+i)->sprite, NULL);
    }
    SDL_FreeSurface(tmp_planetes);
}

void decharge_sprite_systeme(_planete **systeme, int nb_planetes)
{
    int i;
    for (i=0; i<nb_planetes; i++)
        SDL_FreeSurface((systeme[i])->sprite);
}

void init_planete(_planete *planete, char *src_sprite, char *etat_planete)
{
}

void progression(SDL_Surface *ecran) {
    SDL_Surface *map=NULL;
    float zoom;
    _planete *systeme;
    int i;

    systeme = malloc(sizeof(_planete)*9);
    // Affichage de la map de fond de campagne en fonction de l'écran du joueur a retravailler pour les écran autre que 16/9
    map = IMG_Load("images/map_fond_campagne.jpg");
    test_surface(map, 106);
    zoom = (ecran->h*1.0)/(map->h*1.0);
    map = rotozoomSurface(map, 0, zoom, 1);
    SDL_BlitSurface(map, NULL, ecran, NULL);
    SDL_Flip(ecran);
    printf("%d", sizeof(_planete));

    // Placement et établissement des entités : Planètes
    charge_sprite_systeme(&systeme, 9);
    for (i=0;i<NB_PLANETES;i++) {
        init_pos(&((systeme+i)->position), 10, 10);
        SDL_BlitSurface((systeme+i)->sprite, NULL, ecran, &((systeme+i)->position));
    }
    SDL_Flip(ecran);

    // Pause permettant les tests.
    pause();
    SDL_FreeSurface(map);
    decharge_sprite_systeme(&systeme, 9);
}
