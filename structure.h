typedef enum{HORIZONTAL, VERTICAL} _sens;

typedef struct {
    int nuance;
    int taille_x;
    int taille_y;
    _sens sens;
} _degrade;

typedef enum {BAS, MOYEN, HAUT} _charge;
typedef enum {TIR_LASER, OBUS, RAYON_LASER} _arme;
//typedef enum {CAPA1, CAPA2, CAPA3} _capacite;

typedef struct {
    int poid;
    int acceleration;
    int vitesse_max;
    _charge bouclier;
    _charge vie;
    _arme arme;
    //_capacite capacite;

    int position_x;
    int position_y;
} _vaisseau;
