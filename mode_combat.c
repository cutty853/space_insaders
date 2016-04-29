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
    SDL_Event action;
    TTF_Font *police_texte=NULL;
    int continuer=1, temps_actuel=0, temps_precedent=0, nb_pos_to_up_ecran=0, etat_console[2]={0}, etat_tir = 0;
    SDL_Surface *save_screen = NULL;
    SDL_Rect *pos_to_up_console;
    SDL_Rect pos_to_up_ecran[9], pos_to_up_tir[2]; /// 9 = nombre actuel de nouvelles positions.
    _vaisseau v_player, v_ia1;
    _explosion boom;
    _tir pew;

    /// Zone pour les commandes a effectué dès l'affichage de la carte
        /// Chargement de la map
    charge_niveau(ecran);
    charge_sprite_explosion(&boom);
    charge_sprite_tir(&pew);
        /// ia:
    init_vaisseau(&v_ia1, IA, 100, 0, 0.1,8, HAUT, HAUT, TIR_LASER, 1000, 300, 4, 90);
    charge_sprite_bouclier(&v_ia1);
    charge_sprite_vie(&v_ia1);
        /// joueur:
    init_vaisseau(&v_player, JOUEUR, 100, 0, 0.1, 8, HAUT, HAUT, TIR_LASER, 100, 300, 4, 270);
    charge_sprite_bouclier(&v_player);
    charge_sprite_vie(&v_player);
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
    while (continuer) {
        /// IL FAUT D'ABORD "CACHER" LES ANCIENNES SURFACES PUIS FAIRE LES ACTIONS (déplacement) PUIS REAFFICHER LES SURFACES AVEC LES NOUVELLES POSITIONS !
        pos_to_up_ecran[0] = eff_bouclier(ecran, &v_ia1, save_screen);
        pos_to_up_ecran[1] = eff_vie(ecran, &v_ia1, save_screen);
        pos_to_up_ecran[2] = eff_vaisseau(ecran, &v_ia1, save_screen);
        pos_to_up_ecran[3] = eff_vaisseau(ecran, &v_player, save_screen);

        if (etat_tir) {
            pos_to_up_tir[0] = eff_tir(ecran, save_screen, &pew);
            pos_to_up_tir[1] = aff_tir(ecran, &pew);
        }

        pos_to_up_ecran[4] = aff_vaisseau(ecran, &v_ia1, save_screen);/// TOUJOURS afficher le vaisseau en premier dans l'appelle des fonction (dans cette version de la fonction).
        pos_to_up_ecran[5] = aff_bouclier(ecran, &v_ia1);
        pos_to_up_ecran[6] = aff_vie(ecran, &v_ia1);

        /// L'ia joue en première:
        tour_ia(&v_ia1, &v_player, ecran);

        /// Test de l'action du joueur
        SDL_PollEvent(&action);
        switch (action.type) {
            case SDL_QUIT:
                continuer=0;
                break;
            case SDL_KEYDOWN:
                switch (action.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        continuer=0;
                        //menu(ecran);
                        break;
                    case SDLK_w:
                        vitesse_joueur(&v_player, AVANT);
                        break;
                    case SDLK_s:
                        vitesse_joueur(&v_player, ARRIERE);
                        break;
                    case SDLK_a:
                        v_player.angle+= v_player.vitesse_rotation;
                        v_player.etat_rotation = 1;
                        break;
                    case SDLK_d:
                        v_player.angle-= v_player.vitesse_rotation;
                        v_player.etat_rotation = 1;
                        break;
                    case SDLK_c:
                        v_player.vitesse=0;
                        //v_player.rotation=0;
                        break;
                    case SDLK_F3:
                        if (etat_console[1]==0)
                            etat_console[0]=1;
                        else if (etat_console[1]==1) {
                            etat_console[0]=0;
                        }
                        break;
                    case SDLK_KP1:
                        v_player.vie.charge = VIDE;
                        boom.phase=0;
                        break;
                    case SDLK_SPACE:
                        etat_tir=1;
                        init_tir(&pew, v_player);
                        break;
                    default:
                        break;
                }
                break;
            case SDL_KEYUP:
                switch (action.key.keysym.sym) {
                    case SDLK_F3:
                        if (etat_console[0]==1)
                            etat_console[1]=1;
                        else if (etat_console[0]==0) {
                            etat_console[1]=0;
                            SDL_BlitSurface(save_screen, &(pos_to_up_console[0]), ecran, &(pos_to_up_console[0]));
                            SDL_UpdateRects(ecran, 1, pos_to_up_console);
                        }
                        break;
                    default:
                        break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                switch (action.button.button) {
                    case SDL_BUTTON_LEFT: // Fonctionnalité plus trop à jour..
                        charge_niveau(ecran);
                        v_player.position.x = action.button.x;
                        v_player.position.y = action.button.y;
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;

        }


        /// Gestion du temps pour éviter la surexploitation du CPU
        temps_actuel=SDL_GetTicks();
        if (temps_actuel - temps_precedent > CALCUL_FPS(FPS)) {
            temps_precedent=temps_actuel;
        }else{
            SDL_Delay(30 - (temps_actuel - temps_precedent));
        }


        /// Zone pour placer les commandes a faire après la pause du jeu


        nb_pos_to_up_ecran = 7;
        switch (v_player.vie.charge) {
            case BAS:
            case MOYEN:
            case HAUT:
                if ((v_player.vitesse !=0) || (action.key.keysym.sym == SDLK_a) || (action.key.keysym.sym == SDLK_d) || (action.button.button == SDL_BUTTON_LEFT)) {
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

        /// console de test
        if (etat_console[0] && etat_console[1]) {
            pos_to_up_console[0] = aff_console(ecran, v_player, save_screen, police_texte);
            SDL_UpdateRects(ecran, 1, pos_to_up_console);
        }

        /// AFFICHAGE:
        SDL_UpdateRects(ecran, 2, pos_to_up_tir); // Pas optimisé, affichage permanant même quand pas de tir
        SDL_UpdateRects(ecran, nb_pos_to_up_ecran, pos_to_up_ecran);
    }

    decharge_sprite_explosion(&boom);
    decharge_sprite_bouclier(&v_ia1);
    decharge_sprite_vie(&v_ia1);
    SDL_FreeSurface(v_ia1.sprite);
    SDL_FreeSurface(v_player.sprite);
    decharge_sprite_bouclier(&v_player);
    decharge_sprite_vie(&v_player);
}
