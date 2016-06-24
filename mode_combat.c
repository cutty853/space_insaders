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


void play(SDL_Surface *ecran, int *niveau){
    _input action;
    TTF_Font *police_texte = NULL;
    int nbr_ia = 0, nbr_ia_morte = 0, niveau_augmente = 0;
    int temps_precedent = 0, temps_precedent_joueur = 0, temps_precedent_ia = 0, active_regen_joueur = 0, active_regen_ia = 0, temps_actuel = 0;
    int nb_pos_to_up_ecran = 21, nbr_pos_to_up_tir_ia, etat_interface[NB_ETAT_INTERFACE], i = 0;
    float vitesse_jeu = 1.0;
    SDL_Surface *save_screen = NULL;
    SDL_Rect *pos_to_up_console, pos_to_up_tir_joueur[2], *pos_to_up_tir_ia = NULL;
    SDL_Rect *pos_to_up_ecran = malloc(nb_pos_to_up_ecran*sizeof(SDL_Rect));
    _vaisseau v_joueur, *tab_v_ia = NULL;
    _explosion boom_joueur, *boom_ia = NULL;
    etat_interface[CONSOLE] = 0;

    ///Zone pour les commandes a effectué dès l'affichage de la carte:
    memset(&action,0,sizeof(action));

    ///Chargmement des caractéristiques du niveau joué:
    pos_to_up_tir_ia = init_niveau_general(niveau, &nbr_ia, &nbr_pos_to_up_tir_ia);
    tab_v_ia = init_niveau_ia(niveau, nbr_ia);
    v_joueur = init_niveau_joueur(niveau);

    ///Chargement des explosions:
    boom_joueur = init_explosions_joueur(nbr_ia);
    boom_ia = init_explosions_ia(nbr_ia);

    ///Chargement de la map:
    charge_niveau(ecran);

    /// test console
    pos_to_up_console = malloc(1*sizeof(SDL_Rect));
    police_texte = TTF_OpenFont("polices/geo_sans_light.ttf", 18);
    test_police(police_texte, 202);

    /// Sauvegarde de l'écran
    save_screen = SDL_DisplayFormat(ecran);

    /// BOUCLE DU JEU:
    while (!action.key[SDLK_ESCAPE] && !action.quit){
        /// EFFACEMENT:
         ///EFF. IA:
        for(i=0; i<nbr_ia; i++){
            switch(i){
                case 0:
                    pos_to_up_ecran[0] = eff_bouclier(ecran, &tab_v_ia[0], save_screen);
                    pos_to_up_ecran[1] = eff_vie(ecran, &tab_v_ia[0], save_screen);
                    pos_to_up_ecran[2] = eff_vaisseau(ecran, &tab_v_ia[0], save_screen);
                    break;
                case 1:
                    pos_to_up_ecran[3] = eff_bouclier(ecran, &tab_v_ia[1], save_screen);
                    pos_to_up_ecran[4] = eff_vie(ecran, &tab_v_ia[1], save_screen);
                    pos_to_up_ecran[5] = eff_vaisseau(ecran, &tab_v_ia[1], save_screen);
                    break;
            }
        }
          ///EFF. JOUEUR:
        pos_to_up_ecran[6] = eff_bouclier(ecran, &v_joueur, save_screen);
        pos_to_up_ecran[7] = eff_vie(ecran, &v_joueur, save_screen);
        pos_to_up_ecran[8] = eff_vaisseau(ecran, &v_joueur, save_screen);

        /// GESTION DES TIRS:
          ///TIRS IA:
        for(i=0; i<nbr_ia; i++){
            gestion_distance_tir(&tab_v_ia[i]);
            if(tab_v_ia[i].tir.etat == 0){
                eff_tir(ecran, save_screen, &tab_v_ia[i]);
                decharge_sprite_tir(&tab_v_ia[i]);
                charge_sprite_tir(&tab_v_ia[i]);
            }else{
                switch(i){
                    case 0:
                        pos_to_up_tir_ia[0] = eff_tir(ecran, save_screen, &tab_v_ia[i]);
                        calcul_pos_tir(&tab_v_ia[i]);
                        pos_to_up_tir_ia[1] = aff_tir(ecran, &tab_v_ia[i]);
                        break;
                    case 1:
                        pos_to_up_tir_ia[2] = eff_tir(ecran, save_screen, &tab_v_ia[i]);
                        calcul_pos_tir(&tab_v_ia[i]);
                        pos_to_up_tir_ia[3] = aff_tir(ecran, &tab_v_ia[i]);
                    break;
                }
                if (col_aabb_cercle(&(tab_v_ia[i].tir.hitbox.aabb), &(v_joueur.hitbox.cercle))==1 && tab_v_ia[i].tir.etat==1){
                    degat_tir(&v_joueur, &tab_v_ia[i]);
                    tab_v_ia[i].tir.etat = 0;
                    eff_tir(ecran, save_screen, &tab_v_ia[i]);
                    decharge_sprite_tir(&tab_v_ia[i]);
                    charge_sprite_tir(&tab_v_ia[i]);
                }
            }
        }
          ///TIR JOUEUR:
        gestion_distance_tir(&v_joueur);
        if(v_joueur.tir.etat == 0){
            eff_tir(ecran, save_screen, &v_joueur);
            decharge_sprite_tir(&v_joueur);
            charge_sprite_tir(&v_joueur);
        }else{
            pos_to_up_tir_joueur[0] = eff_tir(ecran, save_screen, &v_joueur);
            calcul_pos_tir(&v_joueur);
            pos_to_up_tir_joueur[1] = aff_tir(ecran, &v_joueur);
            for(i=0; i<nbr_ia; i++){
                if (col_aabb_cercle(&(v_joueur.tir.hitbox.aabb), &(tab_v_ia[i].hitbox.cercle))==1 && v_joueur.tir.etat == 1){
                    degat_tir(&tab_v_ia[i], &v_joueur);
                    v_joueur.tir.etat = 0;
                    eff_tir(ecran, save_screen, &v_joueur);
                    decharge_sprite_tir(&v_joueur);
                    charge_sprite_tir(&v_joueur);
                }
            }
        }

        /// MOUVEMENT:
          ///MVT. IA:
        for(i=0; i<nbr_ia; i++){
            tour_ia(&tab_v_ia[i], &v_joueur, ecran);
            switch (tab_v_ia[i].vie.charge) {
                case BAS:
                case MOYEN:
                case HAUT:
                    if (col_cercle_cercle(&(tab_v_ia[i].hitbox.cercle), &(v_joueur.hitbox.cercle)) != 1){ /// Collison entre vaisseaux.
                        degat_collisions(&tab_v_ia[i], &v_joueur);
                    }
                    calcul_pos_vaisseau(&tab_v_ia[i], ecran);
                    calcul_pos_bouclier(&tab_v_ia[i]);
                    calcul_pos_vie(&tab_v_ia[i]);
                    calcul_pos_hitbox_vaisseau(&tab_v_ia[i]);
                    calcul_pos_hitbox_tir(&(tab_v_ia[i].tir));

                    switch(i){
                        case 0:
                            pos_to_up_ecran[9] = aff_vaisseau(ecran, &tab_v_ia[i], save_screen);/// TOUJOURS afficher le vaisseau en premier dans l'appelle des fonction (dans cette version de la fonction).
                            pos_to_up_ecran[10] = aff_bouclier(ecran, &tab_v_ia[i]);
                            pos_to_up_ecran[11] = aff_vie(ecran, &tab_v_ia[i]);
                            break;
                        case 1:
                            pos_to_up_ecran[12] = aff_vaisseau(ecran, &tab_v_ia[i], save_screen);/// TOUJOURS afficher le vaisseau en premier dans l'appelle des fonction (dans cette version de la fonction).
                            pos_to_up_ecran[13] = aff_bouclier(ecran, &tab_v_ia[i]);
                            pos_to_up_ecran[14] = aff_vie(ecran, &tab_v_ia[i]);
                            break;
                    }
                    break;
                case VIDE:
                    if (boom_ia[i].phase < NB_SPRITES_EXPLOSION) {
                        switch(i){/// Choses en double nécessaire car on est dans une boucle for.
                            case 0:
                                pos_to_up_ecran[15] = aff_explosion(ecran, &boom_ia[i], tab_v_ia[i]);
                                boom_ia[i].phase++;
                                supprime_hitbox(&(tab_v_ia[i].tir.hitbox));
                                supprime_hitbox(&(tab_v_ia[i].hitbox));
                                break;
                            case 1:
                                pos_to_up_ecran[16] = aff_explosion(ecran, &boom_ia[i], tab_v_ia[i]);
                                boom_ia[i].phase++;
                                supprime_hitbox(&(tab_v_ia[i].tir.hitbox));
                                supprime_hitbox(&(tab_v_ia[i].hitbox));
                                break;
                        }
                    }else
                        supprime_vaisseau(&tab_v_ia[i]);
                    break;
            }
        }
          ///MVT. JOUEUR:
        tour_joueur(&action, &v_joueur, &boom_joueur, &vitesse_jeu);
        switch (v_joueur.vie.charge) {
            case BAS:
            case MOYEN:
            case HAUT:
                for(i=0; i<nbr_ia; i++){
                    if (col_cercle_cercle(&(tab_v_ia[i].hitbox.cercle), &(v_joueur.hitbox.cercle)) != 1){ /// Collison entre vaisseaux.
                        degat_collisions(&v_joueur, &tab_v_ia[i]);
                    }
                }
                calcul_pos_vaisseau(&v_joueur, ecran);
                calcul_pos_bouclier(&v_joueur);
                calcul_pos_vie(&v_joueur);
                calcul_pos_hitbox_vaisseau(&v_joueur);
                calcul_pos_hitbox_tir(&(v_joueur.tir));

                pos_to_up_ecran[17] = aff_vaisseau(ecran, &v_joueur, save_screen);/// TOUJOURS afficher le vaisseau en premier dans l'appelle des fonction (dans cette version de la fonction).
                pos_to_up_ecran[18] = aff_bouclier(ecran, &v_joueur);
                pos_to_up_ecran[19] = aff_vie(ecran, &v_joueur);
                break;
            case VIDE:
                if (boom_joueur.phase < NB_SPRITES_EXPLOSION){
                    pos_to_up_ecran[20] = aff_explosion(ecran, &boom_joueur, v_joueur);
                    boom_joueur.phase++;
                    supprime_hitbox(&(v_joueur.tir.hitbox));
                    supprime_hitbox(&(v_joueur.hitbox));
                }else{
                    supprime_vaisseau(&v_joueur);
                    while (!action.key[SDLK_ESCAPE] && !action.quit){/// Boucle infini pour attendre l'ouverture du menu par le joueur une fois qu'il est mort.
                        tour_joueur(&action, &v_joueur, &boom_joueur, &vitesse_jeu);
                    }
                }
                break;
        }

        /// CONSOLE:
        if (action.key[SDLK_F3]) {
            if (etat_interface[CONSOLE]) {
                etat_interface[CONSOLE] = 0;
                SDL_BlitSurface(save_screen, &(pos_to_up_console[0]), ecran, &(pos_to_up_console[0]));
                SDL_UpdateRects(ecran, 1, pos_to_up_console);
                action.key[SDLK_F3] = 0; // Ici on fait remonter la touche du joueur de façon manuelle
            } else {
                etat_interface[CONSOLE] = 1;
                action.key[SDLK_F3] = 0; // Ici on fait remonter la touche du joueur de façon manuelle
            }
        }
        if (etat_interface[CONSOLE]) {
            pos_to_up_console[0] = aff_console(ecran, tab_v_ia[0], save_screen, police_texte);
            SDL_UpdateRects(ecran, 1, pos_to_up_console);
        }

        /// AFFICHAGE:
        SDL_UpdateRects(ecran, nbr_pos_to_up_tir_ia, pos_to_up_tir_ia);
        SDL_UpdateRects(ecran, 2, pos_to_up_tir_joueur);
        SDL_UpdateRects(ecran, nb_pos_to_up_ecran, pos_to_up_ecran);

        /// GESTION DU TEMPS: (pour éviter la surexploitation du CPU)
        temps_actuel = SDL_GetTicks();
        if (temps_actuel - temps_precedent > CALCUL_FPS(FPS))
            temps_precedent = temps_actuel;
        else
            SDL_Delay(30*vitesse_jeu - (temps_actuel - temps_precedent));

        ///Gestion des boucliers:
          ///Joueur:
        if(v_joueur.bouclier.charge != HAUT && active_regen_joueur == 0){
            temps_precedent_joueur = SDL_GetTicks();
            active_regen_joueur = 1;
        }
        if(v_joueur.bouclier.charge != HAUT){
            if(temps_actuel - temps_precedent_joueur > NBR_SECONDES_RECHARGE_BOUCLIER*1000){/// Si 5 secondes se sont écoulées.
                v_joueur.bouclier.charge ++;
                temps_precedent_joueur = temps_actuel;
                active_regen_joueur = 0;
            }
        }
          ///IA:
        if(tab_v_ia[i].bouclier.charge != HAUT && active_regen_ia == 0){
            temps_precedent_ia = SDL_GetTicks();
            active_regen_ia = 1;
        }
        for(i=0; i<nbr_ia; i++){
            if(tab_v_ia[i].bouclier.charge != HAUT){
                if(temps_actuel - temps_precedent_ia > NBR_SECONDES_RECHARGE_BOUCLIER*1000){/// Si 5 secondes se sont écoulées.
                    tab_v_ia[i].bouclier.charge ++;
                    temps_precedent_ia = temps_actuel;
                    active_regen_ia = 0;
                }
            }
        }

        /// Gestion de victoire du niveau:
        nbr_ia_morte = 0;
        for(i=0; i<nbr_ia; i++){
            if(tab_v_ia[i].vie.charge == VIDE)
                nbr_ia_morte ++;
        }
        if(nbr_ia_morte == nbr_ia && niveau_augmente == 0){
            (*niveau) ++;
            niveau_augmente = 1;
        }
    }

    /// LIBERATION:
    TTF_CloseFont(police_texte);
    decharge_sprite_explosion(&boom_joueur);
    for(i=0; i<nbr_ia; i++){
        decharge_sprite_bouclier(&tab_v_ia[i]);
        decharge_sprite_vie(&tab_v_ia[i]);
        decharge_sprite_tir(&tab_v_ia[i]);
        SDL_FreeSurface(tab_v_ia[i].sprite);
        decharge_sprite_explosion(&boom_ia[i]);
    }
    SDL_FreeSurface(v_joueur.sprite);
    decharge_sprite_bouclier(&v_joueur);
    decharge_sprite_vie(&v_joueur);
    decharge_sprite_tir(&v_joueur);
}
