#ifndef MEMMANAGER_H
#define	MEMMANAGER_H
#include "Memory.h"

//Initialise les structures de données du gestionnaire.
void initmem(int memTotal, int strategy);

//Cette fonction alloue un nouveau bloc de mémoire.
int aloumem(int size);

//Cette fonction applique la strategie First-fit
int firstFit(int size);

//Cette fonction applique la strategie Best-fit
int bestFit(int size);

//Cette fonction applique la strategie Worst-fit
int worstFit(int size);

//Cette fonction applique la strategie Next-fit
int nextFit(int size);

//Cette fonction libère un bloc de mémoire similaire à free.
int liberemem(int index);

//Restructure la mémoire pour éviter d'avoir 2 blocs voisin de mémoire identique libre
int restructMem(MemBloc *prevBloc, MemBloc *currentBloc, MemBloc *nextBloc);

//Cette fonction libère toute la mémoire.
void freeMem(void);

//Cette fonction retourne le nombre de blocs de mémoire libres.
int nbloclibres(void);

//Cette fonction retourne le nombre de blocs alloués présentement            
int nblocalloues(void);

//Combien de mémoire libre (non allouée) au total.
int memlibre(void);

//Taille du plus grand bloc libre.
int mem_pgrand_libre(void);
        
//Combien de petits blocs (plus petit que maxTaillePetit) non alloués y a-t-il présentement en mémoire?
int mem_small_free(int maxSize);

//Est-ce qu’un octet (byte) particulier est alloué 
int mem_est_alloue(int byteNbr);

//Cette fonction affiche tout les blocs de mémoire présent
void displayMem(void);

#endif	/* MEMMANAGER_H */

