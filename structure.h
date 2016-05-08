#ifndef DEF_INCLUDE7
#define DEF_INCLUDE7

#include "constantes.h"

#endif // DEF_INCLUDE7

typedef enum{HORIZONTAL, VERTICAL} _sens;

typedef struct {
    int nuance;
    int taille_x;
    int taille_y;
    _sens sens;
} _degrade;

typedef enum{JOUEUR, IA} _intelligence;
typedef enum{CHERCHE, ATTAQUE, FUIT, INDEPENDENT} _comportement;
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
typedef struct{
    SDL_Surface *sprite[NB_SPRITES_EXPLOSION];
    SDL_Rect position;
    int phase;
} _explosion;
typedef struct{
    SDL_Surface *sprite;
    SDL_Rect position;
    _arme type;
    float vitesse;
    int angle;
    int etat;
    int temps_passe;
} _tir;


typedef struct {
    SDL_Surface *sprite;
    int poid;
    float vitesse, acceleration; // En prixel par seconde*pow(-2)
    int vitesse_max, vitesse_min;
    int vitesse_rotation, angle;
    int etat_rotation;
    _intelligence intelligence;
    _comportement comportement;
    _bouclier bouclier;
    _vie vie;
    _arme arme;
    _tir tir;
    //_capacite capacite;
    SDL_Rect position;


    int angle_de_decalage; // TEST

} _vaisseau;


typedef struct {
    char key[SDLK_LAST];
    int mousex, mousey;
    int mousexrel, mouseyrel;
    char mousebuttons[8];
    char quit;
} _input;
typedef struct {
    int x, y, rayon;
} _cercle;
