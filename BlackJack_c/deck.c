#include "deck.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct Card {
    char* suit;
    char* num;
    int value;
    int score;
};

int* get_rand_deck() {
    int deck_size = 52;
    int *deck = malloc(deck_size * sizeof(int));
    srand((unsigned) time(NULL));
    
    int i, j;
    for(i = 0 ; i < deck_size; i++) {
        int r = rand() % 52 + 1;

        int has = 1;
        while(has > 0) {
            has = 0;
            r = rand() % 52 + 1;
            for(j = 0; j < i; j++) {
                if(r == deck[j])
                    has++;
            }
        }
        deck[i] = r;
    }
    
    return deck;
}

struct Card* get_deck() {
    int* num_deck = get_rand_deck();
    int size = 52;
    struct Card* deck = malloc(size * sizeof(struct Card));
    
    char* suits[4] = {"Spades", "Hearts", "Diamonds", "Clubs"};
    char* nums[13] = {"Ace", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King"};
    int score[13] = {11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10};
    
    int i;
    for(i = 0; i < size; i++) {
        int n = num_deck[i];
        if(num_deck[i] >= 1 && num_deck[i] <= 13)
            deck[i].suit = suits[0];
        if(num_deck[i] >= 14 && num_deck[i] <= 26) {
            n -= 13;
            deck[i].suit = suits[1];
        }
        if(num_deck[i] >= 27 && num_deck[i] <= 39) {
            n -= 26;
            deck[i].suit = suits[2];
        }
        if(num_deck[i] >= 40 && num_deck[i] <= 52) {
            n -= 39;
            deck[i].suit = suits[3];
        }
        
        deck[i].value = n;
        n--;
        deck[i].num = nums[n];
        deck[i].score = score[n];
    }
    
    return deck;
}
