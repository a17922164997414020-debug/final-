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
void total(int totall)
{
   
    printf("total money: %d\n", totall);
}
int main(void) {
    srand(time(NULL));

    card deck[52];
    card player[10];
    card dealer[10];
    int a = 0;
    int playercard = 0;
    int dealercard = 0;
    int playerScore = 0;
    int dealerScore = 0;
    int money = 0;
    int total_money = 0;
    int again = 1;
    const char* face[] = { "Ace", "Deuce", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King" };
    const char* suit[] = { "Hearts", "Diamonds", "Clubs", "Spades" };

    fillDeck(deck, face, suit);
    shuffle(deck);

    printf("how much money do you have? ");
    scanf(" %d", &total_money);
    while (total_money >= 0 && again == 1)
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


        printf("\n--- start ---");
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
    if(total_money == 0 && again==1)
		printf("you have no money to play\n");
    else
		printf("You have %d . welcome to come again next time!\n",total_money);

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