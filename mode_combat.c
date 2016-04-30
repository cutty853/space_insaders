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

    #include "mode_campagne.h"
    #include "utilitaire.h"
    #include "mode_campagne.h"
    #include "moteur_ia.h"
    #include "moteur_joueur.h"
    #include "moteur_affichage.h"
#endif


void play(SDL_Surface *ecran) {
    _input action;
    TTF_Font *police_texte=NULL;
    int temps_actuel=0, temps_precedent=0, nb_pos_to_up_ecran=0, etat_interface[NB_ETAT_INTERFACE];
    SDL_Surface *save_screen = NULL;
    SDL_Rect *pos_to_up_console;
    SDL_Rect pos_to_up_ecran[9], pos_to_up_tir_ia[2], pos_to_up_tir_joueur[2]; /// 9 = nombre actuel de nouvelles positions.
    _vaisseau v_player, v_ia1;
    _explosion boom;
    etat_interface[CONSOLE]=0;

    /// Zone pour les commandes a effectué dès l'affichage de la carte
    memset(&action,0,sizeof(action));
        /// Chargement de la map
    charge_niveau(ecran);
    charge_sprite_explosion(&boom);
        /// ia:
    init_vaisseau(&v_ia1, IA, CHERCHE, 100, 0, 0.1, 8, HAUT, HAUT, TIR_LASER, 1000, 300, 3, 90);
    charge_sprite_bouclier(&v_ia1);
    charge_sprite_vie(&v_ia1);
    charge_sprite_tir(&v_ia1);
        /// joueur:
    init_vaisseau(&v_player, JOUEUR, INDEPENDENT, 100, 0, 0.1, 8, HAUT, HAUT, OBUS, 100, 300, 3, 270);
    charge_sprite_bouclier(&v_player);
    charge_sprite_vie(&v_player);
    charge_sprite_tir(&v_player);
        /// test console
    pos_to_up_console = malloc(sizeof(SDL_Rect)*1);
    police_texte = TTF_OpenFont("polices/geo_sans_light.ttf", 18);
    test_police(police_texte, 202);
        ///Affichage de la barre de vie & de la barre du bouclier du joueur:
    barre_vie_joueur(ecran, v_player);
    barre_bouclier_joueur(ecran, v_player);
        /// Sauvegarde de l'écran
    save_screen = SDL_DisplayFormat(ecran);

    /// boucle du jeu:
    while (!action.key[SDLK_ESCAPE] && !action.quit) {
        if(v_ia1.tir.etat == 1)
            v_ia1.tir.temps_passe ++;
        if(v_ia1.tir.temps_passe%50 == 0){ //Permet de reset le tir, 50 est pris totalement au hasard ! (à cause du rotozoom qui va en fait changer la rotation de l'image source)
            v_ia1.tir.etat = 0;

            eff_tir(ecran, save_screen, &v_ia1);
            decharge_sprite_tir(&v_ia1);
            charge_sprite_tir(&v_ia1);
        } // apporte une erreur: le laser s'arrête dès que on repasse dans cette condition, ce qui est norml vu le code. (peut peut être être (lol) utilisé comme une "portée max" du lasez ?)

        if(v_player.tir.etat == 1)
            v_player.tir.temps_passe ++;
        if(v_player.tir.temps_passe%50 == 0){ //Permet de reset le tir, 50 est pris totalement au hasard ! (à cause du rotozoom qui va en fait changer la rotation de l'image source)
            v_player.tir.etat = 0;

            eff_tir(ecran, save_screen, &v_player);
            decharge_sprite_tir(&v_player);
            charge_sprite_tir(&v_player);
        } // apporte une erreur: le laser s'arrête dès que on repasse dans cette condition, ce qui est norml vu le code. (peut peut être être (lol) utilisé comme une "portée max" du lasez ?)


        /// IL FAUT D'ABORD "CACHER" LES ANCIENNES SURFACES PUIS FAIRE LES ACTIONS (déplacement) PUIS REAFFICHER LES SURFACES AVEC LES NOUVELLES POSITIONS !
        pos_to_up_ecran[0] = eff_bouclier(ecran, &v_ia1, save_screen);
        pos_to_up_ecran[1] = eff_vie(ecran, &v_ia1, save_screen);
        pos_to_up_ecran[2] = eff_vaisseau(ecran, &v_ia1, save_screen);
        pos_to_up_ecran[3] = eff_vaisseau(ecran, &v_player, save_screen);

        if (v_ia1.tir.etat == 1){
            pos_to_up_tir_ia[0] = eff_tir(ecran, save_screen, &v_ia1);
            pos_to_up_tir_ia[1] = aff_tir(ecran, &v_ia1);
        }
        if (v_player.tir.etat == 1){
            pos_to_up_tir_joueur[0] = eff_tir(ecran, save_screen, &v_player);
            pos_to_up_tir_joueur[1] = aff_tir(ecran, &v_player);
        }

        pos_to_up_ecran[4] = aff_vaisseau(ecran, &v_ia1, save_screen);/// TOUJOURS afficher le vaisseau en premier dans l'appelle des fonction (dans cette version de la fonction).
        pos_to_up_ecran[5] = aff_bouclier(ecran, &v_ia1);
        pos_to_up_ecran[6] = aff_vie(ecran, &v_ia1);

        /// L'ia joue en première:
        tour_ia(&v_ia1, &v_player, ecran);


        /// Test de l'action du joueur
        //tour_joueur(&action, v_player, &etat_interface, save_screen, ecran, police_texte, boom); // Ne fonctionne pas ((((a travailler))))
        etat_action_joueur(&action);
            // Gestion du clavier
        if (action.key[SDLK_w]) {
            vitesse_joueur(&v_player, AVANT);
        }
        if (action.key[SDLK_s]) {
            vitesse_joueur(&v_player, ARRIERE);
        }
        if (action.key[SDLK_a]) {
            v_player.angle+= v_player.vitesse_rotation;
            v_player.etat_rotation = 1;
        }
        if (action.key[SDLK_d]) {
            v_player.angle-= v_player.vitesse_rotation;
            v_player.etat_rotation = 1;
        }
        if (action.key[SDLK_c]) {
            v_player.vitesse = 0;
        }
        if (action.key[SDLK_SPACE] && !v_player.tir.etat) {
            v_player.tir.temps_passe = 0;
            v_player.tir.etat = 1;
            init_tir(&v_player);
        }
        if (action.key[SDLK_F3]) {
            if (etat_interface[CONSOLE]) {
                etat_interface[CONSOLE]=0;
                SDL_BlitSurface(save_screen, &(pos_to_up_console[0]), ecran, &(pos_to_up_console[0]));
                SDL_UpdateRects(ecran, 1, pos_to_up_console);
                action.key[SDLK_F3] = 0; // Ici on fait remonter la touche du joueur de façon manuelle
            } else {
                etat_interface[CONSOLE]=1;
                action.key[SDLK_F3] = 0; // Ici on fait remonter la touche du joueur de façon manuelle
            }
        }
        if (action.key[SDLK_KP1]) {
            v_player.vie.charge = VIDE;
            boom.phase=0;
//            action.key[SDLK_KP1]=0;
        }
            // Gestion de la souris
        if (action.mousebuttons[SDL_BUTTON_LEFT]) {
            charge_niveau(ecran);
            init_pos(&(v_player.position), action.mousex, action.mousey);
        }


        /// Gestion du temps pour éviter la surexploitation du CPU
        temps_actuel=SDL_GetTicks();
        if (temps_actuel - temps_precedent > CALCUL_FPS(FPS)) {
            temps_precedent=temps_actuel;
        } else {
            SDL_Delay(30 - (temps_actuel - temps_precedent));
        }


        /// Zone pour placer les commandes a faire après la pause du jeu


        nb_pos_to_up_ecran = 7;
        switch (v_player.vie.charge) {
            case BAS:
            case MOYEN:
            case HAUT:
                if ((v_player.vitesse !=0)/* || (action.key.keysym.sym == SDLK_a) || (action.key.keysym.sym == SDLK_d) || (action.button.button == SDL_BUTTON_LEFT)*/) {
                    pos_to_up_ecran[7] = aff_vaisseau(ecran, &v_player, save_screen);
                    nb_pos_to_up_ecran = 8;
                }
                break;
            case VIDE:
                if (boom.phase < NB_SPRITES_EXPLOSION) {
                    pos_to_up_ecran[7] = eff_vaisseau(ecran, &v_player, save_screen);
                    pos_to_up_ecran[8] = aff_explosion(ecran, &boom, v_player);
                    boom.phase++;
                    nb_pos_to_up_ecran = 9;
                }
                break;
        }

        /// CONSOLE
        if (etat_interface[CONSOLE]) {
            pos_to_up_console[0] = aff_console(ecran, v_player, save_screen, police_texte);
            SDL_UpdateRects(ecran, 1, pos_to_up_console);
        }
        /// AFFICHAGE:
        SDL_UpdateRects(ecran, 2, pos_to_up_tir_ia); // Pas optimisé, affichage permanant même quand pas de tir
        SDL_UpdateRects(ecran, 2, pos_to_up_tir_joueur); // Pas optimisé, affichage permanant même quand pas de tir
        SDL_UpdateRects(ecran, nb_pos_to_up_ecran, pos_to_up_ecran);
    }

    decharge_sprite_explosion(&boom);
    decharge_sprite_bouclier(&v_ia1);
    decharge_sprite_vie(&v_ia1);
    decharge_sprite_tir(&v_ia1);
    SDL_FreeSurface(v_ia1.sprite);
    SDL_FreeSurface(v_player.sprite);
    decharge_sprite_bouclier(&v_player);
    decharge_sprite_vie(&v_player);
    decharge_sprite_tir(&v_player);
}
