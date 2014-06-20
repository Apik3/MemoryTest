#include <stdio.h>
#include <stdlib.h>
#include "MemManager.h"

int test(void);

int main(int argc, char** argv) {
    
    if(argc == 1)
    {
        printf("Memory: 1024 bytes\n");
        printf("Strategy: First-fit\n");
        
        initmem(2048, 1);
		run(30);
        freeMem();
    }
    else if(argc == 2)
    {
		printf("Memory: 1024 bytes\n");

		if (atoi(argv[1]) == 1)
			printf("Strategy: First-fit\n");
		else if (atoi(argv[1]) == 2)
			printf("Strategy: Best-fit\n");
		else if (atoi(argv[1]) == 3)
			printf("Strategy: Worst-fit\n");
		else if (atoi(argv[1]) == 4)
			printf("Strategy: Next-fit\n");
		else {
			printf("Aucune stratégie correspond à ce chiffre,"
				" veuillez choisir entre 1 et 4\n");

			return (EXIT_FAILURE);
		}

		initmem(2048, atoi(argv[1]));
		run(30);
		freeMem();
    }
    else if(argc == 3)
    {
        printf("Memory: %d bytes\n", atoi(argv[2]));
        
        if(atoi(argv[1]) == 1)
            printf("Strategy: First-fit\n");
        else if(atoi(argv[1]) == 2)
            printf("Strategy: Best-fit\n");
        else if(atoi(argv[1]) == 3)
            printf("Strategy: Worst-fit\n");
        else if(atoi(argv[1]) == 4)
            printf("Strategy: Next-fit\n");
        else {
            printf("Aucune stratégie correspond à ce chiffre,"
                    " veuillez choisir entre 1 et 4\n");
            
            return (EXIT_FAILURE);
        }
        
        initmem(atoi(argv[2]), atoi(argv[1]));
		run(30);
        freeMem();
    }
	else if (argc == 4)
	{
		printf("Memory: %d bytes\n", atoi(argv[2]));

		if (atoi(argv[1]) == 1)
			printf("Strategy: First-fit\n");
		else if (atoi(argv[1]) == 2)
			printf("Strategy: Best-fit\n");
		else if (atoi(argv[1]) == 3)
			printf("Strategy: Worst-fit\n");
		else if (atoi(argv[1]) == 4)
			printf("Strategy: Next-fit\n");
		else {
			printf("Aucune strategie correspond à ce chiffre,"
				" veuillez choisir entre 1 et 4\n");

			return (EXIT_FAILURE);
		}

		initmem(atoi(argv[2]), atoi(argv[1]));
		run( atoi(argv[3]) );
		freeMem();
	}
    else
    {
        printf("\nil y a trop d'arguments de declare\n");
        printf("LOG710_LAB2 [strategie d'allocation] [taille de la memoire] [nombre de transactions]\n");
    }

	printf("[Appuyer sur une touche]");
	getch();
    return (EXIT_SUCCESS);
    
}

int run(int nbrTransaction)
{
	//crée un seed pour les nombres aléatoires mis en mémoire
	time_t t;
	srand((unsigned) time(&t));

	for (int i = 0; i < nbrTransaction; i++) 
	{
		if ((rand() % 4) < 3)
			aloumem( (rand() % 512) + 1);
		else
		{
			int nBloc = nblocalloues() + nbloclibres();
			liberemem( (rand() % nBloc) + 1);
		}

		displayMem();
	}
    
    return 0;
}
