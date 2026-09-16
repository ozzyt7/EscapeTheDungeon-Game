#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <ctype.h>


// returning 0 means player exits game, returning 1 means wants to restart game, returning 2 means player defeated monster and continues to next stage
void preGame();
int firstBttl(int*, int*, int*);
int secondBttl(int*, int*, int*);
int thirdBttl(int*, int*, int*); 

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
    char name[100];

    preGame();
    printf("Enter your username: ");
    fgets(name, sizeof(name), stdin);
    int inputLen = strlen(name);
    if (inputLen > 0 && name[inputLen - 1] == '\n'){
        name[inputLen - 1] = '\0';
    }
    printf("\nGood Luck on your journey %s!\n", name);
    sleep(1);

    while (1){ 
        int hp = 100, shield = 0, pw = 3;
        int *ptrPw = &pw, *ptrHp = &hp, *ptrShield = &shield;

        int firstPhase = firstBttl(ptrHp, ptrPw, ptrShield);
        if (!firstPhase){
            break;
        }
        if (firstPhase == 1){
            continue;
        }
        int secondPhase = secondBttl(ptrHp, ptrPw, ptrShield);
        if (!secondPhase){
            break;
        }
        if (secondPhase == 1){
            continue;
        }

	    int finalPhase = thirdBttl(ptrHp, ptrPw, ptrShield);
        if (!finalPhase){
	        break;
	    }
	    if (finalPhase == 1){
	        continue;
	    }
	    printf("Conratulations %s! You defeated all the monsters and escaped the dungeon! Well done!", name);
        break;
    }

    printf("\nThank you for playing!\n\n");

}

void preGame(){
    printf("\nWelcome to The Cursed Dungeon!\nMonsters lurk these halls and devour whoever they encounter.\nYou are trapped and must defeat all the monsters to escape.\n");
    printf("There are 2 regular monsters and 1 boss monster.\nYou start with 100 health points and 3 power attacks.\n");
    printf("Basic attacks do 10 damage points and power attacks do 30 damage points.\nThe monsters only do 10 damage points.\nAfter defeating each monster you gain 100 shield points.\n\n");

}

int firstBttl(int *hp, int *pw, int *shield) {
    int input, taunt = 0, villainHp = 50, firstMsgDisplay = 1;
    char firstMsg[50] = "HUH, YOU THINK YOU CAN BEAT ME? BRING IT!", deathMsg[50] = "WAIT I WAS BESTED?! NO, THIS CAN'T BE!!!";

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
	
	    if (*hp <= 0){
            break;
        }

        sleep(1);
        printf("\nHealth: %d \t Shield: %d \t Power Attacks: %d\n", *hp,*shield, *pw);
        printf("[1] Basic Attack \t [2] Power Attack\n\nChoose your attack: ");
        if (scanf("%d", &input) != 1){
            printf("\nInvalid Input. Please choose either only 1 or 2.\n");
            while (getchar() != '\n');
            continue;
        }
    
        if (input == 1){
            villainHp -= 10;
            printf("\nYou did 10 damage to the enemy!\n\n");
        }
        else if (input == 2 && *pw > 0){
            villainHp -= 30;
            (*pw)--;
            printf("\nYou did 30 damage to the enemy!\n\n");
        }
        else if (input == 2 && *pw == 0){
            printf("\nYou are out of Power Attacks!\n");
            taunt++;
            continue;
        }
        else{
            printf("\nYou can only do a Basic Attack or Power Attack!\n\n");
            taunt++; 
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
            *shield += 100;
	        sleep(2);
            return 2;
        }
        printf("The monster does 10 damage to you!\n");
        *hp -= 10;

    }
    return playAgain();
}


int secondBttl(int *hp, int *pw, int *shield){
    int input,n = 0, villainHp = 120, firstMsgDisplay = 1;
    char firstMsg[70] = "HAHA!! WHAT PUNY HUMAN THINKS HE CAN CHALLENGE ME?!", deathMsg[50] = "WHAT?! HOW COULD THIS HAPPEN TO ME!!!";


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

        if (*hp <= 0){
            break;
        }
        printf("\nHealth: %d \t Shield: %d \t Power Attacks: %d\n", *hp,*shield, *pw);
        printf("[1] Basic Attack \t [2] Power Attack\n\nChoose your attack: ");      
 
        if (scanf("%d", &input) != 1){
            printf("\nInvalid Input. Please choose either only 1 or 2.\n");
            while (getchar() != '\n');
            continue;   
        }

        switch (input){
            case 1:
                villainHp -= 10;
                printf("\nYou did 10 damage to the enemy!\n");
		        break;
            case 2:
                if (*pw > 0){
                    villainHp -= 30;
                    (*pw)--;
                    printf("\nYou did 30 damage to the enemy!\n");
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
            *shield += 100;
	    sleep(2);
            return 2;        
        }
       
       if (*shield > 0){
           *shield -= 10;
        }
        else{
           *hp -= 10;
        }
        printf("\nThe monster does 10 damage to you!\n");
   }
           
   return playAgain();
}

int thirdBttl(int *hp, int *pw, int *shield){
    int input, villainHp = 200, firstMsgDisplay = 1;
    char firstMsg[70] = "OOOOHH HEHEEEE!! HOW DELICIOUS, MORE FLESH FOR ME TO CONSUME!!", deathMsg[50] = "NOOOOOO!! MY FOOD!! YOU WOULD'VE BEEN SO TASTY!!!";
                
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

        if (*hp <= 0){
            break;
        }

        printf("\nHealth: %d \t Shield: %d \t Power Attacks: %d\n", *hp,*shield, *pw);
        printf("[1] Basic Attack \t [2] Power Attack\n\nChoose your attack: ");
       
        if (scanf("%d", &input) != 1){
           printf("\nInvalid Input. Please choose either only 1 or 2.\n");
           while (getchar() != '\n');
           continue;
       }

        switch (input){
            case 1:
                villainHp -= 10;
                printf("\nYou did 10 damage to the enemy!\n");
                break;
            case 2:
                if (*pw > 0){
                    villainHp -= 30;
                    (*pw)--;
                    printf("\nYou did 30 damage to the enemy!\n");
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
           printf("You vanquished the monster!\n\n");
           sleep(1);
           return 2;
        }            
                 
        if (*shield > 0){
           *shield -= 10;
        }
        else{
           *hp -= 10;
        }
        printf("\nThe monster does 10 damage to you!\n");
    }
  
    return playAgain();

}
