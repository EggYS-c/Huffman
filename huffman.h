/**
 * @file huffman.h
 * @brief Implémentation de l'algorithme de compression Huffman
 * @author Votre Nom
 * @date 2025
 * 
 * Ce fichier contient les structures et fonctions nécessaires pour
 * effectuer une compression de texte utilisant l'algorithme de Huffman.
 */

#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ========================================================================
   DÉFINITIONS ET CONSTANTES
   ======================================================================== */

/**
 * @def TAILLE_MAX_Compress
 * @brief Taille maximale du tableau de texte compressé
 */
#define TAILLE_MAX_Compress 100

/* ========================================================================
   STRUCTURES
   ======================================================================== */

/**
 * @struct noeud
 * @brief Structure représentant un nœud dans l'arbre de Huffman
 * 
 * Cette structure contient les informations sur un caractère et sa position
 * dans l'arbre de Huffman, incluant son code binaire et ses liens vers
 * les nœuds enfants.
 */
struct noeud {
    uint8_t  c;                      /**< Caractère représenté par le nœud */
    uint32_t occurrence;             /**< Nombre d'occurrences du caractère */
    uint32_t code;                   /**< Code binaire du caractère dans l'arbre */
    uint32_t tailleCode;             /**< Nombre de bits du code */
    struct noeud *gauche;            /**< Pointeur vers le nœud enfant gauche */
    struct noeud *droite;            /**< Pointeur vers le nœud enfant droit */
};

/* ========================================================================
   VARIABLES GLOBALES
   ======================================================================== */

extern uint8_t   texte[];                        /**< Texte non compressé */
extern uint8_t   texteCompress[TAILLE_MAX_Compress]; /**< Texte compressé */
extern uint32_t  tabCaractere[256];              /**< Tableau du nombre d'occurrences de chaque caractère */
extern uint32_t  nbrCaractereTotal;              /**< Nombre total de caractères dans le texte non compressé */
extern uint32_t  nbrCaractereDifferent;          /**< Nombre de caractères différents dans le texte */
extern uint8_t   tailleCompress;                 /**< Taille du texte compressé */
extern uint8_t   entete[];                       /**< En-tête du fichier compressé */
extern struct noeud* arbreHuffman[256];          /**< Tableau de pointeurs vers les nœuds de l'arbre */
extern struct noeud* racine;                     /**< Pointeur vers la racine de l'arbre de Huffman */

/* ========================================================================
   PROTOTYPES DE FONCTIONS
   ======================================================================== */

/**
 * @brief Calcule le nombre d'occurrences de chaque caractère
 * 
 * Parcourt le texte et compte le nombre d'occurrences de chaque caractère
 * dans le tableau tabCaractere.
 * 
 * @param texte Pointeur vers le texte à analyser
 * @param tabCaractere Tableau des occurrences (256 éléments pour ASCII étendu)
 * @return Nombre total de caractères dans le texte
 */
uint32_t occurrence(uint8_t* texte, uint32_t tabCaractere[256]);

/**
 * @brief Crée les feuilles de l'arbre de Huffman
 * 
 * Alloue dynamiquement les nœuds feuilles pour chaque caractère présent
 * dans le texte et les initialise avec leur nombre d'occurrences.
 * 
 * @param arbreHuffman Tableau de pointeurs vers les nœuds de l'arbre
 * @param tabCaractere Tableau des occurrences de chaque caractère
 * @return Nombre de caractères différents trouvés
 */
uint32_t creerFeuille(struct noeud* arbreHuffman[256], uint32_t tabCaractere[256]);

/**
 * @brief Affiche le contenu du tableau d'arbre de Huffman
 * 
 * Fonction de débogage qui affiche les informations de chaque nœud
 * (caractère et nombre d'occurrences).
 * 
 * @param arbreHuffman Tableau de pointeurs vers les nœuds
 * @param taille Nombre de nœuds à afficher
 */
void afficherTabArbreHuffman(struct noeud* arbreHuffman[256], uint32_t taille);

/**
 * @brief Trie l'arbre de Huffman par ordre croissant d'occurrences
 * 
 * Utilise un tri à bulles pour ordonner les nœuds de l'arbre selon
 * leur fréquence d'apparition (du moins fréquent au plus fréquent).
 * 
 * @param arbreHuffman Tableau de pointeurs vers les nœuds à trier
 * @param nbrCaractereDifferent Nombre de caractères différents
 */
void triArbre(struct noeud* arbreHuffman[256], uint32_t nbrCaractereDifferent);

/**
 * @brief Construit l'arbre de Huffman complet
 * 
 * Fusionne progressivement les nœuds de plus faible occurrence pour
 * construire l'arbre binaire de Huffman.
 * 
 * @param arbreHuffman Tableau de pointeurs vers les nœuds
 * @param nbrCaractereDifferent Nombre de caractères différents
 * @return Pointeur vers la racine de l'arbre construit
 */
struct noeud* creerArbre(struct noeud* arbreHuffman[256], uint32_t nbrCaractereDifferent);

/**
 * @brief Parcourt l'arbre de Huffman de manière récursive
 * 
 * Fonction de débogage qui parcourt l'arbre en profondeur et affiche
 * si chaque nœud est une feuille ou un nœud interne.
 * 
 * @param ptrNoeud Pointeur vers le nœud à parcourir
 */
void parcourirArbre(struct noeud* ptrNoeud);

/**
 * @brief Génère les codes binaires pour chaque caractère
 * 
 * Parcourt l'arbre récursivement et assigne à chaque feuille (caractère)
 * son code binaire de Huffman et la taille du code.
 * 
 * @param ptrNoeud Pointeur vers le nœud courant
 * @param code Code binaire en cours de construction
 * @param nbrCaractereDifferent Profondeur actuelle dans l'arbre
 */
void creerCode(struct noeud* ptrNoeud, uint32_t code, uint32_t nbrCaractereDifferent);

/**
 * @brief Recherche l'adresse du nœud contenant un caractère donné
 * 
 * Parcourt l'arbre récursivement pour trouver le nœud correspondant
 * au caractère spécifié.
 * 
 * @param ptrNoeud Pointeur vers le nœud de départ (généralement la racine)
 * @param caractere Caractère à rechercher
 * @return Pointeur vers le nœud contenant le caractère, NULL si non trouvé
 */
struct noeud* getAdress(struct noeud* ptrNoeud, uint8_t caractere);

/**
 * @brief Compresse le texte en utilisant l'arbre de Huffman
 * 
 * Convertit le texte non compressé en une séquence de bits utilisant
 * les codes de Huffman et stocke le résultat dans un tableau d'octets.
 * 
 * @param texte Texte à compresser
 * @param texteCompress Tableau de sortie pour le texte compressé
 * @param racine Pointeur vers la racine de l'arbre de Huffman
 * @return Nombre d'octets du texte compressé
 */
uint32_t compressionTexte(uint8_t texte[], uint8_t texteCompress[TAILLE_MAX_Compress], struct noeud* racine);

/**
 * @brief Crée l'en-tête du fichier compressé
 * 
 * Génère l'en-tête contenant les métadonnées nécessaires pour la décompression :
 * - Taille de l'en-tête
 * - Taille du texte compressé
 * - Nombre de caractères total
 * - Nombre de caractères différents
 * - Table des codes pour chaque caractère
 * 
 * @param entete Tableau de sortie pour l'en-tête
 * @param nbrCaractereTotal Nombre total de caractères dans le texte original
 * @param texteCompress Texte compressé
 * @param tailleCompress Taille du texte compressé en octets
 * @param nbrCaractereDifferent Nombre de caractères différents
 * @param racine Pointeur vers la racine de l'arbre de Huffman
 * @param texte Texte original (pour retrouver les caractères)
 */
void creationEntete(uint8_t* entete, uint32_t nbrCaractereTotal, uint8_t* texteCompress, 
                    uint8_t tailleCompress, uint32_t nbrCaractereDifferent, 
                    struct noeud* racine, uint8_t texte[]);

#endif /* HUFFMAN_H */
