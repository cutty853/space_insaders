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

typedef struct {
    int poid;
    int vitesse; // En pixel par seconde
    int acceleration; // En prixel par seconde*pow(-2)
    int vitesse_max; // En pixel par seconde
    _intelligence intelligence;
    _charge bouclier;
    _charge vie;
    _arme arme;
    int vitesse_rotation;
    float angle;
    int etat_rotation;
    //_capacite capacite;
    SDL_Rect position;
    SDL_Surface *sprite;
} _vaisseau;
