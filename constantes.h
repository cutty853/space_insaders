#ifndef DEF_CONSTANTES
    #define DEF_CONSTANTES

    #define TAUX_SECONDE 30
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
    #define TOT_HAUT_CADRE_MENU (NB_OPTION_MENU * LONG_CADRE_MENU) + ((NB_OPTION_MENU - 1) * ESPACE_CADRE_MENU)
    #define ENUM_TITRE_OPTION_MENU() {"Jouer", "Sauvegarder...", "Charger...", "Quitter"}
    #define ENUM_RETURN_OPTION_MENU() {JOUER, SAUVEGARDER, CHARGER, QUITTER}
    /// PLAY()
    #define NB_TO_UP_RECT 3
    /// AFF_CONSOLE()
    #define NB_STATS_CONSOLE 5
    #define TAILLE_CONSOLE_X 150
    #define TAILLE_CONSOLE_Y 180
    #define ENUM_TITRE_STATS_CONSOLE() {"Position en x : ", "Vitesse : ", "Position en y : ", "Largeur : ", "Hauteur : "}
    #define ENUM_VAR_STATS_CONSOLE() {{sprintf(texte_infosup[0], "%d", (int)vaisseau.position.x), sprintf(texte_infosup[1], "%lf", (double)vaisseau.vitesse), \
    sprintf(texte_infosup[2], "%d", (int)vaisseau.position.y), sprintf(texte_infosup[3], "%d", (int)vaisseau.position.w), \
    sprintf(texte_infosup[4], "%d", (int)vaisseau.position.h)}}
    /// PLAYER
    #define TAILLE_JOUEUR 60
    #define VITESSE_MIN_JOUEUR 2
    #define NB_SPRITES_EXPLOSION 25
    /// GENERAL
    #define FPS 60
    #define PI  3.14159265358979323846264338327950288419716939937510582
#endif // DEF_CONSTANTES

