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
    int temps_actuel=0, temps_precedent=0, nb_pos_to_up_ecran = 14, etat_interface[NB_ETAT_INTERFACE], niveau = 1, i = 0;
    SDL_Surface *save_screen = NULL;
    SDL_Rect *pos_to_up_console;
    SDL_Rect pos_to_up_ecran[14], pos_to_up_tir_ia[2], pos_to_up_tir_joueur[2];
    _vaisseau v_player, v_ia1;
    _explosion boom, boom2;
    FILE* fichier = NULL;
    etat_interface[CONSOLE] = 0;

    /// Zone pour les commandes a effectué dès l'affichage de la carte
    memset(&action,0,sizeof(action));
        /// Chargement de la map
    charge_niveau(ecran);
    charge_sprite_explosion(&boom);
    charge_sprite_explosion(&boom2);
    boom.phase=0;
    boom2.phase=0;

    ///Chargmement des caractéristiques du niveau joué:
    switch(niveau){
        case 1:
            fichier = fopen("ressources/index_partie1.txt", "r"); /// On ouvre le fichier de lvl 1 pour uniquement le lire.
            break;
        case 2:
            fichier = fopen("ressources/index_partie2.txt", "r"); /// On ouvre le fichier de lvl 2 pour uniquement le lire.
            break;
        case 3:
            fichier = fopen("ressources/index_partie3.txt", "r"); /// On ouvre le fichier de lvl 3 pour uniquement le lire.
            break;
        default:
            exit(6666);
    }
    if (fichier != NULL){
        int init_val_int[NBR_VALEURS_TRANSMISES], nbr_ia;
        char titre[] = "IA0";
        /// GEN:
        do{
            fgets(titre, 4, fichier); /// On lit maximum 4 caractères du fichier, on stocke le tout dans "chaine"
        } while(strcmp(titre, "GEN") != 0);
        deplace_curseur(fichier);
        nbr_ia = recup_int(fichier);

        /// IA:
        do{
            fgets(titre, 4, fichier); /// On lit maximum 4 caractères du fichier, on stocke le tout dans "chaine"
        } while(strcmp(titre, "IA1") != 0);
        for(i=0; i<NBR_VALEURS_INT; i++){
            deplace_curseur(fichier);
            init_val_int[i] = recup_int(fichier);
        }
        for(i=NBR_VALEURS_INT; i<NBR_VALEURS_TRANSMISES; i++){
            deplace_curseur(fichier);
            init_val_int[i] = recup_string(fichier);
        }
        init_vaisseau(&v_ia1, CHERCHE, init_val_int);
        init_hitbox(&(v_ia1.hitbox), v_ia1.position.x+(TAILLE_JOUEUR/2), v_ia1.position.y+(TAILLE_JOUEUR/2), TAILLE_JOUEUR/2, 0, v_ia1.position.x, v_ia1.position.y, v_ia1.position.w, v_ia1.position.h);
        charge_sprite_bouclier(&v_ia1);
        charge_sprite_vie(&v_ia1);
        charge_sprite_tir(&v_ia1);

        /// JOUEUR:
        do{
            fgets(titre, 4, fichier); /// On lit maximum 5 caractères du fichier, on stocke le tout dans "chaine"
        } while(strcmp(titre, "JOU") != 0);
        for(i=0; i<NBR_VALEURS_INT; i++){
            deplace_curseur(fichier);
            init_val_int [i] = recup_int(fichier);
        }
        for(i=NBR_VALEURS_INT; i<NBR_VALEURS_TRANSMISES; i++){
            deplace_curseur(fichier);
            init_val_int[i] = recup_string(fichier);
        }
        init_vaisseau(&v_player, INDEPENDENT, init_val_int);
        init_hitbox(&(v_player.hitbox), v_player.position.x+(TAILLE_JOUEUR/2), v_player.position.y+(TAILLE_JOUEUR/2), TAILLE_JOUEUR/2, 0, v_player.position.x, v_player.position.y, v_player.position.w, v_player.position.h);
        charge_sprite_bouclier(&v_player);
        charge_sprite_vie(&v_player);
        charge_sprite_tir(&v_player);

        close(fichier);
    }
    /// test console
    pos_to_up_console = malloc(sizeof(SDL_Rect)*1);
    police_texte = TTF_OpenFont("polices/geo_sans_light.ttf", 18);
    test_police(police_texte, 202);
    /// Sauvegarde de l'écran
    save_screen = SDL_DisplayFormat(ecran);

    /// BOUCLE DU JEU:
    while (!action.key[SDLK_ESCAPE] && !action.quit) {
        /// EFFACE
        pos_to_up_ecran[0] = eff_bouclier(ecran, &v_ia1, save_screen);
        pos_to_up_ecran[1] = eff_vie(ecran, &v_ia1, save_screen);
        pos_to_up_ecran[2] = eff_vaisseau(ecran, &v_ia1, save_screen);

        pos_to_up_ecran[7] = eff_bouclier(ecran, &v_player, save_screen);
        pos_to_up_ecran[8] = eff_vie(ecran, &v_player, save_screen);
        pos_to_up_ecran[9] = eff_vaisseau(ecran, &v_player, save_screen);

        /// GESTION DES TIRS:
        if (v_ia1.tir.etat == 1){
            v_ia1.tir.distance_parcourue ++;
            if(v_ia1.tir.distance_parcourue > v_ia1.tir.distance_max){
                v_ia1.tir.etat = 0;
                eff_tir(ecran, save_screen, &v_ia1);
                decharge_sprite_tir(&v_ia1);
                charge_sprite_tir(&v_ia1);
            }else{
                pos_to_up_tir_ia[0] = eff_tir(ecran, save_screen, &v_ia1);
                calcul_pos_tir(&v_ia1);
                pos_to_up_tir_ia[1] = aff_tir(ecran, &v_ia1);
                if (col_aabb_cercle(&(v_ia1.tir.hitbox.aabb), &(v_player.hitbox.cercle))==1 && v_ia1.tir.etat == 1){
                    if(v_player.bouclier.charge == VIDE){
                        v_player.vie.charge = VIDE;
                        v_ia1.tir.etat = 0;
                        eff_tir(ecran, save_screen, &v_ia1);
                        decharge_sprite_tir(&v_ia1);
                        charge_sprite_tir(&v_ia1);
                    }else{
                        v_player.bouclier.charge = VIDE;
                        v_ia1.tir.etat = 0;
                        eff_tir(ecran, save_screen, &v_ia1);
                        decharge_sprite_tir(&v_ia1);
                        charge_sprite_tir(&v_ia1);
                    }
                }
            }
        }
        if (v_player.tir.etat == 1){
            v_player.tir.distance_parcourue ++;
            if(v_player.tir.distance_parcourue > v_player.tir.distance_max){
                v_player.tir.etat = 0;
                eff_tir(ecran, save_screen, &v_player);
                decharge_sprite_tir(&v_player);
                charge_sprite_tir(&v_player);
            }else{
                pos_to_up_tir_joueur[0] = eff_tir(ecran, save_screen, &v_player);
                calcul_pos_tir(&v_player);
                pos_to_up_tir_joueur[1] = aff_tir(ecran, &v_player);
                if (col_aabb_cercle(&(v_player.tir.hitbox.aabb), &(v_ia1.hitbox.cercle))==1 && v_player.tir.etat == 1){
                    if(v_ia1.bouclier.charge == VIDE){
                        v_ia1.vie.charge = VIDE;
                        v_player.tir.etat = 0;
                        eff_tir(ecran, save_screen, &v_player);
                        decharge_sprite_tir(&v_player);
                        charge_sprite_tir(&v_player);
                    }else{
                        v_ia1.bouclier.charge = VIDE;
                        v_player.tir.etat = 0;
                        eff_tir(ecran, save_screen, &v_player);
                        decharge_sprite_tir(&v_player);
                        charge_sprite_tir(&v_player);
                    }
                }
            }
        }

        /// IA:
        tour_ia(&v_ia1, &v_player, ecran);
        switch (v_ia1.vie.charge) {
            case BAS:
            case MOYEN:
            case HAUT:
                    if (col_cercle_cercle(&(v_player.hitbox.cercle), &(v_ia1.hitbox.cercle))==1) {
                        calcul_pos_vaisseau(&v_ia1, ecran);
                        calcul_pos_bouclier(&v_ia1);
                        calcul_pos_vie(&v_ia1);
                        calcul_pos_hitbox_vaisseau(&v_ia1);
                        calcul_pos_hitbox_tir(&(v_ia1.tir));
                    }else
                        v_ia1.vie.charge=VIDE;
                    pos_to_up_ecran[3] = aff_vaisseau(ecran, &v_ia1, save_screen);/// TOUJOURS afficher le vaisseau en premier dans l'appelle des fonction (dans cette version de la fonction).
                    pos_to_up_ecran[4] = aff_bouclier(ecran, &v_ia1);
                    pos_to_up_ecran[5] = aff_vie(ecran, &v_ia1);
                break;
            case VIDE:
                if (boom2.phase < NB_SPRITES_EXPLOSION) {
                    pos_to_up_ecran[6] = aff_explosion(ecran, &boom2, v_ia1);
                    boom2.phase++;
                    supprime_hitbox(&(v_ia1.tir.hitbox));
                    supprime_hitbox(&(v_ia1.hitbox));
                } else {
                    supprime_vaisseau(&v_ia1);
                }
                break;
        }

        /// JOUEUR:
        tour_joueur(&action, &v_player, &boom);
        switch (v_player.vie.charge) {
            case BAS:
            case MOYEN:
            case HAUT:
                    if (col_cercle_cercle(&(v_ia1.hitbox.cercle), &(v_player.hitbox.cercle))==1) {
                        calcul_pos_vaisseau(&v_player, ecran);
                        calcul_pos_bouclier(&v_player);
                        calcul_pos_vie(&v_player);
                        calcul_pos_hitbox_vaisseau(&v_player);
                        calcul_pos_hitbox_tir(&(v_player.tir));
                    }else
                        v_player.vie.charge=VIDE;
                    pos_to_up_ecran[10] = aff_vaisseau(ecran, &v_player, save_screen);/// TOUJOURS afficher le vaisseau en premier dans l'appelle des fonction (dans cette version de la fonction).
                    pos_to_up_ecran[11] = aff_bouclier(ecran, &v_player);
                    pos_to_up_ecran[12] = aff_vie(ecran, &v_player);
                break;
            case VIDE:
                if (boom.phase < NB_SPRITES_EXPLOSION) {
                    pos_to_up_ecran[13] = aff_explosion(ecran, &boom, v_player);
                    boom.phase++;
                    supprime_hitbox(&(v_player.tir.hitbox));
                    supprime_hitbox(&(v_player.hitbox));
                } else {
                    supprime_vaisseau(&v_player);
                }
                break;
        }

        /// CONSOLE
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
        if (etat_interface[CONSOLE]) {
            pos_to_up_console[0] = aff_console(ecran, v_ia1, save_screen, police_texte);
            SDL_UpdateRects(ecran, 1, pos_to_up_console);
        }

        /// AFFICHAGE:
        SDL_UpdateRects(ecran, 2, pos_to_up_tir_ia); // Pas optimisé, affichage permanant même quand pas de tir
        SDL_UpdateRects(ecran, 2, pos_to_up_tir_joueur); // Pas optimisé, affichage permanant même quand pas de tir
        SDL_UpdateRects(ecran, nb_pos_to_up_ecran, pos_to_up_ecran);

        /// GESTION DU TEMPS: (pour éviter la surexploitation du CPU)
        temps_actuel=SDL_GetTicks();
        if (temps_actuel - temps_precedent > CALCUL_FPS(FPS)) {
            temps_precedent=temps_actuel;
        } else {
            SDL_Delay(30 - (temps_actuel - temps_precedent));
        }
    }

    /// LIBERATION:
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
