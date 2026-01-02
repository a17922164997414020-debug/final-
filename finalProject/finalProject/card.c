#include "card.h"

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

void shuffle_p(card* wDeck) {
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
    if (strcmp(name, "dealer") == 0 || strcmp(name, "player") == 0)
        printf("(%d)", calculate(hand, Count));
    printf("\n");
}