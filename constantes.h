#ifndef DEF_CONSTANTES
    #define DEF_CONSTANTES

    #define TAUX_SECONDE 30
    #define TAILLE_ECRAN_Y 768
    #define TAILLE_ECRAN_X 1366

    enum {JOUER, SAUVEGARDER, CHARGER, QUITTER};
    enum {AVANT, ARRIERE, RDROITE, RGAUCHE};

    #define CENTRER(cadre, bloc) (((cadre)/2) - ((bloc)/2))
    #define CALCUL_FPS(fps) 1000/fps
    #define RADIANATION(angle) (angle)*(2.0*PI)/360.0
    /// MENU
    #define LARGE_CADRE_MENU 300
    #define LONG_CADRE_MENU 50
    #define ESPACE_CADRE_MENU 30
    #define NB_OPTION_MENU 4
    #define TOT_HAUT_CADRE_MENU (NB_OPTION_MENU * LONG_CADRE_MENU) + ((NB_OPTION_MENU - 1) * ESPACE_CADRE_MENU)
    #define ENUM_TITRE_OPTION_MENU() {"Jouer", "Sauvegarder...", "Charger...", "Quitter"}
    #define ENUM_RETURN_OPTION_MENU() {JOUER, SAUVEGARDER, CHARGER, QUITTER}
    /// PLAY()
    #define NB_TO_UP_RECT 2
    /// PLAYER
    #define TAILLE_JOUEUR 50
    /// GENERAL
    #define FPS 60
    #define PI  3.14159265
#endif // DEF_CONSTANTES

