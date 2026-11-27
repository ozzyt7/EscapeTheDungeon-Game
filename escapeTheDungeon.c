#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

struct player {
    int hp;
    int shield;
    int atkDmg;   
    int pw;
    int pwDmg;
    int eDmg;
    int eHpTaken;
    int critCount;
};

struct upgrade {
    int poison;
    int extraHpBonus;
    int shieldBuff;
    int blanketAtk;
    int newLife;
    int lastStand;
    int isLastStandSuccess;
    int isSneakAtk;
    int ashesAtk;
    int explosiveAtk;
    int explosiveTimer;
};

struct monster {
    int hp;
    int dmg;
    int monsterNum;
    int shieldIncrOnDeath; 
    char *monsterMsg[5];

};

void preGame();
int battleFunction(struct player*, struct monster*, struct upgrade*);
int fifthBttl(struct player*, struct monster*, struct upgrade*);

int playAgain();
int atkFunction(int, struct player*, struct monster*, struct upgrade*);
void dmgTakenFunction(int, struct player*, struct monster*, struct upgrade*);
int getInput(char *, int, struct upgrade*);
int critChance(struct player*);
void extraHp(struct player*, struct upgrade*, int);
void displayName(int, char *);
void displayHpBar(int, int, struct monster*);
void displayMsg(int, char *);

void upgradeAdd(struct player*, struct upgrade*, int);
int upgradeCheckpoint(struct upgrade*);
void ashesDecayFunction(struct monster*, struct upgrade*);
void explosiveFunction(struct monster*, struct upgrade*);
void extraLife(struct player*, struct upgrade*);
void lastStandFunction(struct player*, struct upgrade*);
void lastStandSuccessFunction(struct player*, struct upgrade*);
void poisonFunction(struct monster*, struct upgrade*);
int sneakAtkFunction(struct monster*, struct upgrade*, char *, char *, int, int);

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
void fifthMonsterDisplayDeathP1();
void fifthMonsterDisplayDeathP2();
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
    for (int i = 0; i < len; i++){
        if (isspace((unsigned char)name[i]) && name[i] != '\n'){
	        isNotValid++;
            break;
		}
    }
    if (isNotValid || len == 0){
	    printf("Invalid input. Please don't include spaces.\n\n");
        continue;
    }
    break;
    }
    printf("\n\nGood Luck on your journey %s!\n\n", name);
    srand(time(NULL));
    sleep(1);

    while (1){
        struct player user = {120, 0, 10, 5, 30, 50, 30, 0}, *playerPtr = &user;
        struct monster firstM = {60, 10, 1, 30}, secondM = {120, 10, 2, 50}, thirdM = {200, 10, 3, 100}, fourthM = {260, 20, 4, 150}, fifthM = {340, 20, 5};
        struct monster *m1 = &firstM, *m2 = &secondM, *m3 = &thirdM, *m4 = &fourthM, *m5 = &fifthM;
        struct upgrade upgradeList = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, *upgPtr = &upgradeList;
        int upgSelected;

        char m1FirstMsg[60] = "UGH, THERE'S MORE OF YOU? FINE, I'LL DEAL WITH YOU QUICKLY!", m1DeathMsg[59] = "WAIT, I'M DYING! WHAT, I WAS BESTED?! NO, THIS CAN'T BE!!!";
        char m1NameTag[20] = "ESTREED THE ANGULAR", m1Name[20] = "Estreed The Angular";
        m1->monsterMsg[0] = m1NameTag, m1->monsterMsg[1] = m1Name, m1->monsterMsg[2] = m1FirstMsg, m1->monsterMsg[3] = m1DeathMsg;

        char m2FirstMsg[52] = "HAHA!! WHAT PUNY HUMAN THINKS HE CAN CHALLENGE ME?!", m2DeathMsg[44] = "IMPOSSIBLE!! HOW COULD THIS HAPPEN TO ME!!!";
        char m2NameTag[18] = "REGIS THE TWISTED", m2UsrDthMsg[68] = "FOOLISH MORTAL! I PITY YOUR SIMPLE-MINDEDNESS!! I DIDN'T EVEN TRY!!", m2Name[18] = "Regis The Twisted";
        m2->monsterMsg[0] = m2NameTag, m2->monsterMsg[1] = m2Name, m2->monsterMsg[2] = m2FirstMsg, m2->monsterMsg[3] = m2DeathMsg, m2->monsterMsg[4] = m2UsrDthMsg;

        char m3FirstMsg[63] = "OOOOHH HEHEEEE!! HOW DELICIOUS, MORE FLESH FOR ME TO CONSUME!!", m3DeathMsg[50] = "NOOOOOO!! MY FOOD!! YOU WOULD'VE BEEN SO TASTY!!!";
        char m3NameTag[22] = "GROOOB THE INSATIABLE", m3UsrDthMsg[69] = "MINE! MINE! YOU'RE ALL MINEEEEEEE!! I WILL SAVOUR THIS TENDER MEAT!!", m3Name[22] = "Grooob The Insatiable";
        m3->monsterMsg[0] = m3NameTag, m3->monsterMsg[1] = m3Name, m3->monsterMsg[2] = m3FirstMsg, m3->monsterMsg[3] = m3DeathMsg, m3->monsterMsg[4] = m3UsrDthMsg;

        char m4FirstMsg[114] = "A-HUMPH! HOW DARE A MERE HUMAN TRY TO MEASURE UP TO THE LIKES OF ME! YOU AREN'T WORTHY OF BEING IN MY PRESENCE!", m4UsrDthMsg[84] = "KNOW YOUR PLACE MAGGOT!! JUST THE THOUGHT OF THESE FEEBLE BEINGS ASSAULTS MY BRAIN!";
        char m4NameTag[20] = "GILTSPUR THE MIGHTY", m4DeathMsg[108] = "OUTRAGEOUS!!! YOU WILL RUE THIS DAY EARTHLING, I SWEAR IT!! I WILL RISE AGAIN! THE MIGHTY WILL NEVER FALL!!", m4Name[20] = "Giltspur The Mighty";
        m4->monsterMsg[0] = m4NameTag, m4->monsterMsg[1] = m4Name, m4->monsterMsg[2] = m4FirstMsg, m4->monsterMsg[3] = m4DeathMsg, m4->monsterMsg[4] = m4UsrDthMsg;

        char m5FirstMsg[90] = "WHAT'S THIS, YOU SUBDUED THE OTHERS?! HOW INTERESTING, YOU CAME THIS FAR JUST TO LOSE!!", m5DeathMsg[106] = "RAAAAAAHHH!! INCONCEIVABLE! CURSE THOSE INSOLENT CREATURES FOR NOT STOPPING YOU, CAUSING THIS TRAVESTY!!!";
        char m5NameTag[22] = "BLUGGARD THE WRETCHED", m5UsrDthMsg[89] = "THIS WAS QUITE ENTERTAINING! IT WAS A VALIANT EFFORT! STAND PROUD HUMAN YOU ARE STRONG!!", m5Name[22] = "Bluggard The Wretched";
        m5->monsterMsg[0] = m5NameTag, m5->monsterMsg[1] = m5Name, m5->monsterMsg[2] = m5FirstMsg, m5->monsterMsg[3] = m5DeathMsg, m5->monsterMsg[4] = m5UsrDthMsg;


        battleFunction(playerPtr, m1, upgPtr);
	    extraHp(playerPtr, upgPtr, 30);
        upgSelected = upgradeCheckpoint(upgPtr);
        if (upgSelected == -1){
            break;
        }
        upgradeAdd(playerPtr, upgPtr, upgSelected);
        sleep(2);

        int secondPhase = battleFunction(playerPtr, m2, upgPtr);
        if (!secondPhase){
            break;
        }
        if (secondPhase == 1){
            continue;
        }
        extraHp(playerPtr, upgPtr, 30);
        upgSelected = upgradeCheckpoint(upgPtr);
        if (upgSelected == -1){
            break;
        }
        upgradeAdd(playerPtr, upgPtr, upgSelected);
        sleep(2);

	    int thirdPhase = battleFunction(playerPtr, m3, upgPtr);
        if (!thirdPhase){
	        break;
	    }
	    if (thirdPhase == 1){
	        continue;
	    }
        extraHp(playerPtr, upgPtr, 40);
        upgSelected = upgradeCheckpoint(upgPtr);
        if (upgSelected == -1){
            break;
        }
        upgradeAdd(playerPtr, upgPtr, upgSelected);
        sleep(2);

        int fourthPhase = battleFunction(playerPtr, m4, upgPtr);
        if (!fourthPhase){
        break;
        }
        if (fourthPhase == 1){
        continue;
        }
        extraHp(playerPtr, upgPtr, 50);
        upgSelected = upgradeCheckpoint(upgPtr);
        if (upgSelected == -1){
            break;
        }
        upgradeAdd(playerPtr, upgPtr, upgSelected);
        sleep(2);

        int finalPhase = fifthBttl(playerPtr, m5, upgPtr);
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
    printf("\n\n\t\t*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n\t\t|   Welcome To The Cursed Dungeon!!   |\n\t\t*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n");
    printf("\nMonsters lurk these halls and devour whoever they encounter.\nYou are trapped and must defeat all the monsters to escape.\nAre you able to step up to the challenge and conquer the dungeon?\n");
    printf("Or will you, like many others before you, fall victim to these treacherous creatures.\nThat outcome rests in your hands!\n");
    printf("\n\t\t\t  How To Play:\nThere are 5 monsters in the dungeon: 4 regular monsters and 1 boss monster.\nEnter 1 for basic attacks, 2 for power attacks and 3 for enhanced attacks. Basic attacks do 10 damage points and power attacks do 30 damage points.\n");
    printf("During each basic attack, there is a 10%% chance it becomes a critical hit, in which case the attack does double damage points.\nIf you get more than one critical hit in a single playthrough, your basic attack increases by 10 damage points for each critical hit.\n");
    printf("Enhanced attack allows you to deal 50 damage points, but you lose 30 total health including shield points.\nThe monsters deal between 10 to 30 damage points. You gain shield points for defeating monsters.\n");
    printf("Between each battle there is a 10%% chance you stumble upon a health pack and gain extra health points.\nAlso, between battles you reach checkpoints where you can unlock upgrades, which are made up of magical items scattered within the dungeon.");
    printf("\nUpgrades have the chance of showing up at multiple checkpoints. If the same upgrade is chosen at different checkpoints, the effects stack and can continue to stack infinitely.\n\n\n");
}


int battleFunction(struct player *ptrPlayer, struct monster *monsterPtr, struct upgrade *ptrUpg){
    int firstMsgDisplay = 1, maxHp;
    char inputArr[4];
    
    if (monsterPtr->monsterNum == 1){
        maxHp = monsterPtr->hp / 2;
    }
    else{
        maxHp = monsterPtr->hp / 4;
    }
    switch(monsterPtr->monsterNum){
        case 1:
            printf("\nThe First Monster Approaches\n");
            break;
        case 2:
            printf("\nThe Second Monster Approaches\n");
            break;
        case 3:
            printf("\nThe Third Monster Approaches\n");
            break;
        default:
            printf("\nThe Fourth Monster Approaches\n");
            break;
    }
    sleep(1);

    if (ptrUpg->isSneakAtk){
        if (sneakAtkFunction(monsterPtr, ptrUpg, monsterPtr->monsterMsg[0], monsterPtr->monsterMsg[3], monsterPtr->monsterNum, maxHp)){
            sleep(2);
            return 2;
        }
        if (monsterPtr->monsterNum == 1){
            maxHp = monsterPtr->hp / 2;
        }
        else{
            maxHp = monsterPtr->hp / 4;
        }
        sleep(2);
    }
    if (ptrUpg->ashesAtk){
        ashesDecayFunction(monsterPtr, ptrUpg);
        sleep(2);
    }
    
    while (1){
        displayName(monsterPtr->monsterNum, monsterPtr->monsterMsg[0]);
	    displayHpBar(maxHp, monsterPtr->monsterNum, monsterPtr);
        switch(monsterPtr->monsterNum){
        case 1:
            firstMonsterDisplay();
            break;
        case 2:
            secondMonsterDisplay();
            break;
        case 3:
            thirdMonsterDisplay();
            break;
        default:
            fourthMonsterDisplay();
            break;
        }

        if (firstMsgDisplay){
	        displayMsg(monsterPtr->monsterNum, monsterPtr->monsterMsg[2]);
            firstMsgDisplay--;
        }

        if (ptrPlayer->hp <= 0){
		    if (ptrUpg->lastStand){
		        lastStandFunction(ptrPlayer, ptrUpg);
                sleep(2);
		        continue;
		    }
		    if (ptrUpg->newLife){
                extraLife(ptrPlayer, ptrUpg);
                sleep(2);
                continue;
		    }
	        displayMsg(monsterPtr->monsterNum, monsterPtr->monsterMsg[4]);
            break;
        }
        
        printf("\nHealth: %d \t Shield: %d \t Power Attacks: %d\n", ptrPlayer->hp, ptrPlayer->shield, ptrPlayer->pw);
        if (ptrUpg->explosiveAtk){
            printf("[1] Basic Attack\t[2] Power Attack\t[3] Enhanced Attack\t[4] Wolfram Psora Device\n\nChoose your attack: ");
        }
        else{
            printf("[1] Basic Attack \t [2] Power Attack \t [3] Enhanced Attack\n\nChoose your attack: ");
        }
        int input = getInput(inputArr, sizeof(inputArr), ptrUpg);
        if (input == -1){
            return 0;
        }
        if (!input){
            sleep(2);
            continue;
        }
        if (!atkFunction(inputArr[0] - '0', ptrPlayer, monsterPtr, ptrUpg)){
            sleep(2);
            continue;
        }
        if (ptrUpg->explosiveTimer){
            explosiveFunction(monsterPtr, ptrUpg);
        }
        sleep(1);

       if (monsterPtr->hp <= 0){
            if (ptrUpg->isLastStandSuccess){
                lastStandSuccessFunction(ptrPlayer, ptrUpg);
                sleep(2);
            }
            monsterPtr->hp = 0;
            ptrUpg->explosiveTimer = 0;
            displayName(monsterPtr->monsterNum, monsterPtr->monsterMsg[0]);
	        displayHpBar(maxHp, monsterPtr->monsterNum, monsterPtr);
 	        switch(monsterPtr->monsterNum){
            case 1:
                firstMonsterDisplayDeath();
                break;
            case 2:
                secondMonsterDisplayDeath();
                break;
            case 3:
                thirdMonsterDisplayDeath();
                break;
            default:
                fourthMonsterDisplayDeath();
                break;
            }
            displayMsg(monsterPtr->monsterNum, monsterPtr->monsterMsg[3]);
            ptrPlayer->shield += monsterPtr->shieldIncrOnDeath + ptrUpg->shieldBuff;
            printf("\nYou defeated %s! You gained %d shield points!\n\n", monsterPtr->monsterMsg[1], monsterPtr->shieldIncrOnDeath + ptrUpg->shieldBuff);
	        sleep(3);
            return 2;        
        }   
        
        dmgTakenFunction(inputArr[1], ptrPlayer, monsterPtr, ptrUpg);
        sleep(1);
   }
   return playAgain();
}

int fifthBttl(struct player *ptrPlayer, struct monster *monsterPtr, struct upgrade *ptrUpg){
    int firstMsgDisplay = 1, maxHp = monsterPtr->hp / 4, secondPhaseDisplay = 1, secondPhaseTrigger = 0;
    char inputArr[4], phase2Msg[77] = "IT SEEMS I UNDERESTIMATED YOUR ABILITY!! YOU'VE FORCED ME TO PUT MY ALL IN!!";
                
    printf("\nThe Final Monster Approaches\n");  
    sleep(1);
    if (ptrUpg->isSneakAtk){
        if (sneakAtkFunction(monsterPtr, ptrUpg, monsterPtr->monsterMsg[0], monsterPtr->monsterMsg[3], monsterPtr->monsterNum, maxHp)){
            sleep(2);
            return 2;
        }
        maxHp = monsterPtr->hp / 4;
        sleep(2);
    }
    if (ptrUpg->ashesAtk){
        ashesDecayFunction(monsterPtr, ptrUpg);
        sleep(2);
    }

    while (1){
        displayName(monsterPtr->monsterNum, monsterPtr->monsterMsg[0]);
        displayHpBar(maxHp, monsterPtr->monsterNum, monsterPtr);
        if (monsterPtr->hp > 170){
	        fifthMonsterDisplayP1();
	    }
	    else{
	        fifthMonsterDisplayP2();
            secondPhaseTrigger++;
	    }  
        if (firstMsgDisplay){
            displayMsg(monsterPtr->monsterNum, monsterPtr->monsterMsg[2]);
            firstMsgDisplay--;
        }
        if (secondPhaseTrigger && secondPhaseDisplay){ 
            displayMsg(monsterPtr->monsterNum, phase2Msg);
            monsterPtr->dmg = 30;
            printf("\n\nBluggard is ENRAGED!!! He fully comes alive and now deals 30 damage points!\n\n");
            secondPhaseDisplay--;
        }
        if (ptrPlayer->hp <= 0){
		    if (ptrUpg->lastStand){
		        lastStandFunction(ptrPlayer, ptrUpg);
                sleep(2);
		        continue;
		    }
		    if (ptrUpg->newLife){
                extraLife(ptrPlayer, ptrUpg);
                sleep(2);
                continue;
		    }
	        displayMsg(monsterPtr->monsterNum, monsterPtr->monsterMsg[4]);
            break;
        }
                    
        printf("\nHealth: %d \t Shield: %d \t Power Attacks: %d\n", ptrPlayer->hp, ptrPlayer->shield, ptrPlayer->pw);
        if (ptrUpg->explosiveAtk){
            printf("[1] Basic Attack\t[2] Power Attack\t[3] Enhanced Attack\t[4] Wolfram Psora Device\n\nChoose your attack: ");
        }
        else{
            printf("[1] Basic Attack \t [2] Power Attack \t [3] Enhanced Attack\n\nChoose your attack: ");
        }
        int input = getInput(inputArr, sizeof(inputArr), ptrUpg);
        if (input == -1){
            return 0;
        }
        if (!input){
            sleep(2);
            continue;
        }
        if (!atkFunction(inputArr[0] - '0', ptrPlayer, monsterPtr, ptrUpg)){
            sleep(2);
            continue;
        }
        if (ptrUpg->explosiveTimer){
            explosiveFunction(monsterPtr, ptrUpg);
        }
        sleep(1);
                    
        if (monsterPtr->hp <= 0){
            if (ptrUpg->isLastStandSuccess){
                lastStandSuccessFunction(ptrPlayer, ptrUpg);
                sleep(2);
            }
            monsterPtr->hp = 0;
            ptrUpg->explosiveTimer = 0;
            displayName(monsterPtr->monsterNum, monsterPtr->monsterMsg[0]);
            displayHpBar(maxHp, monsterPtr->monsterNum, monsterPtr);
            fifthMonsterDisplayDeathP2();
            displayMsg(monsterPtr->monsterNum, monsterPtr->monsterMsg[3]);
            printf("\nYou defeated %s!\n\n\n", monsterPtr->monsterMsg[1]);
            sleep(4);
            return 2;
        }
        dmgTakenFunction(inputArr[1], ptrPlayer, monsterPtr, ptrUpg);
        sleep(1);

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

int atkFunction(int input, struct player *ptrPlayer, struct monster *ptrMonster, struct upgrade *ptrUpg){
    switch (input){
        case 1:
            if (critChance(ptrPlayer)){
                ptrMonster->hp -= ptrPlayer->atkDmg *2;
                printf("\nYou got a critical hit! You do %d damage to the enemy!\n", ptrPlayer->atkDmg *2);
                break;
            }
            ptrMonster->hp -= ptrPlayer->atkDmg;
            printf("\nYou did %d damage to the enemy!\n", ptrPlayer->atkDmg);
            break;
        case 2:
            if (ptrPlayer->pw > 0){
                ptrMonster->hp -= ptrPlayer->pwDmg;
                (ptrPlayer->pw)--;
                printf("\nYou did %d damage to the enemy!\n", ptrPlayer->pwDmg);
                break;
            }
            else{
                printf("\nYou are out of Power Attacks!\n");
                return 0;
            }
        case 3:
            if (ptrPlayer->hp + ptrPlayer->shield <= 30){
                printf("\nUnable to perform Enhanced Attack! Health is too low!\n");
                return 0;
            }
            if (ptrPlayer->shield > 0){
                ptrPlayer->shield -= 30;
                if (ptrPlayer->shield < 0){
                    ptrPlayer->hp += ptrPlayer->shield;
                    ptrPlayer->shield = 0;
                }
            }
            else{
                ptrPlayer->hp -= ptrPlayer->eHpTaken;
            }
            ptrMonster->hp -= ptrPlayer->eDmg;
            printf("\nEnhanced Damage effect! You sacrifice %d overall health but you inflict %d damage points to the enemy!\n", ptrPlayer->eHpTaken, ptrPlayer->eDmg);
            break;
        case 4:
            if (!ptrUpg->explosiveAtk){
                printf("\nInvalid Input. You haven't acquired the Wolfram Psora Device yet!\n\n");
                return 0;
            }
            explosiveFunction(ptrMonster, ptrUpg);
            return 1;
        default:
            printf("\nYou can only do a Basic Attack, Power Attack or Enhanced Attack!\n");
            return 0;
    }

    if (ptrUpg->poison){
        poisonFunction(ptrMonster, ptrUpg);
    }
    return 1;
}

void dmgTakenFunction(int input, struct player *ptrPlayer, struct monster *ptrMonster, struct upgrade *ptrUpg){
    if (ptrUpg->blanketAtk && (input == 66 || input == 98)){
        (ptrUpg->blanketAtk)--;
        printf("\nYou use the enchantment, and the monster becomes sluggish and drowsy, dealing no damage to you!\n");
        return;
    }

    if (ptrPlayer->shield > 0){
        ptrPlayer->shield -= ptrMonster->dmg;
        if (ptrPlayer->shield < 0){
            ptrPlayer->hp += ptrPlayer->shield;
            ptrPlayer->shield = 0;
            }
        }
   else{
       ptrPlayer->hp -= ptrMonster->dmg;
    }
    printf("\nThe monster does %d damage points to you!\n\n", ptrMonster->dmg);
    return;
}

int getInput(char *input, int inputSize, struct upgrade *upgPtr){
    int c;
    fgets(input, inputSize, stdin);

    if (feof(stdin)){
        printf("\n\nEOF encountered.\n\n");
        return -1;
    }
    if (strchr(input, '\n') == NULL){ //no \n means input too long for buffer so clear stdin
        while ((c = getchar()) != '\n' && c != EOF);
        printf("\nInvalid Input. Please don't include spaces and choose either only 1, 2 or 3.\n\n");
        return 0;
    }
    if (input[1] != 10){
        if (!upgPtr->blanketAtk && (input[1] == 66 || input[1] == 98)){
            printf("\nInvalid Input. You don't possess Istrian's Blanket. Please don't include spaces and choose either only 1, 2 or 3.\n\n");
        }
        else if(input[1] != 66 && input[1] != 98){
            printf("\nInvalid Input. Please don't include spaces and choose either only 1, 2 or 3.\n\n");
        }
        else{
            return 1;
        }
        return 0;
    }
    return 1;
}

int critChance(struct player *ptrPlayer){
    int critNum = (rand() % 10) + 1;

    if (critNum > 9){
	(ptrPlayer->critCount)++;
        if (ptrPlayer->critCount > 1){
	    ptrPlayer->atkDmg += 10;
	}
        return 1;
    }
    return 0;
  
}

void extraHp(struct player *ptrPlayer, struct upgrade *upgPtr, int hpBonus){
    int chanceHp = (rand() % 10) + 1;
    chanceHp += upgPtr->extraHpBonus;
    if (chanceHp > 9){
        ptrPlayer->hp += hpBonus;
        printf("\nYou find a health pack. You gain %d health points\n\n", hpBonus);
        sleep(2);
        return;
    }
    return;
}

void displayName(int monsterNum, char *name){
    switch(monsterNum){
        case 3:
        printf("\n\t\t\t");
        break;
        case 4:
        printf("\n\t\t\t\t  ");
        break;
        case 5:
        printf("\n\t\t\t\t ");
        break;
        default:
        printf("\n\t\t  ");
        break;
    }
    printf("*=*=");
    for (int x = 0; x < strlen(name); x+=2){
        printf("*=");
    }
    printf("*=*\n");
    switch(monsterNum){
        case 3:
        printf("\t\t\t");
        break;
        case 4:
        printf("\t\t\t\t  ");
        break;
        case 5:
        printf("\t\t\t\t ");
        break;
        default:
        printf("\t\t  ");
        break;
    }
    printf("|   %s   |\n", name);
    switch(monsterNum){
        case 3:
        printf("\t\t\t");
        break;
        case 4:
        printf("\t\t\t\t  ");
        break;
        case 5:
        printf("\t\t\t\t ");
        break;
        default:
        printf("\t\t  ");
        break;
    }
    printf("*=*=");
    for (int x = 0; x < strlen(name); x+=2){
        printf("*=");
    }
    printf("*=*\n");
    return;
}

void displayHpBar(int maxHp, int mNum, struct monster *mPtr){
    int hpDisplay;
    if (mNum == 1){
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

void displayMsg(int monsterNum, char *msg){
    switch (monsterNum){
        case 2:
            if (strlen(msg) > 65){
                printf(" ");
                break;
            }
            printf("    ");
            break;
        case 3:
            if (strlen(msg) > 65){
                printf("   ");
                break;
            }
            if (strlen(msg) < 60){
                printf("\t    ");
                break;
            }
            printf("\t");  
            break;
        case 5:
            if (strlen(msg) > 100){
                printf("   ");
                break;
            }
            if (strlen(msg) < 80){
                printf("\t   ");
                break;
            }
            printf("\t");
            break;
        default:
            if (strlen(msg) < 100 && strlen(msg) > 80){
                printf("\t");
                break;
            }
            printf("  ");
            break;
    }
    for (int i = 0; i < strlen(msg); i++){
        printf("-");
    }
    printf("--------\n");

    switch (monsterNum){
        case 2:
            if (strlen(msg) > 65){
                printf(" ");
                break;
            }
            printf("    ");
            break;
        case 3:
            if (strlen(msg) > 65){
                printf("   ");
                break;
            }
            if (strlen(msg) < 60){
                printf("\t    ");
                break;
            }
            printf("\t");  
            break;
        case 4:
            if (strlen(msg) < 100){
                printf("\t");
                break;
            }
            printf("  ");
            break;
        case 5:
            if (strlen(msg) > 100){
                printf("   ");
                break;
            }
            if (strlen(msg) < 80){
                printf("\t   ");
                break;
            }
            printf("\t");
            break;
        default:
            if (strlen(msg) < 100 && strlen(msg) > 80){
                printf("\t");
                break;
            }
            printf("  ");
            break;
    }

    printf("|   %s   |\n", msg);

    switch (monsterNum){
        case 2:
            if (strlen(msg) > 65){
                printf(" ");
                break;
            }
            printf("    ");
            break;
        case 3:
            if (strlen(msg) > 65){
                printf("   ");
                break;
            }
            if (strlen(msg) < 60){
                printf("\t    ");
                break;
            }
            printf("\t");  
            break;
        case 4:
            if (strlen(msg) < 100){
                printf("\t");
                break;
            }
            printf("  ");
            break;
        case 5:
            if (strlen(msg) > 100){
                printf("   ");
                break;
            }
            if (strlen(msg) < 80){
                printf("\t   ");
                break;
            }
            printf("\t");
            break;
        default:
            if (strlen(msg) < 100 && strlen(msg) > 80){
                printf("\t");
                break;
            }
            printf("  ");
            break;
    }
    for (int i = 0; i < strlen(msg); i++){
        printf("-");
    }
    printf("--------\n");
}

void upgradeAdd(struct player *ptrPlayer, struct upgrade *ptrUpg, int upgInput){
    switch (upgInput){
        case 1:
            (ptrUpg->poison)++;
            break;
        case 2:
            ptrUpg->shieldBuff += 30;
            break;
        case 3:
            ptrUpg->extraHpBonus += 1;
            break;
        case 4:
            ptrUpg->isSneakAtk = 1;
            break;
        case 5:
            ptrPlayer->pwDmg += ptrPlayer->pwDmg * 0.2;
            break;
        case 6:
            (ptrUpg->newLife)++; 
            break;
        case 7:
            (ptrUpg->lastStand)++;
            break;
        case 8:
            (ptrUpg->blanketAtk)++;
            break;
        case 9:
            (ptrPlayer->pw)++;
            break;
        case 10:
            (ptrUpg->ashesAtk)++;
            break;
        case 11:
            (ptrUpg->explosiveAtk)++;
            break;
        default:
            printf("\nConfidence and Coolness buff! All upgrades Declined!!\n\n");
            break;
        }
        
        sleep(1);
    return;
}

int upgradeCheckpoint(struct upgrade *ptrUpg){
    int upgNumTotal = 11, option1, option2, option3;

    char upgSelect[3];
    char upg1[32] = "Saw-Scaled Venomous Enrichment\n";    //poison ability
    char upg2[16] = "Wings of Ursus\n";                    //increase shield amount
    char upg3[26] = "Convex's Ocular Fragment\n";          //increase chance of hp packet
    char upg4[29] = "Macrotis Velox's Enchanment\n";       //sneak attack to bypass battle
    char upg5[25] = "Atelerix's Amalgamation\n";           //increase pw damage
    char upg6[17] = "Elixir of Dyggo\n";                   //extra life
    char upg7[35] = "Acheron's Staff of Reaving Bedlam\n"; //last stand attack
    char upg8[19] = "Istrian's Blanket\n";                 //sleep attack
    char upg9[25] = "Arms of Avaritia's Army\n";           //gain pw
    char upg10[20] = "Ashes of Cappensis\n";               //reduce monster starting health
    char upg11[22] = "Wolfram Psora Device\n";             //explosive attack
    char upgDesc1[149] = "Coat your attacks in a poison derived from the elusive underdweller Echis, dealing an additional 5 damage points to all monsters after each attack!\n";
    char upgDesc2[226] = "Belonging to the goddess Patrona, she bestows her blessings only on winners. Gain her favour and honour her in battle.\nUnder the aegis of Patrona and upon victory after each battle, she grants an additional 30 shield points!\n";
    char upgDesc3[184] = "Stolen from the carcass of the giant Convex The Panoptic, who fell in the Great Battle of Strenua, use a shard from his eye to increase your chance of finding a health packet by 10%!\n";
    char upgDesc4[486] = "Twin trickster gods, Sleip and Sylix, love horseplay and tormenting unsuspecting individuals with their antics.\nThey see your plight and wish to aid in your predicament, or so it seems.\nThey offer a potion that will turn you invisible and allow you to sneak past the monster and bypass the next battle. However, the potion has a 60% chance of failing midway through,\nwhich will reveal your plan to the monster, enraging it and causing the monster to start battle with 20% more health!\n";
    char upgDesc5[301] = "Iractus attempted to squirm his way out of a deal made with celestial deities and witnessed his homeworld get destroyed. As the sole survivor and cursed to wander the cosmos in solitude for eons,\ntake this potion infused with all his hatred and agony and increase the damage of power attacks by 20%!\n";
    char upgDesc6[448] = "The benevolent Silvanus, king of the realm Sobek, detests hardship of any kind. Imprisoned by his 13 sons, who all later perished due to infighting, for they all vied for his title.\nUpon returning, Silvanus held no hate in his heart and simply wished to feel the wind kiss his face and see flowers bloom. He takes pity on your suffering and hopes to ease your pain.\nReceive this elixir from his prized treasury and be given a new life upon death!\n"; 
    char upgDesc7[658] = "Meikai, ruler of the underworld, loves a gamble and craves a rich variety of souls. She has an abundance of human souls but few monster souls. Use this staff to summon her and strike a bargain.\nIf you die, Meikai will postpone your trip to the afterlife and grant you the state of Last Stand. With 1 health point, you will get 1 final chance to slay the monster.\nIf successful, Meikai will swap your souls, and the monster will take your place. You will be replenished with full health, and Meikai will drag the monster down to her domain!\nFail to finish off the creature, and be damned to the greuling depths of Meikai's making to suffer for all eternity\n";
    char upgDesc8[542] = "The Istrians, a magical race from the dimension Olm. Every day the sun would lie its head to rest at sunset and awake at sunrise.\nOne night during its slumber, the Istrians snuck up and covered the sun in their bewitched blanket. This entrapped the sun in perpetual dormancy, and it never rose again,\nengulfing the entire dimension in darkness forever. Enchant your attack with this blanket, triggering the monster to sleep for one turn where it will do no damage to you!\n(To use this ability, add the letter b to your attack, ex: 1b or 1B)\n";
    char upgDesc9[384] = "Ridiculed from birth for being born weakest in his clan, Avaritia trained and longed for power. He ascended to its leader, but his greed for control never diminished.\nHe ceaselessly expands his dominion solely using force and extends his arms to those who share his lust for power. Embrace his arms and gain an additional power attack,\nthough he'll eventually come for what's yours!\n";
    char upgDesc10[601] = "Keere, a primordial being, birthed her child Cappensis. Isolated for the majority of her existence, Keere felt joy raising her daughter. However, Cappensis was abducted and later killed,\nwith her corpse being discovered on the outer reaches of the realm of Cornix. Upon viewing her daughter's body, Keere's screams echoed across the universe as she vowed vengeance upon the assailants.\nCharring the body, as her pain was unbearable, she scours the universe to this day searching for the cutthroats. Cover yourself with the cursed ashes, which harm all monsters, reducing their starting\nhealth by 8%!\n";
    char upgDesc11[885] = "Not much is known of the disgraced outlaw Hallog Latro. Wanted in every realm, he gained notoriety by performing brazen heists on high-profile individuals whose wealth stretched across dimensions.\nHis run-and-gun methodology, coupled with his elaborate theatrics during heists, became a spectacle for witnesses to behold while simultaneously thwarting the most sophisticated and complex security systems\ngalaxies had to offer. Tales of his infamous escapades spread throughout, reaching the far edges of the realms, and while copycats have sprung up, very few have managed to live up to his reputation.\nStill on the run, you never know where Hallog will strike next. Use this undetonated explosive device recovered from the scene of one of his crimes.\nAttach it onto monsters where you do no damage for 1 round, but after 2 turns the device will explode, unleashing 60 damage points!\n";

    char *upgArr[11] = {upg1, upg2, upg3, upg4, upg5, upg6, upg7, upg8, upg9, upg10, upg11};
    char *upgArrDesc[11] = {upgDesc1, upgDesc2, upgDesc3, upgDesc4, upgDesc5, upgDesc6, upgDesc7, upgDesc8, upgDesc9, upgDesc10, upgDesc11};

    while (1){
        option1 = rand() % upgNumTotal;
        option2 = rand() % upgNumTotal;
        option3 = rand() % upgNumTotal;

        if (option1 != option2 && option2 != option3 && option1 != option3){
            break;
        }
    }

    while (1){
        printf("\n\t\tCheckpoint!! Available Upgrades:");
        printf("\n\n[1]\t\t\t%s\n%s\n[2]\t\t\t%s\n%s\n[3]\t\t\t%s\n%s\n[4]\t\t\tDecline All Upgrades\n\nChoose your Upgrade: ", *(upgArr + option1), *(upgArrDesc + option1), *(upgArr + option2), *(upgArrDesc + option2), *(upgArr + option3), *(upgArrDesc + option3));
        int input = getInput(upgSelect, sizeof(upgSelect), ptrUpg);
        
        if (input == -1){
            return -1;
        }
        if (!input){
            continue;
        }

        switch(upgSelect[0] - '0'){
            case 1:
                printf("\nUpgrade Added: %s\n", *(upgArr + option1));
                return option1 + 1;
            case 2:
                printf("\nUpgrade Added: %s\n", *(upgArr + option2));
                return option2 + 1;
            case 3: 
                printf("\nUpgrade Added: %s\n", *(upgArr + option3));
                return option3 + 1;
            case 4:
                return 0;
            default:
                printf("\nInvalid Input. Please only select either 1, 2, 3 or 4 and don't include spaces.\n\n");
                continue;
        }
    }
}

void ashesDecayFunction(struct monster *mPtr, struct upgrade *upgPtr){
    int monsterHpDecr = mPtr->hp * (0.08 * upgPtr->ashesAtk);
    mPtr->hp -= monsterHpDecr;
    printf("\nThe curse of the ashes weakens the monster upon approach, dealing %d damage points!\n\n", monsterHpDecr);
    return;
}

void explosiveFunction(struct monster* monsterPtr, struct upgrade* upgPtr){
    if (!upgPtr->explosiveTimer){
        (upgPtr->explosiveAtk)--;
        upgPtr->explosiveTimer = 1;
        printf("\nYou attach the device to the monster and the timer begins to tick down!\n");
        return;
    }

    if (upgPtr->explosiveTimer == 3){
        monsterPtr->hp -= 60;
        upgPtr->explosiveTimer = 0;
        printf("\n\nThe explosive has detonated dealing 60 damage points to the monster!\n\n");
        return;
    }
    (upgPtr->explosiveTimer)++;
    return;
}

void extraLife(struct player *ptrPlayer, struct upgrade *ptrUpg){
    ptrPlayer->hp = 120;
    ptrPlayer->pw = 5;
    (ptrUpg->newLife)--;
    printf("\nYou consume the elixir and are granted an extra life. Make it count!\n");
    return;
}

void lastStandFunction(struct player *ptrPlayer, struct upgrade *ptrUpg){
    ptrPlayer->hp = 1;
    (ptrUpg->lastStand)--;
    ptrUpg->isLastStandSuccess = 1;
    printf("\nTHIS IS LAST STAND!! You have one final attack to destroy the monster!\n");
    return;
}

void lastStandSuccessFunction(struct player *ptrPlayer, struct upgrade *ptrUpg){
    printf("\nYou escaped death and were successful in LAST STAND! You are rewarded with max health points!\n");
    ptrPlayer->hp = 120;
    ptrUpg->isLastStandSuccess = 0;
    return;
}

void poisonFunction(struct monster *mPtr, struct upgrade *upgPtr){
    mPtr->hp -= 5 * upgPtr->poison;
    printf("\nPoison flows through the monster, inflicting another %d damage points!\n", 5 * upgPtr->poison);
    return;
}

int sneakAtkFunction(struct monster *mPtr, struct upgrade *upgPtr, char *monsterName, char *deathMsg, int monsterNum, int monsterHp){
    (upgPtr->isSneakAtk)--;
    int monsterHpIncr, chanceNum = (rand() % 10) + 1;

    if (chanceNum > 6){
        mPtr->hp = 0;
        displayName(monsterNum, monsterName);
        displayHpBar(monsterHp, monsterNum, mPtr);
        switch (monsterNum){
            case 2:
                secondMonsterDisplayDeath();
                break;
            case 3:
                thirdMonsterDisplayDeath();
                break;
            case 4:
                fourthMonsterDisplayDeath();
                break;
            default:
                fifthMonsterDisplayDeathP1();
                break;
        }
        displayMsg(monsterNum, deathMsg);
        printf("\nYou use the potion and successfully sneak past the monster. You attack the creature from behind and defeat it in a single blow!\n");
        return 1;
    }
    monsterHpIncr = mPtr->hp * 0.20;
    mPtr->hp += monsterHpIncr;
    printf("\nYou use the potion and start to sneak past the monster. However, the potion wears off and the monster spots your attempt at deception!\nThe creature becomes ENRAGED and starts the battle with %d more health points!\n", monsterHpIncr);
    return 0;
}

void firstMonsterDisplay(){
    printf("\t           -----------------------------\n");
    printf("\t           \\       ____ \\  / ____      /\n");
    printf("\t            \\     [ ** ]    [ ** ]    /\n");
    printf("\t             \\    [ ** ]    [ ** ]   /\n");
    printf("\t              \\   [ ** ]    [ ** ]  /\n");
    printf("\t               \\                   /\n");
    printf("\t                \\      ^^^^^^     /\n");
    printf("\t                 \\     {_||_}    /\n");
    printf("\t  =--=--=--=--=--=\\      ||     /=--=--=--=--=--=\n");
    printf("\t  | |=--=--=--=--=       ||      =-=-=-=-=-=-=| |\n");
    printf("\t  | |             /  ^..^^^^..^  \\            | |\n");
    printf("\t  | |            /  / ..    .. \\  \\           | |\n");
    printf("\t  ***           /  /  ..    ..  \\  \\          ***\n");
    printf("\t *****         /  /              \\  \\        *****\n");
    printf("\t * * *        /   ^^^^^^^^^^^^^^^^   \\       * * *\n");
    printf("\t*  *  *      /                        \\     *  *  *\n");
    printf("\t            /__________________________\\\n\n");
}

void firstMonsterDisplayDeath(){
    printf("\t           -----------------------------\n");
    printf("\t           \\            \\  /           /\n");
    printf("\t            \\      \\  /      \\  /     /\n");
    printf("\t             \\      \\/        \\/     /\n");
    printf("\t              \\     /\\        /\\    /\n");
    printf("\t               \\   /  \\      /  \\  /\n");
    printf("\t                \\      ^^^^^^     /\n");
    printf("\t                 \\     {_||_}    /\n");
    printf("\t  =--=--=--=--=--=\\      ||     /=--=--=--=--=--=\n");
    printf("\t  | |=--=--=--=--=       ||      =-=-=-=-=-=-=| |\n");
    printf("\t  | |             /  ^..^^^^..^  \\            | |\n");
    printf("\t  | |            /  / ..    .. \\  \\           | |\n");
    printf("\t  ***           /  /  ..    ..  \\  \\          ***\n");
    printf("\t *****         /  /              \\  \\        *****\n");
    printf("\t * * *        /   ^^^^^^^^^^^^^^^^   \\       * * *\n");
    printf("\t*  *  *      /                        \\     *  *  *\n");
    printf("\t            /__________________________\\\n\n");
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
    printf("                                                ______\n");
    printf("                                                {####}       /\\\n");
    printf("                                                |####|    __/  \\\n");
    printf("                                         ____   {####}   / ____/\n");
    printf("                                        /    \\  |    |  / /\n");
    printf("                                       /  /\\  \\/      \\/ /\n");
    printf("                                      /  /  \\           /\n");
    printf("                                      \\_/    \\         /\n");
    printf("                                             |        /\n");
    printf("                                             |       |\n");
    printf("                    --------------------------       --------------------------               #                     #\n");
    printf("                    |                                                         |                #                   #\n");
    printf("                    |_  ____      ____________       ____________      ____  _|                #                  #\n");
    printf("                     * *   |    ------------- \\     / -------------    |   * *                 ########{:}########\n");
    printf("                    / /    |   ____________  \\ \\   / /  ____________   |    \\ \\                  ######{:}######\n");
    printf("                   * *    _|     <.$$$$$.> \\  \\ \\ / /  / <.$$$$$.>     |_    * *                  #####{:}#####\n");
    printf("                  / /    [ ]     <..$$$..>    ;;; ;;;    <..$$$..>     [ ]    \\ \\                  ####{:}####\n");
    printf("                 * *    _[ ]     <...$...>    +++ +++    <...$...>     [ ]_    * *                  ###{:}###\n");
    printf("                / /    [   ]                                           [   ]    \\ \\                 ###{:}###\n");
    printf("               * *     [_  ]      -------------------------------      [  _]     * *                    |\n");
    printf("              / /        [ ]     /                               \\     [ ]        \\ \\                   |\n");
    printf("             * *         [_]     | /\\_________________________/\\ |     [_]         * *                  |\n");
    printf("            / /            |     |/                             \\|     |            \\ \\              *****\n");
    printf("           * *             \\___________________________________________/             * *             * ***\n");
    printf("           \\ \\                                                                        \\ \\           / / |\n");
    printf("            * *                                                                        * *         * *  |\n");
    printf("             \\ \\                                                                        \\ \\       / /   |\n");
    printf("              * *          ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^                             * *     * *\n");
    printf("               \\ \\***-*-*-*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^                              \\ \\***/ /\n");
    printf("                \\_____*_*_*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^                               \\_____/\n");
    printf("                           ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
}

void fifthMonsterDisplayDeathP1(){
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
    printf("                   * *    _|       \\  /    \\        /    \\  /      |_      * *                              #\n");
    printf("                  / /    [ ]        \\/                    \\/       [ ]      \\ \\                  ####{:}####\n");
    printf("                 * *    _[ ]        /\\                    /\\       [ ]_      * *                  ###{:}###\n");
    printf("                / /    [   ]       /  \\                  /  \\      [   ]      \\ \\                 ###{:}###\n");
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

void fifthMonsterDisplayDeathP2(){
    printf("                                                ______\n");
    printf("                                                {####}       /\\\n");
    printf("                                                |####|    __/  \\\n");
    printf("                                         ____   {####}   / ____/\n");
    printf("                                        /    \\  |    |  / /\n");
    printf("                                       /  /\\  \\/      \\/ /\n");
    printf("                                      /  /  \\           /\n");
    printf("                                      \\_/    \\         /\n");
    printf("                                             |        /\n");
    printf("                                             |       |\n");
    printf("                    --------------------------       --------------------------               #                     #\n");
    printf("                    |                                                         |                #                   #\n");
    printf("                    |_  ____      ____________       ____________      ____  _|                #                  #\n");
    printf("                     * *   |    ------------- \\     / -------------    |   * *                 ########{:}########\n");
    printf("                    / /    |   ____________  \\ \\   / /  ____________   |    \\ \\                  ######{:}######\n");
    printf("                   * *    _|        \\  /   \\  \\ \\ / /  /   \\  /        |_    * *                  #####{:}#####\n");
    printf("                  / /    [ ]         \\/       ;;; ;;;       \\/         [ ]    \\ \\                  ####{:}####\n");
    printf("                 * *    _[ ]         /\\       +++ +++       /\\         [ ]_    * *                  ###{:}###\n");
    printf("                / /    [   ]        /  \\                   /  \\        [   ]    \\ \\                 ###{:}###\n");
    printf("               * *     [_  ]      -------------------------------      [  _]     * *                    |\n");
    printf("              / /        [ ]     /                               \\     [ ]        \\ \\                   |\n");
    printf("             * *         [_]     | /\\_________________________/\\ |     [_]         * *                  |\n");
    printf("            / /            |     |/                             \\|     |            \\ \\              *****\n");
    printf("           * *             \\___________________________________________/             * *             * ***\n");
    printf("           \\ \\                                                                        \\ \\           / / |\n");
    printf("            * *                                                                        * *         * *  |\n");
    printf("             \\ \\                                                                        \\ \\       / /   |\n");
    printf("              * *          ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^                             * *     * *\n");
    printf("               \\ \\***-*-*-*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^                              \\ \\***/ /\n");
    printf("                \\_____*_*_*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^                               \\_____/\n");
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
