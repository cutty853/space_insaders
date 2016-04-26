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
    #define bijour

    #include "constantes.h"
#endif

void cadre(SDL_Surface *ecran, int pos_x, int pos_y)
{
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

int menu(SDL_Surface *ecran)
{
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
    test_surface(fond, 100);
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
    test_police(police_titre, 201);
    titre = TTF_RenderText_Blended(police_titre, "SPACE INSADERS", couleur_titre);
    pos_titre.x = CENTRER(ecran->w, titre->w);
    pos_titre.y = LARGE_CADRE_MENU/2;
    SDL_BlitSurface(titre, NULL, ecran, &pos_titre);
    SDL_FreeSurface(titre);
    TTF_CloseFont(police_titre);
        // OPTIONS DU MENU
    police_option = TTF_OpenFont("polices/geo_sans_light.ttf", 26);
    test_police(police_option, 202);
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
    SDL_Rect pos_fond;
    init_pos(&pos_fond, 0, 0);

    /// Affichage du fond de combat:
    fond_combat = IMG_Load("images/map_fond_combat.jpg");
    test_surface(fond_combat, 101);
    SDL_BlitSurface(fond_combat, NULL, ecran, &pos_fond);
    SDL_FreeSurface(fond_combat);
    SDL_Flip(ecran);
}

void barre_vie_joueur(SDL_Surface *ecran, _vaisseau v_joueur) {
    SDL_Surface *barre_vie=NULL;
    SDL_Rect pos_barre_vie;
    pos_barre_vie.x=5;
    pos_barre_vie.y=TAILLE_ECRAN_Y-25;

    /// Affichage de la barre de vie
    barre_vie = SDL_CreateRGBSurface(SDL_HWSURFACE, 150, 20, 32, 0, 0, 0, 0);
    test_surface(barre_vie, 100); ///Verif chargement.
    switch (v_joueur.vie.charge) {
        case VIDE:
            /// plus de vie
            break;
        case BAS:
            SDL_FillRect(barre_vie, NULL, SDL_MapRGB(barre_vie->format, 255, 0, 0)); //Rouge
            break;
        case MOYEN:
            SDL_FillRect(barre_vie, NULL, SDL_MapRGB(barre_vie->format, 255, 165, 0)); //Orange
            break;
        case HAUT:
            SDL_FillRect(barre_vie, NULL, SDL_MapRGB(barre_vie->format, 0, 255, 0)); //Bleu
            break;
    }

    SDL_BlitSurface(barre_vie, NULL, ecran, &pos_barre_vie);
    SDL_Flip(ecran);
    SDL_FreeSurface(barre_vie);
}
void barre_bouclier_joueur(SDL_Surface *ecran, _vaisseau v_joueur) {
    SDL_Surface *barre_bouclier=NULL;
    SDL_Rect pos_barre_bouclier;
    pos_barre_bouclier.x=5;
    pos_barre_bouclier.y=TAILLE_ECRAN_Y-50;

    /// Affichage de la barre du bouclier
    switch (v_joueur.bouclier.charge) {
        case VIDE:
        /// plus de bouclier
        break;
        case BAS:
            barre_bouclier = IMG_Load("images/bouclier_BAS.jpg");
            break;
        case MOYEN:
            barre_bouclier = IMG_Load("images/bouclier_MOYEN.jpg");
            break;
        case HAUT:
            barre_bouclier = IMG_Load("images/bouclier_HAUT.jpg");
            break;
    }
    test_surface(barre_bouclier, 101); ///Verif chargement.
    SDL_BlitSurface(barre_bouclier, NULL, ecran, &pos_barre_bouclier);
    SDL_Flip(ecran);
    SDL_FreeSurface(barre_bouclier);
}

void play(SDL_Surface *ecran) {
    SDL_Event action;
    TTF_Font *police_texte=NULL;
    int continuer=1, temps_actuel=0, temps_precedent=0, nb_pos_to_up_ecran=0, etat_console[2]={0};
    SDL_Surface *save_screen = NULL;
    SDL_Rect *pos_to_up_console;
    SDL_Rect pos_to_up_ecran[9]; /// 9 = nombre actuel de nouvelles positions.
    _vaisseau v_player, v_ia1;
    _explosion boom;

    /// Zone pour les commandes a effectué dès l'affichage de la carte
        /// Chargement de la map
    charge_niveau(ecran);
    charge_sprite_explosion(&boom);
        /// ia:
    init_vaisseau(&v_ia1, IA, 100, 0, 0.1,8,  BAS, HAUT, TIR_LASER, 900, 300, 4, 90);
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


        /// L'ia joue en première:
        /// IL FAUT D'ABORD "CACHER" LES ANCIENNES SURFACES PUIS FAIRE LES ACTIONS (déplacement) PUIS REAFFICHER LES SURFACES AVEC LES NOUVELLES POSITIONS !
        pos_to_up_ecran[0] = eff_bouclier(ecran, &v_ia1, save_screen);
        pos_to_up_ecran[1] = eff_vie(ecran, &v_ia1, save_screen);
        pos_to_up_ecran[2] = eff_vaisseau(ecran, &v_ia1, save_screen);
        pos_to_up_ecran[3] = eff_vaisseau(ecran, &v_player, save_screen);

        tour_ia(&v_ia1, &v_player, ecran);

        /// ZONE POUR PLACER LES COMMANDES A FAIRE AVANT L'ENREGISTREMENT DE L'ACTION DU JOUEUR

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

        /// Zone pour placer les commandes a faires après les actions du joueur, mais avant la pause du jeu


        /// Gestion du temps pour éviter la surexploitation du CPU
        temps_actuel=SDL_GetTicks();
        if (temps_actuel - temps_precedent > CALCUL_FPS(FPS)) {
            temps_precedent=temps_actuel;
        }else{
            SDL_Delay(30 - (temps_actuel - temps_precedent));
        }


        /// Zone pour placer les commandes a faire après la pause du jeu
        pos_to_up_ecran[4] = aff_vaisseau(ecran, &v_ia1, save_screen);/// TOUJOURS afficher le vaisseau en premier dans l'appelle des fonction (dans cette version de la fonction).
        pos_to_up_ecran[5] = aff_bouclier(ecran, &v_ia1);
        pos_to_up_ecran[6] = aff_vie(ecran, &v_ia1);

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

        // console de test
        if (etat_console[0] && etat_console[1]) {
            pos_to_up_console[0] = aff_console(ecran, v_player, save_screen, police_texte);
            SDL_UpdateRects(ecran, 1, pos_to_up_console);
        }

        /// AFFICHAGE:
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
