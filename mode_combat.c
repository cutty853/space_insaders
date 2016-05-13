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
    #include "moteur_physique.h"
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

    /// Zone pour les commandes a effectu� d�s l'affichage de la carte
    memset(&action,0,sizeof(action));
        /// Chargement de la map
    charge_niveau(ecran);
    charge_sprite_explosion(&boom);
        /// ia:
    init_vaisseau(&v_ia1, IA, CHERCHE, 25, 0, 0.1, 8, HAUT, HAUT, TIR_LASER, 1000, 300, 3, 90);
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
        /// Sauvegarde de l'�cran
    save_screen = SDL_DisplayFormat(ecran);
    pos_to_up_ecran[0] = aff_vaisseau(ecran, &v_player, save_screen);
    SDL_UpdateRect(ecran, pos_to_up_ecran[0].x, pos_to_up_ecran[0].y, pos_to_up_ecran[0].w, pos_to_up_ecran[0].y);

    /// boucle du jeu:
    while (!action.key[SDLK_ESCAPE] && !action.quit) {
        /** SUGGESTION
        * Plutot que d'utilis� des tableaux pour mettre a jour l'�cran on pourrait utiliser les listes chain�s
        * ainsi on pourrait plus facilement ajouter et enlever des cases a ce tableau de mises a jour, ceci
        * afin d'�viter de faire des optimisation inutile (a cause de l'allocation de tableau dynamique a chaque
        * tour de boucle)
        */
        if(v_ia1.tir.etat == 1)
            v_ia1.tir.temps_passe ++;
        if(v_ia1.tir.temps_passe%50 == 0){ //Permet de reset le tir, 50 est pris totalement au hasard ! (� cause du rotozoom qui va en fait changer la rotation de l'image source)
            v_ia1.tir.etat = 0;

            eff_tir(ecran, save_screen, &v_ia1);
            decharge_sprite_tir(&v_ia1);
            charge_sprite_tir(&v_ia1);
        }

        if(v_player.tir.etat == 1)
            v_player.tir.temps_passe ++;
        if(v_player.tir.temps_passe%50 == 0){ //Permet de reset le tir, 50 est pris totalement au hasard ! (� cause du rotozoom qui va en fait changer la rotation de l'image source)
            v_player.tir.etat = 0;

            eff_tir(ecran, save_screen, &v_player);
            decharge_sprite_tir(&v_player);
            charge_sprite_tir(&v_player);
        }

        /// IL FAUT D'ABORD "CACHER" LES ANCIENNES SURFACES PUIS FAIRE LES ACTIONS (d�placement) PUIS REAFFICHER LES SURFACES AVEC LES NOUVELLES POSITIONS !
        pos_to_up_ecran[0] = eff_bouclier(ecran, &v_ia1, save_screen);
        pos_to_up_ecran[1] = eff_vie(ecran, &v_ia1, save_screen);
        pos_to_up_ecran[2] = eff_vaisseau(ecran, &v_ia1, save_screen);
        if (v_player.vitesse!=0)
            pos_to_up_ecran[3] = eff_vaisseau(ecran, &v_player, save_screen);

        if (v_ia1.tir.etat == 1){
            pos_to_up_tir_ia[0] = eff_tir(ecran, save_screen, &v_ia1);
            calcul_pos_tir(&v_ia1);
            pos_to_up_tir_ia[1] = aff_tir(ecran, &v_ia1);
        }
        if (v_player.tir.etat == 1){
            pos_to_up_tir_joueur[0] = eff_tir(ecran, save_screen, &v_player);
            calcul_pos_tir(&v_player);
            pos_to_up_tir_joueur[1] = aff_tir(ecran, &v_player);
        }

        /// L'ia joue en premi�re:
        tour_ia(&v_ia1, &v_player, ecran);
        /// Calcul des nouvelles positions d�pendants des actions de l'ia:
        calcul_pos_bouclier(&v_ia1);
        calcul_pos_vie(&v_ia1);
        calcul_pos_vaisseau(&v_ia1, ecran);

        pos_to_up_ecran[4] = aff_vaisseau(ecran, &v_ia1, save_screen);/// TOUJOURS afficher le vaisseau en premier dans l'appelle des fonction (dans cette version de la fonction).
        pos_to_up_ecran[5] = aff_bouclier(ecran, &v_ia1);
        pos_to_up_ecran[6] = aff_vie(ecran, &v_ia1);


        /// Test de l'action du joueur
        tour_joueur(&action, &v_player, &boom);
        if (action.key[SDLK_F3]) {
            if (etat_interface[CONSOLE]) {
                etat_interface[CONSOLE]=0;
                SDL_BlitSurface(save_screen, &(pos_to_up_console[0]), ecran, &(pos_to_up_console[0]));
                SDL_UpdateRects(ecran, 1, pos_to_up_console);
                action.key[SDLK_F3] = 0; // Ici on fait remonter la touche du joueur de fa�on manuelle
            } else {
                etat_interface[CONSOLE]=1;
                action.key[SDLK_F3] = 0; // Ici on fait remonter la touche du joueur de fa�on manuelle
            }
        }

        /// Gestion du temps pour �viter la surexploitation du CPU
        temps_actuel=SDL_GetTicks();
        if (temps_actuel - temps_precedent > CALCUL_FPS(FPS)) {
            temps_precedent=temps_actuel;
        } else {
            SDL_Delay(30 - (temps_actuel - temps_precedent));
        }
        /// Zone pour placer les commandes a faire apr�s la pause du jeu
        nb_pos_to_up_ecran = 7;
        switch (v_player.vie.charge) {
            case BAS:
            case MOYEN:
            case HAUT:
                if ((v_player.vitesse !=0)/* || (action.key.keysym.sym == SDLK_a) || (action.key.keysym.sym == SDLK_d) || (action.button.button == SDL_BUTTON_LEFT)*/) {
                    if (col_boite_boite(&(v_ia1.position), &(v_player.position))==1) {
                        calcul_pos_vaisseau(&v_player, ecran);
                    } else {
                        v_player.vie.charge=VIDE;
                    }
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
            pos_to_up_console[0] = aff_console(ecran, v_ia1, save_screen, police_texte);
            SDL_UpdateRects(ecran, 1, pos_to_up_console);
        }
        /// AFFICHAGE:
        SDL_UpdateRects(ecran, 2, pos_to_up_tir_ia); // Pas optimis�, affichage permanant m�me quand pas de tir
        SDL_UpdateRects(ecran, 2, pos_to_up_tir_joueur); // Pas optimis�, affichage permanant m�me quand pas de tir
        SDL_UpdateRects(ecran, nb_pos_to_up_ecran, pos_to_up_ecran);
    }

    TTF_CloseFont(police_texte);
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
