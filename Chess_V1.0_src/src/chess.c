#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"
#include "rules.h"
#include <ctype.h>
#include <stdbool.h>


int main(int argc, const char * argv[]) {
/**
struct Square board[8][8];
        struct Square backupBoard[8][8];
        initializeBoard(board);
        printBoard(board);
        printf("\nWelcome to Chess! Currently, there is only a 2 person option for playing.\n");
        printf("White pieces will be specified with upper case characters,\nwhile black pieces will be specified with lower case characters.\n");
        printf("Move notation will be specified by starting square then destination square (ex. a2a4)\n");
        bool checkMate = false;
        struct Move *move1 = malloc(sizeof(struct Move));
        int currColor = 0;
        char redoChoice[10]; // Buffer for redo input

        while(!checkMate){
            if(isStalemate(board, currColor)) {
                printf("Stalemate! Ending Game.\n");
                break;
            }

            deepCopyBoard(board, backupBoard); // Backup the board before the move

            if(possibleCheck(board, currColor)){ // First check for check
                printf("Player is in Check!\n");
                if(!canEscapeCheck(board, currColor)){ // No escape; game over
                    printf("Checkmate!! Game Over!\n");
                    checkMate = true;
                    break;
                }
                else{
                    bool check = false;
                    while(!check){
                        printBoard(board);
                        getPlayerMove(move1);
                        if(outOfCheck(board, move1, currColor)){ // Test if player move removes check
                            makeMove(board, move1, currColor);
                            printBoard(board);
                            printf("Do you want to redo this move? (yes/no): ");
                            fgets(redoChoice, sizeof(redoChoice), stdin);
                            redoChoice[strcspn(redoChoice, "\n")] = '\0'; // Remove newline character
                            if (strcmp(redoChoice, "yes") == 0) {
                                deepCopyBoard(backupBoard, board); // Restore the backup
                                printf("Redoing the move...\n");
                                printBoard(board);
                                continue; // Skip changing the player
                            }
                            check = true;
                        }
                        else{
                            printf("Try again!\n"); // Redo loop if not out of check
                        }
                    }
                }
            }
            else{
                bool invalidMove = false;
                while(!invalidMove){
                    getPlayerMove(move1);
                    if(isValidMove(board, move1, currColor)){
                        makeMove(board, move1, currColor);
                        printBoard(board);
                        printf("Do you want to redo this move? (yes/no): ");
                        fgets(redoChoice, sizeof(redoChoice), stdin);
                        redoChoice[strcspn(redoChoice, "\n")] = '\0'; // Remove newline character
                        if (strcmp(redoChoice, "yes") == 0) {
                            deepCopyBoard(backupBoard, board); // Restore the backup
                            printf("Redoing the move...\n");
                            printBoard(board);
                            continue; // Skip changing the player
                        }
                       invalidMove = true;
                    }
                    else{
                        printf("Invalid Move! Try again!\n");
                    }
                }
            }
            currColor = 1 - currColor; // Switch player after a valid move
        }
return 0;

struct Square board [8][8];
    struct Move* rMove = malloc(sizeof(struct Move));
            initializeBoard(board);
            printBoard(board);
            printf("%d",evaluateBoard(board, 0));
            printf("\nWelcome to Chess! Currently, there is only a 2 person option for playing.\nA computer option will be added soon!\n");
            printf("White pieces will be specified with upper case characters,\nwhile black pieces will be specifed with lower case characters.\n");
            printf("Move notation will be specified by starting square then destination square (ex. a2a4)\n");
            bool checkMate = false;
            struct Move * move1 = malloc(sizeof(struct Move));
            int currColor;
            currColor = 0;
            while(!checkMate){
                if(isStalemate(board,currColor)){
                    printf("Stalemate! Ending Game.");
                    break;
                }
                if(possibleCheck(board,currColor)){ // First check for check
                    printf("Player is in Check!\n");
                    if(!canEscapeCheck(board,currColor)){ // No escape; game over
                        printf("Checkmate!! Game Over!\n");
                        checkMate = true;
                        return 0;
                    }
                    else{
                        bool check = false;
                        while(!check){
                            printBoard(board);
                            getPlayerMove(move1);
                            if(outOfCheck(board,move1,currColor)){ // Test if player move removes check
                                makeMove(board,move1,currColor);
                                check = true;
                                printBoard(board);
                                
                            }
                            else{
                                printf("Try again!\n"); // Redo loop if not out of check
                            }
                        }
                    }
                }
                else{ //
                    bool invalidMove = false;
                    while(!invalidMove){
                        getPlayerMove(move1);
                        if(isValidMove(board,move1,currColor)){
                            invalidMove = true;
                            makeMove(board,move1,currColor);
                            printBoard(board);
                            break;
                        }
                        else{printf("Invalid Move! Try again!\n");}
                    }
                }
                currColor = 1;
                if(isStalemate(board,currColor)){
                    printf("Stalemate! Ending Game.");
                    break;
                }
                if(!canEscapeCheck(board,currColor)){
                    printf("Game Over!");
                    checkMate = true;
                    return 0;
                }
                move1 = selectMove(board, currColor);
                makeMove(board, move1, currColor);
                printBoard(board);
                if(!canEscapeCheck(board,currColor)){
                    printf("Game Over!");
                    checkMate = true;
                    return 0;
                }
                currColor = 0;
            }
    return 0;
**/
	if(remove("log.txt")==0){
		printf("log.txt deleted.\n");
	}
	else{
		printf("no log.txt to clear\n");
	}
	char input[100]; // Assuming input will not exceed 100 characters
    printf("Welcome to Chess, Please select game mode (PVP,PVE): ");
    fgets(input, sizeof(input), stdin); // Read string from standard input
    input[strcspn(input,"\n")]='\0';
	while(strcmp(input,"PVP")!=0 && strcmp(input,"PVE")!=0){
	    printf("Try again, Please select game mode (PVP,PVE): ");
    	fgets(input, sizeof(input), stdin);
	    input[strcspn(input,"\n")]='\0';
	}

	if(strcmp(input,"PVP")==0){
		PlayerMode_1();
	}
	else{
		PlayerMode_2();
	}
	//PlayerMode_2();
	return 0;

}
