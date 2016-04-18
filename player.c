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
    /// Suggestion : ajouté
    if (sens == AVANT) {
        if (v_joueur->vitesse == 0) {
            v_joueur->vitesse = VITESSE_MIN_JOUEUR;
        } else {
            if ((v_joueur->vitesse + (v_joueur->acceleration/100.0)) < v_joueur->vitesse_max)
                v_joueur->vitesse += (v_joueur->acceleration/100.0);
        }
        if (v_joueur->vitesse < VITESSE_MIN_JOUEUR && v_joueur->vitesse > -VITESSE_MIN_JOUEUR) {
            v_joueur->vitesse = 0;
        }
    }
    // A modifié la vitesse minimum est désormais un champ de la structure vaisseau
    if (sens == ARRIERE) {
        if (v_joueur->vitesse == 0) {
            v_joueur->vitesse = -VITESSE_MIN_JOUEUR;
        } else {
            if ((v_joueur->vitesse - (v_joueur->acceleration/100.0)) > -v_joueur->vitesse_max)
                v_joueur->vitesse -= (v_joueur->acceleration/100.0);
        }
        if (v_joueur->vitesse < VITESSE_MIN_JOUEUR && v_joueur->vitesse > -VITESSE_MIN_JOUEUR) {
            v_joueur->vitesse = 0;
        }
    }
}

void charge_sprite_explosion (_explosion *boom)
{
    SDL_Surface *tmp_boom;
    SDL_Rect case_courante;
    int i;

    tmp_boom = IMG_Load("images/explosion.png");
    test_surface(tmp_boom, 150);
    init_pos (&case_courante, 0, 0);
    case_courante.w = 65;
    case_courante.h = 65;
    for (i=0 ; i<NB_SPRITES_EXPLOSION ; i++) {
        if ((i%5 == 0) && i!=0) {
            case_courante.y += 65;
            case_courante.x = 0;
        } else {
            case_courante.x += 65;
        }
        boom->sprite[i] = IMG_Load("images/transparent_explosion.png");
        test_surface(boom->sprite[i], 104+i);
        SDL_BlitSurface(tmp_boom, &case_courante, boom->sprite[i], NULL);
        SDL_SetAlpha(boom->sprite[i], SDL_SRCALPHA, 255);
    }
}

SDL_Rect eff_vaisseau(SDL_Surface *ecran, _vaisseau *vaisseau, SDL_Surface *save_screen)
{
    SDL_BlitSurface(save_screen, &(vaisseau->position), ecran, &(vaisseau->position));
    return vaisseau->position;
}

// ça fait vraiment des petites fonctions...
SDL_Rect aff_explosion (SDL_Surface *ecran, _vaisseau *vaisseau)
{
    init_pos(&(vaisseau->explosion->position), vaisseau->position.x, vaisseau->position.y);
    SDL_BlitSurface(vaisseau->explosion->sprite[vaisseau->explosion->phase], NULL, ecran, &(vaisseau->explosion->position));
    return vaisseau->position;
}

void decharge_sprite_explosion (_explosion *boom)
{
    int i;
    for (i=0;i<NB_SPRITES_EXPLOSION;i++)
        SDL_FreeSurface(boom->sprite[i]);
}

void charge_sprite_laser(_tir *tir, _vaisseau vaisseau)
{
    tir->type = vaisseau.arme;
    tir->sprite = NULL;
    switch (tir->type) {
        case TIR_LASER:
            tir->sprite = IMG_Load("images/tir_laser.png");
            break;
        case RAYON_LASER: // Risque d'etre dur a mettre en image
            tir->sprite = IMG_Load("images/rayon_laser.png");
            break;
        case OBUS:
            tir->sprite = IMG_Load("images/obus.png");
            break;
    }
    test_surface(tir->sprite, 180);
}

void charge_tir (_tir *tir, _vaisseau vaisseau)
{
    charge_sprite_laser(tir, vaisseau);
    tir->angle = (int)vaisseau.angle; // Le cast est temporaire
    tir->degat = vaisseau.tir.degat;
    tir->type = vaisseau.tir.type;
    init_pos (&(tir->position), vaisseau.position.x, vaisseau.position.y);
    tir->sprite = vaisseau.tir.sprite;
    tir->vitesse = vaisseau.tir.vitesse;
}

SDL_Rect eff_tir (SDL_Surface *ecran, _tir *tir, SDL_Surface *save_screen)
{
    SDL_BlitSurface(save_screen, &(tir->position), ecran, &(tir->position));
    return tir->position;
}

SDL_Rect aff_tir (SDL_Surface *ecran, _tir *tir)
{
    SDL_Surface *tmp_tir = NULL;

    tmp_tir = rotozoomSurface(tir->sprite, (double)tir->angle, 1.0, 1);
    test_surface(tmp_tir, 180);
    tir->position.x = tir->vitesse * cos(RADIANATION(tir->angle));
    tir->position.y = tir->vitesse * (-sin(RADIANATION(tir->angle)));
    SDL_BlitSurface(tmp_tir, NULL, ecran, &(tir->position));
    SDL_FreeSurface(tmp_tir);

    return tir->position;
}
