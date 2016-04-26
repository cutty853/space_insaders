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
            v_joueur->vitesse = VITESSE_MINI_COS;
        } else {
            if ((v_joueur->vitesse + (v_joueur->acceleration)) < v_joueur->vitesse_max)
                v_joueur->vitesse += (v_joueur->acceleration);
        }
        if (v_joueur->vitesse < VITESSE_MINI_COS && v_joueur->vitesse > -VITESSE_MINI_COS) {
            v_joueur->vitesse = 0;
        }
    }
    // A modifié la vitesse minimum est désormais un champ de la structure vaisseau
    if (sens == ARRIERE) {
        if (v_joueur->vitesse == 0) {
            v_joueur->vitesse = -VITESSE_MINI_COS;
        } else {
            if ((v_joueur->vitesse - v_joueur->acceleration) > v_joueur->vitesse_min)
                v_joueur->vitesse -= v_joueur->acceleration;
        }
        if (v_joueur->vitesse < VITESSE_MINI_COS && v_joueur->vitesse > -VITESSE_MINI_COS) {
            v_joueur->vitesse = 0;
        }
    }
}

void charge_sprite_explosion (_explosion *boom)
{
    /// le pointeur sur boom peut etre mis a NULL pour que le chargement de boom ne soit pas effectué (sert a la fonction charge_niveau)
    SDL_Surface *tmp_boom;
    SDL_Rect case_courante;
    int i;

    tmp_boom = IMG_Load("images/explosion.png");
    test_surface(tmp_boom, 102);
    init_pos (&case_courante, 0, 0);
    case_courante.w = LARGEUR_EXPLOSION;
    case_courante.h = HAUTEUR_EXPLOSION;
    for (i=0 ; i<NB_SPRITES_EXPLOSION ; i++) {
        if ((i%5 == 0) && i!=0) {
            case_courante.y += 65;
            case_courante.x = 0;
        } else {
            case_courante.x += 65;
        }
        boom->sprite[i] = IMG_Load("images/transparent_explosion.png");
        test_surface(boom->sprite[i], 103);
        SDL_BlitSurface(tmp_boom, &case_courante, boom->sprite[i], NULL);
        test_surface(boom->sprite[i], 302+i);
    }
    SDL_FreeSurface(tmp_boom);
}

// ça fait vraiment des petites fonctions...
SDL_Rect aff_explosion (SDL_Surface *ecran, _explosion *boom, _vaisseau vaisseau)
{
    init_pos(&(boom->position), vaisseau.position.x, vaisseau.position.y);
    SDL_BlitSurface(boom->sprite[boom->phase], NULL, ecran, &(boom->position));
    return vaisseau.position;
}

void decharge_sprite_explosion (_explosion *boom)
{
    int i;
    for (i=0;i<NB_SPRITES_EXPLOSION;i++)
        SDL_FreeSurface(boom->sprite[i]);
}

void charge_sprite_tir (_tir *pew)
{
    pew->sprite = IMG_Load("images/tir_laser.png");
    pew->vitesse = 3.5;
}

void init_tir (_tir *pew, _vaisseau vaisseau)
{
    // Initialisation de tous les parametres du tir
    init_pos(&(pew->position), vaisseau.position.x+TAILLE_JOUEUR, vaisseau.position.y+(TAILLE_JOUEUR/2.0)-5);
    pew->type = vaisseau.arme;
    pew->angle = vaisseau.angle;
}

SDL_Rect eff_tir(SDL_Surface *ecran, SDL_Surface *save_screen, _tir *pew)
{
    SDL_BlitSurface(save_screen, &(pew->position), ecran, &(pew->position));
    return pew->position;
}

SDL_Rect aff_tir (SDL_Surface *ecran, _tir *pew)
{
    // Calcul du mouvement
    pew->position.x += pew->vitesse * (-sin(RADIANATION(pew->angle)));
    pew->position.y += pew->vitesse * (-cos(RADIANATION(pew->angle)));

    printf("=====================================\n");
    printf("            Etats du tir\n");
    printf("angle : %d\n", pew->angle);
    printf("=====================================\n");

    // Blit de la surface avec ces nouvelles position
    SDL_BlitSurface(pew->sprite, NULL, ecran, &(pew->position));
    return pew->position;
}






