#ifndef DEF_CONSTANTES
    #define DEF_CONSTANTES

    #define TAUX_SECONDE 20
    #define TAILLE_ECRAN_Y 768
    #define TAILLE_ECRAN_X 1366

    enum {JOUER, SAUVEGARDER, CHARGER, QUITTER};
    enum {AVANT, ARRIERE, RDROITE, RGAUCHE};
    enum {AVANCE, RECUL, TOURNE}; /// ordre de mouvement
    enum {EN_BAS, EN_HAUT, DROITE, GAUCHE, BAS_DROITE, BAS_GAUCHE, HAUT_DROITE, HAUT_GAUCHE}; /// Position relative
    enum {POSITIF, NEGATIF, DROIT}; /// sens de rotation

    #define CENTRER(cadre, bloc) (((cadre)/2) - ((bloc)/2))
    #define CALCUL_FPS(fps) 1000/fps
    #define RADIANATION(angle) (angle)*(2.0*PI)/360.0
    /// MENU
    #define LARGE_CADRE_MENU 300
    #define LONG_CADRE_MENU 50
    #define ESPACE_CADRE_MENU 30
    #define NB_OPTION_MENU 4
    #define TOT_HAUT_CADRE_MENU (NB_OPTION_MENU * LONG_CADRE_MENU) + ((3) * ESPACE_CADRE_MENU)
    #define ENUM_TITRE_OPTION_MENU() {"Jouer", "Sauvegarder...", "Charger...", "Quitter"}
    #define ENUM_RETURN_OPTION_MENU() {JOUER, SAUVEGARDER, CHARGER, QUITTER}
    /// PLAYER
    #define FPS 60
    #define TAILLE_JOUEUR 50
    /// GENERAL
    #define PI  3.14159265
#endif // DEF_CONSTANTES

