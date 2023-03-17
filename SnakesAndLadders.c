// Jak Cullinane

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct boardSquare {
    
    int squareNumber;
    struct boardSquare *previousBoardSquare;
    struct boardSquare *nextBoardSquare;
    struct boardSquare *snakeOrLadder;
    int isFinal;
};


// CREATE BOARD SQUARE
struct boardSquare *createBoardSquare (const int number) {
    
    struct boardSquare *newBoardSquare = NULL;
    newBoardSquare = malloc(sizeof(struct boardSquare));
    newBoardSquare->squareNumber = number;
    newBoardSquare->previousBoardSquare = NULL;
    newBoardSquare->nextBoardSquare = NULL;
    newBoardSquare->snakeOrLadder = NULL;
    return newBoardSquare;
}


// BOARD MAKER
struct boardSquare *boardMaker (const int input) {
    
    // creating the head
    int sizeOfBoard = input;
    struct boardSquare *head = NULL;
    head = malloc(sizeof(struct boardSquare));
    head->squareNumber = 1;
    head->previousBoardSquare = NULL;
    head->nextBoardSquare = NULL;
    head->snakeOrLadder = NULL;
    
    struct boardSquare *previous = head;
    
    // creating everything else
    for (int i = 2; i <= sizeOfBoard; ++i) {
        struct boardSquare *current = createBoardSquare(i);
        current = malloc(sizeof(struct boardSquare));
        current->squareNumber = i;
        current->previousBoardSquare = previous;
        
        previous->nextBoardSquare = current;
        
        if (i == sizeOfBoard) {
            current->isFinal = 1;
        } else {
            current->isFinal = 0;
        }
        
        previous = current;
    }
     
    return head;
}


// SNAKE AND LADDER MAKER
const char* snakeAndLadderMaker(int snakes, int ladders, int boardSize, struct boardSquare *head) {
    
    srand(time(NULL));
    char *report = malloc(5000);
    char *temp = malloc(500);
    
    strcat(report, "\nSNAKES\n========================================\n");
    for (int i = 0; i < snakes; i++) {
        
        // finding the head of the snake and the tail
        int headOfSnake = rand()%(boardSize-4); // rand starts at 0 so 64-4 means I have a range of 60 values from 0 to 60
        headOfSnake += 3;                       // here I'm pushing that range up so I have a range of 60 values from 3 to 63
        int tailOfSnake = rand()%9;
        tailOfSnake++;                          // making sure it at least moves the player up 1 space
        
        // making sure there isn't already a snake or ladder on the square 
        while (1) {
            struct boardSquare *temp = head;
            for (int j = 0; j < headOfSnake; j++) {
                temp = temp->nextBoardSquare;
            }
            
            if (temp->snakeOrLadder != NULL) {
                headOfSnake = rand()%(boardSize-4);
                headOfSnake += 2;
            } else {
                break;
            }
        }
        
        // making sure the tail isn't out of range;
        if (tailOfSnake >= headOfSnake) {
            int fix = tailOfSnake - headOfSnake;
            tailOfSnake = tailOfSnake - (fix+1);
        }
        
        // moving to the head of the snake
        struct boardSquare *start1 = head;
        for (int j = 1; j <= headOfSnake; j++) {
           start1 = start1->nextBoardSquare;
        }
        
        // moving to the tail of the snake
        struct boardSquare *end1 = start1;
        for (int x = 1; x <= tailOfSnake; x++) {
            end1 = end1->previousBoardSquare;
        }
        
        // creating the connection between the head and the tail of the snake
        start1->snakeOrLadder = end1;
        
        snprintf(temp, 500, "Snake at square %d and tail at square %d\n", start1->squareNumber, end1->squareNumber);
        strcat(report, temp);
    }
    
    strcat(report, "\nLADDERS\n========================================\n");
    for (int i = 0; i < ladders; i++) {
        
        // finding the bottom and the top of the ladder
        int bottomOfLadder = rand()%(boardSize-4); // rand starts at 0 so 64-4 means I have a range of 60 values from 0 to 60
        bottomOfLadder += 2;                       // here I'm pushing that range up so I have a range of 61 values from 2 to 62
        int topOfLadder = rand()%9;    
        topOfLadder++;                             // making sure the ladder moves at least 1 space
        topOfLadder += bottomOfLadder;
        
        // making sure there isn't already a snake or ladder on the square 
        while (1) {
            struct boardSquare *temp = head;
            for (int j = 0; j < bottomOfLadder; j++) {
                temp = temp->nextBoardSquare;
            }
            
            if (temp->snakeOrLadder != NULL) {
                bottomOfLadder = rand()%(boardSize-4);
                bottomOfLadder += 2;
                topOfLadder = rand()%9;    
                topOfLadder++;
                topOfLadder += bottomOfLadder;

            } else {
                break;
            }
        }
        
        // making sure the top isn't out of range
        if (topOfLadder > boardSize) {
            topOfLadder = boardSize;
        }
        
        // moving to the bottom of the ladder
        struct boardSquare *start2 = head;
        for (int j = 0; j < bottomOfLadder; j++) {
           start2 = start2->nextBoardSquare;
        }
        
        
        // moving to the top of the ladder
        struct boardSquare *end2 = head;
        for (int x = 0; x < topOfLadder; x++) {
            end2 = end2->nextBoardSquare;
        }
        
        snprintf(temp, 500, "Ladder at square %d and top at square %d\n", bottomOfLadder+1, topOfLadder+1);
        strcat(report, temp);
        
        // creating the connection between the bottom and top of the ladder
        start2->snakeOrLadder = end2;
    }
    
    return report;
}


// DICE ROLL
int diceRoll() {
    
    int roll = rand()%6;
    roll += 1;
    return roll;
}


// PLAYER MOVEMENT
struct boardSquare *playerMovement(int diceRoll, struct boardSquare *currentPosition) {
    
    // move forward the amount the dice rolled
    struct boardSquare *newPosition = currentPosition;
    for (int i = 0; i < diceRoll; i++) {
        newPosition = newPosition->nextBoardSquare;
    }
    
    return newPosition;
}


void printBoard(int boardSize, struct boardSquare *head, int cursor) {
    
    struct boardSquare *currentPosition = head;
    char *view = malloc(5000);
    char *temp = malloc(500);
    strcat(view, "");
    strcat(temp, "");
    int snakes = 0;
    int ladders = 0;
    
    // iterating through every square on the board
    for (int i = 1; i <= boardSize; i ++) {    
        
        // if the cursor is on this square add <P> in front
        if (i == cursor) {
            int position = currentPosition->squareNumber;
            
            // check if the current position has a snake or ladder on it
            if (currentPosition->snakeOrLadder != NULL) {
                int movePosition = currentPosition->snakeOrLadder->squareNumber;
                
                // check if its a snake or ladder
                if (movePosition < position) {
                    snprintf(temp, 500, "<P>    [%d] SNAKE DOWN TO %d\n", position, movePosition);
                } else {
                    snprintf(temp, 500, "<P>    [%d] LADDER UP TO %d\n", position, movePosition);
                }
                
            } else {
                snprintf(temp, 500, "<P>    [%d]\n", position);
            }
            
            strcat(view, temp);
            
            // if there isn't another square break
            if (currentPosition->nextBoardSquare == NULL) {
                break;
                
            // otherwise move to the next square    
            } else {
                currentPosition = currentPosition->nextBoardSquare;
            }
        
        // if the cursor isn't on this square    
        } else {
            int position = currentPosition->squareNumber;
            
            // check if the current position has a snake or ladder on it
            if (currentPosition->snakeOrLadder != NULL) {
                int movePosition = currentPosition->snakeOrLadder->squareNumber;
                
                // check if its a snake or ladder
                if (movePosition < position) {
                    snprintf(temp, 500, "       [%d] SNAKE DOWN TO %d\n", position, movePosition);
                } else {
                    snprintf(temp, 500, "       [%d] LADDER UP TO %d\n", position, movePosition);
                }
                
            } else {
                snprintf(temp, 500, "       [%d]\n", position);
            }
            
            strcat(view, temp);
            
            // if there isn't another square break
            if (currentPosition->nextBoardSquare == NULL) {
                break;
            
            // otherwise move to the next square    
            } else {
                currentPosition = currentPosition->nextBoardSquare;
            }
        }
    }
    
    printf("%s\n", view);
    free(view);
    free(temp);
}


int main() {
    
    // VARIABLES
    int cursor = 1;
    int turn = 0;
    struct boardSquare *currentPosition;
    struct boardSquare *oldPosition;
    struct boardSquare *head;
    int ladders = 0;
    int snakes = 0;
    int boardSize = rand()%32;
    boardSize += 32;
    int dice= 0;
    int game = 1;
    FILE *report;
    char progressReport[5000];
    char temp[500];
    
    // CREATING BOARD
    head = boardMaker(boardSize);
    currentPosition = head;
    oldPosition = currentPosition;
    
    printf("\n==================\nSnakes and Ladders\n==================\n\n");

    // CREATING SNAKES AND LADDERS ON THE BOARD
    while (1) {
        char input[8];
        printf("How many snakes would you like (max 5): ");
        fgets(input, 8, stdin);
        snakes = atoi(input);
        if (snakes > 0 && snakes <= 5) {
            break;
        } else {
            printf("\nPlease enter a valid input\n");
        }
    }
    while (1) {
        char input2[8];
        printf("How many ladders would you like (max 5): ");
        fgets(input2, 8, stdin);
        ladders = atoi(input2);
        if (ladders > 0 && ladders <= 5) {
            break;
        } else {
            printf("\nPlease enter a valid input\n");
        }
    }
    
    printf("\n===========================================\n\n");
    
    snprintf(temp, 500, "\nDETAILS\n==============\nBoard Size: %d\nSnakes: %d\nLadders: %d\n==============\n", boardSize, snakes, ladders);
    strcat(progressReport, temp);
    strcat(progressReport, snakeAndLadderMaker(snakes, ladders, boardSize, head));
    snprintf(temp,  500, "\nPLAYER MOVEMENT\n========================================\n");
    strcat(progressReport, temp);
    
    // THE GAME
    while (game = 1) {
        
        cursor = currentPosition->squareNumber;  
        printBoard(boardSize, head, cursor);
        printf("\n===========================================\n\n");

        // DICE ROLL
        while (1) {
            char input3[4];
            int choice = 0;
            printf("\nRoll Dice [1]\nExit [2]\n>> ");
            fgets(input3, 4, stdin);
            choice = atoi(input3);
            
            // roll dice
            if ( choice == 1) {
            printf("\nRolling Dice... ");
            turn++;
            dice = diceRoll();
            printf("You got %d\n", dice);
            break;
            
            // exit game
            } else if (choice == 2) {
                printf("\nYou exited the game\n");
                
                // REPORT
                report = fopen("JakCullinane_Assignment2.txt","w");

                if (report == NULL) {
                    printf("Error!");   
                    exit(1);             
                }
    
                fprintf(report, "%s", progressReport);
                fclose(report);
                
                exit(0);
            
            // error    
            } else {
                printf("\nPlease enter a valid input\n");
            }
        }
        
        // WIN CONDITION
        if (currentPosition->squareNumber+dice >= boardSize) {
            printf("Congratulations you passed the final square of %d and won!\n", boardSize);
            break;
        }
        
        // MOVING POSITION
        currentPosition = playerMovement(dice, currentPosition);
        oldPosition = currentPosition;
        cursor = currentPosition->squareNumber; 
        snprintf(temp, 500, "Turn:%d Dice:%d Position:%d \n", turn, dice, cursor);
        strcat(progressReport, temp);
        
        // CHECKING FOR SNAKE OR LADDER
        while (1) {
            
            // if there is a snake or ladder
            if (currentPosition->snakeOrLadder != NULL) {
                
                printBoard(boardSize, head, cursor);
                currentPosition = currentPosition->snakeOrLadder;
                cursor = currentPosition->squareNumber; 
                
                if (oldPosition->squareNumber > currentPosition->squareNumber) {
                    printf("\nYou landed on a snake! You moved from position %d to %d\n", oldPosition->squareNumber, currentPosition->squareNumber);
                    snprintf(temp, 500, "You landed on a snake! You moved from position %d to %d\n", oldPosition->squareNumber, currentPosition->squareNumber);
                    strcat(progressReport, temp);
                    
                } else if (oldPosition->squareNumber < currentPosition->squareNumber) {
                    printf("\nYou landed on a ladder! You moved from position %d to %d\n", oldPosition->squareNumber, currentPosition->squareNumber);
                    snprintf(temp, 500, "You landed on a ladder! You moved from position %d to %d\n", oldPosition->squareNumber, currentPosition->squareNumber);
                    strcat(progressReport, temp);
                }
                
                oldPosition = currentPosition;
            
            // if there isn't a snake or ladder    
            } else {
                break;
            }
        }

    }
    
    // REPORT
    report = fopen("JakCullinane_Assignment2.txt","w");

    if (report == NULL) {
        printf("Error!");   
        exit(1);             
    }
    
    fprintf(report, "%s", progressReport);
    fclose(report);
    
    // EXIT
    return 0;
}



