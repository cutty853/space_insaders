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

void col_tir_objet(SDL_Surface *ecran){

}

/* ********************************************************************* */
/*obtenirPixel : permet de récupérer la couleur d'un pixel
Paramètres d'entrée/sortie :
SDL_Surface *surface : la surface sur laquelle on va récupérer la couleur d'un pixel
int x : la coordonnée en x du pixel à récupérer
int y : la coordonnée en y du pixel à récupérer

Uint32 resultat : la fonction renvoie le pixel aux coordonnées (x,y) dans la surface
*/
Uint32 obtenirPixel(SDL_Surface *surface, _vaisseau *vaisseau){
    int x = 5;//vaisseau->tir.position.x+5;
    int y = 5;//vaisseau->tir.position.y+1;

    /*nbOctetsParPixel représente le nombre d'octets utilisés pour stocker un pixel.
    En multipliant ce nombre d'octets par 8 (un octet = 8 bits), on obtient la profondeur de couleur
    de l'image : 8, 16, 24 ou 32 bits.*/
    int nbOctetsParPixel = surface->format->BytesPerPixel;
    /* Ici p est l'adresse du pixel que l'on veut connaitre */
    /*surface->pixels contient l'adresse du premier pixel de l'image*/
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * nbOctetsParPixel;

    /*Gestion différente suivant le nombre d'octets par pixel de l'image*/
    switch(nbOctetsParPixel){
        case 1:
            return *p;

        case 2:
            return *(Uint16 *)p;

        case 3:
            /*Suivant l'architecture de la machine*/
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;

        case 4:
            return *(Uint32 *)p;

        /*Ne devrait pas arriver, mais évite les erreurs*/
        default:
            return 0;
    }
}

