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

SDL_Rect* init_niveau_general(int *niveau, int *nbr_ia, int *nbr_pos_to_up_tir_ia){
    FILE *fichier = NULL;
    char chemin_niveau[] = {"ressources/index_partie0.txt"};

    sprintf(chemin_niveau, "ressources/index_partie%i.txt", *niveau);
    fichier = fopen(chemin_niveau, "r");/// On ouvre le ficher correspondant au niveau actuel.
    if (fichier != NULL){
        trouve_titre(fichier, 4, "GEN");
        deplace_curseur(fichier);
        *nbr_ia = recup_int(fichier);
        *nbr_pos_to_up_tir_ia = 2*(*nbr_ia);
        SDL_Rect *pos_to_up_tir_ia = malloc( (*nbr_pos_to_up_tir_ia)*sizeof(SDL_Rect) );
        close(fichier);
        return(pos_to_up_tir_ia);
    }else
        exit(6006);
}
_vaisseau* init_niveau_ia(int *niveau, int nbr_ia){
    _vaisseau *tab_v_ia = NULL;
    FILE *fichier = NULL;
    int init_val[NBR_VALEURS_TRANSMISES], i = 0, j = 0;
    char chemin_niveau[] = {"ressources/index_partie0.txt"};


    tab_v_ia = malloc(nbr_ia*sizeof(_vaisseau));/// Initialisation du tableau des vaisseaux ia en fonction de leur nombre.
    sprintf(chemin_niveau, "ressources/index_partie%i.txt", *niveau);
    fichier = fopen(chemin_niveau, "r");/// On ouvre le ficher correspondant au niveau actuel.
    if (fichier != NULL){
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
        close(fichier);
        return(tab_v_ia);
    }else
        exit(6016);
}
_vaisseau init_niveau_joueur(int *niveau){
    _vaisseau v_joueur;
    FILE *fichier = NULL;
    int init_val[NBR_VALEURS_TRANSMISES], i = 0;
    char chemin_niveau[] = {"ressources/index_partie0.txt"};

    sprintf(chemin_niveau, "ressources/index_partie%i.txt", *niveau);
    fichier = fopen(chemin_niveau, "r");/// On ouvre le ficher correspondant au niveau actuel.
    if (fichier != NULL){
        trouve_titre(fichier, 4, "JOU");
        for(i=0; i<NBR_VALEURS_INT; i++){
            deplace_curseur(fichier);
            init_val [i] = recup_int(fichier);
        }
        for(i=NBR_VALEURS_INT; i<NBR_VALEURS_TRANSMISES; i++){
            deplace_curseur(fichier);
            init_val[i] = recup_string(fichier);
        }
        init_vaisseau(&v_joueur, init_val);
        init_hitbox(&(v_joueur.hitbox), v_joueur.position.x+(TAILLE_JOUEUR/2), v_joueur.position.y+(TAILLE_JOUEUR/2), TAILLE_JOUEUR/2, 0, v_joueur.position.x, v_joueur.position.y, v_joueur.position.w, v_joueur.position.h);
        charge_sprite_bouclier(&v_joueur);
        charge_sprite_vie(&v_joueur);
        charge_sprite_tir(&v_joueur);
        close(fichier);
        return(v_joueur);
    }else
        exit(6026);
}


_explosion init_explosions_joueur(int nbr_ia){
    _explosion boom_joueur;
    charge_sprite_explosion(&boom_joueur);
    boom_joueur.phase = 0;
    return(boom_joueur);
}
_explosion* init_explosions_ia(int nbr_ia){
    int i = 0;
    _explosion *boom_ia = malloc(nbr_ia*sizeof(_explosion));

    for(i=0; i<nbr_ia; i++){
        charge_sprite_explosion(&boom_ia[i]);
        boom_ia[i].phase = 0;
    }
    return(boom_ia);
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
    }else
        exit(656);
}

void init_pos(SDL_Rect* position, int x, int y){
    position->x = x;
    position->y = y;
}

void init_tir(_vaisseau *vaisseau){/// Initialisation de tous les parametres du tir
    if(vaisseau->vitesse > vaisseau->vitesse_min){
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
                vaisseau->tir.distance_max = 15;
                vaisseau->tir.vitesse = vaisseau->vitesse_max + 7;
                break;
            case RAYON_LASER:
                vaisseau->tir.degats = 3;
                vaisseau->tir.distance_max = 10;
                vaisseau->tir.vitesse = vaisseau->vitesse_max + 25;
                break;
        }
    }else
        vaisseau->tir.etat = 0; ///Interdit le tir lors de la collision (seul cas ou la condition n'est pas remplise).
}

void init_hitbox(_hitbox* h, int xCercle, int yCercle, int rCercle, int nb_points, int xAABB, int yAABB, int wAABB, int hAABB) {
    if (nb_points==0)
        h->polygone.points = NULL;
    else
        h->polygone.points = malloc(sizeof(_point)*nb_points);
    h->polygone.nb_points = nb_points;
    h->cercle.centre.x = xCercle;
    h->cercle.centre.y = yCercle;
    h->cercle.rayon = rCercle;
    init_pos(&(h->aabb), xAABB, yAABB);
    h->aabb.w = wAABB;
    h->aabb.h = hAABB;
}
