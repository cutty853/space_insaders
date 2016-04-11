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

void vitesse_player(_vaisseau *v_player, int sens) {
    if (sens == AVANT)
        v_player->vitesse += v_player->acceleration;
    if (sens == ARRIERE)
        v_player->vitesse -= v_player->acceleration;
}

SDL_Rect* aff_player(SDL_Surface *ecran, SDL_Surface *surface_player, _vaisseau *v_player, SDL_Surface* save_screen) {
    SDL_Rect *pos_to_update;
    pos_to_update = malloc(sizeof(SDL_Rect)*2);
    v_player->position.w = TAILLE_JOUEUR*2;
    v_player->position.h = TAILLE_JOUEUR*2;

    // Effacement de l'ancien joueur rempalce charge_niveau
    SDL_BlitSurface(save_screen, &(v_player->position), ecran, &(v_player->position));
    pos_to_update[0] = v_player->position;
//    SDL_UpdateRect(ecran, v_player->position.x, v_player->position.y, v_player->position.w, v_player->position.h);

    // Calcul des positions
    v_player->position.x += (v_player->vitesse)*cos(RADIANATION(v_player->rotation));
    v_player->position.y += (v_player->vitesse)*(-sin(RADIANATION(v_player->rotation)));

    // Affichage du joueur
    surface_player = rotozoomSurface(surface_player, v_player->rotation, 1.0, 1);
    /* La rotation prend plus de place en fonction de l'angle, il serais donc judicieux
    de voir pour effectuer un décalage du joueur en fonction de l'angle, ou bien de lui prévoir une surface plus grande et
    de tout gérer (coordonnées, etc...) a partir de cette surface qui contiendra au final la surface de l'image.
    */
    SDL_BlitSurface(surface_player, NULL, ecran, &(v_player->position));
    pos_to_update[1] = v_player->position;
    SDL_FreeSurface(surface_player);
//    SDL_UpdateRect(ecran, v_player->position.x, v_player->position.y, v_player->position.w, v_player->position.h);


    return pos_to_update;
}


SDL_Rect aff_console (SDL_Surface *ecran, _vaisseau vaisseau,  SDL_Surface* save_screen, TTF_Font *police_texte) {
    SDL_Surface *info[NB_STATS_CONSOLE]={NULL};
    char texte_info[NB_STATS_CONSOLE][50]= ENUM_TITRE_STATS_CONSOLE(), texte_infosup[NB_STATS_CONSOLE][20]=ENUM_VAR_STATS_CONSOLE();
    SDL_Rect pos_texte, pos_to_up;
    SDL_Color rouge={255, 0, 0};
    int i;

    init_pos(&pos_to_up, TAILLE_ECRAN_X-TAILLE_CONSOLE_X, TAILLE_ECRAN_Y-TAILLE_CONSOLE_Y);
    pos_to_up.w=TAILLE_CONSOLE_X;
    pos_to_up.h=TAILLE_CONSOLE_Y;
    init_pos(&pos_texte, (int)pos_to_up.x, (int)pos_to_up.y);
    SDL_BlitSurface(save_screen, &pos_to_up, ecran, &pos_texte);

//    sprintf(texte_infosup[1], "%d", (int)vaisseau.rotation);

    for (i=0; i<NB_STATS_CONSOLE ; i++) {
        strcat(texte_info[i], texte_infosup[i]);
        info[i] = TTF_RenderText_Blended(police_texte, texte_info[i], rouge);
        test_surface(info[i], 106+i);
        pos_texte.y += 30;
        SDL_BlitSurface(info[i], NULL, ecran, &pos_texte);
        SDL_FreeSurface(info[i]);
    }

    return pos_to_up;
}

