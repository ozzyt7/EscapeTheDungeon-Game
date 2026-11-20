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
    int atkDmg;
    int pwDmg;
    int critCount;
};

struct monster {
    int hp;
    int dmg;
};

void preGame();
int firstBttl(struct player*, struct monster*);
int secondBttl(struct player*, struct monster*);
int thirdBttl(struct player*, struct monster*); 
int fourthBttl(struct player*, struct monster*);
int fifthBttl(struct player*, struct monster*);
int playAgain();
int critChance(struct player*);
void displayHpBar(int, int, struct monster*);
int getInput();
void extraHp(struct player*, int);
void displayMsg(int, char *);
void firstMonsterDisplay();
void firstMonsterDisplayDeath();
void secondMonsterDisplay();
void secondMonsterDisplayDeath();
void thirdMonsterDisplay();
void thirdMonsterDisplayDeath();
void fourthMonsterDisplay();
void fourthMonsterDisplayDeath();
void fifthMonsterDisplayP1();
void fifthMonsterDisplayP2();
void fifthMonsterDisplayDeath();
void endDisplay();

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
    
    if (feof(stdin)){
        free(name);
	    printf("\n\nEOF encountered.\n\n");
	    return 0;
    }
    int isNotValid = 0;
    if (len == 0){
        isNotValid++;
    }
    for (int i = 0; i < len; i++){
        if (isspace((unsigned char)name[i]) && name[i] != '\n'){
	        isNotValid++;
            break;
		}
    }
    if (isNotValid){
	    printf("Invalid input. Please don't include spaces.\n\n");
        continue;
    }
    break;
    }
    
    printf("\n\nGood Luck on your journey %s!\n\n", name);

    sleep(1);

    while (1){ 
        struct player user = {120, 0, 5, 10, 30, 0};
        struct player *playerPtr = &user;
        struct monster firstM = {60, 10}, secondM = {120, 10}, thirdM = {200, 10}, fourthM = {260, 20}, fifthM = {340, 20};
        struct monster *m1 = &firstM, *m2 = &secondM, *m3 = &thirdM, *m4 = &fourthM, *m5 = &fifthM;

        int firstPhase = firstBttl(playerPtr, m1);
        if (!firstPhase){
            break;
        }
        if (firstPhase == 1){
            continue;
        }
	    extraHp(playerPtr, 30);
        int secondPhase = secondBttl(playerPtr, m2);
        if (!secondPhase){
            break;
        }
        if (secondPhase == 1){
            continue;
        }
        extraHp(playerPtr, 30);
	    int thirdPhase = thirdBttl(playerPtr, m3);
        if (!thirdPhase){
	        break;
	    }
	    if (thirdPhase == 1){
	        continue;
	    }
        extraHp(playerPtr, 40);
        int fourthPhase = fourthBttl(playerPtr, m4);
        if (!fourthPhase){
        break;
        }
        if (fourthPhase == 1){
        continue;
        }
        extraHp(playerPtr, 50);
        int finalPhase = fifthBttl(playerPtr, m5);
        if (!finalPhase){
        break;
        }
        if (finalPhase == 1){
        continue;
        }
        endDisplay(); 
	    char endMsg[52] = "YOU FIND A DOORWAY LEADING TO THE EXIT AND ESCAPE!!";
        printf("\n\t*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n\t|   %s   |\n\t*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n\n", endMsg);
	    printf("Congratulations %s!!!\nYou defeated all the monsters!! You discover a passage with a flight of stairs! You ascend the staircase and escape the dungeon! Well done!", name);
        break;
    }
    free(name);
    printf("\n\nThank you for playing!!\n\n");
    return 0;
}

void preGame(){
    printf("\n\n\t\t*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n\t\t| Welcome To The Cursed Dungeon!! |\n\t\t*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n");
    printf("\nMonsters lurk these halls and devour whoever they encounter.\nYou are trapped and must defeat all the monsters to escape.\nAre you able to step up to the challenge and conquer the dungeon?\n");
    printf("Or will you like many others before you fall victim to these treacherous creatures.\nThat outcome rests in your hands!\n");
    printf("\n\t\t\t  How To Play:\nThere are 5 monsters in the dungeon: 4 regular monsters and 1 boss monster.\nEnter 1 for basic attacks, 2 for power attacks and 3 for enhanced attacks. Basic attacks do 10 damage points and power attacks do 30 damage points.\n");
    printf("During each basic attack, there is a 20%% chance it becomes a critical hit, in which case the attack does double damage points.\nIf you get more than one critical hit in a single playthrough, your basic attack increases by 10 damage points for each critical hit.\n");
    printf("Enhanced attack allows you to deal 50 damage points, but you lose 30 total health including shield points.\nThe monsters deal between 10 to 30 damage points. You gain shield points for defeating monsters.\n");
    printf("Between each battle there is a 10%% chance you stumble upon a health pack and gain extra health points.\nUnlike with regular monsters, the boss monster has 2 phases where the creature's damage will increase.\n\n\n");
}

int firstBttl(struct player *pStats, struct monster *monsterPtr) {
    int firstMsgDisplay = 1, maxHp = monsterPtr->hp / 2, isFirst = 1, shieldIncr = 30;
    char firstMsg[60] = "UGH, THERE'S MORE OF YOU? FINE, I'LL DEAL WITH YOU QUICKLY!", deathMsg[44] = "WAIT, I WAS BESTED?! NO, THIS CAN'T BE!!!", name[20] = "ESTREED THE ANGULAR";

    printf("\nThe First Monster Approaches\n");
    sleep(1);

    while (1){
        sleep(1);
	    printf("\n\t*=*=*=*=*=*=*=*=*=*=*=*=*=*\n\t|   %s   |\n\t*=*=*=*=*=*=*=*=*=*=*=*=*=*\n", name);
	    displayHpBar(maxHp, isFirst, monsterPtr);
        firstMonsterDisplay();
        if (firstMsgDisplay){
	        displayMsg(1, firstMsg);
            firstMsgDisplay--;	
        }
	    if ((pStats -> hp) <= 0){
	        break;
        }

        sleep(1);
        printf("\nHealth: %d \t Shield: %d \t Power Attacks: %d\n", pStats -> hp, pStats -> shield, pStats -> pw);
        printf("[1] Basic Attack \t [2] Power Attack \t [3] Enhanced Attack\n\nChoose your attack: ");
	    int input = getInput();
	    if (input == -1){
	        return 0;
	    }
	    if (input == -2){
            continue;
        }
        switch(input){
            case 1:
                if (critChance(pStats)){
                    monsterPtr->hp -= (pStats->atkDmg * 2);
                    printf("\nYou got a critical hit! You do %d damage points to the enemy!\n", pStats->atkDmg *2); 
                    break; 
                }
                monsterPtr->hp -= pStats->atkDmg;
	            printf("\nYou did %d damage points to the enemy!\n", pStats->atkDmg);
       	        break;
	        case 2:
                if (pStats -> pw > 0){
                    monsterPtr->hp -= pStats->pwDmg;
		            (pStats -> pw)--;
                    printf("\nYou did %d damage points to the enemy!\n", pStats->pwDmg);
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
	        displayHpBar(maxHp, isFirst, monsterPtr);
            firstMonsterDisplayDeath();
	        displayMsg(1, deathMsg);
            (pStats->shield) += shieldIncr;
            printf("\nYou defeated Estreed The Angular! You gained %d shield points!\n\n", shieldIncr);
	        sleep(2);
            return 2;
        }
        printf("\nThe monster does %d damage points to you!\n", monsterPtr->dmg);
        (pStats -> hp) -= monsterPtr->dmg;

    }
    return playAgain();
}

int secondBttl(struct player *pStats, struct monster *monsterPtr){
    int firstMsgDisplay = 1, maxHp = monsterPtr->hp / 4, shieldIncr = 50, isFirst = 0;
    char firstMsg[52] = "HAHA!! WHAT PUNY HUMAN THINKS HE CAN CHALLENGE ME?!", deathMsg[44] = "IMPOSSIBLE!! HOW COULD THIS HAPPEN TO ME!!!", name[18] = "REGIS THE TWISTED", usrDthMsg[76] = "FOOLISH MORTAL! I PITY YOUR SIMPLE-MINDEDNESS!! I DIDN'T EVEN TRY!!";

    printf("\nThe Second Monster Approaches\n");
    sleep(1);

    while (1){
        sleep(1);
        printf("\n\t\t  *=*=*=*=*=*=*=*=*=*=*=*=*\n\t\t  |   %s   |\n\t\t  *=*=*=*=*=*=*=*=*=*=*=*=*\n", name);
	    displayHpBar(maxHp, isFirst, monsterPtr);
        secondMonsterDisplay();
        if (firstMsgDisplay){
	        displayMsg(2, firstMsg);
            firstMsgDisplay--;
        }

        if ((pStats -> hp) <= 0){
	        displayMsg(2, usrDthMsg);
            break;
        }
        printf("\nHealth: %d \t Shield: %d \t Power Attacks: %d\n", pStats -> hp, pStats -> shield, pStats -> pw);
        printf("[1] Basic Attack \t [2] Power Attack \t [3] Enhanced Attack\n\nChoose your attack: ");
        int input = getInput();
        if (input == -1){
            return 0;
        }
        if (input == -2){
            continue;
        }

        switch (input){
            case 1:
	            if (critChance(pStats)){
		            monsterPtr->hp -= pStats->atkDmg * 2;
		            printf("\nYou got a critical hit! You do %d damage points to the enemy!\n", pStats->atkDmg *2);
		            break;
		        }   
                monsterPtr->hp -= pStats->atkDmg;
                printf("\nYou did %d damage points to the enemy!\n", pStats->atkDmg);
		        break;
            case 2:
                if (pStats -> pw > 0){
                    monsterPtr->hp -= pStats->pwDmg;
                    (pStats -> pw)--;
                    printf("\nYou did %d damage points to the enemy!\n", pStats->pwDmg);
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
	        displayHpBar(maxHp, isFirst, monsterPtr);
 	        secondMonsterDisplayDeath();
            displayMsg(2, deathMsg);
            (pStats->shield) += shieldIncr;
            printf("\nYou defeated Regis The Twisted! You gained %d shield points!\n\n", shieldIncr);
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

int thirdBttl(struct player *pStats, struct monster *monsterPtr){
    int firstMsgDisplay = 1, maxHp = monsterPtr->hp / 4, isFirst = 0, shieldIncr = 100, attkNum = 0;
    char firstMsg[63] = "OOOOHH HEHEEEE!! HOW DELICIOUS, MORE FLESH FOR ME TO CONSUME!!", deathMsg[50] = "NOOOOOO!! MY FOOD!! YOU WOULD'VE BEEN SO TASTY!!!", name[22] = "GROOOB THE INSATIABLE", usrDthMsg[69] = "MINE! MINE! YOU'RE ALL MINEEEEEEE!! I WILL SAVOUR THIS TENDER MEAT!!";

    printf("\nThe Third Monster Approaches\n");
    sleep(1);         
    while (1){
        sleep(1);

        printf("\n\t\t\t*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\n\t\t\t|   %s   |\n\t\t\t*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\n", name);
	    displayHpBar(maxHp, isFirst, monsterPtr);
	    thirdMonsterDisplay();

        if (firstMsgDisplay){
            displayMsg(3, firstMsg);
            firstMsgDisplay--;
        }
        if ((pStats -> hp) <= 0){
	        displayMsg(3, usrDthMsg);       
	        break;
        }

        printf("\nHealth: %d \t Shield: %d \t Power Attacks: %d\n", pStats->hp, pStats->shield, pStats->pw);
        printf("[1] Basic Attack \t [2] Power Attack \t [3] Enhanced Attack\n\nChoose your attack: ");
        int input = getInput();
        if (input == -1){
            return 0;
        }
        if (input == -2){
            continue;
        }
        switch (input){
            case 1:
                if (critChance(pStats)){
                    monsterPtr->hp -= pStats->atkDmg *2;
		            printf("\nYou got a critical hit! You do %d damage to the enemy!\n", pStats->atkDmg *2);
		            break;
		        }
                monsterPtr->hp -= pStats->atkDmg;
                printf("\nYou did %d damage to the enemy!\n", pStats->atkDmg);
                break;
            case 2:
                if ((pStats -> pw) > 0){
                    monsterPtr->hp -= pStats->pwDmg;
                    (pStats->pw)--;
                    printf("\nYou did %d damage to the enemy!\n", pStats->pwDmg);
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
 	        displayHpBar(maxHp, isFirst, monsterPtr);
	        thirdMonsterDisplayDeath();
            displayMsg(3, deathMsg);
	        pStats->shield += shieldIncr;
            printf("\nYou defeated Grooob The Insatiable! You gained %d shield points!\n\n", shieldIncr);
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

int fourthBttl(struct player *pStats, struct monster *monsterPtr){
    int firstMsgDisplay = 1, maxHp = monsterPtr->hp / 4, shieldIncr = 150, isFirst = 0;
    char firstMsg[105] = "HOW DARE A MERE HUMAN TRY TO MEASURE UP TO THE LIKES OF ME! YOU AREN’T WORTHY OF BEING IN MY PRESENCE!", usrDthMsg[84] = "KNOW YOUR PLACE MAGGOT!! JUST THE THOUGHT OF THESE FEEBLE BEINGS ASSAULTS MY BRAIN!";
    char name[20] = "GILTSPUR THE MIGHTY", deathMsg[108] = "OUTRAGEOUS!!! YOU WILL RUE THIS DAY EARTHLING, I SWEAR IT!! I WILL RISE AGAIN! THE MIGHTY WILL NEVER FALL!!";
                    
    printf("\nThe Fourth Monster Approaches\n");
    sleep(1);  
    while (1){
        sleep(1);
        printf("\n\t\t\t\t  *=*=*=*=*=*=*=*=*=*=*=*=*=*\n\t\t\t\t  |   %s   |\n\t\t\t\t  *=*=*=*=*=*=*=*=*=*=*=*=*=*\n", name);
        displayHpBar(maxHp, isFirst, monsterPtr);
        fourthMonsterDisplay();
        if (firstMsgDisplay){
            displayMsg(4, firstMsg);
            firstMsgDisplay--;
        }       
        if ((pStats->hp) <= 0){
            displayMsg(4, usrDthMsg);
            break;   
        }
        printf("\nHealth: %d \t Shield: %d \t Power Attacks: %d\n", pStats -> hp, pStats -> shield, pStats -> pw);
        printf("[1] Basic Attack \t [2] Power Attack \t [3] Enhanced Attack\n\nChoose your attack: ");
        int input = getInput();
        if (input == -1){
            return 0;
        }
        if (input == -2){
            continue;
        }       
        switch (input){
            case 1:
                if (critChance(pStats)){
                    monsterPtr->hp -= pStats->atkDmg * 2;
                    printf("\nYou got a critical hit! You do %d damage points to the enemy!\n", pStats->atkDmg *2);
                    break;   
                }
                monsterPtr->hp -= pStats->atkDmg;
                printf("\nYou did %d damage points to the enemy!\n", pStats->atkDmg);
                break;
            case 2: 
                if (pStats->pw > 0){
                    monsterPtr->hp -= pStats->pwDmg;
                    pStats->pw--;
                    printf("\nYou did %d damage points to the enemy!\n", pStats->pwDmg);
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
            printf("\n\t\t\t\t  *=*=*=*=*=*=*=*=*=*=*=*=*=*\n\t\t\t\t  |   %s   |\n\t\t\t\t  *=*=*=*=*=*=*=*=*=*=*=*=*=*\n", name);
            displayHpBar(maxHp, isFirst, monsterPtr);
            fourthMonsterDisplayDeath();
            displayMsg(4, deathMsg);
            pStats->shield += shieldIncr;
            printf("\nYou defeated the Giltspur The Mighty! You gained %d shield points!\n\n", shieldIncr);
            sleep(2);
            return 2;
        }              
       if (pStats->shield > 0){
            pStats->shield -= monsterPtr->dmg;
            if (pStats->shield < 0){
                pStats->hp += pStats->shield;
                pStats->shield = 0;
            }
        }
        else{
           pStats->hp -= monsterPtr->dmg;
        }
        printf("\nThe monster does %d damage points to you!\n", monsterPtr->dmg);
   }
   return playAgain();

}
int fifthBttl(struct player *pStats, struct monster *monsterPtr){
    int firstMsgDisplay = 1, maxHp = monsterPtr->hp / 4, isFirst = 0, secondPhaseDisplay = 1, secondPhaseTrigger = 0;
    char firstMsg[90] = "WHAT’S THIS, YOU SUBDUED THE OTHERS?! HOW INTERESTING, YOU CAME THIS FAR JUST TO LOSE!!", deathMsg[108] = "RAAAAAAHHH!! INCONCEIVABLE! CURSE THOSE INSOLENT CREATURES FOR NOT STOPPING YOU LEADING TO THIS TRAVESTY!!!";
    char name[22] = "BLUGGARD THE WRETCHED", usrDthMsg[89] = "THIS WAS QUITE ENTERTAINING! IT WAS A VALIANT EFFORT! STAND PROUD HUMAN YOU ARE STRONG!!", phase2Msg[77] = "IT SEEMS I UNDERESTIMATED YOUR ABILITY!! YOU'VE FORCED ME TO PUT MY ALL IN!!";
                
    printf("\nThe Final Monster Approaches\n");  
    sleep(1);
    while (1){
        sleep(1);
        printf("%s\n", name);
        printf("\n\t\t\t\t  *=*=*=*=*=*=*=*=*=*=*=*=*=*=*\n\t\t\t\t  |   %s   |  \n\t\t\t\t  *=*=*=*=*=*=*=*=*=*=*=*=*=*=*\n", name);
        displayHpBar(maxHp, isFirst, monsterPtr);
        if (monsterPtr->hp > 170){
	        fifthMonsterDisplayP1();
	    }
	    else{
	        fifthMonsterDisplayP2();
            secondPhaseTrigger++;
	    }  
        if (firstMsgDisplay){
            displayMsg(5, firstMsg);
            firstMsgDisplay--;
        }
        if (secondPhaseTrigger && secondPhaseDisplay){ 
            displayMsg(5, phase2Msg);
            printf("\n\nBluggard is ENRAGED!!! He fully comes alive and now does 30 damage points!\n\n");
            monsterPtr->dmg = 30;
            secondPhaseDisplay--;
        }
        if (pStats->hp <= 0){
            displayMsg(5, usrDthMsg);
            break;
        }
                    
        printf("\nHealth: %d \t Shield: %d \t Power Attacks: %d\n", pStats->hp, pStats->shield, pStats->pw);
        printf("[1] Basic Attack \t [2] Power Attack \t [3] Enhanced Attack\n\nChoose your attack: ");
        int input = getInput();
        if (input == -1){
            return 0;
        }
        if (input == -2){
            continue;
        }
                    
        switch (input){
            case 1:
                if (critChance(pStats)){
                    monsterPtr->hp -= pStats->atkDmg *2;
                    printf("\nYou got a critical hit! You do %d damage to the enemy!\n", pStats->atkDmg *2);
                    break;
                }
                monsterPtr->hp -= pStats->atkDmg;
                printf("\nYou did %d damage to the enemy!\n", pStats->atkDmg);
                break;
            case 2:
                if (pStats->pw > 0){
                    monsterPtr->hp -= pStats->pwDmg;
                    (pStats->pw)--;
                    printf("\nYou did %d damage to the enemy!\n", pStats->pwDmg);
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
            printf("\n\t\t\t\t  *=*=*=*=*=*=*=*=*=*=*=*=*=*=*\n\t\t\t\t  |   %s   |  \n\t\t\t\t  *=*=*=*=*=*=*=*=*=*=*=*=*=*=*\n", name);
            displayHpBar(maxHp, isFirst, monsterPtr);
            fifthMonsterDisplayDeath();
            displayMsg(5, deathMsg);
            printf("\nYou defeated the Bluggard The Wretched!\n\n\n");
            sleep(1);
            return 2;
        }
                 
        if (pStats->shield > 0){
            pStats->shield -= monsterPtr->dmg;
            if (pStats->shield < 0){
                pStats->hp += pStats->shield;
                pStats->shield = 0;
            }
        }
        else{
            pStats->hp -= monsterPtr->dmg; 
        }
        printf("\nThe monster does %d damage points to you!\n\n\n", monsterPtr->dmg);
    }     
    return playAgain();
}

int playAgain(){   
    while (1){
        char usrInput[5], *c = usrInput, r;
        printf("\nYou died! Play Again?\n[1] Yes  [2] No\nInput: ");
        fgets(usrInput, sizeof(usrInput), stdin);
	    if (feof(stdin)){
            printf("\n\nEOF encountered.\n\n");
            return 0;
        }

        if (strchr(usrInput, '\n') == NULL){
            printf("\nInvalid input. Please enter 1 or 2, or type yes or no!\n\n");
            while ((r = getchar()) != '\n' && r != EOF);
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

        printf("\n\nInvalid input. Please enter 1 or 2 or type yes or no!\n\n");
    }

    return 0;
}

int critChance(struct player *p){
    srand(time(NULL));
    int critNum = (rand() % 10) + 1;

    if (critNum > 8){
	(p->critCount)++;
        if (p->critCount > 1){
	    p->atkDmg += 10;
	}
        return 1;
    }
    return 0;
  
}

void displayHpBar(int maxHp, int isFirstMonster, struct monster *mPtr){
    int hpDisplay;
    if (isFirstMonster){
        hpDisplay = mPtr->hp / 2;  
}
    else {
        hpDisplay = mPtr->hp / 4;
    }
    if (mPtr->hp >= 100){
        printf("\n                 -");
    }
    else{
        printf("\n               -");
    }
    for (int r = 0; r < maxHp + 1; r++){
        printf("-");
    }
    printf("\nVillain Hp: %d", mPtr->hp);
	if (mPtr->hp >= 100){
        printf("  |");
    }
    else if (mPtr->hp < 100 && mPtr->hp >= 10){       
        printf(" |");
    }
	else{
        printf("  |");
	}

    for (int j = 0; j < hpDisplay; j++){
        printf("#");
    }
    if (hpDisplay < maxHp){
        for (int k = hpDisplay; k < maxHp; k++){
            printf(" ");
        }
    }
    if (mPtr->hp >= 100){
        printf("|\n                 -");
    }
    else{  
        printf("|\n               -");
    }
    for (int t = 0; t < maxHp + 1; t++){
        printf("-");
    }
    printf("\n\n");
}

int getInput(){
    char input[3], c;
    fgets(input, sizeof(input), stdin);

    if (feof(stdin)){
        printf("\n\nEOF encountered.\n\n");
        return -1;
    }
    if (strchr(input, '\n') == NULL){ // if input is too long for buffer clear stdin after input
        while ((c = getchar()) != '\n' && c != EOF);
        printf("\nInvalid Input. Please don't include spaces and choose either only 1, 2 or 3.\n");
        return -2;
    }
    return input[0] - '0';

}

void displayMsg(int monster, char *msg){
    switch (monster){
        case 1:
            printf("  ");
            break;
        case 2:
            printf("    ");
            break;
        default:
            if (strlen(msg) > 100){
            printf("  ");
            }
            else if (strlen(msg) < 60){
                printf("\t     "); 
            }
            else{
                printf("\t");
            }
            break;
    }
    for (int i = 0; i < strlen(msg); i++){
        printf("-");
    }
    if (strlen(msg) + 1 >= 108){
        printf("--------\n");
    }
    else if (strlen(msg) + 1 >= 90 && strlen(msg) + 1 < 108){
        printf("------\n");
    }
    else{
        printf("--------\n");
    }
    switch (monster){
        case 1:
            printf("  ");
            break;
        case 2:     
            printf("    ");
            break;  
        default:
            if (strlen(msg) > 100){ 
                printf("  ");
            }
            else if (strlen(msg) < 60){
                printf("\t     "); 
            }
            else{ 
                printf("\t");
            }
            break;
    }
    printf("|   %s   |\n", msg);
    switch (monster){
        case 1:         
            printf("  ");
            break;
        case 2: 
            printf("    ");
            break;
        default:
            if (strlen(msg) > 100){ 
            printf("  ");
            }
            else if (strlen(msg) < 60){
                printf("\t     ");
            }
            else{
                printf("\t");
            }
            break;
    }
    for (int i = 0; i < strlen(msg); i++){
        printf("-");
    }
    if (strlen(msg) + 1 >= 108){
    printf("--------\n");
    }
    else if (strlen(msg) + 1 >= 90 && strlen(msg) + 1 < 108){
    printf("------\n");
    }
    else{
        printf("--------\n");
    }

}

void extraHp(struct player *ptr, int hpBonus){
    srand(time(NULL));
    int chanceHp = (rand() % 10) + 1;

    if (chanceHp > 9){
        ptr->hp += hpBonus;
        printf("\nYou found a health pack. You gained %d health points\n", hpBonus);
        sleep(1);
        return;
    }
    return;
}

void firstMonsterDisplay(){
    printf("\t\t     -/\\--------/\\-\n");
    printf("\t\t     |    \\  /    |\n");
    printf("\t\t     |   -    -   |\n");
    printf("\t\t     |  --------  |\n");
    printf("\t\t     --------------\n");  
}

void firstMonsterDisplayDeath(){
    printf("\t\t   -/\\--------/\\-\n");
    printf("\t\t   |    \\  /    |\n");
    printf("\t\t   |  \\/    \\/  |\n");
    printf("\t\t   |  /\\    /\\  |\n"); 
    printf("\t\t   |  --------  |\n");
    printf("\t\t   --------------\n");
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

void fourthMonsterDisplay(){
printf("\t   {--------------}                                                     {--------------}\n");
printf("\t   |              |            *        *        *        *             |              |\n");
printf("\t   {-----* *------}            |\\      / \\      / \\      /|             {-----* *------}\n");
printf("\t         * *                   | \\    /   \\    /   \\    / |                   * *\n");
printf("\t         * *                   |  \\__/     \\__/     \\__/  |                   * *\n");
printf("\t         * *         ----------|                          |-----------        * *\n");
printf("\t         * *        / _____    |__________________________|    ______ \\       * *\n");
printf("\t         * *       / /    |                                    |     \\ \\      * *\n");
printf("\t         * *       \\/     |  ++++++++++++++    ++++++++++++++  |      \\/      * *\n");
printf("\t         * *              |  ++++++++++++++    ++++++++++++++  |              * *\n");
printf("\t         * *              |  ++          ++    ++          ++  |              * *\n");
printf("\t         * *              |  ++  ]:**:[  ++    ++  ]:**:[  ++  |              * *\n");
printf("\t         * *              |  ++  ]:**:[  ++    +*  ]:**:[  ++  |              * *\n");
printf("\t         * *              |  ++          ++    ++          ++  |              * *\n");
printf("\t         * ****************  ++                            ++  **************** *\n");
printf("\t         |                   ++     <=================>    ++                   |\n");
printf("\t         * ****************  ++     |                 |    ++  **************** *\n");
printf("\t         * *              |  ++     |                 |    ++  |              * *\n");
printf("\t         * *              |  ++     <=================>    ++  |              * *\n");
printf("\t         * *              |  ++                            ++  |              * *\n");
printf("\t         * *              |  ++          ++    ++          ++  |              * *\n");
printf("\t         * *              |  ++  ]:**:[  ++    ++  ]:**:[  ++  |              * *\n");
printf("\t         * *              |  ++  ]:**:[  ++    ++  ]:**:[  ++  |              * *\n");
printf("\t         * *       /\\     |  ++          ++    ++          ++  |     /\\       * *\n");
printf("\t         * *       \\ \\____|  ++++++++++++++    ++++++++++++++  |____/ /       * *\n");
printf("\t         * *        \\______  ++++++++++++++    ++++++++++++++  ______/        * *\n");
printf("\t         * *               \\______                     _______/               * *\n");
printf("\t         * *                      \\         ??        /                       * *\n");
printf("\t         * *                       \\         ?       /                        * *\n");
printf("\t   {-----* *------}                 \\_______________/                   {-----* *------}\n");
printf("\t   |              |                                                     |              |\n");
printf("\t   {--------------}                                                     {--------------}\n");
}

void fourthMonsterDisplayDeath(){
printf("\t   {--------------}                                                     {--------------}\n");  
printf("\t   |              |            *        *        *        *             |              |\n");   
printf("\t   {-----* *------}            |\\      / \\      / \\      /|             {-----* *------}\n");
printf("\t         * *                   | \\    /   \\    /   \\    / |                   * *\n"); 
printf("\t         * *                   |  \\__/     \\__/     \\__/  |                   * *\n"); 
printf("\t         * *         ----------|                          |-----------        * *\n");
printf("\t         * *        / _____    |__________________________|    ______ \\       * *\n");
printf("\t         * *       / /    |                                    |     \\ \\      * *\n");
printf("\t         * *       \\/     |  ++++++++++++++    ++++++++++++++  |      \\/      * *\n");
printf("\t         * *              |  ++++++++++++++    ++++++++++++++  |              * *\n");
printf("\t         * *              |  ++          ++    ++          ++  |              * *\n");
printf("\t         * *              |  ++   \\  /   ++    ++  \\  /    ++  |              * *\n");
printf("\t         * *              |  ++    \\/    ++    +*   \\/     ++  |              * *\n");
printf("\t         * *              |  ++    /\\    ++    ++   /\\     ++  |              * *\n");
printf("\t         * ****************  ++   /  \\             /  \\    ++  **************** *\n");
printf("\t         |                   ++     <=================>    ++                   |\n");  
printf("\t         * ****************  ++     |                 |    ++  **************** *\n");
printf("\t         * *              |  ++     |                 |    ++  |              * *\n");
printf("\t         * *              |  ++     <=================>    ++  |              * *\n");
printf("\t         * *              |  ++   \\  /             \\  /    ++  |              * *\n");
printf("\t         * *              |  ++    \\/    ++    ++   \\/     ++  |              * *\n");
printf("\t         * *              |  ++    /\\    ++    ++   /\\     ++  |              * *\n");
printf("\t         * *              |  ++   /  \\   ++    ++  /  \\    ++  |              * *\n");
printf("\t         * *       /\\     |  ++          ++    ++          ++  |     /\\       * *\n");
printf("\t         * *       \\ \\____|  ++++++++++++++    ++++++++++++++  |____/ /       * *\n");
printf("\t         * *        \\______  ++++++++++++++    ++++++++++++++  ______/        * *\n");
printf("\t         * *               \\______                     _______/               * *\n");
printf("\t         * *                      \\         ??        /                       * *\n");
printf("\t         * *                       \\         ?       /                        * *\n");
printf("\t   {-----* *------}                 \\_______________/                   {-----* *------}\n");
printf("\t   |              |                                                     |              |\n"); 
printf("\t   {--------------}                                                     {--------------}\n");
}

void fifthMonsterDisplayP1(){
printf("                                               _____\n");
printf("                                               {###}       /\\\n");
printf("                                               |###|    __/  \\\n");
printf("                                        ____   {###}   / ____/\n");
printf("                                       /    \\  |   |  / /\n");
printf("                                      /  /\\  \\/     \\/ /\n");
printf("                                     /  /  \\          /\n");
printf("                                     \\_/    \\        /\n");
printf("                                            |       /\n");
printf("                                            |      |\n");   
printf("                    -------------------------      --------------------------\n");
printf("                    |                                                       |\n");
printf("                    |_  ____                                       ______  _|                                 #\n");
printf("                     * *   |                                       |     * *                                 #\n");
printf("                    / /    |  _____________          ____________  |      \\ \\                               #\n");
printf("                   * *    _|    %%%%%%%%%%%%%%%%%%%% \\        / <.$$$$$.>    |_      * *                              #\n");
printf("                  / /    [ ]      %%%%%%%%%%%%              <..$$$..>    [ ]      \\ \\                  ####{:}####\n");
printf("                 * *    _[ ]       %%%%%%%%               <...$...>    [ ]_      * *                  ###{:}###\n");
printf("                / /    [   ]                                       [   ]      \\ \\                 ###{:}###\n");
printf("               * *     [_  ]      ---------------------------      [  _]       * *                    |\n");
printf("              / /        [ ]     /                           \\     [ ]          \\ \\                   |\n");
printf("             * *         [_]     | /\\_____________________/\\ |     [_]           * *                  |\n");
printf("            / /            |     |/                         \\|     |              \\ \\              *****\n");
printf("           * *             \\_______________________________________/               * *             * ***\n");
printf("           \\ \\                                                                      \\ \\           / / |\n");
printf("            * *                                                                      * *         * *  |\n");
printf("             \\ \\                                                                      \\ \\       / /   |\n");
printf("              * *          ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^                           * *     * *\n");
printf("               \\ \\***-*-*-*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^                            \\ \\***/ /\n"); 
printf("                \\_____*_*_*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^                             \\_____/\n");
printf("                           ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
}

void fifthMonsterDisplayP2(){
printf("                                               _____\n");
printf("                                               {###}       /\\\n");
printf("                                               |###|    __/  \\\n");
printf("                                        ____   {###}   / ____/\n");
printf("                                       /    \\  |   |  / /\n");
printf("                                      /  /\\  \\/     \\/ /\n");
printf("                                     /  /  \\          /\n");
printf("                                     \\_/    \\        /\n");
printf("                                            |       /\n");
printf("                                            |      |\n");
printf("                    -------------------------      --------------------------\n");
printf("                    |                                                       |\n");
printf("                    |_  ____                                       ______  _|                                 #\n");
printf("                     * *   |                                       |     * *                                 #\n");
printf("                    / /    |   ____________          ____________  |      \\ \\                               #\n");
printf("                   * *    _|     <.$$$$$.> \\        / <.$$$$$.>    |_      * *                              #\n");
printf("                  / /    [ ]     <..$$$..>            <..$$$..>    [ ]      \\ \\                  ####{:}####\n");
printf("                 * *    _[ ]     <...$...>            <...$...>    [ ]_      * *                  ###{:}###\n");
printf("                / /    [   ]                                       [   ]      \\ \\                 ###{:}###\n");
printf("               * *     [_  ]      ---------------------------      [  _]       * *                    |\n");
printf("              / /        [ ]     /                           \\     [ ]          \\ \\                   |\n");
printf("             * *         [_]     | /\\_____________________/\\ |     [_]           * *                  |\n");
printf("            / /            |     |/                         \\|     |              \\ \\              *****\n");
printf("           * *             \\_______________________________________/               * *             * ***\n");
printf("           \\ \\                                                                      \\ \\           / / |\n");
printf("            * *                                                                      * *         * *  |\n");
printf("             \\ \\                                                                      \\ \\       / /   |\n");
printf("              * *          ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^                           * *     * *\n");
printf("               \\ \\***-*-*-*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^                            \\ \\***/ /\n");
printf("                \\_____*_*_*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^                             \\_____/\n");
printf("                           ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
}

void fifthMonsterDisplayDeath(){
printf("                                               _____\n");
printf("                                               {###}       /\\\n");
printf("                                               |###|    __/  \\\n");
printf("                                        ____   {###}   / ____/\n");
printf("                                       /    \\  |   |  / /\n");
printf("                                      /  /\\  \\/     \\/ /\n");
printf("                                     /  /  \\          /\n");
printf("                                     \\_/    \\        /\n");
printf("                                            |       /\n");
printf("                                            |      |\n");
printf("                    -------------------------      --------------------------\n");
printf("                    |                                                       |\n");
printf("                    |_  ____                                       ______  _|\n");
printf("                     * *   |                                       |     * *\n");
printf("                    / /    |   ____________          ____________  |      \\ \\\n");
printf("                   * *    _|       \\  /    \\        /    \\  /      |_      * *\n");
printf("                  / /    [ ]        \\/                    \\/       [ ]      \\ \\                     #{:}#\n");
printf("                 * *    _[ ]        /\\                    /\\       [ ]_      * *                    #{:}#\n");
printf("                / /    [   ]       /  \\                  /  \\      [   ]      \\ \\                   #{:}#\n");
printf("               * *     [_  ]      ---------------------------      [  _]       * *                ##  |  #\n");
printf("              / /        [ ]     /                           \\     [ ]          \\ \\             #     |   ##\n");
printf("             * *         [_]     | /\\_____________________/\\ |     [_]           * *                  |    #\n");
printf("            / /            |     |/                         \\|     |              \\ \\        #     *****    ##\n");
printf("           * *             \\_______________________________________/               * *             * ***\n");
printf("           \\ \\                                                                      \\ \\           / / |\n");
printf("            * *                                                                      * *         * *  |\n");
printf("             \\ \\                                                                      \\ \\       / /   |\n");
printf("              * *          ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^                           * *     * *\n");
printf("               \\ \\***-*-*-*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^                            \\ \\***/ /\n");
printf("                \\_____*_*_*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^                             \\_____/\n");
printf("                           ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
}

void endDisplay(){
printf("\t\t                   /\\\n");
printf("\t\t               ___/  \\___\n");
printf("\t\t            __/ ________ \\__\n");
printf("\t\t         __/    |      |    \\__\n");
printf("\t\t        /      _|______|_      \\\n");
printf("\t\t       |       |        |       |\n");
printf("\t\t       |      _|________|_      |\n");
printf("\t\t       |      |          |      |\n");
printf("\t\t       |     _|__________|_     |\n");
printf("\t\t       |     |            |     |\n");
printf("\t\t       |    _|____________|_    |\n");
printf("\t\t       |    |              |    |\n");
printf("\t\t       |   _|______________|_   |\n");
printf("\t\t       |   |                |   |\n");
printf("\t\t       |   |                |   |\n");
printf("\t\t       |________________________|\n");
}
