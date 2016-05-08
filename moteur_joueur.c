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

    #include "moteur_joueur.h"
    #include "utilitaire.h"
    #include "mode_combat.h"
    #include "mode_campagne.h"
    #include "moteur_ia.h"
    #include "moteur_affichage.h"
    #include "moteur_physique.h"
#endif

void etat_action_joueur(_input* in)
{
	SDL_Event event;
        in->mousebuttons[SDL_BUTTON_WHEELUP] = 0;
        in->mousebuttons[SDL_BUTTON_WHEELDOWN] = 0;
	while(SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			in->key[event.key.keysym.sym]=1;
			break;
		case SDL_KEYUP:
			in->key[event.key.keysym.sym]=0;
			break;
		case SDL_MOUSEMOTION:
			in->mousex=event.motion.x;
			in->mousey=event.motion.y;
			in->mousexrel=event.motion.xrel;
			in->mouseyrel=event.motion.yrel;
			break;
		case SDL_MOUSEBUTTONDOWN:
			in->mousebuttons[event.button.button]=1;
			break;
		case SDL_MOUSEBUTTONUP:
                        if (event.button.button!=SDL_BUTTON_WHEELUP && event.button.button!=SDL_BUTTON_WHEELDOWN)
			   in->mousebuttons[event.button.button]=0;
			break;
		case SDL_QUIT:
			in->quit = 1;
			break;
		default:
			break;
		}
	}
}

/* La fcontion n'est pas efficace et ne fonctionne pas
void tour_joueur(_input *action, _vaisseau *v_joueur, int **etat_interface, SDL_Surface *save_screen, SDL_Surface *ecran, TTF_Font *police_texte, _explosion *boom)
{
    SDL_Rect pos_to_up_console[1];
    etat_action_joueur(action);
        // Gestion du clavier
    if (action->key[SDLK_w]) {
        vitesse_joueur(v_joueur, AVANT);
    }
    if (action->key[SDLK_s]) {
        vitesse_joueur(v_joueur, ARRIERE);
    }
    if (action->key[SDLK_a]) {
        v_joueur->angle+= v_joueur->vitesse_rotation;
        v_joueur->etat_rotation = 1;
    }
    if (action->key[SDLK_d]) {
        v_joueur->angle-= v_joueur->vitesse_rotation;
        v_joueur->etat_rotation = 1;
    }
    if (action->key[SDLK_c]) {
        v_joueur->vitesse = 0;
    }
    if (action->key[SDLK_SPACE] && !v_joueur->tir.etat) {
        v_joueur->tir.temps_passe = 0;
        v_joueur->tir.etat = 1;
        init_tir(v_joueur);
    }
    if (action->key[SDLK_F3]) {
        if ((*etat_interface)[CONSOLE]) {
            (*etat_interface)[CONSOLE]=0;
            SDL_BlitSurface(save_screen, &(pos_to_up_console[0]), ecran, &(pos_to_up_console[0]));
            SDL_UpdateRects(ecran, 1, pos_to_up_console);
        } else {
            (*etat_interface)[CONSOLE]=1;
            pos_to_up_console[0] = aff_console(ecran, *v_joueur, save_screen, police_texte);
            SDL_UpdateRects(ecran, 1, pos_to_up_console);
            action->key[SDLK_F3] = 0; // Ici on fait remonter la touche du joueur de façon manuelle
        }
    }
    if (action->key[SDLK_KP1]) {
        v_joueur->vie.charge = VIDE;
        boom->phase=0;
    }
        // Gestion de la souris
    if (action->mousebuttons[SDL_BUTTON_LEFT]) {
        charge_niveau(ecran);
        init_pos(&(v_joueur->position), action->mousex, action->mousey);
    }
}
*/

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
