#include "deck.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 * creates a full standard deck with 52 cards, shuffled without repeats
 */

// card struct to keep track of each cards information
struct Card {
    char* suit;
    char* num;
    int value;
    int score;
};

/*
 * gets an array of random non-repeating ints to later turn into a deck
 * @return  a random array of ints
 */
int* get_rand_deck() {
    int deck_size = 52; // size of deck
    int *deck = malloc(deck_size * sizeof(int));
    srand((unsigned) time(NULL));   // seeds RNG
    
    int i, j;
    for(i = 0 ; i < deck_size; i++) {
        int r = rand() % 52 + 1;    // gets a random nymber between 1-52

        int has = 1;
        // checks to make sure random number doesn't already apear in the array
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

/*
 * creates a randomly shuffled deck of cards
 * @return  array of Card structs
 */
struct Card* get_deck() {
    int* num_deck = get_rand_deck();    // gets random array of ints
    int size = 52;
    struct Card* deck = malloc(size * sizeof(struct Card));
    
    // sets values to represent the values of the Cards
    char* suits[4] = {"Spades", "Hearts", "Diamonds", "Clubs"};
    char* nums[13] = {"Ace", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King"};
    int score[13] = {11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10};
    
    int i;
    // creates a deck of cards based off of the random int array
    for(i = 0; i < size; i++) {
        int n = num_deck[i];
        // spades
        if(num_deck[i] >= 1 && num_deck[i] <= 13)
            deck[i].suit = suits[0];
        // hearts
        if(num_deck[i] >= 14 && num_deck[i] <= 26) {
            n -= 13;
            deck[i].suit = suits[1];
        }
        // diamonds
        if(num_deck[i] >= 27 && num_deck[i] <= 39) {
            n -= 26;
            deck[i].suit = suits[2];
        }
        // clubs
        if(num_deck[i] >= 40 && num_deck[i] <= 52) {
            n -= 39;
            deck[i].suit = suits[3];
        }
        // sets current cards information
        deck[i].value = n;
        n--;
        deck[i].num = nums[n];
        deck[i].score = score[n];
    }
    
    return deck;
}
