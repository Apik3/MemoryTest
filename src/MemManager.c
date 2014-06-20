#include <stdio.h>
#include <stdlib.h>
#include "MemManager.h"
#include <windows.h>

Memory *memory = NULL;
//const int true = 1;
//const int true = 0;
int indexNext = 1;

//Initialise les structures de données du gestionnaire.
void initmem(int memTotal, int strategy)
{
    memory=malloc(sizeof(*memory));
    memory->totalMemory = memTotal;
    memory->strategy = strategy;
    
    MemBloc *memBloc = malloc(sizeof(*memBloc));
    memBloc->memorySize = memTotal;
    memBloc->occupy = FALSE;
    memBloc->previous = NULL;
    memBloc->next = NULL;
    
    memory->first = memBloc;
}

//Cette fonction libère un bloc de mémoire similaire à free.
int liberemem(int indexBloc)
{
	if ((nblocalloues() + nbloclibres()) < indexBloc)
	{
		printf("**L'index du bloc à retirer n'existe pas\n");
		return FALSE;
	}
    
    MemBloc *actualMemBloc = memory->first;
    int i = 1;
    int isFinish = FALSE;
    
    while(actualMemBloc != NULL && isFinish == FALSE)
    {
        if(i == indexBloc)
        {
            MemBloc *prevBloc = actualMemBloc->previous;
            MemBloc *nextBloc = actualMemBloc->next;

            if(actualMemBloc->occupy == FALSE)
                isFinish = TRUE;
            else
            {
                actualMemBloc->occupy = FALSE;
                isFinish = restructMem(prevBloc, actualMemBloc, nextBloc);
            }
            break;
        }
        
        actualMemBloc = actualMemBloc->next;
        i++;
    }
    
    if(memory->strategy == 4)
    {
        if(indexNext > (nbloclibres()+nblocalloues()) )
            indexNext = (nbloclibres()+nblocalloues());
    }
    
    if(isFinish)
        printf("%de bloc liberer\n", indexBloc);
    
    return isFinish;
}

//Restructure la mémoire pour éviter d'avoir 2 blocs voisin de mémoire identique libre
int restructMem(MemBloc *prevBloc, MemBloc *currentBloc, MemBloc *nextBloc)
{
    int isFinish = FALSE;
    
    while(!isFinish)
    {
        if(currentBloc->occupy == FALSE && prevBloc!= NULL && prevBloc->occupy == FALSE)
        {
            prevBloc->memorySize += currentBloc->memorySize;
            free(currentBloc);
            
            prevBloc->next = nextBloc;
            if(nextBloc!= NULL)
                nextBloc->previous = prevBloc;
            currentBloc = prevBloc;
            prevBloc = prevBloc->previous;
        }
        else if(currentBloc->occupy == FALSE && nextBloc!= NULL && nextBloc->occupy == FALSE)
        {
            nextBloc->memorySize += currentBloc->memorySize;
            free(currentBloc);
            
            nextBloc->previous = prevBloc;
            if(prevBloc!= NULL)
                prevBloc->next = nextBloc;
            else
                memory->first = nextBloc;
            currentBloc = nextBloc;
            nextBloc = nextBloc->next;
        }
        else
            isFinish = TRUE;
    }
    
    return TRUE;
}

//Cette fonction libère toute la mémoire.
void freeMem(void)
{
    MemBloc *actualMemBloc = memory->first;
    
    while(actualMemBloc != NULL)
    {
        MemBloc *bloc = actualMemBloc;
        actualMemBloc = actualMemBloc->next;
        free(bloc);
    }
    
    free(memory);
}

//Cette fonction alloue un nouveau bloc de mémoire.
int aloumem(int size)
{
    if(memory->totalMemory < size)
    {
        printf("**La taille du bloc est supérieure a la taille totale de la memoire\n");
        return FALSE;
    }
    else if(nbloclibres() == 0)
    {
        printf("**Il ne reste plus de bloc libres dans la memoire\n");
        return FALSE;
    }
    else if(memlibre() < size)
    {
        printf("**La taille du bloc est superieure a la taille totale de la memoire libre\n");
        return FALSE;
    }
    else if(mem_pgrand_libre() < size)
    {
        printf("**La taille du bloc ne rentre pas dans les blocs libres\n");
        return FALSE;
    }
    else if(size == 0)
    {
        printf("**Impossible de stocker 0Bytes de memoire\n");
        return FALSE;
    }
    else if(size < 0)
    {
        printf("**Depuis quand que la memoire negative existe??\n");
        return FALSE;
    }
    
    int isFinish = FALSE;
    if(memory->strategy == 1)
        isFinish = firstFit(size);
    else if(memory->strategy == 2)
        isFinish = bestFit(size);
    else if(memory->strategy == 3)
        isFinish = worstFit(size);
    else if(memory->strategy == 4)
        isFinish = nextFit(size);
    else
        printf("!!error!!");
    
    if(isFinish)
        printf("%dbytes alloues\n", size);
    
    return isFinish;
}

//Cette fonction applique la strategie First-fit
int firstFit(int size)
{
    MemBloc *actualMemBloc = memory->first;
    int isFinish = FALSE;
    
    while(actualMemBloc != NULL && isFinish == FALSE)
    {
        if(actualMemBloc->occupy == FALSE)
        {
            if(actualMemBloc->memorySize > size)
            {
                actualMemBloc->memorySize -= size;

                MemBloc *memBloc = malloc(sizeof(*memBloc));
                memBloc->occupy = FALSE;
                memBloc->memorySize = actualMemBloc->memorySize;
                memBloc->previous = actualMemBloc;
                memBloc->next = actualMemBloc->next;
                
                if(actualMemBloc->next != NULL)
                    actualMemBloc->next->previous = memBloc;

                actualMemBloc->next = memBloc;
                actualMemBloc->occupy = TRUE;
                actualMemBloc->memorySize = size;
                
                isFinish = TRUE;
            }
            else if(actualMemBloc->memorySize == size)
            {
                actualMemBloc->occupy = TRUE;
                isFinish = TRUE;
            }
        }
        
        actualMemBloc = actualMemBloc->next;
    }
    
    return isFinish;
}

//Cette fonction applique la strategie Best-fit
int bestFit(int size)
{
    MemBloc *actualMemBloc = memory->first;
    int isFinish = FALSE;
    int bestFit = -1;
    int index = -1;
    int i = 0;
    
    while(actualMemBloc != NULL && isFinish == FALSE)
    {
        if(actualMemBloc->occupy == FALSE)
        {
            if(actualMemBloc->memorySize == size)
            {
                actualMemBloc->occupy = TRUE;
                isFinish = TRUE;
            }
            else if(actualMemBloc->memorySize > size)
            {
                if(actualMemBloc->memorySize-size < bestFit || bestFit == -1)
                {
                    index = i;
                    bestFit = actualMemBloc->memorySize-size;
                }
            }
        }
        
        actualMemBloc = actualMemBloc->next;
        i++;
    }
    
    
    if(isFinish == FALSE)
    {
        i=0;
        MemBloc *actualMemBloc = memory->first;
        while(actualMemBloc != NULL && isFinish == FALSE)
        {
            if(i == index)
            {
                actualMemBloc->memorySize -= size;

                MemBloc *memBloc = malloc(sizeof(*memBloc));
                memBloc->occupy = FALSE;
                memBloc->memorySize = actualMemBloc->memorySize;
                memBloc->previous = actualMemBloc;
                memBloc->next = actualMemBloc->next;
                
                if(actualMemBloc->next != NULL)
                    actualMemBloc->next->previous = memBloc;

                actualMemBloc->next = memBloc;
                actualMemBloc->occupy = TRUE;
                actualMemBloc->memorySize = size;

                isFinish = TRUE; 
            }
            
            actualMemBloc = actualMemBloc->next;
            i++;
        }
    }
    
    return isFinish;
}

//Cette fonction applique la strategie Worst-fit
int worstFit(int size)
{
    MemBloc *actualMemBloc = memory->first;
    int isFinish = FALSE;
    int worstFit = mem_pgrand_libre();
    
    while(actualMemBloc != NULL && isFinish == FALSE)
    {
        if(actualMemBloc->occupy == FALSE)
        {
            if(actualMemBloc->memorySize == worstFit && worstFit != size)
            {
                actualMemBloc->memorySize -= size;

                MemBloc *memBloc = malloc(sizeof(*memBloc));
                memBloc->occupy = FALSE;
                memBloc->memorySize = actualMemBloc->memorySize;
                memBloc->previous = actualMemBloc;
                memBloc->next = actualMemBloc->next;
                
                if(actualMemBloc->next != NULL)
                    actualMemBloc->next->previous = memBloc;

                actualMemBloc->next = memBloc;
                actualMemBloc->occupy = TRUE;
                actualMemBloc->memorySize = size;

                isFinish = TRUE; 
            }
            else if(actualMemBloc->memorySize == worstFit && worstFit == size)
            {
                actualMemBloc->occupy = TRUE;
                isFinish = TRUE;
            }
        }
        
        actualMemBloc = actualMemBloc->next;
    }
    
    return isFinish;
}

//Cette fonction applique la strategie Next-fit
int nextFit(int size)
{
    
    MemBloc *actualMemBloc = memory->first;
    int i = 1;
    int isFinish = FALSE;
    
    if(indexNext != 1)
    {
        while(actualMemBloc != NULL)
        {
            if(i == indexNext)
            {
                break;
            }

            actualMemBloc = actualMemBloc->next;
            i++;
        }
    }
        
    do
    {
        if(actualMemBloc->occupy == FALSE)
        {
            if(actualMemBloc->memorySize > size)
            {
                actualMemBloc->memorySize -= size;

                MemBloc *memBloc = malloc(sizeof(*memBloc));
                memBloc->occupy = FALSE;
                memBloc->memorySize = actualMemBloc->memorySize;
                memBloc->previous = actualMemBloc;
                memBloc->next = actualMemBloc->next;
                
                if(actualMemBloc->next != NULL)
                    actualMemBloc->next->previous = memBloc;
                
                actualMemBloc->next = memBloc;
                actualMemBloc->occupy = TRUE;
                actualMemBloc->memorySize = size;
                
                isFinish = TRUE;
            }
            else if(actualMemBloc->memorySize == size)
            {
                actualMemBloc->occupy = TRUE;
                isFinish = TRUE;
            }
        }
        
        actualMemBloc = actualMemBloc->next;
        if(actualMemBloc == NULL)
        {
            actualMemBloc = memory->first;
            i = 1;
        }
        else
            i++;
    }while(i != indexNext && isFinish == FALSE);
    
    indexNext = i;
   
    return isFinish;
}

//Combien de petits blocs (plus petit que maxTaillePetit) non alloués y a-t-il présentement en mémoire?
int mem_small_free(int maxSize)
{
    MemBloc *actualMemBloc = memory->first;
    int count = 0;
    
    while(actualMemBloc != NULL)
    {
        if(actualMemBloc->memorySize < maxSize)
        {
            if(actualMemBloc->occupy == FALSE)
                count++;
        }
        
        actualMemBloc = actualMemBloc->next;
    }
    
    return count;
}

//Cette fonction retourne le nombre de blocs de mémoire libres.
int nbloclibres(void)
{
    MemBloc *actualMemBloc = memory->first;
    int count = 0;
    
    while(actualMemBloc != NULL)
    {
        if(actualMemBloc->occupy == FALSE)
            count++;
        
        actualMemBloc = actualMemBloc->next;
    }
    
    return count;
}

//Cette fonction retourne le nombre de blocs alloués présentement            
int nblocalloues(void)
{
    MemBloc *actualMemBloc = memory->first;
    int count = 0;
    
    while(actualMemBloc != NULL)
    {
        if(actualMemBloc->occupy == TRUE)
            count++;
        
        actualMemBloc = actualMemBloc->next;
    }
    
    return count;
}

//Combien de mémoire libre (non allouée) au total.
int memlibre(void)
{
    MemBloc *actualMemBloc = memory->first;
    int count = 0;
    
    while(actualMemBloc != NULL)
    {
        if(actualMemBloc->occupy == FALSE)
            count += actualMemBloc->memorySize;
        
        actualMemBloc = actualMemBloc->next;
    }
    
    return count;
}

//Taille du plus grand bloc libre.
int mem_pgrand_libre(void)
{
    MemBloc *actualMemBloc = memory->first;
    int maxBloc = 0;
    
    while(actualMemBloc != NULL)
    {
        if(actualMemBloc->memorySize > maxBloc)
            maxBloc = actualMemBloc->memorySize;
        
        actualMemBloc = actualMemBloc->next;
    }
    
    return maxBloc;
}

//Est-ce qu’un octet (byte) particulier est alloué 
int mem_est_alloue(int byteNbr)
{
    MemBloc *actualMemBloc = memory->first;
    int count = 0;
    
    while(actualMemBloc != NULL)
    {
        count += actualMemBloc->memorySize;
        //printf("%d\n", count);
        
        if(byteNbr <= count)
        {
            if(actualMemBloc->occupy == TRUE)
                return TRUE;
            
            return FALSE;
        }
        
        actualMemBloc = actualMemBloc->next;
    }
    
    return FALSE;
}

void displayMem(void)
{
    MemBloc *actualMemBloc = memory->first;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	while (actualMemBloc != NULL)
	{
		//écrit en rouge pour signifier qu'il est occupé
		if (actualMemBloc->occupy == TRUE)
		{
			SetConsoleTextAttribute(hConsole, 4);
			printf("%d/", actualMemBloc->memorySize);
		}
        //écrit en vert pour signifier qu'il est libre
		else
		{
			SetConsoleTextAttribute(hConsole, 2);
			printf("%d/", actualMemBloc->memorySize);
		}
        
        actualMemBloc = actualMemBloc->next;
    }
	SetConsoleTextAttribute(hConsole, 15);
    printf("\n\n");
}
