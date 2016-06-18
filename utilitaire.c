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
int aleatoire(int mini, int maxi){/// random entre le minimum demande et le maximum demande (inclus).
    return ( (rand()%maxi)+mini );
}

void init_vaisseau(_vaisseau *vaisseau, int tab_init_val[NBR_VALEURS_TRANSMISES]){
    vaisseau->position.x = tab_init_val[0];
    vaisseau->position.y = tab_init_val[1];
    vaisseau->angle = tab_init_val[2];
    vaisseau->poid = tab_init_val[3];
    vaisseau->vitesse_max = tab_init_val[4];
    vaisseau->vitesse_rotation = tab_init_val[5];
    vaisseau->arme = tab_init_val[6];
    vaisseau->bouclier.charge = tab_init_val[7];
    vaisseau->vie.charge = tab_init_val[8];
    vaisseau->intelligence = tab_init_val[9];

    vaisseau->vitesse_min = -(vaisseau->vitesse_max/3);
    vaisseau->vitesse = 0.0;
    vaisseau->acceleration = ((vaisseau->poid)*(vaisseau->vitesse_max))/1000.0; ///accélération dépendante du poid.
    vaisseau->tir.etat = 0; /// à l'initialisation, aucun des vaisseaux ne tir.
    vaisseau->etat_rotation = 0; /// à l'initialisation, aucun des vaisseaux ne tourne.


    if(vaisseau->intelligence == IA){
        vaisseau->sprite = IMG_Load("images/vaisseau_ia.png");
        test_surface(vaisseau->sprite, 104);
        vaisseau->vaisseau_ia.seuil_intelligence = tab_init_val[10];
        vaisseau->vaisseau_ia.comportement = CHERCHE;
        vaisseau->vaisseau_ia.angle_de_decalage = 0;/// n'a pas encore choisi de cible.
        switch(vaisseau->vaisseau_ia.seuil_intelligence){
            case IA_NOVICE:
                vaisseau->vaisseau_ia.val_seuil_intelligence = 25;
                break;
            case IA_NORMAL:
                vaisseau->vaisseau_ia.val_seuil_intelligence = 50;
                break;
            case IA_EXPERTE:
                vaisseau->vaisseau_ia.val_seuil_intelligence = 75;
                break;
            default:
                exit(636);
        }
    }else if(vaisseau->intelligence == JOUEUR){/// il s'agit du joueur.
        vaisseau->sprite = IMG_Load("images/joueur_ship.png");
        test_surface(vaisseau->sprite, 105);
    }else{
        exit(656);
    }
}
void init_pos(SDL_Rect* position, int x, int y){
    position->x=x;
    position->y=y;
}
void init_tir (_vaisseau *vaisseau){/// Initialisation de tous les parametres du tir
    vaisseau->tir.etat = 1;
    init_pos(&(vaisseau->tir.position), vaisseau->position.x+(TAILLE_JOUEUR/2.0), vaisseau->position.y+(TAILLE_JOUEUR/2.0));
    vaisseau->tir.angle = vaisseau->angle;
    vaisseau->tir.distance_parcourue = 0;

    switch(vaisseau->arme){
        case TIR_LASER:
            vaisseau->tir.degats = 1;
            vaisseau->tir.distance_max = 15;
            vaisseau->tir.vitesse = vaisseau->vitesse_max + 15;
            break;
        case OBUS:
            vaisseau->tir.degats = 2;
            vaisseau->tir.distance_max = 12;
            vaisseau->tir.vitesse = vaisseau->vitesse_max + 7;
            break;
        case RAYON_LASER:
            vaisseau->tir.degats = 3;
            vaisseau->tir.distance_max = 10;
            vaisseau->tir.vitesse = vaisseau->vitesse_max + 25;
            break;
    }
}

void mouvement_vaisseau (int action, int sens, _vaisseau *vaisseau){
    switch (action){
        case AVANCE:
            if (vaisseau->vitesse < vaisseau->vitesse_max)
                vaisseau->vitesse += vaisseau->acceleration;
            break;
        case RECUL:
            if (vaisseau->vitesse > vaisseau->vitesse_min)
                vaisseau->vitesse -= vaisseau->acceleration;
            break;
        case TOURNE:
            vaisseau->etat_rotation = 1;
            if(sens == POSITIF){
                vaisseau->angle += vaisseau->vitesse_rotation;
            }
            if(sens == NEGATIF){
                vaisseau->angle -= vaisseau->vitesse_rotation;
            }
            break;
        case RIEN:
            /// L'ia choisi de ne rien changer à sa trajectoire.
            break;
    }
}

void deplace_curseur(FILE *fichier){
    char deux_points = ':';
    do{/// Boucle de lecture des caractères un à un
        deux_points = fgetc(fichier); /// On lit le caractère
    } while (deux_points != ':'); /// On continue tant que fgetc n'a pas retourné EOF (fin de fichier)
}
void trouve_titre(FILE *fichier, int taille_string, char titre_voulu[]){
    char *titre = malloc(taille_string*sizeof(char));
    do{
        fgets(titre, taille_string, fichier); /// On lit maximum 4 caractères du fichier, on stocke le tout dans "titre".
    } while(strcmp(titre, titre_voulu) != 0);
}
int recup_int(FILE* fichier){
    int val_int = 0;
    fscanf(fichier, "%i", &val_int);
    return(val_int);
}
int recup_string(FILE* fichier){
    char val_string[4];
    fgets(val_string, 4, fichier);
    if(strcmp(val_string, "TIR") == 0){
        return(TIR_LASER);
    }else if(strcmp(val_string, "OBU") == 0){
        return(OBUS);
    }else if(strcmp(val_string, "RAY") == 0){
        return(RAYON_LASER);
    }else if(strcmp(val_string, "VID") == 0){
        return(VIDE);
    }else if(strcmp(val_string, "BAS") == 0){
        return(BAS);
    }else if(strcmp(val_string, "MOY") == 0){
        return(MOYEN);
    }else if(strcmp(val_string, "HAU") == 0){
        return(HAUT);
    }else if(strcmp(val_string, "JOU") == 0){
        return(JOUEUR);
    }else if(strcmp(val_string, "ART") == 0){
        return(IA);
    }else if(strcmp(val_string, "NOV") == 0){
        return(IA_NOVICE);
    }else if(strcmp(val_string, "NOR") == 0){
        return(IA_NORMAL);
    }else if(strcmp(val_string, "EXP") == 0){
        return(IA_EXPERTE);
    }else{
        exit(4673);
    }
}

void degat_tir(_vaisseau *vaisseau_touche, _vaisseau *vaisseau_tireur){
    switch(vaisseau_touche->bouclier.charge){
        case BAS:
        case MOYEN:
        case HAUT:
            vaisseau_touche->bouclier.charge -= vaisseau_tireur->tir.degats;
            switch(vaisseau_touche->bouclier.charge){
                case VIDE:
                case BAS:
                case MOYEN:
                case HAUT:
                    break;
                default:
                    vaisseau_touche->bouclier.charge = VIDE;/// Remise à zéro pour éviter les valeurs inconnues.
                    break;
            }
            break;
        case VIDE:
            vaisseau_touche->vie.charge -= vaisseau_tireur->tir.degats;
            switch(vaisseau_touche->vie.charge){
                case VIDE:
                case BAS:
                case MOYEN:
                case HAUT:
                    break;
                default:
                    vaisseau_touche->vie.charge = VIDE;/// Remise à zéro pour éviter les valeurs inconnues.
                    break;
            }
            break;
    }
}
void degat_collisions(_vaisseau *vaisseau){
    if(vaisseau->bouclier.charge > VIDE)
        vaisseau->bouclier.charge --;
    vaisseau->vitesse = -1.0*(  vaisseau->vitesse_max+( vaisseau->vitesse_max/(2*(vaisseau->vitesse_max-(vaisseau->vitesse-1))) )  );
}

void gestion_distance_tir(_vaisseau *vaisseau){
    if (vaisseau->tir.etat == 1){
        vaisseau->tir.distance_parcourue ++;
        if(vaisseau->tir.distance_parcourue > vaisseau->tir.distance_max)
            vaisseau->tir.etat = 0;
    }
}

void calcul_pos_bouclier(_vaisseau *vaisseau){
    vaisseau->bouclier.position.x = vaisseau->position.x;
    vaisseau->bouclier.position.y = (vaisseau->position.y)+60+10; /// +60 = taille verticale du vaisseau, +10 pou la taille de rotation.
}
void calcul_pos_vie(_vaisseau *vaisseau){
    vaisseau->vie.position.x = vaisseau->bouclier.position.x;
    vaisseau->vie.position.y = (vaisseau->bouclier.position.y)+5; /// +5 = epaisseur de la barre de bouclier.
}
void calcul_pos_vaisseau(_vaisseau *vaisseau, SDL_Surface *ecran){
    SDL_Surface *tmp_rotation = NULL;

    /// Calcul des positions:
    vaisseau->position.x += (vaisseau->vitesse)*sin(-RADIANATION(vaisseau->angle));
    vaisseau->position.y += (vaisseau->vitesse)*(-cos(RADIANATION(vaisseau->angle)));

    /// Vérification des sorties d'écran:
    if(vaisseau->position.x >= ecran->w)
        vaisseau->position.x = TAILLE_VAISSEAU;
    else if(vaisseau->position.x <= 0)
        vaisseau->position.x = ecran->w-TAILLE_VAISSEAU;
    if(vaisseau->position.y >= ecran->h)
        vaisseau->position.y = TAILLE_VAISSEAU;
    else if(vaisseau->position.y <= 0)
        vaisseau->position.y = ecran->h-TAILLE_VAISSEAU;

    SDL_FreeSurface(tmp_rotation);
}
void calcul_pos_tir(_vaisseau *vaisseau){
    vaisseau->tir.position.x += vaisseau->tir.vitesse * sin(-RADIANATION(vaisseau->tir.angle));
    vaisseau->tir.position.y += vaisseau->tir.vitesse * (-cos(RADIANATION(vaisseau->tir.angle)));
}

void supprime_vaisseau (_vaisseau *vaisseau){
    vaisseau->position.x = 0;
    vaisseau->position.y = 0;
    vaisseau->angle = 0;
    vaisseau->poid = 0;
    vaisseau->vitesse_max = 0;
    vaisseau->vitesse_min = 0;
    vaisseau->vitesse = 0.0;
    vaisseau->acceleration = 0.0;
    vaisseau->vitesse_rotation = 0;
    vaisseau->arme = 0;
    vaisseau->bouclier.charge = 0;
    vaisseau->vie.charge = 0;
    vaisseau->vaisseau_ia.seuil_intelligence = 0;
    vaisseau->vaisseau_ia.val_seuil_intelligence = 0;
    vaisseau->vaisseau_ia.comportement = 0;
    vaisseau->vaisseau_ia.angle_de_decalage = 0;
    vaisseau->tir.etat = 0;
    vaisseau->sprite = NULL;
}
