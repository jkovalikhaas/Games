#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deck.h"

extern struct Card* get_deck(void);

typedef struct Card {
    char* suit;
    char* num;
    int value;
    int score;
} card;

typedef struct Player {
    char name[20];
    int money;
    int wins;
    int losses;
    double ratio;
    int bjs;
    int restarts;
} player;

void player_info(struct Player p) {
    printf("%s's Stats\n", p.name);
    printf("\tMoney \t\t$%d\n", p.money);
    printf("\tWins \t\t%d\n", p.wins);
    printf("\tLosses \t\t%d\n", p.losses);
    printf("\tWin\\Loss Ratio \t%3.2f\n", p.ratio);
    printf("\tBlack Jacks \t%d\n", p.bjs);
    printf("\tRestarts \t%d\n", p.restarts);
}

char* get_file_name(char* name) {
    char* file = malloc(sizeof(char) * 30);
    strcat(file, name);
    strcat(file, ".txt");

    return file;
}

void write_to_file(struct Player p) {
    char* file = get_file_name(p.name);

    FILE *fp;
    fp = fopen(file, "w+");
   
    fprintf(fp, "%s\n", p.name);
    fprintf(fp, "%d\n", p.money);
    fprintf(fp, "%d\n", p.wins);
    fprintf(fp, "%d\n", p.losses);
    fprintf(fp, "%lf\n", p.ratio);
    fprintf(fp, "%d\n", p.bjs);
    fprintf(fp, "%d\n", p.restarts);
    fclose(fp);
}

struct Player create_player(char* name) {
    struct Player n_player;
    
    int i;
    for(i = 0; i < 20; i++) {
        n_player.name[i] = name[i];
    }
    n_player.money = 100;
    n_player.wins = 0;
    n_player.losses = 0;
    n_player.ratio = 0;
    n_player.bjs = 0;
    n_player.restarts = 0;
    
    write_to_file(n_player);
    
    return n_player;
}

// check if file exists
int file_exists(const char* file){
    /* try to open file to read */
    FILE *fp;
    if ((fp = fopen(file, "r"))) {
        fclose(fp);
        return 1;
    }
    return 0;
}

struct Player read_player(char* f) {
    struct Player p;
    
    char* file = get_file_name(f);
    
    FILE *fp;
    if(file_exists(file) == 0)
        create_player(f);
    fp = fopen(file, "r");
    
    fscanf(fp, "%s", p.name);
    fscanf(fp, "%d", &p.money);
    fscanf(fp, "%d", &p.wins);
    fscanf(fp, "%d", &p.losses);
    fscanf(fp, "%lf", &p.ratio);
    fscanf(fp, "%d", &p.bjs);
    fscanf(fp, "%d", &p.restarts);
    fclose(fp);
    
    return p;
}

int print_score(struct Card* d, int place) {
    int i, score = 0;
    for(i = 0; i <= place; i++)
        score += d[i].score;
    
    printf("\t\tCurrent Score: %d", score);
    
    if(score == 21) {
        if(place < 2) {
            printf("\nBlack Jack!\n");
            return 2;
        }
        printf("\n");
        return 1;
    }
    
    for(i = 0; i <= place; i++) {
        if(d[i].value == 1) {
            score -= 10;
            printf(" or %d", score);
            if(score == 21) {
                if(place < 2) {
                    printf("\nBlack Jack!\n");
                    return 2;
                }
                printf("\n");
                return 1;
            }
        }
    }

    if(score > 21) {
        printf("\nYou Bust!\n");
        return 3;
    }
    
    printf("\n");
    return 0;
}

int print_hand(struct Card* d, int place) {
    printf("Your Hand:\n");
    
    int i;
    for(i = 0; i <= place; i++) {
        printf("\t%s of %s\n", d[i].num, d[i].suit);
    }
    return print_score(d, place);
}

int comp_score(struct Card* d, int p, int new_p) {
    int i, score = 0;
    for(i = p; i <= new_p; i++)
        score += d[i].score;
    
    return score;
}

int computer(struct Card* d, int place) {
    place++;
    int new_place = place + 1, i;
    
    int end = 0, score = comp_score(d, place, new_place);
    while(end == 0) {
        if(score == 21)
            end = 1;
        else if(score > 21) {
            for(i = place; i <= new_place; i++) {
                if(score > 21 && d[i].value == 1)
                    score -= 10;
            }
            if(score > 21)
                end = 1;
        }
        else {
            new_place++;
            if((score + d[new_place].score) <= 21) {
                score = comp_score(d, place, new_place);
            } else {
                new_place--;
                end = 1;
            }
        }
    }
    
    printf("\nComputer Hand: \n");
    for(i = place; i <= new_place; i++)
        printf("\t%s of %s\n", d[i].num, d[i].suit);
    
    return score;
}

int main(int argc, const char * argv[]) {
    printf("Welcome to Black Jack!\n\n");
    
    printf("\tUsername: ");
    char* user = malloc(sizeof(char) * 20);
    scanf("%s", user);
    printf("\n");
    
    struct Player player = read_player(user);
    
    printf("Welcome %s\n", player.name);
    
    int finish = 0;
    do {
        player_info(player);
        printf("\n");
        
        printf("\nYour Money: %d\n\n", player.money);
        
        struct Card* deck = get_deck();
        
        int place = 1;  // place in deck (starts at 1 for two card hand (1 and 0))
        int bet = 0;
        
        char done = print_hand(deck, place);;
        while(done == 0) {
            int valid = 0;
            while(valid == 0) {
                printf("\nHow much to bet? ");
                
                int temp = 0;
                int status = scanf("%d", &temp);
                while(status != 1) {
                    while((temp = getchar()) != EOF && temp != '\n');
                    printf("Invalid Bet\n");
                    printf("How much to bet? ");
                    status = scanf("%d", &temp);
                }
                
                if(temp > player.money) {
                    printf("You don't have enough money.\n");
                    valid = 0;
                } else if (temp < 0) {
                    printf("Invalid Bet\n");
                    valid = 0;
                } else {
                    bet += temp;
                    valid = 1;
                }
                
                printf("\n");
            }
            
            printf("\nHit or Stay (h/s) ");
            char hit[1] = " ";
            scanf("%s", hit);
            printf("\n");
            
            if(hit[0] == 'h') {
                place++;
                done = print_hand(deck, place);
            } else if(hit[0] == 's')
                done = 1;
            else
                printf("Invalid Input\n");
        }
        
        int i, final_score = 0;
        for(i = 0; i <= place; i++)
            final_score += deck[i].score;
        
        for(i = 0; i <= place; i++) {
            if(final_score > 21 && deck[i].value == 1)
                final_score -= 10;
        }
        
        printf("Final Score: %d\n", final_score);
        
        if(done > 1) {
            if(done == 2) {
                player.wins++;
                player.bjs++;
                printf("\nYou Win!\n");
                player.money *= 2;
            }
            if(done == 3) {
                player.losses++;
                printf("\nYou Lose!\n");
                player.money -= bet;
            }
        }
        else {
            int c_score = computer(deck, place);
            printf("\nComputer Score: %d\n", c_score);
            if(c_score > 21)
                printf("Computer Busts!\tYou Win!\n");
            else if(c_score < final_score){
                player.wins++;
                printf("\nYou Win!\n");
                player.money += bet;
            }
            else if(c_score >= final_score){
                player.losses++;
                printf("\nYou Lose!\n");
                player.money -= bet;
            }
            
        }
        printf("\n");
        
        if(player.money <= 0) {
            printf("You have ran out of money...\n");
            printf("Would you like to restart with $100 and keep you stats? (y/n) ");
            
            char restart[1] = " ";
            scanf("%s", restart);
            printf("\n");
            
            if(restart[0] == 'y') {
                player.restarts++;
                player.money = 100;
            } else
                finish = 1;
        } else {
            printf("\nPlay Again? (y/n) ");
            
            char play[1] = " ";
            scanf("%s", play);
            printf("\n");
            
            if(play[0] == 'y')
                finish = 0;
            else if(play[0] == 'n')
                finish = 1;
            else
                finish = 0;
        }
        
        if(player.losses == 0)
            player.ratio = 100.0;
        else
            player.ratio = (double) player.wins / (double) player.losses;
        
        write_to_file(player);
    } while(finish == 0);
    
    player_info(player);
    printf("\n");
    
    return 0;
}
