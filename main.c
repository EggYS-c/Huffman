#include <stdio.h>
#include <stdint.h>

typedef struct donnee_struct{
    char nom;
    double proba;
}donnee;

typedef struct hoffman_struct{
    char nomFeuille[10];
    double valeurFeuille;
    int* droite;
    int* gauche;
}hoffman;

int Init_Hoffman(void);

int main(void){
    int valeur, echange;

    valeur = Init_Hoffman();
    
}

int Init_Hoffman(void){
    int valeur;
    donnee* feuille;

    printf("Combien de valeurs dans Hoffman ?\n");
    scanf("%d",valeur);
    
    
    for(int i = 0 ; i< valeur ; i++){
	donnee structure_proba[valeur];
        structure_proba[i].nom = "EV%d",i+1;
        structure_proba[i].proba = 0.00;
    }
    
    for(int i = 0 ; i< valeur ; i++){
        printf("-----------------------\n");
        printf("Nom de %d\n",i+1);
        scanf("%s",structure_proba[i].nom);
        printf("Proba de %d\n",i+1);
        scanf("%f",structure_proba[i].proba);
        printf("-----------------------\n");
    }
    
    for(int i = 0 ; i< valeur ; i++){
        printf("-----------------------\n");
        printf("En résumé, nous avons \n");
        printf("%s\t",structure_proba[i].nom);
    }
    printf("\n");
    for(int i = 0 ; i< valeur ; i++){
        printf("%f\t",structure_proba[i].proba);
    }
    printf("\n");
    printf("-----------------------\n");
    

    return valeur;

}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct donnee_struct {
    char nom[10];
    double proba;
} donnee;

int Init_Hoffman(void);

int main(void) {
    int valeur = Init_Hoffman();
    printf("Valeur retournée : %d\n", valeur);
    return 0;
}

int Init_Hoffman(void) {
    int valeur;
    printf("Combien de valeurs dans Hoffman ?\n");
    scanf("%d", &valeur);  // CORRECTION: &valeur

    // Allocation dynamique
    donnee* structure_proba = malloc(valeur * sizeof(donnee));
    if (structure_proba == NULL) {
        printf("Erreur d'allocation mémoire\n");
        return -1;
    }

    // Initialisation des noms automatiques et probabilités à 0
    for (int i = 0; i < valeur; i++) {
        snprintf(structure_proba[i].nom, sizeof(structure_proba[i].nom), "EV%d", i + 1);
        structure_proba[i].proba = 0.0;
    }

    // Remplissage depuis l'utilisateur
    for (int i = 0; i < valeur; i++) {
        printf("-----------------------\n");
        printf("Nom de %d: ", i + 1);
        scanf("%s", structure_proba[i].nom);

        printf("Proba de %d: ", i + 1);
        scanf("%lf", &structure_proba[i].proba);  // CORRECTION: & et %lf
        printf("-----------------------\n");
    }

    // Résumé
    printf("En résumé, nous avons:\n");
    for (int i = 0; i < valeur; i++) {
        printf("%s\t", structure_proba[i].nom);
    }
    printf("\n");
    for (int i = 0; i < valeur; i++) {
        printf("%.2lf\t", structure_proba[i].proba);
    }
    printf("\n-----------------------\n");

    // Libération mémoire
    free(structure_proba);
    return valeur;
}
