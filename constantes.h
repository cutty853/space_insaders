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
    #define NB_TO_UP_RECT 3
    /// AFF_CONSOLE()
    #define NB_STATS_CONSOLE 5
    #define TAILLE_CONSOLE_X 150
    #define TAILLE_CONSOLE_Y 180
    #define ENUM_TITRE_STATS_CONSOLE() {"Vitesse : ", "Position en x : ", "Position en y : ", "Largeur : ", "Hauteur : "}
    #define ENUM_VAR_STATS_CONSOLE() {sprintf(texte_infosup[0], "%d", vaisseau.vitesse), sprintf(texte_infosup[1], "%d", (int)vaisseau.position.x), \
    sprintf(texte_infosup[2], "%d", (int)vaisseau.position.y), sprintf(texte_infosup[3], "%d", (int)vaisseau.position.w), \
    sprintf(texte_infosup[4], "%d", (int)vaisseau.position.h)}
    /// PLAYER
    #define TAILLE_JOUEUR 50
    /// GENERAL
    #define FPS 60
    #define PI  3.14159265
#endif // DEF_CONSTANTES

