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

    #include "moteur_affichage.h"
    #include "moteur_physique.h"
    #include "utilitaire.h"
    #include "mode_combat.h"
    #include "mode_campagne.h"
    #include "moteur_ia.h"
    #include "moteur_joueur.h"
#endif

// COLLISIONS
int col_point_boite (int x, int y, SDL_Rect *boite) {
    if ((x >= boite->x) && (x <= boite->x+boite->w) && (y >= boite->y) && (y <= boite->y+boite->h))
        return 1;
    else
        return 0;
}

int col_boite_boite (SDL_Rect *repere, SDL_Rect *boite) {
    if ((boite->x >= repere->x + repere->w) || (boite->y >= repere->y + repere->h) || (boite->x + boite->w <= repere->x) || (boite->y + boite->h <= repere->y))
        return 1;
    else
        return 0;
}

int col_point_cercle (int x, int y, _cercle* c) {
    // La fonction sqrt n'a pas été utilisé, car inutile et trop demandeuse de ressources
    int distance = (x-c->x)*(x-c->x) + (y-c->y)*(y-c->y);
    if (distance <= c->rayon*c->rayon)
        return 1;
    else
        return 0;
}
