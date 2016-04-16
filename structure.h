#include "constantes.h"

typedef enum{HORIZONTAL, VERTICAL} _sens;

typedef struct {
    int nuance;
    int taille_x;
    int taille_y;
    _sens sens;
} _degrade;

typedef enum{JOUEUR, IA} _intelligence;
typedef enum {VIDE, BAS, MOYEN, HAUT} _charge;
typedef enum {TIR_LASER, OBUS, RAYON_LASER} _arme;
//typedef enum {CAPA1, CAPA2, CAPA3} _capacite;

typedef struct{
    SDL_Surface *sprite[NB_SPRITES_BOUCLIER];
    SDL_Rect position;
    _charge charge;
} _bouclier;
typedef struct{
    SDL_Surface *sprite[NB_SPRITES_VIE];
    SDL_Rect position;
    _charge charge;
} _vie;

typedef struct {
    int poid;
    int vitesse; // En pixel par seconde
    int acceleration; // En prixel par seconde*pow(-2)
    int vitesse_max; // En pixel par seconde
    int vitesse_min;
    _intelligence intelligence;
    _bouclier bouclier;
    _vie vie;
    _arme arme;
    int vitesse_rotation;
    int angle;
    int etat_rotation;
    //_capacite capacite;
    SDL_Rect position;
    SDL_Surface *sprite;


    int angle_de_decalage; // TEST
} _vaisseau;
