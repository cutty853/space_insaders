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
    #include "moteur_initialisation.h"
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

void supprime_hitbox (_hitbox* h) {
    h->polygone.points = NULL;
    h->polygone.nb_points = 0;
    h->cercle.centre.x = 0;
    h->cercle.centre.y = 0;
    h->cercle.rayon = 0;
    init_pos(&(h->aabb), 0, 0);
    h->aabb.w = 0;
    h->aabb.h = 0;
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

//SDL_Rect eff_hitbox_aabb(SDL_Rect *aabb, SDL_Surface *save_screen, SDL_Surface *ecran) {
//    SDL_BlitSurface(save_screen, aabb, ecran, aabb);
//    return *aabb;
//}
//
//SDL_Rect aff_hitbox_aabb (SDL_Rect *aabb, SDL_Surface *ecran) {
//    SDL_Surface* ligne[2] = {NULL};
//    SDL_Rect pos_hitbox;
//    ligne[0] = SDL_CreateRGBSurface(SDL_HWSURFACE, aabb->w, 2, 32, 0, 0, 0, 0);
//    ligne[1] = SDL_CreateRGBSurface(SDL_HWSURFACE, 2, aabb->h, 32, 0, 0, 0, 0);
//    SDL_FillRect(ligne[0], NULL, SDL_MapRGB(ligne[0]->format, 0, 255, 0));
//    SDL_FillRect(ligne[1], NULL, SDL_MapRGB(ligne[1]->format, 0, 255, 0));
//
//    init_pos(&pos_hitbox, aabb->x, aabb->y);
//    SDL_BlitSurface(ligne[0], NULL, ecran, &pos_hitbox);
//    init_pos(&pos_hitbox, aabb->x, aabb->y+aabb->h-2);
//    SDL_BlitSurface(ligne[0], NULL, ecran, &pos_hitbox);
//    init_pos(&pos_hitbox, aabb->x, aabb->y);
//    SDL_BlitSurface(ligne[1], NULL, ecran, &pos_hitbox);
//    init_pos(&pos_hitbox, aabb->x+aabb->w-2, aabb->y);
//    SDL_BlitSurface(ligne[1], NULL, ecran, &pos_hitbox);
//
//    return *aabb;
//}
//
//SDL_Rect aff_hitbox_cercle (_cercle *c, SDL_Surface* ecran, int x, int y) {
//    SDL_Surface *carre=SDL_CreateRGBSurface(SDL_HWSURFACE, 32, c->rayon, c->rayon, 0, 0, 0, 0);
//    SDL_Surface *tmp_rotation=NULL;
//    SDL_Rect pos, pre_pos;
//    int i;
//    init_pos(&pos, 100, 200);
//    SDL_FillRect(carre, NULL, SDL_MapRGB(carre->format, 0, 255, 0));
//    for (i=0; i<360; i++) {
//        pre_pos.w = carre->w;
//        pre_pos.h = carre->h;
//        tmp_rotation = rotozoomSurface(carre, i, 1, 1);
//        if (i==1000) {
//            pos.x -= ((pos.w - c->rayon)-(pre_pos.w - c->rayon))/2;
//            pos.y -= ((pos.h - c->rayon)-(pre_pos.h - c->rayon))/2;
//        }
//        SDL_BlitSurface(tmp_rotation, NULL, ecran, &pos);
//    }
//}
