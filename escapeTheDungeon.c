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

struct monster {
    int hp;
    int dmg;
};

void preGame();
int firstBttl(struct player*, struct monster*, int*, int*, int*);
int secondBttl(struct player*, struct monster*, int*, int*, int*);
int thirdBttl(struct player*, struct monster*, int*, int* , int*); 
int playAgain();
int critChance(int*, int*);
void firstMonsterDisplay();
void firstMonsterDisplayDeath();
void secondMonsterDisplay();
void secondMonsterDisplayDeath();
void thirdMonsterDisplay();
void thirdMonsterDisplayDeath();

//returning 0 means player exits game, returning 1 means wants to restart game, returning 2 means player advances to next stage
int main(){
    preGame();    
    int cap = 10;
    char *name = (char *)malloc(cap);
    while (1){
        int len = 0;
        printf("Enter your username: ");
        while (fgets(name + len, cap - len, stdin)){
            len += strlen(name + len);
	    //num of chars in name
            if (len > 0 && name[len - 1] == '\n') {
                name[len - 1] = '\0';
			len--; 
                break;
            } 
            if (len >= cap - 1){
                cap *= 2;
                char *tmp = (char *)realloc(name, cap);
                if (!tmp) {
                    free(name);
                    return 0;
                }
                name = tmp;
            }
        }
    int isValid = 0;
    if (len == 0){
	    	isValid++;
    }
    for (int i = 0; i < len; i++){
        if (isspace((unsigned char)name[i]) && name[i] != '\n'){
	        isValid++;
            break;
		}
    }
    if (isValid){
	    printf("Invalid input. Please don't include spaces.\n\n");
        continue;
    }
    break;
    }
    
    printf("\n\nGood Luck on your journey %s!\n\n", name);

    sleep(1);

    while (1){ 
        struct player user = {100, 0, 3};
        struct player *playerPtr = &user;
        struct monster firstM = {50, 10}, secondM = {120, 10}, thirdM = {200, 10};
        struct monster *m1 = &firstM, *m2 = &secondM, *m3 = &thirdM;
	    int atkDmg = 10, pwDmg = 30, critCount = 0;
	    int *atkPtr = &atkDmg, *pwDmgPtr = &pwDmg, *critPtr = &critCount; 

        int firstPhase = firstBttl(playerPtr, m1, atkPtr, pwDmgPtr, critPtr);
        if (!firstPhase){
            break;
        }
        if (firstPhase == 1){
            continue;
        }
        int secondPhase = secondBttl(playerPtr, m2, atkPtr, pwDmgPtr, critPtr);
        if (!secondPhase){
            break;
        }
        if (secondPhase == 1){
            continue;
        }
	    int finalPhase = thirdBttl(playerPtr, m3, atkPtr, pwDmgPtr, critPtr);
        if (!finalPhase){
	        break;
	    }
	    if (finalPhase == 1){
	        continue;
	    }
	    char endMsg[12] = "YOU ESCAPE!";
        printf("\n\t\t\t*-*-*-*-*-*-*-*-*-*\n\t\t\t|   %s   |\n\t\t\t*-*-*-*-*-*-*-*-*-*\n\n", endMsg);
	    printf("Conratulations %s! You defeated all the monsters and escaped the dungeon! Well done!", name);
        break;
    }
    free(name);
    printf("\nThank you for playing!!\n\n");
    return 0;
}

void preGame(){
    printf("\n\n\t\t*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n\t\t| Welcome To The Cursed Dungeon!! |\n\t\t*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n");
    printf("\nMonsters lurk these halls and devour whoever they encounter.\nYou are trapped and must defeat all the monsters to escape.\nAre you able to step up to the challenge and conquer the dungeon?\n");
    printf("Or will you like many others before you fall victim to these treacherous creatures.\nThat outcome rests in your hands.\n");
    printf("\nHow To Play:\nThere are 3 monsters in the dungeon: 2 regular monsters and 1 boss monster.\nEnter 1 for basic attacks, 2 for power attacks and 3 for enhanced attacks. Basic attacks do 10 damage points and power attacks do 30 damage points.\n");
    printf("During each attack there is a 30%% chance it becomes a critical hit in which case the attack does double damage points.\nIf you get more than one critical hit in a single playthrough your basic attack increases by 10 damage points for each critical hit.\n");
    printf("Enhanced attack allows you to deal 50 damage points but you give up 30 total health including shield points.\nThe monsters only do 10 damage points. You gain shield points for defeating monsters.\n\n\n");
}

int firstBttl(struct player *pStats, struct monster *monsterPtr, int *atkDmg, int *pwDmg, int *critCount) {
    int c, firstMsgDisplay = 1, maxHp = monsterPtr->hp / 2;
    char input[3], firstMsg[42] = "HUH, YOU THINK YOU CAN BEAT ME? BRING IT!", deathMsg[44] = "WAIT, I WAS BESTED?! NO, THIS CAN'T BE!!!", name[20] = "ESTREED THE ANGULAR";

    printf("\nThe First Monster Approaches\n");
    sleep(1);

    while (1){
        sleep(1);
	    printf("\n\t*=*=*=*=*=*=*=*=*=*=*=*=*=*\n\t|   %s   |\n\t*=*=*=*=*=*=*=*=*=*=*=*=*=*\n", name);
	    printf("\n               -");
        for (int r = 0; r < maxHp + 1; r++){
	        printf("-");
	    }
	    printf("\nVillain Hp: %d |", monsterPtr->hp);
        int hpDisplay = monsterPtr->hp / 2;
	    for (int j = 0; j < hpDisplay; j++){
            printf("#");
        }
        if (hpDisplay < maxHp){
            for (int k = hpDisplay; k < maxHp; k++){
            printf(" ");
        }
        }
        printf("|\n               -");
	    for (int t = 0; t < maxHp + 1; t++){
	    printf("-");
	    }
	    printf("\n\n");
        firstMonsterDisplay();
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
        printf("[1] Basic Attack \t [2] Power Attack \t [3] Enhanced Attack\n\nChoose your attack: ");
        fgets(input, sizeof(input), stdin);
	    if (strchr(input, '\n') == NULL){ // if input is too long for buffer clear stdin after input
	        while ((c = getchar()) != '\n' && c != EOF);
	        printf("\nInvalid Input. Please don't include spaces and choose either only 1, 2 or 3.\n");	
	        continue;
	    }
	    if (input[0] != '1' && input[0] != '2' && input[0] != '3'){
	        printf("\nInvalid Input. Please don't include spaces and choose either only 1, 2 or 3.\n");
            continue;
	    }

        switch(input[0] - '0'){
            case 1:
                if (critChance(critCount, atkDmg)){
                    monsterPtr->hp -= ((*atkDmg) * 2);
                    printf("\nYou got a critical hit! You do %d damage points to the enemy!\n", *atkDmg *2); 
                    break; 
                }
                monsterPtr->hp -= (*atkDmg);
	            printf("\nYou did %d damage points to the enemy!\n", *atkDmg);
       	        break;
	        case 2:
                if (pStats -> pw > 0){
                    monsterPtr->hp -= (*pwDmg);
		            (pStats -> pw)--;
                    printf("\nYou did %d damage points to the enemy!\n", *pwDmg);
	                break;
		        }
	            else{
		            printf("\nYou are out of Power Attacks!\n");
                    continue;	
		        }
         
	    case 3:
	      if (pStats->hp <= 30){
            printf("\nUnable to perform Enhanced Attack! Health is too low!\n");
    		continue;
            }
            pStats->hp -= 30;
	        monsterPtr->hp -= 50;
	        printf("\nEnhanced Damage effect! You sacrifice 30 overall health but you inflict 50 damage points to the enemy!\n\n");
            break;
 	        default:
                printf("\nYou can only do a Basic Attack, Power Attack or Enhanced Attack!\n\n");
                continue;
        }
        sleep(1);
        if (monsterPtr->hp <= 0){
            monsterPtr->hp = 0;
 	        printf("\n\t*=*=*=*=*=*=*=*=*=*=*=*=*=*\n\t|   %s   |\n\t*=*=*=*=*=*=*=*=*=*=*=*=*=*\n", name);
	        printf("\n               -");
            for (int r = 0; r < maxHp + 1; r++){
                printf("-");
            }
            printf("\nVillain Hp: %d  |", monsterPtr->hp);
            int hpDisplay = monsterPtr->hp / 2;
	        for (int z = 0; z < maxHp; z++){   
                printf(" ");
            }
            printf("|\n               -");
            for (int t = 0; t < maxHp + 1; t++){
                printf("-");
            }
            printf("\n\n");
            firstMonsterDisplayDeath();
            printf("   ---");

            for (int i = 0; i < strlen(deathMsg); i++){
                printf("-");
            }
            printf("-----\n   |   %s   |\n   ---", deathMsg);
            for (int i = 0; i < strlen(deathMsg); i++){
                printf("-");
            }
            printf("-----\n");
            (pStats->shield) += 30;
            printf("\nYou defeated the monster! You gained %d shield points!\n\n", pStats->shield);
	        sleep(2);
            return 2;
        }
        printf("\nThe monster does %d damage points to you!\n", monsterPtr->dmg);
        (pStats -> hp) -= monsterPtr->dmg;

    }
    return playAgain();
}

int secondBttl(struct player *pStats, struct monster *monsterPtr, int *atkDmg, int *pwDmg, int *critCount){
    int c, firstMsgDisplay = 1, maxHp = monsterPtr->hp / 4;
    char input[3], firstMsg[52] = "HAHA!! WHAT PUNY HUMAN THINKS HE CAN CHALLENGE ME?!", deathMsg[44] = "IMPOSSIBLE!! HOW COULD THIS HAPPEN TO ME!!!", name[18] = "REGIS THE TWISTED";

    printf("\nThe Second Monster Approaches\n");
    sleep(1);

    while (1){
        sleep(1);
        printf("\n\t\t  *=*=*=*=*=*=*=*=*=*=*=*=*\n\t\t  |   %s   |\n\t\t  *=*=*=*=*=*=*=*=*=*=*=*=*\n", name);
	    printf("\n                -");
        for (int r = 0; r < maxHp + 1; r++){   
            printf("-");
        }
        printf("\nVillain Hp: %d", monsterPtr->hp);
        int hpDisplay = monsterPtr->hp / 4;
        if (monsterPtr->hp < 100){
    	    printf("  |");
	    }
	    else{
	        printf(" |"); 
	    }
        for (int j = 0; j < hpDisplay; j++){
            printf("#");
        }
        if (hpDisplay < maxHp){
            for (int k = hpDisplay; k < maxHp; k++){
                printf(" ");
            }
        }
	    printf("|\n                -");
        for (int t = 0; t < maxHp + 1; t++){
            printf("-");
        }
        printf("\n\n");
        secondMonsterDisplay();
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
        printf("[1] Basic Attack \t [2] Power Attack \t [3] Enhanced Attack\n\nChoose your attack: ");
	    fgets(input, sizeof(input), stdin);                    
        if (strchr(input, '\n') == NULL){
            while ((c = getchar()) != '\n' && c != EOF);
            printf("\nInvalid Input. Please don't include spaces and choose either only 1, 2 or 3.\n");
            continue;
        }
        if (input[0] != '1' && input[0] != '2' && input[0] != '3'){
            printf("\nInvalid Input. Please don't include spaces and choose either only 1, 2 or 3.\n");
            continue;
        }

        switch (input[0] - '0'){
            case 1:
	            if (critChance(critCount, atkDmg)){
		            monsterPtr->hp -= *atkDmg * 2;
		            printf("\nYou got a critical hit! You do %d damage points to the enemy!\n", *atkDmg *2);
		            break;
		        }   
                monsterPtr->hp -= *atkDmg;
                printf("\nYou did %d damage points to the enemy!\n", *atkDmg);
		        break;
            case 2:
                if (pStats -> pw > 0){
                    monsterPtr->hp -= *pwDmg;
                    (pStats -> pw)--;
                    printf("\nYou did %d damage points to the enemy!\n", *pwDmg);
                    break;
                    
                }
                else{
                    printf("\nYou are out of Power Attacks!\n");
                    continue;		    
                }
	        case 3:
                if (pStats->hp <= 30){
                    printf("\nUnable to perform Enhanced Attack! Health is too low!\n");
                    continue;
                }
                if (pStats->shield > 0){
	                pStats->shield -= 30;
                    if (pStats->shield < 0){
		                pStats->hp += pStats->shield;
		                pStats->shield = 0;
		            }                   
	            }
                else{
                    pStats->hp -= 30;
	            }
                monsterPtr->hp -= 50;
                printf("\nEnhanced Damage effect! You sacrifice 30 overall health but you inflict 50 damage points to the enemy!\n");
                break;
            default:
                printf("\nYou can only do Basic Attack, Power Attack or Enhanced Attack!\n\n");
       		    continue;
        }
       sleep(1);

       if (monsterPtr->hp <= 0){
            monsterPtr->hp = 0;
		printf("\n\t\t  *=*=*=*=*=*=*=*=*=*=*=*=*\n\t\t  |   %s   |\n\t\t  *=*=*=*=*=*=*=*=*=*=*=*=*\n", name);
	        printf("\n                -");
            for (int r = 0; r < maxHp + 1; r++){
                printf("-");
            }
            printf("\nVillain Hp: %d   |", monsterPtr->hp);
	        for (int z = 0; z < maxHp; z++){   
                printf(" ");
            }
            printf("|\n                -"); 
            for (int t = 0; t < maxHp + 1; t++){
                printf("-");
            }
            printf("\n\n");   
 	        secondMonsterDisplayDeath();
            printf("         ---");
            for (int i = 0; i < strlen(deathMsg); i++){
  	    	    printf("-");
            }
            printf("-----\n         |   %s   |\n         ---", deathMsg);
            for (int i = 0; i < strlen(deathMsg); i++){
    	    	printf("-");
            }
            printf("-----\n");
            (pStats->shield) += 30;
            printf("\nYou defeated the monster! You gained 50 shield points!\n\n");

	        sleep(2);
            return 2;        
        }
       
       if (pStats->shield > 0){
           (pStats -> shield) -= monsterPtr->dmg;
        }
        else{
           (pStats -> hp) -= monsterPtr->dmg;
        }
        printf("\nThe monster does %d damage points to you!\n", monsterPtr->dmg);
   }
           
   return playAgain();
}

int thirdBttl(struct player *pStats, struct monster *monsterPtr, int *atkDmg, int *pwDmg, int *critCount){
    int c, firstMsgDisplay = 1, maxHp = monsterPtr->hp / 4;
    char input[3], firstMsg[63] = "OOOOHH HEHEEEE!! HOW DELICIOUS, MORE FLESH FOR ME TO CONSUME!!", deathMsg[50] = "NOOOOOO!! MY FOOD!! YOU WOULD'VE BEEN SO TASTY!!!", name[22] = "GROOOB THE INSATIABLE";           
    printf("\nThe Final Monster Approaches\n");
    sleep(1);   
           
    while (1){
        sleep(1);
        printf("\n\t\t\t*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\n\t\t\t|   %s   |\n\t\t\t*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\n", name);
	    printf("\n                -");
        for (int r = 0; r < maxHp + 1; r++){
            printf("-");
        }
        printf("\nVillain Hp: %d", monsterPtr->hp);
        int hpDisplay = monsterPtr->hp / 4;
        if (monsterPtr->hp < 100){
            printf("  |");
        }
        else{
        printf(" |");
        }
        for (int j = 0; j < hpDisplay; j++){
            printf("#");
        }
        if (hpDisplay < maxHp){
        for (int k = hpDisplay; k < maxHp; k++){
            printf(" ");
        }
        }
        printf("|\n                -");
        for (int t = 0; t < maxHp + 1; t++){
            printf("-");
        }
        printf("\n\n");
	    thirdMonsterDisplay();

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
        printf("[1] Basic Attack \t [2] Power Attack \t [3] Enhanced Attack\n\nChoose your attack: ");
	    fgets(input, sizeof(input), stdin);                    
        if (strchr(input, '\n') == NULL){
            while ((c = getchar()) != '\n' && c != EOF);
            printf("\nInvalid Input. Please don't include spaces and choose either only 1, 2 or 3.\n");
            continue;
        }
        if (input[0] != '1' && input[0] != '2' && input[0] != '3'){
            printf("\nInvalid Input. Please don't include spaces and choose either only 1, 2 or 3.\n");
            continue;
        }
              
        switch (input[0] - '0'){
            case 1:
                if (critChance(critCount, atkDmg)){
                    monsterPtr->hp -= *atkDmg *2;
		            printf("\nYou got a critical hit! You do %d damage to the enemy!\n", *atkDmg *2);
		            break;
		        }
                monsterPtr->hp -= *atkDmg;
                printf("\nYou did %d damage to the enemy!\n", *atkDmg);
                break;
            case 2:
                if ((pStats -> pw) > 0){
                    monsterPtr->hp -= *pwDmg;
                    (pStats->pw)--;
                    printf("\nYou did %d damage to the enemy!\n", *pwDmg);
                    break;
        
                }
                else{  
                    printf("\nYou are out of Power Attacks!\n");
                    continue;
                }
	        case 3:
                if (pStats->hp + pStats->shield <= 30){
                    printf("\nUnable to perform Enhanced Attack! Health is too low!\n");
                    continue;
                }
                if (pStats->shield > 0){
                    pStats->shield -= 30;
                    if (pStats->shield < 0){
                        pStats->hp += pStats->shield;  
                        pStats->shield = 0;
                    }
                }
                else{
                    pStats->hp -= 30;
                }
                monsterPtr->hp -= 50;
                printf("\nEnhanced Damage effect! You sacrifice 30 overall health but you inflict 50 damage points to the enemy!\n");
                break;
            default:  
                printf("\nYou can only do a Basic Attack, Power Attack or Enhanced Attack!\n");
                continue;
        }
        sleep(1);
   
        if (monsterPtr->hp <= 0){
            monsterPtr->hp = 0;
            printf("\n\t\t\t*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\n\t\t\t|   %s   |\n\t\t\t*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\n", name);
	        printf("\n                -");
            for (int r = 0; r < maxHp + 1; r++){
                printf("-");
            }
            printf("\nVillain Hp: %d   |", monsterPtr->hp);
	        for (int z = 0; z < maxHp; z++){   
                printf(" ");
            }
            printf("|\n                -");
            for (int t = 0; t < maxHp + 1; t++){
                printf("-");
            }
            printf("\n\n");
	        thirdMonsterDisplayDeath();
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
            pStats->shield -= monsterPtr->dmg;
        }
        else{
            pStats->hp -= monsterPtr->dmg;
        }
        printf("\nThe monster does %d damage points to you!\n", monsterPtr->dmg);
    }
  
    return playAgain();

}

int playAgain(){   
    while (1){
        char usrInput[5], *c = usrInput;
        printf("\nYou died! Play Again?\n[1] Yes  [2] No\nInput: ");
        fgets(usrInput, sizeof(usrInput), stdin);
        if (strchr(usrInput, '\n') == NULL){
            printf("Invalid input. Please enter 1 or 2, or type yes or no!\n");
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

void firstMonsterDisplay(){
    printf("\t\t  -/\\--------/\\-\n");
    printf("\t\t  |    \\  /    |\n");
    printf("\t\t  |   -    -   |\n");
    printf("\t\t  |  --------  |\n");
    printf("\t\t  --------------\n");  
}

void firstMonsterDisplayDeath(){
    printf("\t\t  -/\\--------/\\-\n");
    printf("\t\t  |    \\  /    |\n");
    printf("\t\t  |  \\/    \\/  |\n");
    printf("\t\t  |  /\\    /\\  |\n"); 
    printf("\t\t  |  --------  |\n");
    printf("\t\t  --------------\n");
}


void secondMonsterDisplay(){
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
}
void secondMonsterDisplayDeath(){
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
}
void thirdMonsterDisplay(){
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
}
void thirdMonsterDisplayDeath(){
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
}
