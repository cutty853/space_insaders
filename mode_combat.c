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


void play(SDL_Surface *ecran, int *niveau){
    _input action;
    TTF_Font *police_texte = NULL;
    int init_val[NBR_VALEURS_TRANSMISES], nbr_ia = 0, nbr_ia_morte = 0;
    int temps_actuel = 0, temps_precedent = 0, nb_pos_to_up_ecran = 21, nb_pos_to_up_tir_ia = 4, etat_interface[NB_ETAT_INTERFACE], i = 0, j = 0;
    float vitesse_jeu = 1.0;
    SDL_Surface *save_screen = NULL;
    SDL_Rect *pos_to_up_console, pos_to_up_tir_joueur[2];
    SDL_Rect *pos_to_up_tir_ia = malloc(nb_pos_to_up_tir_ia*sizeof(SDL_Rect));
    SDL_Rect *pos_to_up_ecran = malloc(nb_pos_to_up_ecran*sizeof(SDL_Rect));
    _vaisseau v_player, *tab_v_ia ;
    _explosion boom, boom2;
    FILE* fichier = NULL;
    etat_interface[CONSOLE] = 0;

    /// Zone pour les commandes a effectué dès l'affichage de la carte
    memset(&action,0,sizeof(action));

    /// Chargement de la map
    charge_niveau(ecran);
    charge_sprite_explosion(&boom);
    charge_sprite_explosion(&boom2);
    boom.phase = 0;
    boom2.phase = 0;

    ///Chargmement des caractéristiques du niveau joué:
    char chemin_niveau[] = {"ressources/index_partie0.txt"};
    sprintf(chemin_niveau, "ressources/index_partie%i.txt", *niveau);
    fichier = fopen(chemin_niveau, "r");/// On ouvre le ficher correspondant au niveau actuel.
    if (fichier != NULL){
        /// GENERAL:
        trouve_titre(fichier, 4, "GEN");
        deplace_curseur(fichier);
        nbr_ia = recup_int(fichier);
        tab_v_ia = malloc(nbr_ia*sizeof(_vaisseau));/// Initialisation du tableau des vaisseaux ia en fonction de leur nombre.

        /// Plusieurs IA:
        for(j = 0; j<nbr_ia; j++){
            char titre_voulu[] = "IA0";
            sprintf(titre_voulu, "IA%i", j+1);
            trouve_titre(fichier, 4, titre_voulu);
            for(i=0; i<NBR_VALEURS_INT; i++){
                deplace_curseur(fichier);
                init_val[i] = recup_int(fichier);
            }
            for(i=NBR_VALEURS_INT; i<NBR_VALEURS_TRANSMISES; i++){
                deplace_curseur(fichier);
                init_val[i] = recup_string(fichier);
            }
            init_vaisseau(&tab_v_ia[j], init_val);
            init_hitbox(&(tab_v_ia[j].hitbox), tab_v_ia[j].position.x+(TAILLE_JOUEUR/2), tab_v_ia[j].position.y+(TAILLE_JOUEUR/2), TAILLE_JOUEUR/2, 0, tab_v_ia[j].position.x, tab_v_ia[j].position.y, tab_v_ia[j].position.w, tab_v_ia[j].position.h);
            charge_sprite_bouclier(&tab_v_ia[j]);
            charge_sprite_vie(&tab_v_ia[j]);
            charge_sprite_tir(&tab_v_ia[j]);
        }

        /// JOUEUR:
        trouve_titre(fichier, 4, "JOU");
        for(i=0; i<NBR_VALEURS_INT; i++){
            deplace_curseur(fichier);
            init_val [i] = recup_int(fichier);
        }
        for(i=NBR_VALEURS_INT; i<NBR_VALEURS_TRANSMISES; i++){
            deplace_curseur(fichier);
            init_val[i] = recup_string(fichier);
        }
        init_vaisseau(&v_player, init_val);
        init_hitbox(&(v_player.hitbox), v_player.position.x+(TAILLE_JOUEUR/2), v_player.position.y+(TAILLE_JOUEUR/2), TAILLE_JOUEUR/2, 0, v_player.position.x, v_player.position.y, v_player.position.w, v_player.position.h);
        charge_sprite_bouclier(&v_player);
        charge_sprite_vie(&v_player);
        charge_sprite_tir(&v_player);

        close(fichier);
    }else
        exit(606);

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
        pos_to_up_ecran[6] = eff_bouclier(ecran, &v_player, save_screen);
        pos_to_up_ecran[7] = eff_vie(ecran, &v_player, save_screen);
        pos_to_up_ecran[8] = eff_vaisseau(ecran, &v_player, save_screen);

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
                if (col_aabb_cercle(&(tab_v_ia[i].tir.hitbox.aabb), &(v_player.hitbox.cercle))==1 && tab_v_ia[i].tir.etat == 1){
                    degat_tir(&v_player, &tab_v_ia[i]);
                    tab_v_ia[i].tir.etat = 0;
                    eff_tir(ecran, save_screen, &tab_v_ia[i]);
                    decharge_sprite_tir(&tab_v_ia[i]);
                    charge_sprite_tir(&tab_v_ia[i]);
                }
            }
        }
          ///TIR JOUEUR:
        gestion_distance_tir(&v_player);
        if(v_player.tir.etat == 0){
            eff_tir(ecran, save_screen, &v_player);
            decharge_sprite_tir(&v_player);
            charge_sprite_tir(&v_player);
        }else{
            pos_to_up_tir_joueur[0] = eff_tir(ecran, save_screen, &v_player);
            calcul_pos_tir(&v_player);
            pos_to_up_tir_joueur[1] = aff_tir(ecran, &v_player);
            for(i=0; i<nbr_ia; i++){
                if (col_aabb_cercle(&(v_player.tir.hitbox.aabb), &(tab_v_ia[i].hitbox.cercle))==1 && v_player.tir.etat == 1){
                    degat_tir(&tab_v_ia[i], &v_player);
                    v_player.tir.etat = 0;
                    eff_tir(ecran, save_screen, &v_player);
                    decharge_sprite_tir(&v_player);
                    charge_sprite_tir(&v_player);
                }
            }
        }

        /// MOUVEMENT:
          ///MVT. IA:
        for(i=0; i<nbr_ia; i++){
            tour_ia(&tab_v_ia[i], &v_player, ecran);
            switch (tab_v_ia[i].vie.charge) {
                case BAS:
                case MOYEN:
                case HAUT:
                    if (col_cercle_cercle(&(tab_v_ia[i].hitbox.cercle), &(v_player.hitbox.cercle)) != 1){ /// Collison entre vaisseaux.
                        degat_collisions(&tab_v_ia[i]);
                        degat_collisions(&v_player);
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
                    nbr_ia_morte ++;
                    if(nbr_ia_morte == nbr_ia){/// Gestion de victoire du niveau.
                        (*niveau) ++;
                    }
                    if (boom2.phase < NB_SPRITES_EXPLOSION) {
                        switch(i){/// Choses en double nécessaire car on est dans une boucle for.
                            case 0:
                                pos_to_up_ecran[15] = aff_explosion(ecran, &boom2, tab_v_ia[i]);
                                boom2.phase++;
                                supprime_hitbox(&(tab_v_ia[i].tir.hitbox));
                                supprime_hitbox(&(tab_v_ia[i].hitbox));
                                break;
                            case 1:
                                pos_to_up_ecran[16] = aff_explosion(ecran, &boom2, tab_v_ia[i]);
                                boom2.phase++;
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
        tour_joueur(&action, &v_player, &boom, &vitesse_jeu);
        switch (v_player.vie.charge) {
            case BAS:
            case MOYEN:
            case HAUT:
                for(i=0; i<nbr_ia; i++){
                    if (col_cercle_cercle(&(tab_v_ia[i].hitbox.cercle), &(v_player.hitbox.cercle)) != 1){ /// Collison entre vaisseaux.
                        degat_collisions(&v_player);
                        degat_collisions(&tab_v_ia[i]);
                    }
                }
                calcul_pos_vaisseau(&v_player, ecran);
                calcul_pos_bouclier(&v_player);
                calcul_pos_vie(&v_player);
                calcul_pos_hitbox_vaisseau(&v_player);
                calcul_pos_hitbox_tir(&(v_player.tir));

                pos_to_up_ecran[17] = aff_vaisseau(ecran, &v_player, save_screen);/// TOUJOURS afficher le vaisseau en premier dans l'appelle des fonction (dans cette version de la fonction).
                pos_to_up_ecran[18] = aff_bouclier(ecran, &v_player);
                pos_to_up_ecran[19] = aff_vie(ecran, &v_player);
                break;
            case VIDE:
                if (boom.phase < NB_SPRITES_EXPLOSION){
                    pos_to_up_ecran[20] = aff_explosion(ecran, &boom, v_player);
                    boom.phase++;
                    supprime_hitbox(&(v_player.tir.hitbox));
                    supprime_hitbox(&(v_player.hitbox));
                }else{
                    supprime_vaisseau(&v_player);
                    while (!action.key[SDLK_ESCAPE] && !action.quit){/// Boucle infini pour attendre l'ouverture du menu par le joueur une fois qu'il est mort.
                        tour_joueur(&action, &v_player, &boom, &vitesse_jeu);
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
        SDL_UpdateRects(ecran, nb_pos_to_up_tir_ia, pos_to_up_tir_ia);
        SDL_UpdateRects(ecran, 2, pos_to_up_tir_joueur); // Pas optimisé, affichage permanant même quand pas de tir
        SDL_UpdateRects(ecran, nb_pos_to_up_ecran, pos_to_up_ecran);

        /// GESTION DU TEMPS: (pour éviter la surexploitation du CPU)
        temps_actuel=SDL_GetTicks();
        if (temps_actuel - temps_precedent > CALCUL_FPS(FPS))
            temps_precedent=temps_actuel;
        else
            SDL_Delay(30*vitesse_jeu - (temps_actuel - temps_precedent));
    }

    /// LIBERATION:
    TTF_CloseFont(police_texte);
    decharge_sprite_explosion(&boom);
    for(i=0; i<nbr_ia; i++){
        decharge_sprite_bouclier(&tab_v_ia[i]);
        decharge_sprite_vie(&tab_v_ia[i]);
        decharge_sprite_tir(&tab_v_ia[i]);
        SDL_FreeSurface(tab_v_ia[i].sprite);
    }
    SDL_FreeSurface(v_player.sprite);
    decharge_sprite_bouclier(&v_player);
    decharge_sprite_vie(&v_player);
    decharge_sprite_tir(&v_player);
}
