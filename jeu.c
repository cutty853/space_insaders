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

void cadre(SDL_Surface *ecran, int pos_x, int pos_y) {
    SDL_Surface *cadre[12]={NULL};
    SDL_Rect pos_cadre;
    int largeur, hauteur, i;

    largeur = LARGE_CADRE_MENU;
    hauteur = LONG_CADRE_MENU;
    pos_cadre.x=pos_x;
    pos_cadre.y=pos_y;

    cadre[0] = SDL_CreateRGBSurface(SDL_HWSURFACE, largeur, hauteur, 32, 0, 0, 0, 0);
    SDL_FillRect(cadre[0], NULL, SDL_MapRGB(cadre[0]->format, 0, 0, 0));
    SDL_BlitSurface(cadre[0], NULL, ecran, &pos_cadre);
    largeur-=10;
    hauteur-=10;
    cadre[1] = SDL_CreateRGBSurface(SDL_HWSURFACE, largeur, hauteur, 32, 0, 0, 0, 0);
    SDL_FillRect(cadre[1], NULL, SDL_MapRGB(cadre[1]->format, 33, 33, 33));
    pos_cadre.x+=5;
    pos_cadre.y+=5;
    SDL_BlitSurface(cadre[1], NULL, ecran, &pos_cadre);
    for (i=0 ; i<10 ; i++) {
        cadre[i+2] = SDL_CreateRGBSurface(SDL_HWSURFACE, largeur-(i+1)*2, hauteur-(i+1)*2, 32, 0, 0, 0, 0);
        SDL_FillRect(cadre[i+2], NULL, SDL_MapRGB(cadre[i+2]->format, 33+((i+1)*3), 33+((i+1)*3),33+((i+1)*3)));
        pos_cadre.x++;
        pos_cadre.y++;
        SDL_BlitSurface(cadre[i+2], NULL, ecran, &pos_cadre);
    }
    SDL_Flip(ecran);
    for (i=0; i<12 ; i++)
        SDL_FreeSurface(cadre[i]);
}

int menu(SDL_Surface *ecran) {
    /** Fonction servant a afficher un menu et attendant l'action du joueur.
      * La fonction retour JOUER si le joueur clique sur "jouer", SAUVEGARDER si le joueur clique sur "Sauvegarder...",
      * CHARGER si le joueur clique sur "Charger...", QUITTER si le joueur clique sur "Quitter"
      * Elle retour -1 si la fonction n'arrive pas a charger quoi que ce soit
      */

    /// SUGGESTION pour la fonction == Faire une gestion efficace des erreurs avec un return de menu cohérant a ces erreurs

    SDL_Surface *fond = NULL, *titre = NULL, *option[NB_OPTION_MENU]= {NULL};
    SDL_Rect pos_fond, pos_titre, pos_option[NB_OPTION_MENU];
    SDL_Color couleur_titre = {240, 170, 23}, couleur_option = {242, 242, 242};
    SDL_Event choix;
    TTF_Font *police_titre=NULL, *police_option=NULL;
    char texte_option[NB_OPTION_MENU][16]=ENUM_TITRE_OPTION_MENU();
    int i, continuer=1, ret[NB_OPTION_MENU]=ENUM_RETURN_OPTION_MENU();

    pos_fond.x=0;
    pos_fond.y=0;

    //Placement d'un image de fond PS: La taille de l'image ne s'ajuste pas a l'écran (pour l'instant)
    fond = IMG_Load("images/background.jpg");
    test_surface(fond, 10);
    SDL_BlitSurface(fond, NULL, ecran, &pos_fond);
    SDL_Flip(ecran);
    SDL_FreeSurface(fond);

    //Affichage des boutons
    for (i=0; i<NB_OPTION_MENU ; i++) {
        pos_option[i].x = CENTRER(ecran->w, LARGE_CADRE_MENU);
        pos_option[i].y = CENTRER(ecran->h, (LONG_CADRE_MENU*4+ESPACE_CADRE_MENU*3))+i*(LONG_CADRE_MENU+ESPACE_CADRE_MENU);
        cadre(ecran, (int)pos_option[i].x, (int)pos_option[i].y);
    }

    //Ajouter les textes à cet endroit
        // TITRE
    police_titre = TTF_OpenFont("polices/coalition.ttf", 52);
    titre = TTF_RenderText_Blended(police_titre, "SPACE INSADERS", couleur_titre);
    test_surface(titre, 11);
    pos_titre.x = CENTRER(ecran->w, titre->w);
    pos_titre.y = LARGE_CADRE_MENU/2;
    SDL_BlitSurface(titre, NULL, ecran, &pos_titre);
    SDL_FreeSurface(titre);
    TTF_CloseFont(police_titre);
        // OPTIONS DU MENU
    police_option = TTF_OpenFont("polices/geo_sans_light.ttf", 26);
    for (i=0; i<NB_OPTION_MENU ; i++) {
        option[i] = TTF_RenderText_Blended(police_option, texte_option[i], couleur_option);
        pos_option[i].x += CENTRER(LARGE_CADRE_MENU, option[i]->w);
        pos_option[i].y += 10;
        SDL_BlitSurface(option[i], NULL, ecran, &pos_option[i]);
        SDL_FreeSurface(option[i]);
    }
    TTF_CloseFont(police_option);
    SDL_Flip(ecran);

    // Ajouter/Faire le code de gestion d'evenements pour les différentes options
    while (continuer) {
        SDL_WaitEvent(&choix);
        switch (choix.type) {
            case SDL_QUIT:
                continuer=0;
                break;
            case SDL_KEYDOWN:
                switch (choix.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        continuer=0;
                        break;
                    default:
                        break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                switch (choix.button.button) {
                    case SDL_BUTTON_LEFT:
                        // Voir si je peut pas faire une fonction en plus pour tester si le curseur est dans une région prédéfinis
                        if ((choix.button.x > CENTRER(ecran->w, LARGE_CADRE_MENU)) && (choix.button.x < CENTRER(ecran->w, LARGE_CADRE_MENU)+LARGE_CADRE_MENU))
                            for (i=0 ; i<NB_OPTION_MENU ; i++)
                                if ((choix.button.y > CENTRER(ecran->h, TOT_HAUT_CADRE_MENU)+(i*(LONG_CADRE_MENU+ESPACE_CADRE_MENU))) && (choix.button.y < CENTRER(ecran->h, TOT_HAUT_CADRE_MENU)+(LONG_CADRE_MENU+i*(LONG_CADRE_MENU+ESPACE_CADRE_MENU))))
                                    return ret[i];
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }

    }
    return -1;
}

void charge_niveau (SDL_Surface *ecran) {
    SDL_Surface *fond_combat=NULL;
    SDL_Rect pos_fond, pos_barre_ia[2], pos_barre_player[2];
    _vaisseau v_joueur, v_ia;

    // Initialisation des positions
    init_pos(&pos_fond, 0, 0);
    init_pos(&pos_barre_ia[0], 700, 300);
    init_pos(&pos_barre_ia[1], 700, 325);
    init_pos(&pos_barre_player[0], 5, TAILLE_ECRAN_Y-5);
    init_pos(&pos_barre_player[1], 5, TAILLE_ECRAN_Y-30);

    v_joueur.vie=HAUT;
    v_joueur.bouclier=BAS;
    init_vaisseau(&v_ia);
    // Affichage du fond de combat
    fond_combat = IMG_Load("images/map_fond_combat.jpg");
    SDL_BlitSurface(fond_combat, NULL, ecran, &pos_fond);
    SDL_Flip(ecran);
    SDL_FreeSurface(fond_combat);

    //Barre de vie & bouclier du joueur
    barre_vie(ecran, v_joueur, pos_barre_player[0]);
    barre_bouclier(ecran, v_joueur, pos_barre_player[1]);
    //Affichage de la barre de vie & de la barre du bouclier de l'ia:
    barre_vie(ecran, v_ia, pos_barre_ia[0]);
    barre_bouclier(ecran, v_ia, pos_barre_ia[1]);
}

void barre_vie(SDL_Surface *ecran, _vaisseau v_joueur, SDL_Rect pos_barre) {
    /** CETTE FONCTION AFFICHE DES BARRES DE VIE*/
    SDL_Surface *surface_barre_vie=NULL;

    // Affichage de la barre de vie
    surface_barre_vie = SDL_CreateRGBSurface(SDL_HWSURFACE, 150, 20, 32, 0, 0, 0, 0);
    switch (v_joueur.vie) {
        case BAS:
            SDL_FillRect(surface_barre_vie, NULL, SDL_MapRGB(surface_barre_vie->format, 255, 0, 0));
            test_surface(surface_barre_vie, 101);
            break;
        case MOYEN:
            SDL_FillRect(surface_barre_vie, NULL, SDL_MapRGB(surface_barre_vie->format, 255, 165, 0));
            test_surface(surface_barre_vie, 101);
            break;
        case HAUT:
            SDL_FillRect(surface_barre_vie, NULL, SDL_MapRGB(surface_barre_vie->format, 0, 255, 0));
            test_surface(surface_barre_vie, 101);
            break;
    }
    SDL_BlitSurface(surface_barre_vie, NULL, ecran, &pos_barre);
    SDL_Flip(ecran);
    SDL_FreeSurface(surface_barre_vie);
}

void barre_bouclier(SDL_Surface *ecran, _vaisseau v, SDL_Rect pos_barre) {
    /** CETTE FONCTION AFFICHE DES BARRES DE BOUCLIER */
    SDL_Surface *surface_barre_bouclier=NULL;

    // Affichage de la barre du bouclier
    switch (v.bouclier) {
        case BAS:
            surface_barre_bouclier = IMG_Load("images/bouclier_BAS.jpg");
            test_surface(surface_barre_bouclier, 100);
            break;
        case MOYEN:
            surface_barre_bouclier = IMG_Load("images/bouclier_MOYEN.jpg");
            test_surface(surface_barre_bouclier, 100);
            break;
        case HAUT:
            surface_barre_bouclier = IMG_Load("images/bouclier_HAUT.jpg");
            test_surface(surface_barre_bouclier, 100);
            break;
    }
    SDL_BlitSurface(surface_barre_bouclier, NULL, ecran, &pos_barre);
    SDL_Flip(ecran);
    SDL_FreeSurface(surface_barre_bouclier);
}

void play(SDL_Surface *ecran) {
    SDL_Event action;
    int continuer=1, temps_actuel=0, temps_precedent=0;
    SDL_Surface *player=NULL;
    _vaisseau v_player;

    /// Zone pour les commandes a effectué des l'affichage de la carte
    charge_niveau(ecran);
    init_pos(&(v_player.position), 20, CENTRER(TAILLE_ECRAN_Y, 50)); //place le joueur a gauche de l'écran
    player=IMG_Load("images/player_ship.png");
    SDL_BlitSurface(player, NULL, ecran, &v_player.position);
    SDL_Flip(ecran);
    v_player.acceleration=3;
    v_player.vitesse=0;
    v_player.vitesse_max=0;
    v_player.rotation=45;


    while (continuer) {
        /// ZONE POUR PLACER LES COMMANDES A FAIRE AVANT L'ENREGISTREMENT DE L'ACTION DU JOUEUR


        // Test de l'action du joueur
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
                        vitesse_player(&v_player, AVANT);
                        break;
                    case SDLK_s:
                        vitesse_player(&v_player, ARRIERE);
                        break;
                    case SDLK_a:
                        v_player.rotation+=5;
                        break;
                    case SDLK_d:
                        v_player.rotation-=5;
                        break;
                    case SDLK_c:
                        v_player.vitesse=0;
                        v_player.rotation=0;
                        break;
                    default:
                        break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                switch (action.button.button) {
                    case SDL_BUTTON_LEFT:
                        charge_niveau(ecran);
                        v_player.position.x = action.button.x;
                        v_player.position.y = action.button.y;
                        aff_player(ecran, player, &v_player);
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }

        /// Zone pour placer les commandes a faires après les actions du joueur, mais avant la pause du jeu


        // Gestion du temps pour éviter la surexploitation du CPU
        temps_actuel=SDL_GetTicks();
        if (temps_actuel - temps_precedent > CALCUL_FPS(FPS)) {
            temps_precedent=temps_actuel;
        } else {
            SDL_Delay(30 - (temps_actuel - temps_precedent));
        }

        /// Zone pour placer les commandes a faire après la pause du jeu
        aff_player(ecran, player, &v_player);
    }

    /// Zone pour les commandes a effectué avant le déchargement de la carte
    SDL_FreeSurface(player);
}






