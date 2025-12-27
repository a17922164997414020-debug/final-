#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include <ctype.h>
#include<windows.h>
typedef struct _card {
    const char* face;
    const char* suit;
    int value; 
} card;

void fillDeck(card* wDeck, const char* wFace[], const char* wSuit[]);
void shuffle(card* wDeck);
int calculate(card hand[], int cardCount);
void printHand(card hand[], int cardCount, const char* name);
void total(int totall);
void endprent(int moneyy,int again);
int roulate(int num[38]);
int main(void) {
    srand(time(NULL));

    card deck[52];
    card player[10];
    card dealer[10];
    int num;
    int a = 0;
	int item = 1;
    int playercard = 0;
    int dealercard = 0;
    int playerScore = 0;
    int dealerScore = 0;
    int money = 0;
    int total_money = 0;
    int again = 1;
    int roulette[38] = { 0,1,13,36,24,3,15,34,22,5,17,32,20,7,11,30,26,9,28,0,2,14,35,23,4,16,33,21,6,18,31,19,8,12,29,25,10,27 };
    const char* face[] = { "Ace", "Deuce", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King" };
    const char* suit[] = { "Hearts", "Diamonds", "Clubs", "Spades" };

    fillDeck(deck, face, suit);
    shuffle(deck);

    printf("how much money do you have? ");
    scanf(" %d", &total_money);
	printf("would you like to participate?black jack(1) roulette(0) ");
	scanf(" %d", &item);
    bb:
    while (total_money >= 0 && again == 1 && item == 0)
    {      
        printf("\n---roulette start ---");
        do
        {
            printf("\nhow much do you want to bet? ");
            scanf(" %d", &money);
            if (money <= 0 || money > total_money)
                printf("you dont have that munch money\n");
        } while (money <= 0 || money > total_money);
        
		int asw=0;
        int single;

        printf("even(0) or odd(1) or single bet(2)");
		scanf(" %d", &asw);
     
        if (asw == 2)
        {
            printf("which number do you want to bet on?(1-36) ");
            scanf(" %d", &single);    
        }
		num=roulate(roulette);
        printf("\nthe number is %d\n", roulette[num]);
        
        if (asw == 2)
        {
            if (roulette[num] == single)
            {
                printf("you win 35 times :) you win %d\n", money * 35);
                total_money += money * 35;
                total(total_money);
            }
            else
            {
                printf("you lose %d\n", money);
                total_money -= money;
				total(total_money);
            }

        }
        else
        {
            if (roulette[num] % 2 == 0 && roulette[num] != 0 && asw == 0)
            {
                printf("\neven number,you win 2 times :) you win %d\n", money );
                total_money += money;
                total(total_money);
            }
            else  if (roulette[num] % 2 == 1 && roulette[num] != 0 && asw == 1)
            {
                printf("\nodd number,you win 2 times :) you win %d\n", money);
                total_money += money;
                total(total_money);
            }
            else
            {
                printf("\nyou lose %d\n", money);
                total_money -= money;
                total(total_money);
            }

         
        }
        printf("\nDo you want to play again? yes(1) no(0) ");
        scanf(" %d", &again);
        if (total_money == 0)
            break;
    }
	//black jack/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    while (total_money >= 0 && again == 1&& item==1)
    {

        fillDeck(deck, face, suit);
        shuffle(deck);
        dealerScore = 0;
        playerScore = 0;
        dealercard = 0;
        playercard = 0;
        player[playercard++] = deck[a++];
        dealer[dealercard++] = deck[a++];
        player[playercard++] = deck[a++];
        dealer[dealercard++] = deck[a++];


        printf("\n---black jack start ---");
        do
        {
            printf("\nhow much do you want to bet? ");
            scanf(" %d", &money);
            if(money <= 0 || money > total_money)
				printf("you dont have that munch money\n");
		} while (money <= 0 || money > total_money);
            

        printf("\ndealer: [?], [%s of %s]\n", dealer[1].face, dealer[1].suit);


       
        while (1) {
            int choice = 0;
            printHand(player, playercard, "player");
            playerScore = calculate(player, playercard);

            if (playerScore > 21) {
                printf("player>21 dealer win,and you lose %d\n", money);
                total_money -= money;
				total(total_money);
                goto aa;
            }

            printf("need(1) or stop(0) ");
            scanf(" %d", &choice);

            if (tolower(choice) == 1) {
                player[playercard++] = deck[a++];
            }
            else {
                break;
            }
        }


        printf("\n--- dealer round ---\n");
        printHand(dealer, dealercard, "dealer");
		Sleep(1000);
        while (calculate(dealer, dealercard) < 17) {
            dealer[dealercard++] = deck[a++];
            Sleep(1000);
            printHand(dealer, dealercard, "dealer");
        }


        playerScore = calculate(player, playercard);
        dealerScore = calculate(dealer, dealercard);

        printf("\nplayer:%d \ndealer: %d \n", playerScore, dealerScore);
        Sleep(2000);
        if (dealerScore > 21) {
            printf("dealer>21,and player win:) you win %d\n", money);
			total_money += money;
            total(total_money);
        }
        else if (playerScore > dealerScore) {
            printf("player win :) you win %d\n", money);
			total_money += money;
            total(total_money);
        }
        else if (playerScore < dealerScore) {
            printf("dealer win :( you lose %d\n", money);
			total_money -= money;
            total(total_money);
        }
        else {
            printf("Draw\n");
        }
    aa:
            
		printf("\nDo you want to play again? yes(1) no(0) ");
		scanf(" %d", &again);
        if (total_money == 0)
            break;
    }
    if (total_money != 0 && again == 0)
    { 
        printf("you still have %d to play\n", total_money);
		printf("do you want play another item? black jack(1) roulette(0)");
        scanf(" %d", &item);
        if (item ==0||item==1 )
        {
            again = 1;
            goto bb;
        }
	}
    endprent(total_money,again);
	system("pause");
    return 0;
}

void fillDeck(card* wDeck, const char* wFace[], const char* wSuit[]) {
    for (int i = 0; i < 52; i++) {
        wDeck[i].face = wFace[i % 13];
        wDeck[i].suit = wSuit[i / 13];
        int val = (i % 13) + 1;
        if (val > 10) val = 10;
        if (val == 1) val = 11;
        wDeck[i].value = val;
    }
}

void shuffle(card* wDeck) {
    for (int i = 0; i < 52; i++) {
        int j = rand() % 52;
        card temp = wDeck[i];
        wDeck[i] = wDeck[j];
        wDeck[j] = temp;
    }
}
int calculate(card hand[], int cardCount) {
    int total = 0;
    int aceCount = 0;

    for (int i = 0; i < cardCount; i++) {
        total += hand[i].value;
        if (hand[i].value == 11) aceCount++;
    }
    while (total > 21 && aceCount > 0) {
        total -= 10;
        aceCount--;
    }
    return total;
}


void printHand(card hand[], int Count, const char* name) {
    printf("%s : ", name);
    for (int i = 0; i < Count; i++) {
        printf("[%s of %s] ", hand[i].face, hand[i].suit);
    }
    if(name == "dealer")
		printf("(%d)", calculate(hand,Count));
    printf("\n");
}
void endprent(int money,int again)
{
    if (money == 0 && again == 1)
        printf("you have no money to play\n");
    else
        printf("You have %d . welcome to come again next time!\n", money);
}
void total(int totall)
{

    printf("total money: %d\n", totall);
}
int roulate(int roulette[38])
{
    int number = rand() % 38 + 114;
    for (int i = 0; i <= number; i++)
    {
        printf(" %d", roulette[i % 38]);
        if (i < 20)
        {
            Sleep(20);
        }
        else if (i >= 20 && i < 40)
        {
            Sleep(30);
        }
        else if (i >= 40 && i < 60)
        {
            Sleep(40);
        }
        else if (i >= 60 && i<80)
        {
            Sleep(50);
        }
        else if (i >= 80 && i < 100)
        {
            Sleep(70);
        }
        else if (i>=100 && i-number < -10)
        {
            Sleep(80);
		}
        else if (i- number >= -10 && i- number < -5)
        {
            Sleep(150);
        }
        else
        {
			Sleep(200);
        }
        
    }
    return number % 38;

}