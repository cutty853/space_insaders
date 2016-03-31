#ifndef DEF_INCLUDE
    #define DEF_INCLUDE
    #include <stdlib.h>
    #include <stdio.h>
    #include <SDL/SDL.h>
    #include <SDL/SDL_image.h>
    #include <SDL/SDL_ttf.h>
    #include "structure.h"
    #include "utilitaire.h"
    #include "jeu.h"
    #include "ia.h"
    #include "player.h"

    #include "constantes.h"
#endif

int check_player_action (SDL_Surface *ecran) {
    SDL_Event action;
    int continuer=1, temps_actuel=0, temps_precedent=0;
    SDL_Surface *player;
    SDL_Rect pos_player;

    pos_player.x=0;
    pos_player.y=0;

    while (continuer) {
        SDL_PollEvent(&action);

        // Test de l'action du joueur
        switch (action.type) {
            case SDL_KEYDOWN:
                switch (action.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        continuer=0;
                        break;
                    default:
                        break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                switch (action.button.button) {
                    case SDL_BUTTON_LEFT:
                        charge_niveau(ecran);
                        pos_player.x = action.button.x;
                        pos_player.y = action.button.y;
                        player = IMG_Load("images/player_ship.png");
                        test_surface(player, 105);
                        SDL_BlitSurface(player, NULL, ecran, &pos_player);
                        SDL_Flip(ecran);
                        SDL_FreeSurface(player);
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }

        // Gestion du temps pour éviter la surexploitation du CPU
        temps_actuel=SDL_GetTicks();
        if (temps_actuel - temps_precedent > CALCUL_FPS(FPS)) {
            temps_precedent=temps_actuel;
        } else {
            SDL_Delay(30 - (temps_actuel - temps_precedent));
        }

    }
}







