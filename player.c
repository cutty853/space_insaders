#ifndef DEF_INCLUDE
    #define DEF_INCLUDE
    #include <stdlib.h>
    #include <stdio.h>
    #include <SDL/SDL.h>
    #include <SDL/SDL_image.h>
    #include <SDL/SDL_ttf.h>
    #include <SDL/SDL_rotozoom.h>
    #include <math.h>
    #include "structure.h"
    #include "utilitaire.h"
    #include "jeu.h"
    #include "ia.h"
    #include "player.h"

    #include "constantes.h"
#endif

void vitesse_joueur(_vaisseau *v_joueur, int sens) {
    if (sens == AVANT)
        v_joueur->vitesse += v_joueur->acceleration;
    if (sens == ARRIERE)
        v_joueur->vitesse -= v_joueur->acceleration;
}
