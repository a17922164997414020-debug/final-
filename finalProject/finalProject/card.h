#ifndef CARDS_H
#define CARDS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct _card {
    const char* face;
    const char* suit;
    int value;
} card;

void fillDeck(card* wDeck, const char* wFace[], const char* wSuit[]);
void shuffle_p(card* wDeck);
int calculate(card hand[], int cardCount);
void printHand(card hand[], int Count, const char* name);

#endif