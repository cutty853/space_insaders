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


void cadre(SDL_Surface *ecran, int pos_x, int pos_y){
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

int menu(SDL_Surface *ecran){
    /** Fonction servant a afficher un menu et attendant l'action du joueur.
      * La fonction retour JOUER si le joueur clique sur "jouer", SAUVEGARDER si le joueur clique sur "Sauvegarder...",
      * CHARGER si le joueur clique sur "Charger...", QUITTER si le joueur clique sur "Quitter"
      * Elle retour -1 si la fonction n'arrive pas a charger quoi que ce soit
      */

    /// SUGGESTION pour la fonction == Faire une gestion efficace des erreurs avec un return de menu coh�rant a ces erreurs

    SDL_Surface *fond = NULL, *titre = NULL, *option[NB_OPTION_MENU]= {NULL};
    SDL_Rect pos_fond, pos_titre, pos_option[NB_OPTION_MENU];
    SDL_Color couleur_titre = {240, 170, 23}, couleur_option = {242, 242, 242};
    SDL_Event choix;
    TTF_Font *police_titre=NULL, *police_option=NULL;
    double zoom;
    char texte_option[NB_OPTION_MENU][16]=ENUM_TITRE_OPTION_MENU();
    int i, continuer=1, ret[NB_OPTION_MENU]=ENUM_RETURN_OPTION_MENU();

    pos_fond.x=0;
    pos_fond.y=0;

    //Placement d'un image de fond PS: La taille de l'image ne s'ajuste pas a l'�cran (pour l'instant)
    fond = IMG_Load("images/background.jpg");
    zoom = (ecran->w*1.0)/(fond->w*1.0);
    fond = rotozoomSurface(fond, 0, zoom, 1);
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

    //Ajouter les textes � cet endroit
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

    // Ajouter/Faire le code de gestion d'evenements pour les diff�rentes options
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
                        // Voir si je peut pas faire une fonction en plus pour tester si le curseur est dans une r�gion pr�d�finis
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
    SDL_Surface *fond_combat = NULL;
    SDL_Rect pos_fond;
    double zoom;
    init_pos(&pos_fond, 0, 0);

    /// Affichage du fond de combat:
    fond_combat = IMG_Load("images/map_fond_combat.jpg");
    zoom = (ecran->w*1.0)/(fond_combat->w*1.0);
    fond_combat = rotozoomSurface(fond_combat, 0, zoom, 1);
    test_surface(fond_combat, 101);
    SDL_BlitSurface(fond_combat, NULL, ecran, &pos_fond);
    SDL_FreeSurface(fond_combat);
    SDL_Flip(ecran);
}

SDL_Rect eff_vie(SDL_Surface *ecran, _vaisseau *vaisseau, SDL_Surface *save_screen){
    SDL_BlitSurface(save_screen, &(vaisseau->vie.position), ecran, &(vaisseau->vie.position));
    return vaisseau->vie.position;
}
SDL_Rect eff_bouclier(SDL_Surface *ecran, _vaisseau *vaisseau, SDL_Surface *save_screen){
    SDL_BlitSurface(save_screen, &(vaisseau->bouclier.position), ecran, &(vaisseau->bouclier.position));
    return vaisseau->bouclier.position;
}
SDL_Rect eff_vaisseau(SDL_Surface *ecran, _vaisseau *vaisseau, SDL_Surface *save_screen){
    SDL_BlitSurface(save_screen, &(vaisseau->position), ecran, &(vaisseau->position));
    return vaisseau->position;
}
SDL_Rect eff_tir(SDL_Surface *ecran, SDL_Surface *save_screen, _vaisseau *vaisseau){
    SDL_BlitSurface(save_screen, &(vaisseau->tir.position), ecran, &(vaisseau->tir.position));
    return vaisseau->tir.position;
}

SDL_Rect aff_bouclier(SDL_Surface *ecran, _vaisseau *vaisseau){
    switch (vaisseau->bouclier.charge) {
        case VIDE:
            SDL_BlitSurface(vaisseau->bouclier.sprite[0], NULL, ecran, &(vaisseau->bouclier.position));
            break;
        case BAS:
            SDL_BlitSurface(vaisseau->bouclier.sprite[1], NULL, ecran, &(vaisseau->bouclier.position));
            break;
        case MOYEN:
            SDL_BlitSurface(vaisseau->bouclier.sprite[2], NULL, ecran, &(vaisseau->bouclier.position));
            break;
        case HAUT:
            SDL_BlitSurface(vaisseau->bouclier.sprite[3], NULL, ecran, &(vaisseau->bouclier.position));
            break;
    }
    return vaisseau->bouclier.position;
}
SDL_Rect aff_vie(SDL_Surface *ecran, _vaisseau *vaisseau){
    switch (vaisseau->vie.charge) {
        case VIDE:
            SDL_BlitSurface(vaisseau->vie.sprite[0], NULL, ecran, &(vaisseau->vie.position));
            break;
        case BAS:
            SDL_BlitSurface(vaisseau->vie.sprite[1], NULL, ecran, &(vaisseau->vie.position));
            break;
        case MOYEN:
            SDL_BlitSurface(vaisseau->vie.sprite[2], NULL, ecran, &(vaisseau->vie.position));
            break;
        case HAUT:
            SDL_BlitSurface(vaisseau->vie.sprite[3], NULL, ecran, &(vaisseau->vie.position));
            break;
    }
    return vaisseau->vie.position;
}
SDL_Rect aff_vaisseau(SDL_Surface *ecran, _vaisseau *vaisseau, SDL_Surface *save_screen){
    SDL_Surface *tmp_rotation = NULL;
    SDL_Rect pre_pos_vaisseau;

    /// La condition permet le d�calage du joueur lors de son angle, afin que la rotation se fasse r�ellement par rapport au centre du sprite:
    pre_pos_vaisseau.w = vaisseau->position.w;
    pre_pos_vaisseau.h = vaisseau->position.h;
    tmp_rotation = rotozoomSurface(vaisseau->sprite, vaisseau->angle, 1.0, 1);
    SDL_BlitSurface(tmp_rotation, NULL, ecran, &(vaisseau->position));
    if (vaisseau->etat_rotation == 1) {
        vaisseau->position.x -= ((vaisseau->position.w - TAILLE_JOUEUR)-(pre_pos_vaisseau.w - TAILLE_JOUEUR))/2;
        vaisseau->position.y -= ((vaisseau->position.h - TAILLE_JOUEUR)-(pre_pos_vaisseau.h - TAILLE_JOUEUR))/2;
        vaisseau->etat_rotation = 0;
    }
    SDL_BlitSurface(tmp_rotation, NULL, ecran, &(vaisseau->position));
    SDL_FreeSurface(tmp_rotation);
    return vaisseau->position; /// La fonction retourne un tableau de 2 positions qui servira a update une r�gion sp�cifique de la carte (se tableau a �t� malloc il est donc � free)
}
SDL_Rect aff_tir (SDL_Surface *ecran, _vaisseau *vaisseau){
    SDL_Surface *tir;
    /// Blit de la surface avec ces nouvelles positions:
    tir = rotozoomSurface(vaisseau->tir.sprite, vaisseau->tir.angle, 1.0, 1);
    SDL_BlitSurface(tir, NULL, ecran, &(vaisseau->tir.position));
    SDL_FreeSurface(tir);

    return vaisseau->tir.position;
}
SDL_Rect aff_explosion (SDL_Surface *ecran, _explosion *boom, _vaisseau vaisseau){
    init_pos(&(boom->position), vaisseau.position.x, vaisseau.position.y);
    SDL_BlitSurface(boom->sprite[boom->phase], NULL, ecran, &(boom->position));
    return vaisseau.position;
}

void charge_sprite_bouclier(_vaisseau *vaisseau){
    SDL_Surface *tmp_bouclier = NULL;
    SDL_Rect case_courante;
    int i;

    tmp_bouclier = IMG_Load("images/ia_bouclier.jpg");
    test_surface(tmp_bouclier, 106);
    init_pos (&case_courante, 0, 0);
    case_courante.w = LARGEUR_BOUCLIER;
    case_courante.h = HAUTEUR_BOUCLIER;
    for(i=0; i<NB_SPRITES_BOUCLIER; i++){ /// sprite[0] = bouclier vide.
        vaisseau->bouclier.sprite[i] = SDL_CreateRGBSurface(SDL_HWSURFACE, case_courante.w, case_courante.h, 32, 0, 0, 0, 0);
        SDL_BlitSurface(tmp_bouclier, &case_courante, vaisseau->bouclier.sprite[i], NULL);
        case_courante.y += 5;
    }
    SDL_FreeSurface(tmp_bouclier);
}
void charge_sprite_vie(_vaisseau *vaisseau){
    int i;
    switch (vaisseau->intelligence) {
        case IA:
            for (i=0;i<NB_SPRITES_VIE;i++)
                vaisseau->vie.sprite[i] = SDL_CreateRGBSurface(SDL_HWSURFACE, 40, 5, 32, 0, 0, 0, 0);
            break;
        case JOUEUR:
            for (i=0;i<NB_SPRITES_VIE;i++)
                vaisseau->vie.sprite[i] = SDL_CreateRGBSurface(SDL_HWSURFACE, 150, 20, 32, 0, 0, 0, 0);
            break;
    }
    for (i=0;i<NB_SPRITES_VIE;i++)
        vaisseau->vie.sprite[i] = SDL_CreateRGBSurface(SDL_HWSURFACE, 40, 5, 32, 0, 0, 0, 0);

    SDL_FillRect(vaisseau->vie.sprite[0], NULL, SDL_MapRGB((vaisseau->vie.sprite[0])->format, 33, 33, 33)); /// Gris
    SDL_FillRect(vaisseau->vie.sprite[1], NULL, SDL_MapRGB((vaisseau->vie.sprite[0])->format, 255, 0, 0)); ///Rouge
    SDL_FillRect(vaisseau->vie.sprite[2], NULL, SDL_MapRGB((vaisseau->vie.sprite[1])->format, 255, 165, 0)); ///Orange
    SDL_FillRect(vaisseau->vie.sprite[3], NULL, SDL_MapRGB((vaisseau->vie.sprite[2])->format, 0, 255, 0)); ///Bleu
}
void charge_sprite_tir (_vaisseau *vaisseau){
    switch(vaisseau->arme){
        case TIR_LASER:
            vaisseau->tir.sprite = SDL_CreateRGBSurface(SDL_HWSURFACE, 2, 20, 32, 255, 0, 0, 255);
            SDL_FillRect(vaisseau->tir.sprite, NULL, SDL_MapRGB((vaisseau->tir.sprite)->format, 255, 0, 0)); ///Rouge.
            break;
        case OBUS:
            vaisseau->tir.sprite = SDL_CreateRGBSurface(SDL_HWSURFACE, 4, 10, 32, 255, 165, 0, 255);
            SDL_FillRect(vaisseau->tir.sprite, NULL, SDL_MapRGB((vaisseau->tir.sprite)->format, 255, 165, 0));///Orange.
            break;
        case RAYON_LASER:
            vaisseau->tir.sprite = SDL_CreateRGBSurface(SDL_HWSURFACE, 4, 10, 32, 0, 0, 255, 255);
            SDL_FillRect(vaisseau->tir.sprite, NULL, SDL_MapRGB((vaisseau->tir.sprite)->format, 0, 0, 255));///Bleu.
            break;
        default:
            break;
    }
}
void charge_sprite_explosion (_explosion *boom){
/// le pointeur sur boom peut etre mis a NULL pour que le chargement de boom ne soit pas effectu� (sert a la fonction charge_niveau)
    SDL_Surface *tmp_boom;
    SDL_Rect case_courante;
    int i;

    tmp_boom = IMG_Load("images/explosion.png");
    test_surface(tmp_boom, 102);
    init_pos (&case_courante, 0, 0);
    case_courante.w = LARGEUR_EXPLOSION;
    case_courante.h = HAUTEUR_EXPLOSION;
    for (i=0 ; i<NB_SPRITES_EXPLOSION ; i++) {
        if ((i%5 == 0) && i!=0) {
            case_courante.y += 65;
            case_courante.x = 0;
        } else {
            case_courante.x += 65;
        }
        boom->sprite[i] = IMG_Load("images/transparent_explosion.png");
        test_surface(boom->sprite[i], 103);
        SDL_BlitSurface(tmp_boom, &case_courante, boom->sprite[i], NULL);
        test_surface(boom->sprite[i], 302+i);
    }
    SDL_FreeSurface(tmp_boom);
}

/// Les deux fonctions suivantes ne sont pas forc�ment tr�s utiles, mais permettent une sorte de centralisation...
void decharge_sprite_vie(_vaisseau *vaisseau){
    int i;
    for (i=0;i<NB_SPRITES_VIE;i++)
        SDL_FreeSurface(vaisseau->vie.sprite[i]);
}
void decharge_sprite_bouclier(_vaisseau *vaisseau){
    int i;
    for (i=0;i<NB_SPRITES_BOUCLIER;i++)
        SDL_FreeSurface(vaisseau->bouclier.sprite[i]);
}
void decharge_sprite_explosion (_explosion *boom){
    int i;
    for (i=0;i<NB_SPRITES_EXPLOSION;i++)
        SDL_FreeSurface(boom->sprite[i]);
}
void decharge_sprite_tir (_vaisseau *vaisseau){
    SDL_FreeSurface(vaisseau->tir.sprite);
}

void degrade(_degrade prop_deg, SDL_Surface *ecran, SDL_Rect pos_degrade) {
    int i, taux, taille, couleur;
    SDL_Surface **rectangle_degrade;

    switch (prop_deg.sens) {
        case HORIZONTAL:
            taille = prop_deg.taille_x;
            break;
        case VERTICAL:
            taille = prop_deg.taille_y;
            break;
    }
    if (prop_deg.nuance>256)
        prop_deg.nuance=256;
    if (prop_deg.nuance>taille)
        prop_deg.nuance=taille;
    couleur=256/prop_deg.nuance;
    taux = taille/prop_deg.nuance;
    rectangle_degrade = malloc(sizeof(SDL_Surface*)*(prop_deg.nuance));
    switch (prop_deg.sens) {
        case HORIZONTAL:
            prop_deg.taille_x=taux;
            break;
        case VERTICAL:
            prop_deg.taille_y=taux;
            break;
    }

    for (i=0; i<prop_deg.nuance; i++) {
        rectangle_degrade[i]=SDL_CreateRGBSurface(SDL_HWSURFACE, prop_deg.taille_x, prop_deg.taille_y, 32, 0, 0, 0, 0);
        if (taille>=prop_deg.nuance) {
            SDL_FillRect(rectangle_degrade[i], NULL, SDL_MapRGB(rectangle_degrade[i]->format, i*couleur, i*couleur, i*couleur));
            SDL_BlitSurface(rectangle_degrade[i], NULL, ecran, &pos_degrade);
            switch (prop_deg.sens) {
                case HORIZONTAL:
                    pos_degrade.x+=taux;
                    break;
                case VERTICAL:
                    pos_degrade.y+=taux;
                    break;
            }
        }
    }

    SDL_Flip(ecran);
    for (i = 0 ; i < prop_deg.nuance ; i++)
        SDL_FreeSurface(rectangle_degrade[i]);
}

SDL_Rect aff_console (SDL_Surface *ecran, _vaisseau vaisseau,  SDL_Surface* save_screen, TTF_Font *police_texte) {
    SDL_Surface *info[NB_STATS_CONSOLE]={NULL};
    char texte_info[NB_STATS_CONSOLE][50]= ENUM_TITRE_STATS_CONSOLE(), texte_infosup[NB_STATS_CONSOLE][20]= ENUM_VAR_STATS_CONSOLE();
    SDL_Rect pos_texte, pos_to_up;
    SDL_Color rouge={255, 0, 0};
    int i;

    init_pos(&pos_to_up, 0, TAILLE_ECRAN_Y-TAILLE_CONSOLE_Y-45);
    pos_to_up.w=TAILLE_CONSOLE_X;
    pos_to_up.h=TAILLE_CONSOLE_Y;
    init_pos(&pos_texte, (int)pos_to_up.x, (int)pos_to_up.y);
    SDL_BlitSurface(save_screen, &pos_to_up, ecran, &pos_texte);

    for (i=0; i<NB_STATS_CONSOLE ; i++) {
        strcat(texte_info[i], texte_infosup[i]);
        info[i] = TTF_RenderText_Blended(police_texte, texte_info[i], rouge);
        pos_texte.y += 30;
        SDL_BlitSurface(info[i], NULL, ecran, &pos_texte);
        SDL_FreeSurface(info[i]);
    }

    return pos_to_up;
}
