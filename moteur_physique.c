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
int col_point_boite (_point *x, SDL_Rect *boite) {
    if ((x->x >= boite->x) && (x->x <= boite->x+boite->w) && (x->y >= boite->y) && (x->y <= boite->y+boite->h))
        return 1;
    else
        return 0;
}

int col_point_cercle (int x, int y, _cercle* c) {
    // La fonction sqrt n'a pas été utilisé, car inutile et trop demandeuse de ressources
    int distance = (x-(c->centre.x))*(x-(c->centre.x)) + (y-(c->centre.y))*(y-(c->centre.y));
    if (distance <= (c->rayon*c->rayon))
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


int col_cercle_cercle (_cercle *c1, _cercle *c2) {
    int distance = ((c1->centre.x)-(c2->centre.x))*((c1->centre.x)-(c2->centre.x)) + ((c1->centre.y)-(c2->centre.y))*((c1->centre.y)-(c2->centre.y));
    if (distance >= (c1->rayon*c1->rayon)+(c2->rayon*c2->rayon))
        return 1;
    else
        return 0;
}

int col_polygone_cercle (_polygone *p, _cercle* c){
    int i;
    for (i=0; i<p->nb_points; i++) {
        if (col_point_cercle(p->points[i].x, p->points[i].y, c))
            return 1;
    }
    return 0;
}

int col_polygone_point (_polygone *p, _point* x) {
    int i;
    _point a, b;
    _vecteur d, e;
    float det;
    for (i=0; i<p->nb_points ; i++) {
        a=p->points[i];
        if (i==p->nb_points-1)
            b = p->points[0];
        else
            b = p->points[i+1];
        d.x = b.x - a.x;
        d.y = b.y - a.y;
        e.x = x->x - a.x;
        e.y = x->y - a.y;
        det = (d.x*e.y) - (d.y*e.x);
        if (det<=0)
            return 0;
    }
    return 1;
}

int col_aabb_cercle (SDL_Rect* aabb, _cercle *c) {
    if(col_point_cercle(aabb->x, aabb->y, c) || col_point_cercle(aabb->x+aabb->w, aabb->y, c) || col_point_cercle(aabb->x+aabb->w, aabb->y+aabb->h, c) || col_point_cercle(aabb->x, aabb->y+aabb->h, c))
        return 1;
    else
        return 0;
}

void init_hitbox(_hitbox* h, int xCercle, int yCercle, int rCercle, int nb_points, int xAABB, int yAABB, int wAABB, int hAABB) {
    if (nb_points==0)
        h->polygone.points = NULL;
    else
        h->polygone.points = malloc(sizeof(_point)*nb_points);
    h->polygone.nb_points = nb_points;
    h->cercle.centre.x = xCercle;
    h->cercle.centre.y = yCercle;
    h->cercle.rayon = rCercle;
    init_pos(&(h->aabb), xAABB, yAABB);
}

void calcul_pos_hitbox_vaisseau(_vaisseau *v) {
    int zoom=v->position.w/TAILLE_JOUEUR;
    v->hitbox.cercle.centre.x = v->position.x+(TAILLE_JOUEUR/2)*zoom;
    v->hitbox.cercle.centre.y = v->position.y+(TAILLE_JOUEUR/2)*zoom;
    v->hitbox.cercle.rayon = TAILLE_JOUEUR/2;
}

void calcul_pos_hitbox_tir(_tir* t) {
    t->hitbox.aabb.x += (t->vitesse)*sin(-RADIANATION(t->angle));
    t->hitbox.aabb.y += (t->vitesse)*(-cos(RADIANATION(t->angle)));
}

void transform_aabb_polygone(SDL_Rect* aabb, _polygone *p) {
    p->points = malloc(sizeof(_point)*4);
    p->nb_points = 4;
    p->points[0].x = aabb->x;
    p->points[0].y = aabb->y;
    p->points[1].x = aabb->x+aabb->w;
    p->points[1].y = aabb->y;
    p->points[2].x = aabb->x+aabb->w;
    p->points[2].y = aabb->y+aabb->h;
    p->points[3].x = aabb->x;
    p->points[3].y = aabb->y+aabb->h;
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
