#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
 
struct player {
    int hp;
    int shield;
    int pw;

};

// returning 0 means player exits game, returning 1 means wants to restart game, returning 2 means player advances to next stage
void preGame();
int firstBttl(struct player*, int*, int*, int*);
int secondBttl(struct player*, int*, int*, int*);
int thirdBttl(struct player*, int*, int* , int*); 
int critChance(int*, int*);

int playAgain(){
    while (getchar() != '\n');
    while (1){
        char usrInput[5], *c = usrInput;
        printf("\nYou died! Play Again?\n [1] Yes  [2]No\nInput: ");
        fgets(usrInput, sizeof(usrInput), stdin);
        if (strchr(usrInput, '\n') == NULL){
            printf("Invalid input. Please enter 1 or 2 or type yes or no!\n");
            while (getchar() != '\n');
            continue;
        }
        while (*c != '\n'){
      	    *c = tolower(*c);
	        c++;
        }

        if (strcmp(usrInput, "yes\n") == 0 || strcmp(usrInput, "1\n") == 0 || strcmp(usrInput, "1\n  ")==0 || strcmp(usrInput, " 1\n ") == 0 
        || strcmp(usrInput, "  1\n") == 0 || strcmp(usrInput, "1 \n") == 0 || strcmp(usrInput, "1  \n") == 0){
            return 1;
        }
        if (strcmp(usrInput, "no\n") == 0 || strcmp(usrInput, " no\n") == 0 || strcmp(usrInput, "no\n ") == 0 || strcmp(usrInput, "no \n") == 0 
        || strcmp(usrInput, "2\n") == 0 || strcmp(usrInput, "  2\n") == 0 || strcmp(usrInput, "2\n  ") == 0 || strcmp(usrInput, "2 \n") == 0 || strcmp(usrInput, "2  \n") == 0){
            break;
        }
     
        printf("Invalid input. Please enter 1 or 2 or type yes or no!");
    }

    return 0;
}


int main(){
    preGame();

    char name[100];
    printf("Enter your username: ");
    fgets(name, sizeof(name), stdin);
    int inputLen = strlen(name);
    if (inputLen > 0 && name[inputLen - 1] == '\n'){
        name[inputLen - 1] = '\0';
    }
    printf("\nGood Luck on your journey %s!\n\n", name);
    sleep(1);

    while (1){ 
        struct player user = {100, 0, 3};
        struct player *playerPtr = &user; 
	int atkDmg = 10, pwDmg = 30, critCount = 0;
	int *atkPtr = &atkDmg, *pwDmgPtr = &pwDmg, *critPtr = &critCount; 

        int firstPhase = firstBttl(playerPtr, atkPtr, pwDmgPtr, critPtr);
        if (!firstPhase){
            break;
        }
        if (firstPhase == 1){
            continue;
        }
        int secondPhase = secondBttl(playerPtr, atkPtr, pwDmgPtr, critPtr);
        if (!secondPhase){
            break;
        }
        if (secondPhase == 1){
            continue;
        }

	    int finalPhase = thirdBttl(playerPtr, atkPtr, pwDmgPtr, critPtr);
        if (!finalPhase){
	        break;
	    }
	    if (finalPhase == 1){
	        continue;
	    }
	    printf("Conratulations %s! You defeated all the monsters and escaped the dungeon! Well done!", name);
        break;
    }
    
    printf("\nThank you for playing!!\n\n");
    return 0;
}

void preGame(){
    printf("\n\n*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n| Welcome To The Cursed Dungeon!! |\n*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n");
    printf("\nMonsters lurk these halls and devour whoever they encounter.\nYou are trapped and must defeat all the monsters to escape.\nAre you able to conquer the monsters\n");
    printf("Or will you like many others before you fall victim to these treacherous creatures\nThat power rests in your hands\n");
    printf("\nHow To Play:\nThere are 3 monsters in the dungeon; 2 regular monsters and 1 boss monster.\nInput 1 for basic attacks and 2 for power attacks. Basic attacks do 10 damage points and power attacks do 30 damage points.\n");
    printf("During each attack there is a 30%% chance it becomes a critical hit in which case the attack does double damage points.\nIf you get more than one critical hit in a single playthrough your basic attack increases by 10 damage points for each critical hit.\n");
    printf("The monsters only do 10 damage points. After defeating each monster you gain 100 shield points.\n\n\n");
}

int firstBttl(struct player *pStats, int *atkDmg, int *pwDmg, int *critCount) {
    int input, villainHp = 50, firstMsgDisplay = 1;
    char firstMsg[42] = "HUH, YOU THINK YOU CAN BEAT ME? BRING IT!", deathMsg[43] = "WAIT I WAS BESTED?! NO, THIS CAN'T BE!!!";

    printf("\nThe First Monster Approaches\n");
    sleep(1);

    while (1){
        sleep(1);
	    printf("\nVillain Hp: %d\n", villainHp);
	    printf("\t\t  -/\\--------/\\-\n");
        printf("\t\t  |    \\  /    |\n");
        printf("\t\t  |   -    -   |\n");
        printf("\t\t  |  --------  |\n");  
        printf("\t\t  --------------\n");
     	
	    if (firstMsgDisplay > 0){
            printf("  ---");
	        for (int i = 0; i < strlen(firstMsg); i++){
	            printf("-");
	        }
	        printf("-----\n  |   %s   |\n  ---", firstMsg);
	        for (int i = 0; i < strlen(firstMsg); i++){ 
                printf("-");
            }
	        printf("-----\n");
	        firstMsgDisplay--;
	    }
	
	    if ((pStats -> hp) <= 0){
            break;
        }

        sleep(1);
        printf("\nHealth: %d \t Shield: %d \t Power Attacks: %d\n", pStats -> hp, pStats -> shield, pStats -> pw);
        printf("[1] Basic Attack \t [2] Power Attack\n\nChoose your attack: ");
        if (scanf("%d", &input) != 1){
            printf("\nInvalid Input. Please choose either only 1 or 2.\n");
            while (getchar() != '\n');
            continue;
        }
        
        switch(input){
            case 1:
            if (critChance(critCount, atkDmg)){
                villainHp -= ((*atkDmg) * 2);
                printf("\nYou got a critical hit! You do %d damage to the enemy!\n\n", *atkDmg *2); 
                break; 
           
            }

            villainHp -= (*atkDmg);
	        printf("\nYou did %d damage to the enemy!\n\n", *atkDmg);
       	    break;

	    case 2:
            if (pStats -> pw > 0){
                villainHp -= (*pwDmg);
		(pStats -> pw)--;
                printf("\nYou did %d damage to the enemy!\n\n", *pwDmg);
	        break;
		}
	    else{
		    printf("\nYou are out of Power Attacks!\n");
            continue;	
		}
 	    default:
            printf("\nYou can only do a Basic Attack or Power Attack!\n\n");
            continue;
        }
        sleep(1);

        if (villainHp <= 0){
	        printf("\t\t  -/\\--------/\\-\n");
            printf("\t\t  |    \\  /    |\n");
            printf("\t\t  |  \\/    \\/  |\n");
            printf("\t\t  |  /\\    /\\  |\n");
            printf("\t\t  |  --------  |\n");
            printf("\t\t  --------------\n");
            printf("   ---");

            for (int i = 0; i < strlen(deathMsg); i++){
                printf("-");
            }
            printf("-----\n   |   %s   |\n   ---", deathMsg);
            for (int i = 0; i < strlen(deathMsg); i++){
                printf("-");
            }
            printf("-----\n");
            printf("\nYou defeated the monster! You gained 100 shield points!\n\n");
            (pStats->shield) += 100;
	        sleep(2);
            return 2;
        }
        printf("The monster does 10 damage to you!\n");
        (pStats -> hp) -= 10;

    }
    return playAgain();
}


int secondBttl(struct player *pStats, int *atkDmg, int *pwDmg, int *critCount){
    int input,n = 0, villainHp = 120, firstMsgDisplay = 1;
    char firstMsg[52] = "HAHA!! WHAT PUNY HUMAN THINKS HE CAN CHALLENGE ME?!", deathMsg[44] = "IMPOSSIBLE!! HOW COULD THIS HAPPEN TO ME!!!";


    printf("\nThe Second Monster Approaches\n");
    sleep(1);

    while (1){
        sleep(1);
        printf("\nVillainHp: %d\n", villainHp);
        printf("\t\t        ________________\n");
        printf("\t\t       /                \\\n");
        printf("\t\t       \\      \\  /      /\n");
        printf("\t\t        |   {}    {}   |\n");
        printf("\t\t       /    {}    {}    \\\n");
        printf("\t\t       \\   ----------   /\n");
        printf("\t\t       |  /          \\  |\n");
        printf("\t\t       |  \\          /  |\n");
        printf("\t\t       |   ----------   |\n");
        printf("\t\t       / ___  ___  ____ \\\n");
        printf("\t\t      / /  / /   \\ \\   \\ \\\n");
        printf("\t\t     / /  / /     \\ \\   \\ \\\n");
        printf("\t\t     \\/  / /       \\ \\   \\/\n");
        printf("\t\t        / /         \\ \\\n");
        printf("\t\t        \\/           \\/\n");
        
        if (firstMsgDisplay > 0){
   	    printf("    ---");
	    for (int i = 0; i < strlen(firstMsg); i++){
                printf("-");
            }
            printf("-----\n    |   %s   |\n    ---", firstMsg);
            for (int i = 0; i < strlen(firstMsg); i++){
                printf("-");
            }
            printf("-----\n");
            firstMsgDisplay--;
        }

        if ((pStats -> hp) <= 0){
            break;
        }
        printf("\nHealth: %d \t Shield: %d \t Power Attacks: %d\n", pStats -> hp, pStats -> shield, pStats -> pw);
        printf("[1] Basic Attack \t [2] Power Attack\n\nChoose your attack: ");      

        if (scanf("%d", &input) != 1){
            printf("\nInvalid Input. Please choose either only 1 or 2.\n");
            while (getchar() != '\n');
            continue;   
        }

        switch (input){
            case 1:
	        if (critChance(critCount, atkDmg)){
		    villainHp -= *atkDmg * 2;
		    printf("\nYou got a critical hit! You do %d damage to the enemy!\n\n", *atkDmg *2);
		    break;
		}
                villainHp -= *atkDmg;
                printf("\nYou did %d damage to the enemy!\n", *atkDmg);
		        break;
            case 2:
                if (pStats -> pw > 0){
                    villainHp -= *pwDmg;
                    (pStats -> pw)--;
                    printf("\nYou did %d damage to the enemy!\n", *pwDmg);
                    break;
                    
                }
                else{
                    printf("\nYou are out of Power Attacks!\n");
                    continue;		    
                }
            default:
                printf("\nYou can only do Basic Attacks or Power Attacks!\n");
       		    continue;
        }
       sleep(1);

       if (villainHp <= 0){
            printf("\t\t       ________________\n");
            printf("\t\t      /                \\\n");
            printf("\t\t      \\      \\  /      /\n");
            printf("\t\t       |   \\/    \\/   |\n");
            printf("\t\t      /    /\\    /\\    \\\n");
            printf("\t\t      \\   ----------   /\n");
            printf("\t\t      |  /          \\  |\n");
            printf("\t\t      |  \\          /  |\n");
            printf("\t\t      |   ----------   |\n");
            printf("\t\t      / ___  ___  ____ \\\n");
            printf("\t\t     / /  / /   \\ \\   \\ \\\n");
            printf("\t\t    / /  / /     \\ \\   \\ \\\n");
            printf("\t\t    \\/  / /       \\ \\   \\/\n");
            printf("\t\t       / /         \\ \\\n");
            printf("\t\t       \\/           \\/\n");
            printf("         ---");
            for (int i = 0; i < strlen(deathMsg); i++){
  	    	printf("-");
            }
            printf("-----\n         |   %s   |\n         ---", deathMsg);
            for (int i = 0; i < strlen(deathMsg); i++){
    	    	printf("-");
            }
            printf("-----\n");
            printf("\nYou beat the monster! You gained 100 shield points!\n");
            (pStats -> shield) += 100;
	    sleep(2);
            return 2;        
        }
       
       if (pStats->shield > 0){
           (pStats -> shield) -= 10;
        }
        else{
           (pStats -> hp) -= 10;
        }
        printf("\nThe monster does 10 damage to you!\n");
   }
           
   return playAgain();
}

int thirdBttl(struct player *pStats, int *atkDmg, int *pwDmg, int *critCount){
    int input, villainHp = 200, firstMsgDisplay = 1;
    char firstMsg[63] = "OOOOHH HEHEEEE!! HOW DELICIOUS, MORE FLESH FOR ME TO CONSUME!!", deathMsg[50] = "NOOOOOO!! MY FOOD!! YOU WOULD'VE BEEN SO TASTY!!!";
                
    printf("\nThe Final Monster Approaches\n");
    sleep(1);   
           
    while (1){
        sleep(1);
        printf("\nVillainHp: %d\n", villainHp);
        printf("\t                     ____________________________\n");
        printf("\t  /\\                /                            \\                /\\\n");
        printf("\t  ||                \\      _____/     \\_____     /                ||\n");
        printf("\t  | ************     \\                          /     ************ |\n");
        printf("\t  ************ |      \\      ???       ???     /      | ************\n");
        printf("\t             | |      /      ???       ???     \\      | |\n");
        printf("\t             | *******                          ******* |\n");
        printf("\t             *********    ___________________  *********\n");
        printf("\t                      \\   \\                 /  /\n");
        printf("\t                      /    \\               /   \\\n");
        printf("\t                     /      \\_____      __/     \\\n");
        printf("\t                    /             \\    |         \\\n");
        printf("\t                   /               |   |          \\\n");
        printf("\t                   \\_______________|   |__________/\n");
        printf("\t                                   | + |\n");
        printf("\t                                   | + |\n");
        printf("\t                                   | + |\n");
        printf("\t                                   \\___/\n");
        
        if (firstMsgDisplay > 0){
	        printf("        ---");
                for (int i = 0; i < strlen(firstMsg); i++){
          	      printf("-");
        	}
           	printf("-----\n        |   %s   |\n        ---", firstMsg);
            	for (int i = 0; i < strlen(firstMsg); i++){
                	printf("-");
            	}
            	printf("-----\n");
            	firstMsgDisplay--;
        }

        if ((pStats -> hp) <= 0){
            break;
        }

        printf("\nHealth: %d \t Shield: %d \t Power Attacks: %d\n", pStats->hp, pStats->shield, pStats->pw);
        printf("[1] Basic Attack \t [2] Power Attack\n\nChoose your attack: ");
       
        if (scanf("%d", &input) != 1){
           printf("\nInvalid Input. Please choose either only 1 or 2.\n");
           while (getchar() != '\n');
           continue;
       }

        switch (input){
            case 1:
	 	if (critChance(critCount, atkDmg)){
		    villainHp -= *atkDmg *2;
		    printf("\nYou got a critical hit! You do %d damage to the enemy!\n\n", *atkDmg *2);
		    break;
		}
                villainHp -= *atkDmg;
                printf("\nYou did %d damage to the enemy!\n", *atkDmg);
                break;
            case 2:
                if ((pStats -> pw) > 0){
                    villainHp -= *pwDmg;
                    (pStats->pw)--;
                    printf("\nYou did %d damage to the enemy!\n", *pwDmg);
                    break;
        
                }
                else{  
                    printf("\nYou are out of Power Attacks!\n");
                    continue;
                }
           default:  
                printf("\nYou can only do Basic Attacks or Power Attacks!\n");
                continue;
        }
        sleep(1);
   
        if (villainHp <= 0){
           printf("\t                     ____________________________\n");
           printf("\t  /\\                /                            \\                /\\\n");
           printf("\t  ||                \\      _____/     \\_____     /                ||\n");
           printf("\t  | ************     \\                          /     ************ |\n");
           printf("\t  ************ |      \\      \\/        \\/      /      | ************\n");
           printf("\t             | |      /      /\\        /\\      \\      | |\n");
           printf("\t             | *******                          ******* |\n");
           printf("\t             *********    ___________________  *********\n");
           printf("\t                      \\   \\                 /  /\n");
           printf("\t                      /    \\               /   \\\n");
           printf("\t                     /      \\_____      __/     \\\n");
           printf("\t                    /             \\    |         \\\n");
           printf("\t                   /               |   |          \\\n");
           printf("\t                   \\_______________|   |__________/\n");
           printf("\t                                   | + |\n");
           printf("\t                                   | + |\n");
           printf("\t                                   | + |\n");
           printf("\t                                   \\___/\n");
           printf("              ---");
           for (int i = 0; i < strlen(deathMsg); i++){
               printf("-");
           }
           printf("-----\n              |   %s   |\n              ---", deathMsg);
           for (int i = 0; i < strlen(deathMsg); i++){
               printf("-");
           }
           printf("-----\n");
           printf("\n\nYou vanquished the monster!\n\n");
           sleep(1);
           return 2;
        }            
                 
        if ((pStats->shield) > 0){
           pStats->shield -= 10;
        }
        else{
           pStats->hp -= 10;
        }
        printf("\nThe monster does 10 damage to you!\n");
    }
  
    return playAgain();

}

int critChance(int *critCount, int *atkDmg){
    srand(time(NULL));
    int critNum = (rand() % 10) + 1;

    if (critNum > 7){
	(*critCount)++;
        if (*critCount > 1){
	    *atkDmg += 10;
	}
        return 1;
    }
    return 0;
  
}
