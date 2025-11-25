
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"

#define TAILLE_MAX_Compress 100


uint8_t	texte[]="aaaabbbccd";				                //Texte non compressé
//uint8_t texte[]="abcdefffgg";
//uint8_t	texte[]="Demir Grappo, Thessa, Adrianna Grappo, Baby Montego, Idrian Sepulki, Kissandra Vorcien";

uint8_t    texteCompress[TAILLE_MAX_Compress];		        //Texte compressé
uint32_t	tabCaractere[256] = {0};			            //Tableau du nombre d'occurrence de chaque caratère
uint32_t	nbrCaractereTotal = 0;				            //Nombre de caractère total dans le texte non compressé
uint32_t	nbrCaractereDifferent = 0;			            //Nombre de caractère différent dans le texte non compressé
uint8_t    tailleCompress = 0;
uint8_t    entete[1024]={0};

struct noeud*   arbreHuffman[256] = {NULL};			        //arbre de Huffman
struct noeud*   racine  = NULL;


void main (void){
	//GPIO_Init();
	//USART2_Init();
  printf("---------------------------------------Welcome into main-------------------------------------------\r\n");
  
  nbrCaractereTotal = occurrence (texte, tabCaractere);
  
  printf("---------------------------------------occurrence done---------------------------------------------\r\n");
  
  nbrCaractereDifferent = creerFeuille (arbreHuffman, tabCaractere);
  
  printf("---------------------------------------creerFeuille done-------------------------------------------\r\n");
  
  afficherTabArbreHuffman (arbreHuffman, nbrCaractereDifferent);
  
  printf("---------------------------------------affiArbre done----------------------------------------------\r\n");
  
  triArbre (arbreHuffman,nbrCaractereDifferent);
  
  printf("---------------------------------------triArbre done-----------------------------------------------\r\n");
  
  afficherTabArbreHuffman (arbreHuffman, nbrCaractereDifferent);
  racine = creerArbre(arbreHuffman, nbrCaractereDifferent);
  
  printf("---------------------------------------creerArbre done---------------------------------------------\r\n");
  
  parcourirArbre(racine);
  
  printf("---------------------------------------parcourirArbre done-----------------------------------------\r\n");
  
  creerCode(racine,0,0);
  
  printf("---------------------------------------creerCode done----------------------------------------------\r\n");
  
  tailleCompress = compressionTexte(texte,texteCompress,racine);
  
  printf("------------------------------------compressionTexte done-------------------------------------------\r\n");
  
  creationEntete(entete,nbrCaractereTotal,texteCompress,tailleCompress, nbrCaractereDifferent,racine, texte);
  
  printf("-------------------------------------CreationEntete done--------------------------------------------\r\n");
  
  decompressEntete(entete,nbrCaractereTotal,texteCompress,tailleCompress, nbrCaractereDifferent,arbreHuffman);
  
  printf("------------------------------------decompressEntete done--------------------------------------------\r\n");
  
  racine = creerArbre(arbreHuffman, nbrCaractereDifferent);
  
  printf("---------------------------------creerArbreDecompress done-------------------------------------------\r\n");
  
  parcourirArbre(racine);
  
  printf("--------------------------------parcourirArbreDecompress done----------------------------------------\r\n");
  reconstructionChaine(texteCompress, racine, tailleCompress, nbrCaractereTotal);
}


uint32_t occurrence (uint8_t* texte, uint32_t tabCaractere[256]){
    uint32_t compteur = 0 ;
    for(uint8_t i = 0; i < strlen(texte); i++){
        
        compteur+=1;
        tabCaractere[texte[i]] += 1;
	}
	return compteur;
}


uint32_t creerFeuille (struct noeud* arbreHuffman[256], uint32_t tabCaractere[256]){
  uint8_t compteur = 0;
  struct noeud* n;
  uint8_t* addresseStructure = NULL;
  

	for(uint8_t i = 0; i < 255; i++){
	    
		if(tabCaractere[i] != 0){
			n = malloc (sizeof(struct noeud));
            n->c = i;
			n->occurrence = tabCaractere[i];
			n->code = 0;
			n->tailleCode = 0;
			n->droite = NULL;
			n->gauche = NULL;
      
			arbreHuffman[compteur] = n;
			compteur++;
		}
	}
	
  return compteur;
}

void afficherTabArbreHuffman (struct noeud* arbreHuffman[256],uint32_t nbrCaractereDifferent){
  struct noeud* n;
  
  for(uint8_t i = 0; i < nbrCaractereDifferent; i++){
      
    if (arbreHuffman[i] != NULL){
      n=arbreHuffman[i];
      printf("La feuille du caractère %c existe, il a %d occurrence(s), il a pour code %X\r\n", n->c, n->occurrence,n->code);
    }
  }
}

void triArbre (struct noeud* arbreHuffman[256],uint32_t nbrCaractereDifferent){
  struct noeud* n0;
  struct noeud* n1;
  
  for(uint8_t u = 0; u < nbrCaractereDifferent-1; u++){
      
    for(uint8_t i = 0; i < nbrCaractereDifferent-1; i++){
        
      n0=arbreHuffman[i];
      n1=arbreHuffman[i+1];
      if (n1->occurrence < n0->occurrence){
        arbreHuffman[i]=n1;
        arbreHuffman[i+1]=n0;
      }
    }
  }
}

struct noeud* creerArbre(struct noeud* arbreHuffman[256],uint32_t nbrCaractereDifferent){
  struct noeud* racine;
  struct noeud* n0;
  struct noeud* n1;
  
  for(uint8_t i = 0; i < nbrCaractereDifferent-1; i++){
    n0=arbreHuffman[i];
    n1=arbreHuffman[i+1];
    
    racine = malloc (sizeof(struct noeud));
    racine->c = '!';
    racine->occurrence=n0->occurrence+n1->occurrence;
    racine->gauche=n0;
    racine->droite=n1;
    arbreHuffman[i+1]=racine;
  }
  
  return racine;
}

void parcourirArbre(struct noeud* ptrNoeud){
  if(ptrNoeud->droite == NULL && ptrNoeud->gauche == NULL){
    printf("Je suis une feuille, caractère %c, code %X\r\n",ptrNoeud->c,ptrNoeud->code);
  }
  
  else{
    printf("Je suis un noeud\r\n");
    parcourirArbre(ptrNoeud->gauche);
    parcourirArbre(ptrNoeud->droite);
    
  }
}

void creerCode (struct noeud* ptrNoeud, uint32_t code, uint32_t nbrCaractereDifferent){
    
    if(ptrNoeud->droite == NULL && ptrNoeud->gauche == NULL){
        ptrNoeud->tailleCode = nbrCaractereDifferent;
        ptrNoeud->code = code;
        printf("%c \t code : %d \t taille : %d \r\n", ptrNoeud->c, ptrNoeud->code, ptrNoeud->tailleCode);
    }
    
    else{
        creerCode(ptrNoeud->droite, (code<<1), nbrCaractereDifferent+1);
        creerCode(ptrNoeud->gauche, (code<<1)+1, nbrCaractereDifferent+1);
    }
}

struct noeud* getAdress(struct noeud* ptrNoeud, uint8_t caractere){
    if(ptrNoeud == NULL) return NULL;
    
    if(ptrNoeud->c == caractere){
        return ptrNoeud;
    }
    
    else if(ptrNoeud->droite != NULL && getAdress(ptrNoeud->droite, caractere) != NULL){
        getAdress(ptrNoeud->droite, caractere);
    }
    
    else if(ptrNoeud->gauche != NULL && getAdress(ptrNoeud->gauche, caractere) != NULL){
        getAdress(ptrNoeud->gauche, caractere);
    }
}

uint32_t compressionTexte(uint8_t texte[], uint8_t texteCompress[TAILLE_MAX_Compress], struct noeud* racine){
    uint8_t caractere = 0;
    uint8_t cptBitMem = 0;
    uint8_t cptMot = 0;
    uint8_t compress = 00000000;
    uint8_t index = 0;
    struct noeud* ptrNoeud;
    uint8_t code = 0;
    uint8_t j = 0;
    uint8_t i = 0;
    uint8_t debordement = 0;
    
    while(i<strlen(texte) && i<TAILLE_MAX_Compress){
        index = 0;
        if (debordement == 1){
            index = 1;
            debordement = 0;
        }
        
        switch (index){
            case 0 :
                
            if(texte[i] != caractere){
                ptrNoeud = getAdress(racine, texte[i]);
                caractere = texte[i];
                index = 1;
            }
            
            else{
                index = 1;
            }
    
            case 1 :

                if(cptBitMem+(ptrNoeud->tailleCode) < 9){
                    compress = (compress<<ptrNoeud->tailleCode) + ptrNoeud->code;
                    cptBitMem += (ptrNoeud->tailleCode);
                    i++;
                    break;
                }
                else{
                    index = 2;
                }
            
            case 2 :
            
                code = ptrNoeud->code;
                if (cptBitMem == 8){
                    texteCompress[cptMot] = compress;
                    compress = 00000000;
                    cptMot++;
                    cptBitMem = 0;
                    debordement = 1;
                }
                
                else{
                    j = 8-cptBitMem;
                    compress +=(code>>(ptrNoeud->tailleCode-j));
                    texteCompress[cptMot] = compress;
                    compress = 00000000;
                    cptMot++;
                    compress = (code<<j);
                    cptBitMem = (ptrNoeud->tailleCode-j);
                    i++;
                    break;
                }
                
        }
    }
    
    if(compress != 00000000){
        compress = compress<<(8-cptBitMem);
        texteCompress[cptMot] = compress;
    }
    
    else{
        cptMot--;
    }
    
    for(uint8_t i = 0; i != cptMot+1; i++){
      printf("la case n° %d du tableau contien %X\r\n", i, texteCompress[i]);
    }
    
    return cptMot+1;
}

void creationEntete(uint8_t* entete, uint32_t nbrCaractereTotal, uint8_t* texteCompress, uint8_t tailleCompress, uint32_t nbrCaractereDifferent, struct noeud* racine, uint8_t texte[]){
    uint32_t index = 7;
    struct noeud* ptrNoeud;
    uint32_t tabCaractere[nbrCaractereDifferent];
    uint8_t flagCaractere = 0;
    uint8_t indexCaractere = 0;
    
    for(uint8_t i = 0; i<nbrCaractereDifferent; i++){
        tabCaractere[i] = 0;
    }
    
    entete[2] = tailleCompress;
    entete[3] = 0;
    
    entete[4] = (uint8_t) (nbrCaractereTotal & 0xFF);
    entete[5] = (uint8_t) (nbrCaractereTotal>>8 & 0xFF);
    entete[6] = (uint8_t) (nbrCaractereDifferent & 0xFF);
    
    for (uint8_t i = 0; i < tailleCompress; i++){
        entete[index] = texteCompress[i];
        index++;
    }
    index++;
    for (uint32_t i = 0; i < nbrCaractereTotal; i++){
        
        ptrNoeud = getAdress(racine, texte[i]);
        flagCaractere = 0;
        for (uint32_t j = 0; j < nbrCaractereDifferent; j++){
            
            if(ptrNoeud->c == tabCaractere[j]){
                
                flagCaractere = 1;
                break;
            }
        }
        if(flagCaractere == 0){
            
            entete[index++] = ptrNoeud->c;
            entete[index++] = (uint8_t)(ptrNoeud->tailleCode & 0xFF);
            entete[index++] = (uint8_t)(ptrNoeud->tailleCode >> 8) & 0xFF;

            uint32_t code = ptrNoeud->code;
            entete[index++] = (uint8_t)((code >> 0*8) & 0xFF);
            entete[index++] = (uint8_t)((code >> 1*8) & 0xFF);
            entete[index++] = (uint8_t)((code >> 2*8) & 0xFF);
            entete[index++] = (uint8_t)((code >> 3*8) & 0xFF);
            
            tabCaractere[indexCaractere] = ptrNoeud->c;
            indexCaractere+=1;
        }
    }
    
    entete[0] = (uint8_t)(index & 0xFF);
    entete[1] = (uint8_t)((index >> 8) & 0xFF);
    
    for(uint32_t i = 0; i < index; i++){
        
      printf("la case n° %d du tableau contien %X en hexa\r\n", i, entete[i]);
    }
}

void decompressEntete(uint8_t* entete, uint32_t nbrCaractereTotal, uint8_t* texteCompress, uint8_t tailleCompress, uint32_t nbrCaractereDifferent, struct noeud* arbreHuffman[256]){
    uint32_t tailleEntete = 0;
    struct noeud* ptrNoeud;
    uint32_t buffer = 0;
    uint16_t tailleCode = 0;
    uint32_t code = 0;
    uint8_t compteur = 0;
    uint32_t index = 0;
    
    tailleEntete = entete[index] + (entete[++index] << 8);
    
    tailleCompress = entete[++index];
    index++;
    
    nbrCaractereTotal = entete[++index] + (entete[++index] << 8);
    
    nbrCaractereDifferent = entete[++index];
    index++;
    
    while (index < tailleCompress+7){
        
        texteCompress[index-7] = entete[index];
        index++;
    }
    
    
    while (index < tailleEntete){
	    
			ptrNoeud = malloc (sizeof(struct noeud));
            ptrNoeud->c = entete[++index];
			ptrNoeud->occurrence = 0;

			tailleCode = entete[++index] + (entete[++index] << 8);

			ptrNoeud->tailleCode = tailleCode;

			code = entete[++index] + (entete[++index] << 8) + (entete[++index] << 16) + (entete[++index] << 24);

			ptrNoeud->code = code;
			
			ptrNoeud->droite = NULL;
			ptrNoeud->gauche = NULL;
      
			arbreHuffman[compteur] = ptrNoeud;
			compteur++;
    }
    afficherTabArbreHuffman(arbreHuffman,nbrCaractereDifferent);
}

struct noeud* getChar(struct noeud* ptrNoeud, uint32_t code, uint32_t tailleCode){
    struct noeud* result;
    
    if(ptrNoeud == NULL) return NULL;
    
    if(ptrNoeud->code == code && tailleCode == ptrNoeud->tailleCode){
        
        return ptrNoeud;
    }
    
    else if(ptrNoeud->droite != NULL && getChar(ptrNoeud->droite, code, tailleCode) != NULL){
        
        result = getChar(ptrNoeud->droite, code, tailleCode);
        if(result != NULL) return result;
    }
    
    else if(ptrNoeud->gauche != NULL && getChar(ptrNoeud->gauche, code, tailleCode) != NULL){
        
        result = getChar(ptrNoeud->gauche, code, tailleCode);
        if(result != NULL) return result;
    }
    
}

void reconstructionChaine(uint8_t* texteCompress, struct noeud* racine, uint8_t tailleCompress, uint32_t nbrCaractereTotal){
    uint8_t buffer[TAILLE_MAX_Compress*8] = {0};
    uint8_t index = 0;
    uint8_t code = 0;
    struct noeud* ptrNoeud;
    uint8_t chaine[nbrCaractereTotal];
    int32_t compteur = 7;
    int8_t byteIndex = -1; 
    uint32_t tailleCode = 0;
    
    for (uint16_t i = 0; i < tailleCompress * 8; i++) {
        
        if(i%8 == 0){
            
            byteIndex++;
        }
    
        uint8_t bit = (texteCompress[byteIndex] >> compteur) & 1;
        compteur--;
        if(compteur == -1){
            
            compteur = 7;
        }
    
        buffer[i] = bit;
    
        printf("buffer[%d] = %d\r\n", i, bit);
    }
    
    index = 0;
    compteur = 0;
    ptrNoeud = NULL;
    printf("--------------------------------------------------------------------------------------------------------\r\n");
    printf("---------------------------------Voici la chaine reconstruite-------------------------------------------\r\n");
    printf("--------------------------------------------------------------------------------------------------------\r\n");

    
    while(nbrCaractereTotal != 0 && index<tailleCompress*8){
        
        if(ptrNoeud != NULL && ptrNoeud->droite == NULL && ptrNoeud->gauche == NULL){
            
            printf("%c",ptrNoeud->c);
            compteur = 0;
            nbrCaractereTotal--;
            tailleCode = 0;
        }
        else{
            
            compteur = (compteur << 1) + buffer[index];
            index++;
            tailleCode++;
        }
        
        ptrNoeud = getChar(racine,compteur,tailleCode);
    }
    printf("\r\n--------------------------------------Fin du programme-------------------------------------------------\r\n");

}
