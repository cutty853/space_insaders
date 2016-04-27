#ifndef DEF_INCLUDE
    #define DEF_INCLUDE
    #include <stdlib.h>
    #include <stdio.h>
    #include <math.h>
    #include <SDL/SDL.h>
    #include <SDL/SDL_image.h>
    #include <SDL/SDL_ttf.h>
    #include <SDL/SDL_rotozoom.h>
    #include "structure.h"
    #include "utilitaire.h"
    #include "jeu.h"
    #include "ia.h"
    #include "player.h"


    #include "constantes.h"
#endif

void test_surface(SDL_Surface* surface, int ref_err){
    if (surface==NULL) {
        fprintf(stderr, "[ images ] Erreur n°%d\n Veuillez vous réferer au tableau des erreurs s'il vous plait.\n", ref_err);
        exit(EXIT_FAILURE);
    }
}

void test_police(TTF_Font* police, int ref_err){
    if (police==NULL) {
        fprintf(stderr, "[ polices ] Erreur n°%d\n Veuillez vous réferer au tableau des erreurs s'il vous plait.\n", ref_err);
        exit(EXIT_FAILURE);
    }
}

void init_pos(SDL_Rect* position, int x, int y){
    position->x=x;
    position->y=y;
}

void pause(){
    int continuer=1;
    SDL_Event choix;

    while (continuer) {
        SDL_WaitEvent(&choix);
        switch(choix.type) {
            case SDL_QUIT:
                continuer=0;
                break;
            case SDL_KEYDOWN:
                switch(choix.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        continuer=0;
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }
}

void init_vaisseau(_vaisseau *vaisseau, int intelligence, int poid, float vitesse, float acceleration, int v_max, int bouclier, int vie, int arme, int position_x, int position_y, int v_rotation, int angle){
    vaisseau->intelligence = intelligence;
    vaisseau->poid = poid;
    vaisseau->vitesse = vitesse;
    vaisseau->acceleration = acceleration;
    vaisseau->vitesse_max = v_max;
    vaisseau->vitesse_min = -v_max/3;
    vaisseau->bouclier.charge = bouclier;
    vaisseau->vie.charge = vie;
    vaisseau->arme = arme;
    //vaisseau.capacite=CAPA1;
    vaisseau->vitesse_rotation = v_rotation;
    vaisseau->angle = angle;
    if(intelligence == IA) {
        vaisseau->sprite = IMG_Load("images/vaisseau_ia.png");
        test_surface(vaisseau->sprite, 104);
    }
    else {
        vaisseau->sprite = IMG_Load("images/joueur_ship.png");
        test_surface(vaisseau->sprite, 105);
    }
    vaisseau->position.x = position_x;
    vaisseau->position.y = position_y;
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

SDL_Rect aff_bouclier(SDL_Surface *ecran, _vaisseau *vaisseau){
    vaisseau->bouclier.position.x = vaisseau->position.x;
    vaisseau->bouclier.position.y = (vaisseau->position.y)+60; /// +60 = taille verticale du vaisseau.
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
    vaisseau->vie.position.x = vaisseau->bouclier.position.x;
    vaisseau->vie.position.y = (vaisseau->bouclier.position.y)+5; /// +5 = epaisseur de la barre de bouclier.
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
    SDL_Rect pre_pos_vaisseau;
    SDL_Surface *tmp_rotation=NULL;
    /// Sauvegarde des anciennes positions
    pre_pos_vaisseau.w = vaisseau->position.w;
    pre_pos_vaisseau.h = vaisseau->position.h;
    /// Calcul des positions
    vaisseau->position.x += (vaisseau->vitesse)*sin(-RADIANATION(vaisseau->angle));
    vaisseau->position.y += (vaisseau->vitesse)*(-cos(RADIANATION(vaisseau->angle)));
    /// Affichage du vaisseau
    tmp_rotation = rotozoomSurface(vaisseau->sprite, vaisseau->angle, 1.0, 1);
    SDL_BlitSurface(tmp_rotation, NULL, ecran, &(vaisseau->position));
    if (vaisseau->etat_rotation == 1) {/// Cette condition permet le décalage du joueur lors de son angle, afin que la rotation se fasse réellement par rapport au centre du sprite
        vaisseau->position.x -= ((vaisseau->position.w - TAILLE_JOUEUR)-(pre_pos_vaisseau.w - TAILLE_JOUEUR))/2;
        vaisseau->position.y -= ((vaisseau->position.h - TAILLE_JOUEUR)-(pre_pos_vaisseau.h - TAILLE_JOUEUR))/2;
        vaisseau->etat_rotation = 0;
    }
    SDL_BlitSurface(tmp_rotation, NULL, ecran, &(vaisseau->position));
    SDL_FreeSurface(tmp_rotation);
    return vaisseau->position; /// La fonction retourne un tableau de 2 positions qui servira a update une région spécifique de la carte (se tableau a été malloc il est donc à free)
}

void charge_sprite_bouclier(_vaisseau *vaisseau){
    /// le pointeur sur boom peut etre mis a NULL pour que le chargement de boom ne soit pas effectué (sert a la fonction charge_niveau)
    SDL_Surface *tmp_bouclier;
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

/// Les deux fonctions suivantes ne sont pas forcément très utiles, mais permettent une sorte de centralisation...
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
