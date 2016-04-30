#ifndef DEF_CONSTANTES
    #define DEF_CONSTANTES

    #define TAUX_SECONDE 20
    #define TAILLE_ECRAN_Y 768
    #define TAILLE_ECRAN_X 1366

    enum {JOUER, SAUVEGARDER, CHARGER, QUITTER};
    enum {VAISSEAU, BOUCLIER, VIE}; /// type pour fonction eff_sprite
    enum {AVANT, ARRIERE, RDROITE, RGAUCHE};
    enum {RIEN, AVANCE, RECUL, TOURNE}; /// ordre de mouvement (ia)
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
    /// AFF_CONSOLE()
    #define NB_STATS_CONSOLE 5
    #define TAILLE_CONSOLE_X 200
    #define TAILLE_CONSOLE_Y 180
    #define ENUM_TITRE_STATS_CONSOLE() {"Position en x : ", "Angle : ", "Position en y : ", "Angle de decalage : ", "Hauteur : "}
    #define ENUM_VAR_STATS_CONSOLE() {{sprintf(texte_infosup[0], "%d", (int)vaisseau.position.x), sprintf(texte_infosup[1], "%lf", vaisseau.vitesse), \
    sprintf(texte_infosup[2], "%d", (int)vaisseau.position.y), sprintf(texte_infosup[3], "%i", (int)vaisseau.angle_de_decalage), \
    sprintf(texte_infosup[4], "%d", (int)vaisseau.position.h)}}
    /// PLAYER
    #define TAILLE_JOUEUR 60
    #define VITESSE_MINI_COS 2
    #define LARGEUR_EXPLOSION 65
    #define HAUTEUR_EXPLOSION 65
    #define DEGAT_TIR_LASER 30
    #define DEGAT_RAYON_LASER 200
    #define DEGAT_OBUS 50
    /// INTERFACE
    #define NB_ETAT_INTERFACE 1
    enum {CONSOLE=0};
    /// GENERAL
    #define NB_SPRITES_EXPLOSION 25
    #define NB_SPRITES_BOUCLIER 4
    #define LARGEUR_BOUCLIER 40
    #define HAUTEUR_BOUCLIER 5
    #define NB_SPRITES_VIE 4
    #define FPS 60
    #define PI  3.14159265358979323846264338327950288419716939937510582
#endif // DEF_CONSTANTES

