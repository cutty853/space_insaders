typedef enum{HORIZONTAL, VERTICAL} _sens;

typedef struct {
    int nuance;
    int taille_x;
    int taille_y;
    _sens sens;
} _degrade;

typedef enum {BAS, MOYEN, HAUT} _charge;
typedef enum {TIR_LASER, OBUS, RAYON_LASER} _arme;
//typedef enum {CAPA1, CAPA2} _capacite;

typedef struct {
    int poid;
    int acceleration;
    _charge bouclier;
    _arme arme;
    _charge vie;
    //_capacite capacite;
} _vaisseau;
