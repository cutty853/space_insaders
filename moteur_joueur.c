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

// La fcontion n'est pas efficace et ne fonctionne pas
void tour_joueur(_input *action, _vaisseau *v_joueur, _explosion *boom, float *vitesse_jeu){
    etat_action_joueur(action);
        // Gestion du clavier
    if (action->key[SDLK_w] || v_joueur->vitesse < v_joueur->vitesse_min) {
        vitesse_joueur(v_joueur, AVANT);
    }else if (action->key[SDLK_s]) {/// Ne doit pas pvr avancer ET ralentir.
        vitesse_joueur(v_joueur, ARRIERE);
    }
    if (action->key[SDLK_a]) {
        v_joueur->angle+= v_joueur->vitesse_rotation;
        v_joueur->etat_rotation = 1;
    }else if (action->key[SDLK_d]) { /// Ne doit pas pvr tourner dans les 2 sens.
        v_joueur->angle-= v_joueur->vitesse_rotation;
        v_joueur->etat_rotation = 1;
    }
    if (action->key[SDLK_c]) {
        v_joueur->vitesse = 0;
    }
    if (action->key[SDLK_SPACE] && !v_joueur->tir.etat) {
        init_tir(v_joueur);
        init_hitbox(&(v_joueur->tir.hitbox), 0, 0, 0, 0, v_joueur->tir.position.x, v_joueur->tir.position.y, 6, 6);
    }
    if (action->key[SDLK_KP1]) {
        v_joueur->vie.charge = VIDE;
    }
    ///CHANGEMENT DU TEMPS:
    if (action->key[SDLK_KP_MINUS]) {///Ralenti le temps.
        *vitesse_jeu += 0.05;
    }else if (action->key[SDLK_KP_PLUS]) {///Accélère le temps.
        if (*vitesse_jeu > 0.5)/// à partir de 0.5, le jeu crash.
            *vitesse_jeu -= 0.05;
    }
}



void vitesse_joueur(_vaisseau *v_joueur, int sens) {
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
