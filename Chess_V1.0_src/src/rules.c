#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rules.h"
#include <ctype.h>
#include <stdbool.h>

bool AI;

void getPlayerMove_TEST(struct Move *move, char *input){
    do {
	printf("\nInputing: %s",input);
        input[strcspn(input, "\n")] = '\0'; // Remove trailing newline, if present(i have no clue how it works stack exchange )
	input[strcspn(input,"\r")] = '\0';
    } while (!isValidMoveFormat(input));

    // Assuming valid input, now parse into the move structure
    move->initialCol = input[0] - 'a';
    move->initialRow = '8' - input[1];
    move->destinationCol = input[2] - 'a';
    move->destinationRow = '8' - input[3];

}

int getPlayerMove_rint(struct Move *move) {
    char input[10]; // Buffer for input string
	int result = 0;
    do {
        printf("Enter your move (e.g., e2e4): ");
        fgets(input, sizeof(input), stdin); //

        input[strcspn(input, "\n")] = '\0'; // Remove trailing newline, if present(i have no clue how it works stack exchange )
	input[strcspn(input,"\r")] = '\0';
	if(strcmp(input,"exit")==0){
	result = 1;
	return result;
	}
	if(strcmp(input,"undo")==0){
	result = 2;
	return result;
	}
    } while (!isValidMoveFormat(input));

    // Assuming valid input, now parse into the move structure
    move->initialCol = input[0] - 'a';
    move->initialRow = '8' - input[1];
    move->destinationCol = input[2] - 'a';
    move->destinationRow = '8' - input[3];
	return result;
}

int getPlayerMove_rint_TEST(struct Move *move, char *input) {
    int result = 0;
   do {
	printf("\nInputing: %s",input);
        input[strcspn(input, "\n")] = '\0'; // Remove trailing newline, if present(i have no clue how it works stack exchange )
	input[strcspn(input,"\r")] = '\0';
	if(strcmp(input,"exit")==0){
	result = 1;
	return result;
	}
    } while (!isValidMoveFormat(input));


    // Assuming valid input, now parse into the move structure
    move->initialCol = input[0] - 'a';
    move->initialRow = '8' - input[1];
    move->destinationCol = input[2] - 'a';
    move->destinationRow = '8' - input[3];
	return result;
}

char gameLog[MAX_ROUNDS * 2][LOG_LENGTH];   // one moves takes one line, and each round contains 2 moves
char algeLog[MAX_ROUNDS * 2][10]; // algebraic notation version of log file
int logCount = 0;   // initialize the counter for log

void addLog(int logType, int pieceType, int currColor, int startRow, int startCol, int destRow, int destCol){
    if (logCount >= MAX_ROUNDS) {
        printf("Log storage is full, max storage is %d rounds.\n", MAX_ROUNDS);
        return;
    }

    char startRank = '8' - startRow;
    char destRank = '8' - destRow;
    char startFile = 'a' + startCol;
    char destFile = 'a' + destCol;

    const char* pieceTypes[] = {"pawn", "rook", "bishop", "knight", "queen", "king"};
    const char* pieceTypesSimp[] = {"P", "R", "B", "N", "Q", "K"};
    const char* colors[] = {"White", "Black"};

    char* pieceMovement;
    pieceMovement = (char*)malloc(75 * sizeof(char));  // allocate memory for the temporary string to be added to the log
    char* algeMove;
    algeMove = (char*)malloc(20 * sizeof(char));

    switch(logType){
        case 0: // regular move
            sprintf(pieceMovement, "%s %s moved from %c%c to %c%c.", 
                colors[currColor], pieceTypes[pieceType],
                startFile, startRank, destFile, destRank);
            
            if (pieceType == 0){
                sprintf(algeMove, "%c%c", destFile, destRank);
            } 
            else if (pieceType >= 1 && pieceType <= 5) {
                sprintf(algeMove, "%s%c%c", pieceTypesSimp[pieceType], destFile, destRank);
            }
            break;
        case 1: // capture
            sprintf(pieceMovement, "%s %s captured from %c%c to %c%c.", 
                colors[currColor], pieceTypes[pieceType],
                startFile, startRank, destFile, destRank);

            if (pieceType == 0){
                sprintf(algeMove, "%cx%c%c", startFile, destFile, destRank);
            } 
            else if (pieceType >= 1 && pieceType <= 5) {
                sprintf(algeMove, "%sx%c%c", pieceTypesSimp[pieceType], destFile, destRank);
            }
            break;
        case 2: // castling
            sprintf(pieceMovement, "%s performed a %s side castling.", 
                colors[currColor], pieceTypes[pieceType]);

            if (pieceType == 4){
                strcpy(algeMove, "O-O-O");
            } 
            if (pieceType == 5) {
                strcpy(algeMove, "O-O");
            break;
            }
        case 3: // promotion
            sprintf(pieceMovement, "%s pawn at %c%c promoted to %s.", 
                colors[currColor], destFile, destRank, pieceTypes[pieceType]);

            if (pieceType == 1 || pieceType == 2 || pieceType == 3 || pieceType == 4){
                sprintf(algeMove, "%c%c=%s", destFile, destRank, pieceTypesSimp[pieceType]);
            }
            break;
        case 4: // Check
            logCount--;
            sprintf(pieceMovement, " %s is under check.", colors[currColor]);
            strcat(gameLog[logCount], pieceMovement);

            strcat(algeLog[logCount], "+");
            break;
        
        case 5: // Checkmate
            logCount--;
            sprintf(pieceMovement, " Checkmate for %s!", colors[currColor]);
            strcat(gameLog[logCount], pieceMovement);
            
            strcat(algeLog[logCount], "#");

            logCount++;
            if(currColor == 0){
                strcpy(gameLog[logCount], "Game over! Black wins!");
                strcpy(algeLog[logCount], "0-1");
            }
            else {
                strcpy(gameLog[logCount], "Game over! White wins!");
                strcpy(algeLog[logCount], "1-0");
            }
            
            break;

        case 6: // Stalemate
            strcpy(gameLog[logCount], "Stalemate! Game Draw!");

            strcpy(algeLog[logCount], "1/2-1/2");
            break;
    }

    if(!(logType == 4 || logType == 5 || logType == 6)){    // skip replacing the line under certain log types
        strcpy(gameLog[logCount], pieceMovement);
        strcpy(algeLog[logCount], algeMove);
    }
    logCount++;

    printf("Latest log: %s\n", gameLog[logCount - 1]);  // printing the latest log as player feedback
    printf("Latest algebratic log: %s\n", algeLog[logCount - 1]);   // because it's hard to find the latest move
    free(pieceMovement);
    free(algeMove);
}

void undoLog(){     // undo log is not working yet. This type of log works with the type of undo that reverts a whole round, which are 2 moves instead of 1. Will fix later.
    logCount--;
    gameLog[logCount][0] = '\0';
    algeLog[logCount][0] = '\0';
}

void printLog(){
    unsigned int roundCount = 1;
    printf("Log:\n");
    for(int i = 0; i < MAX_ROUNDS * 2 - 1; i++){
        if(gameLog[i][0] != '\0'){
            printf("%d. ", roundCount);
            printf("%s", gameLog[i]);
            i++;
            printf("    %s\n", gameLog[i]);
            roundCount++;
        }
    }
}

void printAlgeLog(){
    unsigned int roundCount = 1;
    printf("Algebratic log:\n");
    for(int i = 0; i < MAX_ROUNDS * 2 - 1; i++){
        if(algeLog[i][0] != '\0'){
            printf("%d. ", roundCount);
            printf("%s", algeLog[i]);
            i++;
            printf("    %s\n", algeLog[i]);
            roundCount++;
        }
    }
}

void logTotxt(){
    FILE *file = fopen("log.txt", "w");
    if (file == NULL) {
        printf("Error opening file\n");
        return;
    }
    unsigned int roundCount = 1;
    fprintf(file, "Log:\n");
    for(int i = 0; i < MAX_ROUNDS * 2 - 1; i++){
        if(gameLog[i][0] != '\0'){
            fprintf(file, "%d. ", roundCount);
            fprintf(file, "%s", gameLog[i]);
            i++;
            fprintf(file,"    %s\n", gameLog[i]);
            roundCount++;
        }
    }
    unsigned int roundCountAlg = 1;
    fprintf(file, "Algebratic log:\n");
    for(int j = 0; j < MAX_ROUNDS * 2 - 1; j++){
        if(algeLog[j][0] != '\0'){
            fprintf(file, "%d. ", roundCountAlg);
            fprintf(file, "%s", algeLog[j]);
            j++;
            fprintf(file, "    %s\n", algeLog[j]);
            roundCountAlg++;
        }
    } 
    printf("log.txt saved");   
    fclose(file);
    return;
}

int startGame(int gameMode) { // gameMode 0 for PVP, 1 for PVE
    struct Square board[8][8];
    struct Square backupBoard[8][8];
    initializeBoard(board);
    printBoard(board);
    bool checkMate = false;
    struct Move *move = malloc(sizeof(struct Move));
    int currColor = 0; // 0 for white, 1 for black
	int Exit_V = 0;

    while (!checkMate) {
        if (isStalemate(board, currColor)) {
            printf("Stalemate! Ending Game.\n");
            break;
        }

        deepCopyBoard(board, backupBoard); // Backup the board before the move

        if (possibleCheck(board, currColor)) {
            printf("Player %d is in Check!\n", currColor + 1);
            if (!canEscapeCheck(board, currColor)) {
                printf("Checkmate!! Game Over!\n");
                checkMate = true;
                break;
            }
        }

        Exit_V=getPlayerMove_rint(move);
	if(Exit_V==1){
	break;
	}
        makeMove_AI(board, move, currColor);
        printBoard(board);

        // Redo Logic Depending on Mode
        bool redo = false;
        if (gameMode == 0) {
            redo = redoMovePVP(board, backupBoard, currColor);
        } else if (gameMode == 1) {
            redo = redoMovePVE(board, backupBoard, currColor);
        }

        if (!redo) {
            currColor = 1 - currColor; // Switch player
        }
    }

    free(move);
    return 0;
}

int startGame_TEST(int gameMode, char input_string[100][10]) { // gameMode 0 for PVP, 1 for PVE
    struct Square board[8][8];
    struct Square backupBoard[8][8];
    initializeBoard(board);
    printBoard(board);
    bool checkMate = false;
    struct Move *move = malloc(sizeof(struct Move));
    int currColor = 0; // 0 for white, 1 for black
    int Exit_V = 0;
    int input_counter = 0;

    while (!checkMate) {
        if (isStalemate(board, currColor)) {
            printf("Stalemate! Ending Game.\n");
            break;
        }

        deepCopyBoard(board, backupBoard); // Backup the board before the move

        if (possibleCheck(board, currColor)) {
            printf("Player %d is in Check!\n", currColor + 1);
            if (!canEscapeCheck(board, currColor)) {
                printf("Checkmate!! Game Over!\n");
                checkMate = true;
                break;
            }
        }

        Exit_V=getPlayerMove_rint_TEST(move,input_string[input_counter]);
	input_counter++;
	if(Exit_V==1){
	break;
	}
        makeMove(board, move, currColor);
        printBoard(board);

        // Redo Logic Depending on Mode
        bool redo = false;
            redo = redoMovePVP_TEST(board, backupBoard, currColor,input_string[input_counter]);
		input_counter++;
      
        if (!redo) {
            currColor = 1 - currColor; // Switch player
        }
    }

    free(move);
    return 0;
}


bool redoMovePVE(struct Square board[8][8], struct Square backupBoard[8][8], int currentPlayer) {
    if (currentPlayer == 1) { // Assuming Player is always 1 and Computer is 0
        char redoChoice[10]; // Buffer for redo input
        printf("Do you want to redo your last move? (yes/no): ");
        fgets(redoChoice, sizeof(redoChoice), stdin);
        redoChoice[strcspn(redoChoice, "\n")] = '\0'; // Remove newline character

        if (strcmp(redoChoice, "yes") == 0) {
            deepCopyBoard(backupBoard, board); // Restore the backup
            printf("Redoing the move...\n");
            printBoard(board);
            return true;
        }
    }
    return false;
}

bool redoMovePVP(struct Square board[8][8], struct Square backupBoard[8][8], int currentPlayer) {
    char redoChoice[10]; // Buffer for redo input
    printf("Player %d, do you want to redo your last move? (yes/no): ", currentPlayer);
    fgets(redoChoice, sizeof(redoChoice), stdin);
    redoChoice[strcspn(redoChoice, "\n")] = '\0'; // Remove newline character

    if (strcmp(redoChoice, "yes") == 0) {
        int opponent = 1 - currentPlayer;
        printf("Player %d, do you agree to allow a redo? (yes/no): ", opponent);
        fgets(redoChoice, sizeof(redoChoice), stdin);
        redoChoice[strcspn(redoChoice, "\n")] = '\0'; // Remove newline character

        if (strcmp(redoChoice, "yes") == 0) {
            deepCopyBoard(backupBoard, board); // Restore the backup
            printf("Both players agreed. Redoing the move...\n");
            printBoard(board);
            return true;
        } else {
            printf("Redo not agreed. Continuing...\n");
        }
    }
    return false;
}

bool redoMovePVP_TEST(struct Square board[8][8], struct Square backupBoard[8][8], int currentPlayer,char *redoChoice){
    //char redoChoice[10];  Buffer for redo input
    printf("Player %d, do you want to redo your last move? (yes/no): ", currentPlayer);
    //fgets(redoChoice, sizeof(redoChoice), stdin);
    redoChoice[strcspn(redoChoice, "\n")] = '\0'; // Remove newline character
	printf("player input: %s",redoChoice);

    if (strcmp(redoChoice, "yes") == 0) {
        int opponent = 1 - currentPlayer;
        printf("Player %d, do you agree to allow a redo? (yes/no): ", opponent);
        fgets(redoChoice, sizeof(redoChoice), stdin);
        redoChoice[strcspn(redoChoice, "\n")] = '\0'; // Remove newline character

        if (strcmp(redoChoice, "yes") == 0) {
            deepCopyBoard(backupBoard, board); // Restore the backup
            printf("Both players agreed. Redoing the move...\n");
            printBoard(board);
            return true;
        } else {
            printf("Redo not agreed. Continuing...\n");
        }
    }
    return false;
}




void printMenu(){
	printf("\t\t\t\tChessGPT V1.0\n");
	printf("Main Menu:\n");
	printf("\t1. Start Game\n");
	printf("\t2. Log file\n");
	printf("\t3. Rules of Chess\n");
	printf("\t4. Exit program\n");
	printf("Enter an option: ");
}

int addMove(char input_string[100][10], int counter, char *input){
		int result = counter+1;
		strcpy(input_string[counter],input);
		return result;
}

void PlayerMode_1(){
    struct Square board[8][8];
    struct Square backupBoard[8][8];
    initializeBoard(board);
    printBoard(board);
    printf("\nWelcome to Chess!\n");
    printf("White pieces will be specified with upper case characters,\nwhile black pieces will be specified with lower case characters.\n");
    printf("Move notation will be specified by starting square then destination square (ex. a2a4)\n");
    bool checkMate = false;
    struct Move *move1 = malloc(sizeof(struct Move));
    int currColor = 0;
    char redoChoice[10]; // Buffer for redo input
	int exit=0;
        while(!checkMate){
            if(isStalemate(board, currColor)) {
                addLog(6, 0, currColor, 0, 0, 0, 0);    // add log for stalemate
                printf("Stalemate! Ending Game.\n");
                logTotxt();
                break;
            }

            deepCopyBoard(board, backupBoard); // Backup the board before the move

            if(possibleCheck(board, currColor)){ // First check for check
                addLog(4, 0, currColor, 0, 0, 0, 0);        // add log for check
                printf("Player is in Check!\n");
                if(!canEscapeCheck(board, currColor)){ // No escape; game over
                    addLog(5, 0, currColor, 0, 0, 0, 0);    // add log for checkmate
                    printf("Checkmate!! Game Over!\n");
                    printLog(); // automatically print both logs on checkmate
                    printAlgeLog();
                    logTotxt();
                    checkMate = true;
                    break;
                }
                else{
                    bool check = false;
                    while(!check){
                        printBoard(board);
                        //getPlayerMove(move1);
			exit = getPlayerMove_rint(move1);
			if(exit==1){
				break;
			}
                if(outOfCheck(board, move1, currColor)){ // Test if player move removes check
                    makeMove(board, move1, currColor);
                    printBoard(board);
                    printf("Do you want to redo this move? (yes/no): ");
                    fgets(redoChoice, sizeof(redoChoice), stdin);
                    redoChoice[strcspn(redoChoice, "\n")] = '\0'; // Remove newline character
				    redoChoice[strcspn(redoChoice, "\r")] = '\0'; // Remove newline character
			        while(strcmp(redoChoice,"yes")!=0 && strcmp(redoChoice,"no")!=0){
                        printf("Try again.Do you want to redo this move? (yes/no): ");
                        fgets(redoChoice, sizeof(redoChoice), stdin);
                        redoChoice[strcspn(redoChoice, "\n")] = '\0'; // Remove newline character
                        redoChoice[strcspn(redoChoice, "\r")] = '\0'; // Remove newline character
                    }
                    if (strcmp(redoChoice, "yes") == 0) {
                        deepCopyBoard(backupBoard, board); // Restore the backup
                        undoLog();  // undo last log
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
		   if(exit==1){
			break;
		   }
                }
            }
            else{
                bool invalidMove = false;
                while(!invalidMove){
                    //getPlayerMove(move1);
			exit = getPlayerMove_rint(move1);
                        if(exit==1){
                                break;
                        }
                    if(isValidMove(board, move1, currColor)){
                        makeMove(board, move1, currColor);
                        printBoard(board);
                        printf("Do you want to redo this move? (yes/no): ");
                        fgets(redoChoice, sizeof(redoChoice), stdin);
                        redoChoice[strcspn(redoChoice, "\n")] = '\0'; // Remove newline character
			redoChoice[strcspn(redoChoice, "\r")] = '\0'; // Remove newline character
                           while(strcmp(redoChoice,"yes")!=0 && strcmp(redoChoice,"no")!=0){
                                printf("Try again.Do you want to redo this move? (yes/no): ");
                                fgets(redoChoice, sizeof(redoChoice), stdin);
                            redoChoice[strcspn(redoChoice, "\n")] = '\0'; // Remove newline character
                                redoChoice[strcspn(redoChoice, "\r")] = '\0'; // Remove newline character
                                }

                        if (strcmp(redoChoice, "yes") == 0) {
                            deepCopyBoard(backupBoard, board); // Restore the backup
                            undoLog();  // undo last log
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
		if(exit==1){
                        logTotxt();
                        break;
                   }
            }
            currColor = 1 - currColor; // Switch player after a valid move

            //printLog();
            //printAlgeLog();

        }



}
int PlayerMode_2(){
	/**
struct Square board [8][8];
    //struct Move* rMove = malloc(sizeof(struct Move));
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

		int exit = 0;
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
                        //return 0;
			break;
                    }
                    else{
                        bool check = false;
                        while(!check){
                            printBoard(board);
                            //getPlayerMove(move1);
			    exit = getPlayerMove_rint(move1);
                        if(exit==1){
                                break;
                        }
                            if(outOfCheck(board,move1,currColor)){ // Test if player move removes check
                                makeMove(board,move1,currColor);
                                check = true;
                                printBoard(board);
                                
                            }
                            else{
                                printf("Try again!\n"); // Redo loop if not out of check
                            }
                        }
                        if(exit==1){
                                break;
                        }
                    }
                }
                else{ //
                    bool invalidMove = false;
                    while(!invalidMove){
                        //getPlayerMove(move1);
			exit = getPlayerMove_rint(move1);
                        if(exit==1){
                                break;
                        }
                        if(isValidMove(board,move1,currColor)){
                            invalidMove = true;
                            makeMove(board,move1,currColor);
                            printBoard(board);
                            break;
                        }
                        else{printf("Invalid Move! Try again!\n");}
                    }
                        if(exit==1){
                                break;
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
                    //return 0;
		 break;
                }
                move1 = selectMove(board, currColor);
                makeMove(board, move1, currColor);
                printBoard(board);
                if(!canEscapeCheck(board,currColor)){
                    printf("Game Over!");
                    checkMate = true;
                    //return 0;
			break;
                }
                currColor = 0;
            }
**/


/**
AI = true;
    struct Square board [8][8];
            initializeBoard(board);
            printf("%d",evaluateBoard(board, 0));
            printf("\nWelcome to Chess! This is the PVC option.\n");
            printf("White pieces will be specified with upper case characters,\nwhile black pieces will be specifed with lower case characters.\n");
            printf("Move notation will be specified by starting square then destination square (ex. a2a4)\n");
            bool checkMate = false;
            struct Move * move1 = malloc(sizeof(struct Move));
            int currColor;
            currColor = 0;
            char getColor[10];
            bool validColor = false;
		int exit = 0;
            printf("Enter a color. Enter 0 for white, and 1 for black. ");
            while(!validColor){
                fgets(getColor,sizeof(getColor),stdin);
                getColor[strcspn(getColor, "\n")] = '\0';
                if(strcmp(getColor, "0") == 0){
                    validColor = true;
                    currColor = 0;
                }
                
                else if(strcmp(getColor,"1") == 0){
                    validColor = true;
                    currColor = 1;
                }
                else{printf("Invalid Color.");}
            }
            printBoard(board);
    if(currColor == 1){
        
        while(!checkMate){
            currColor = 1-currColor;
            if(isStalemate(board,currColor)){
                printf("Stalemate! Ending Game.");
                //break;
		return 0;
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
            currColor = 1 - currColor;
            if(isStalemate(board,currColor)){
                printf("Stalemate! Ending Game.");
                //break;
		return 0;
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
                        exit=getPlayerMove_rint(move1);
			if(exit==1){
				//break;
				return 0;
			}
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
                    exit=getPlayerMove_rint(move1);
			if(exit==1){
				//break;
				return 0;

			}
                    if(isValidMove(board,move1,currColor)){
                        invalidMove = true;
                        makeMove(board,move1,currColor);
                        printBoard(board);
                        break;
                    }
                    else{printf("Invalid Move! Try again!\n");}
                }
            }
            
        }
    }
    else{
        while(!checkMate){
            if(isStalemate(board,currColor)){
                printf("Stalemate! Ending Game.");
                //break;
		return 0;
            }
            if(possibleCheck(board,currColor)){ // First check for check
                printf("Player is in Check!\n");
                if(!canEscapeCheck(board,currColor)){ // No escape; game over
                    printf("Checkmate!! Game Over!\n");
                    checkMate = true;
                    //return 0;
			return 0;

                }
                else{
                    bool check = false;
                    while(!check){
                        printBoard(board);
                        //getPlayerMove(move1);
			exit=getPlayerMove_rint(move1);
                        if(exit==1){
                                //break;
				return 0;
                        }

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
                    //getPlayerMove(move1);
			exit=getPlayerMove_rint(move1);
                        if(exit==1){
                                //break;
				return 0;

                        }
                    if(isValidMove(board,move1,currColor)){
                        invalidMove = true;
                        makeMove(board,move1,currColor);
                        printBoard(board);
                        break;
                    }
                    else{printf("Invalid Move! Try again!\n");}
                }
            }
            currColor = 1-currColor;
            if(isStalemate(board,currColor)){
                printf("Stalemate! Ending Game.");
                //break;
		return 0;

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
            currColor = 1 - currColor;
        }
    }
            
    return 0;
**/

AI = true;
    struct Square board [8][8];
            initializeBoard(board);
            //printf("%d",evaluateBoard(board, 0));
            printf("\nWelcome to Chess! This is the PVC option.\n");
            printf("White pieces will be specified with upper case characters,\nwhile black pieces will be specifed with lower case characters.\n");
            printf("Move notation will be specified by starting square then destination square (ex. a2a4)\n");
            bool checkMate = false;
            struct Move * move1 = malloc(sizeof(struct Move));
            int currColor;
            currColor = 0;
            char getColor[10];
            bool validColor = false;
            printf("Enter a color. Enter 0 for white, and 1 for black. ");
            while(!validColor){
                fgets(getColor,sizeof(getColor),stdin);
                getColor[strcspn(getColor, "\n")] = '\0';
                if(strcmp(getColor, "0") == 0){
                    validColor = true;
                    currColor = 0;
                }
                
                else if(strcmp(getColor,"1") == 0){
                    validColor = true;
                    currColor = 1;
                }
                else{printf("Invalid Color.");}
            }
            printBoard(board);
    if(currColor == 1){
        
        while(!checkMate){
            currColor = 1-currColor;
            if(isStalemate(board,currColor)){
                addLog(6, 0, currColor, 0, 0, 0, 0);    // add log for stalemate
                printf("Stalemate! Ending Game.");
                logTotxt();
                break;
            }
            if(!canEscapeCheck(board,currColor)){
                addLog(5, 0, currColor, 0, 0, 0, 0);
                printf("Game Over!");
                logTotxt();
                checkMate = true;
                return 0;
            }
            move1 = selectMove(board, currColor);
		/*if(move1==NULL){
                printf("NULL\n");
                }
                else{
                printf("%d%d%d%d\n",move1->initialRow,move1->initialCol,move1->destinationRow,move1->destinationCol);
                }*/
            makeMove(board, move1, currColor);
            printBoard(board);
            if(!canEscapeCheck(board,currColor)){
                addLog(5, 0, currColor, 0, 0, 0, 0);
                printf("Game Over!");
                logTotxt();
                checkMate = true;
                return 0;
            }
            currColor = 1 - currColor;
            if(isStalemate(board,currColor)){
                addLog(6, 0, currColor, 0, 0, 0, 0);    // add log for stalemate
                printf("Stalemate! Ending Game.");
                logTotxt();
                break;
            }
            if(possibleCheck(board,currColor)){ // First check for check
                addLog(4, 0, currColor, 0, 0, 0, 0);        // add log for check
                printf("Player is in Check!\n");
                if(!canEscapeCheck(board,currColor)){ // No escape; game over
                    addLog(5, 0, currColor, 0, 0, 0, 0);
                    printf("Checkmate!! Game Over!\n");
                    checkMate = true;
                    logTotxt();
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
            
        }
    }
    else{
        while(!checkMate){
            if(isStalemate(board,currColor)){
                addLog(6, 0, currColor, 0, 0, 0, 0);    // add log for stalemate
                printf("Stalemate! Ending Game.");
                logTotxt();
                break;
            }
            if(possibleCheck(board,currColor)){ // First check for check
                addLog(4, 0, currColor, 0, 0, 0, 0);        // add log for check
                printf("Player is in Check!\n");
                if(!canEscapeCheck(board,currColor)){ // No escape; game over
                    addLog(5, 0, currColor, 0, 0, 0, 0);
                    printf("Checkmate!! Game Over!\n");
                    logTotxt();
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
            currColor = 1-currColor;
            if(isStalemate(board,currColor)){
                addLog(6, 0, currColor, 0, 0, 0, 0);    // add log for stalemate
                printf("Stalemate! Ending Game.");
                logTotxt();
                break;
            }
            if(!canEscapeCheck(board,currColor)){
                addLog(5, 0, currColor, 0, 0, 0, 0);
                printf("Game Over!");
                logTotxt();
                checkMate = true;
                return 0;
            }
            move1 = selectMove(board, currColor);
            /*if(move1==NULL){
		        printf("NULL\n");
		    }
		    else{
		        printf("%d%d%d%d\n",move1->initialRow,move1->initialCol,move1->destinationRow,move1->destinationCol);
		    }*/
		    makeMove(board, move1, currColor);
            printBoard(board);
            if(!canEscapeCheck(board,currColor)){
                addLog(5, 0, currColor, 0, 0, 0, 0);
                printf("Game Over!");
                logTotxt();
                checkMate = true;
                return 0;
            }
            currColor = 1 - currColor;
        }
    }
return 0;            


}

void PlayerMode_1_TEST(char inputs[100][10]){
struct Square board[8][8];
        struct Square backupBoard[8][8];
        initializeBoard(board);
        printBoard(board);
        printf("White pieces will be specified with upper case characters,\nwhile black pieces will be specified with lower case characters.\n");
        printf("Move notation will be specified by starting square then destination square (ex. a2a4)\n");
        bool checkMate = false;
        struct Move *move1 = malloc(sizeof(struct Move));
        int currColor = 0;
        char redoChoice[10]; // Buffer for redo input
	int exit=0;
	int counter = 0;

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
                        //getPlayerMove(move1);
			//exit = getPlayerMove_rint(move1);
			exit = getPlayerMove_rint_TEST(move1,inputs[counter]);
			if(exit==1){
				break;
			}
			counter++;
                        if(outOfCheck(board, move1, currColor)){ // Test if player move removes check
                            makeMove(board, move1, currColor);
                            printBoard(board);
                            printf("Do you want to redo this move? (yes/no): ");
                            //fgets(redoChoice, sizeof(redoChoice), stdin);
				//redoChoice=inputs[counter];
				strcpy(redoChoice,inputs[counter]);
				counter++;
                            redoChoice[strcspn(redoChoice, "\n")] = '\0'; // Remove newline character
				redoChoice[strcspn(redoChoice, "\r")] = '\0'; // Remove newline character
			   while(strcmp(redoChoice,"yes")!=0 && strcmp(redoChoice,"no")!=0){
				printf("Try again.Do you want to redo this move? (yes/no): ");
				fgets(redoChoice, sizeof(redoChoice), stdin);
                            redoChoice[strcspn(redoChoice, "\n")] = '\0'; // Remove newline character
				redoChoice[strcspn(redoChoice, "\r")] = '\0'; // Remove newline character
				}
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
		   if(exit==1){
			break;
		   }
                }
            }
            else{
                bool invalidMove = false;
                while(!invalidMove){
                    //getPlayerMove(move1);
			//exit = getPlayerMove_rint(move1);
			exit = getPlayerMove_rint_TEST(move1,inputs[counter]);
			if(exit==1){
				break;
			}
			counter++;
                    if(isValidMove(board, move1, currColor)){
                        makeMove(board, move1, currColor);
                        printBoard(board);
                        printf("Do you want to redo this move? (yes/no): ");
                        //fgets(redoChoice, sizeof(redoChoice), stdin);
			strcpy(redoChoice,inputs[counter]);
				counter++;
                        redoChoice[strcspn(redoChoice, "\n")] = '\0'; // Remove newline character
			redoChoice[strcspn(redoChoice, "\r")] = '\0'; // Remove newline character
                           while(strcmp(redoChoice,"yes")!=0 && strcmp(redoChoice,"no")!=0){
                                printf("Try again.Do you want to redo this move? (yes/no): ");
                                fgets(redoChoice, sizeof(redoChoice), stdin);
                            redoChoice[strcspn(redoChoice, "\n")] = '\0'; // Remove newline character
                                redoChoice[strcspn(redoChoice, "\r")] = '\0'; // Remove newline character
                                }

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
		if(exit==1){
                        break;
                   }
            }
            currColor = 1 - currColor; // Switch player after a valid move
        }

}

void PlayerMode_2_TEST(char inputs[100][10]){
	struct Square board [8][8];
    //struct Move* rMove = malloc(sizeof(struct Move));
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
		

		int exit = 0;
		int counter = 0;

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
                        //return 0;
			break;
                    }
                    else{
                        bool check = false;
                        while(!check){
                            printBoard(board);
                            //getPlayerMove(move1);
			    //exit = getPlayerMove_rint(move1);
                        exit = getPlayerMove_rint_TEST(move1,inputs[counter]);
			if(exit==1){
				break;
			}
			counter++;
                            if(outOfCheck(board,move1,currColor)){ // Test if player move removes check
                                makeMove(board,move1,currColor);
                                check = true;
                                printBoard(board);
                                
                            }
                            else{
                                printf("Try again!\n"); // Redo loop if not out of check
                            }
                        }
                        if(exit==1){
                                break;
                        }
                    }
                }
                else{ //
                    bool invalidMove = false;
                    while(!invalidMove){
                        //getPlayerMove(move1);
			//exit = getPlayerMove_rint(move1);
                        exit = getPlayerMove_rint_TEST(move1,inputs[counter]);
			if(exit==1){
				break;
			}
			counter++;
                        if(isValidMove(board,move1,currColor)){
                            invalidMove = true;
                            makeMove(board,move1,currColor);
                            printBoard(board);
                            break;
                        }
                        else{printf("Invalid Move! Try again!\n");}
                    }
                        if(exit==1){
                                break;
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
                    //return 0;
		 break;
                }
                move1 = selectMove(board, currColor);
                makeMove(board, move1, currColor);
                printBoard(board);
                if(!canEscapeCheck(board,currColor)){
                    printf("Game Over!");
                    checkMate = true;
                    //return 0;
			break;
                }
                currColor = 0;
            }

}

void initializeBoard(struct Square gameBoard[8][8]){
    // Set each square's position
    for(int i = 0;i<8;i++){
        for(int j = 0;j<8;j++){
            gameBoard[i][j].currPiece = NULL;
            gameBoard[i][j].row = i;
            gameBoard[i][j].col = j;
        }
    }
    
    for(int i = 0;i<8;i++){
        struct Piece* bPawn = malloc(sizeof(struct Piece)); // Allocate memory
        gameBoard[1][i].currPiece = bPawn;
        gameBoard[1][i].currPiece->firstMove = true;
        gameBoard[1][i].currPiece->color = 1;
        gameBoard[1][i].currPiece->pieceType = 0;
        gameBoard[1][i].currPiece->moveCount = 0;
        
        // Initialize first row of black pawns
    }
    for(int i = 0;i<8;i++){
        struct Piece* bPawn = malloc(sizeof(struct Piece)); // Allocate memory
        gameBoard[6][i].currPiece = bPawn;
        gameBoard[6][i].currPiece->firstMove = true;
        gameBoard[6][i].currPiece->color = 0;
        gameBoard[6][i].currPiece->pieceType = 0;
        gameBoard[6][i].currPiece->moveCount = 0;
        
        // Initialize first row of black pawns
    }
    // initialize rooks
    struct Piece* bRook1 = malloc(sizeof(struct Piece));
    struct Piece* bRook2 = malloc(sizeof(struct Piece));
    struct Piece* wRook1 = malloc(sizeof(struct Piece));
    struct Piece* wRook2 = malloc(sizeof(struct Piece));
    bRook1->color = 1;
    bRook1->pieceType = 1;
    bRook1->firstMove = true;
    bRook1->moveCount = 0;
    bRook2->color = 1;
    bRook2->pieceType = 1;
    bRook2->firstMove = true;
    bRook2->moveCount = 0;
    wRook1->color = 0;
    wRook1->pieceType = 1;
    wRook1->firstMove = true;
    wRook1->moveCount = 0;
    wRook2->color = 0;
    wRook2->pieceType = 1;
    wRook2->firstMove = true;
    wRook2->moveCount = 0;
    gameBoard[0][0].currPiece = bRook1;
    gameBoard[0][7].currPiece = bRook2;
    gameBoard[7][0].currPiece = wRook1;
    gameBoard[7][7].currPiece = wRook2;
    //TODO make into function
    //initialize bishops
    struct Piece* bBish1 = malloc(sizeof(struct Piece));
    struct Piece* bBish2 = malloc(sizeof(struct Piece));
    struct Piece* wBish1 = malloc(sizeof(struct Piece));
    struct Piece* wBish2 = malloc(sizeof(struct Piece));
    bBish1->color = 1;
    bBish1->pieceType = 2;
    bBish1->firstMove = true;
    bBish1->moveCount = 0;
    bBish2->color = 1;
    bBish2->pieceType = 2;
    bBish2->firstMove = true;
    bBish2->moveCount = 0;
    wBish1->color = 0;
    wBish1->pieceType = 2;
    wBish1->firstMove = true;
    wBish1->moveCount = 0;
    wBish2->color = 0;
    wBish2->pieceType = 2;
    wBish2->firstMove = true;
    wBish2->moveCount = 0;
    gameBoard[0][2].currPiece = bBish1;
    gameBoard[0][5].currPiece = bBish2;
    gameBoard[7][2].currPiece = wBish1;
    gameBoard[7][5].currPiece = wBish2;
    //TODO make into function
    //initialize knights
    struct Piece* bKnig1 = malloc(sizeof(struct Piece));
    struct Piece* bKnig2 = malloc(sizeof(struct Piece));
    struct Piece* wKnig1 = malloc(sizeof(struct Piece));
    struct Piece* wKnig2 = malloc(sizeof(struct Piece));
    bKnig1->color = 1;
    bKnig1->pieceType = 3;
    bKnig1->firstMove = true;
    bKnig1->moveCount = 0;
    bKnig2->color = 1;
    bKnig2->pieceType = 3;
    bKnig2->firstMove = true;
    bKnig2->moveCount = 0;
    wKnig1->color = 0;
    wKnig1->pieceType = 3;
    wKnig1->firstMove = true;
    wKnig1->moveCount = 0;
    wKnig2->color = 0;
    wKnig2->pieceType = 3;
    wKnig2->firstMove = true;
    wKnig2->moveCount = 0;
    gameBoard[0][1].currPiece = bKnig1;
    gameBoard[0][6].currPiece = bKnig2;
    gameBoard[7][1].currPiece = wKnig1;
    gameBoard[7][6].currPiece = wKnig2;
    //TODO make into function
    //initialize queens
    struct Piece * bQueen = malloc(sizeof(struct Piece));
    struct Piece * wQueen = malloc(sizeof(struct Piece));
    bQueen->color = 1;
    bQueen->pieceType = 4;
    bQueen->firstMove = true;
    bQueen->moveCount = 0;
    wQueen->color = 0;
    wQueen->pieceType = 4;
    wQueen->firstMove = true;
    wQueen->moveCount = 0;
    gameBoard[0][3].currPiece = bQueen;
    gameBoard[7][3].currPiece = wQueen;
    //TODO make into function
    //initialize kings
    struct Piece * bKing = malloc(sizeof(struct Piece));
    struct Piece * wKing = malloc(sizeof(struct Piece));
    bKing->color = 1;
    bKing->pieceType = 5;
    bKing->firstMove = true;
    bKing->moveCount = 0;
    wKing->color = 0;
    wKing->pieceType = 5;
    wKing->firstMove = true;
    wKing->moveCount = 0;
    gameBoard[0][4].currPiece = bKing;
    gameBoard[7][4].currPiece = wKing;
    //TODO make into function
}
void printBoard(struct Square gameBoard[8][8]) {
    // Column Labels
    printf("   a b c d e f g h\n");

    for (int i = 0; i < 8; i++) {
        printf("%d ", 8 - i); // Row labels
        for (int j = 0; j < 8; j++) {
            struct Piece *piece = gameBoard[i][j].currPiece;
            char symbol = '.'; // Default to empty

            if (piece != NULL) {
                if (piece->color == 0) { // White
                    switch (piece->pieceType) {
                        case 0: symbol = 'P'; break;
                        case 1: symbol = 'R'; break;
                        case 2: symbol = 'B'; break;
                        case 3: symbol = 'N'; break;
                        case 4: symbol = 'Q'; break;
                        case 5: symbol = 'K'; break;
                    }
                } else { // Black
                    switch (piece->pieceType) {
                        case 0: symbol = 'p'; break;
                        case 1: symbol = 'r'; break;
                        case 2: symbol = 'b'; break;
                        case 3: symbol = 'n'; break;
                        case 4: symbol = 'q'; break;
                        case 5: symbol = 'k'; break;
                    }
                }
            }
            printf(" %c", symbol);
        }
        printf("\n");
    }
}
bool isValidMoveFormat(const char *input) {
    if (strlen(input) != 4) {
        printf("Invalid character! Try again! \n");
        return false; // Wrong length
    }

    // Check column letters
    if (input[0] < 'a' || input[0] > 'h' || input[2] < 'a' || input[2] > 'h') {
        printf("Invalid character! Try again! \n");
        return false;
    }

    // Check row numbers
    if (input[1] < '1' || input[1] > '8' || input[3] < '1' || input[3] > '8') {
        printf("Invalid character! Try again! \n");
        return false;
    }

    return true; // Within legal boundaries
}
void getPlayerMove(struct Move *move) {
    char input[10]; // Buffer for input string

    do {
        printf("Enter your move (e.g., e2e4): ");
        fgets(input, sizeof(input), stdin); //
        input[strcspn(input, "\n")] = '\0'; // Remove trailing newline, if present(i have no clue how it works stack exchange )
    } while (!isValidMoveFormat(input));

    // Assuming valid input, now parse into the move structure
    move->initialCol = input[0] - 'a';
    move->initialRow = '8' - input[1];
    move->destinationCol = input[2] - 'a';
    move->destinationRow = '8' - input[3];
}
bool isValidBishopMove(struct Square gameBoard[8][8], const struct Move *move) {
    int startRow = move->initialRow;
    int startCol = move->initialCol;
    int destRow = move->destinationRow;
    int destCol = move->destinationCol;
    struct Piece *piece = gameBoard[startRow][startCol].currPiece;

    // Check if moving piece is a bishop
    if (piece == NULL || piece->pieceType != 2) {
        return false;
    }

    // Check if the move is diagonal
    if (abs(destRow - startRow) != abs(destCol - startCol) && !(destRow == startRow && destCol == startCol)) { // no staying in place
        return false;
    }

    // Check if the path is clear
    int rowDirection = (destRow > startRow) ? 1 : -1;
    int colDirection = (destCol > startCol) ? 1 : -1;

    // Check along the diagonal
    for (int i = startRow + rowDirection, j = startCol + colDirection;
         i != destRow;
         i += rowDirection, j += colDirection) {
        if (gameBoard[i][j].currPiece != NULL) {
            return false; // Path is blocked
        }
    }

    // Ensure destination square contains an opponent's piece or is empty
    return gameBoard[destRow][destCol].currPiece == NULL ||
           gameBoard[destRow][destCol].currPiece->color != piece->color;
}
bool isValidKnightMove(struct Square gameBoard[8][8], const struct Move *move) {
    int startRow = move->initialRow;
    int startCol = move->initialCol;
    int destRow = move->destinationRow;
    int destCol = move->destinationCol;

    struct Piece *piece = gameBoard[startRow][startCol].currPiece;

    // Check if moving piece is a knight
    if (piece == NULL || piece->pieceType != 3) {
        return false;
    }

    // Check for valid L-shaped moves
    int rowDiff = abs(destRow - startRow);
    int colDiff = abs(destCol - startCol);

    if (!((rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2))) {
        return false; // Not a valid L-shape
    }

    // Ensure the target square is empty or contains an opponent's piece
    return gameBoard[destRow][destCol].currPiece == NULL ||
           gameBoard[destRow][destCol].currPiece->color != piece->color;
}
bool isValidRookMove(struct Square gameBoard[8][8], const struct Move *move) {
    int startRow = move->initialRow;
    int startCol = move->initialCol;
    int destRow = move->destinationRow;
    int destCol = move->destinationCol;

    struct Piece *piece = gameBoard[startRow][startCol].currPiece;

    // Check if moving piece is a rook
    if (piece == NULL || piece->pieceType != 1) {
        return false;
    }

    // Check for horizontal or vertical movement
    if (startRow != destRow && startCol != destCol) {
        return false; // Not horizontal or vertical
    }

    // Check if path is clear
    if (startRow == destRow) { // Horizontal movement
        int colDir = (destCol > startCol) ? 1 : -1;
        for (int col = startCol + colDir; col != destCol; col += colDir) {
            if (gameBoard[startRow][col].currPiece != NULL) {
                return false; // Path is obstructed
            }
        }
    } else { // Vertical movement
        int rowDir = (destRow > startRow) ? 1 : -1;
        for (int row = startRow + rowDir; row != destRow; row += rowDir) {
            if (gameBoard[row][startCol].currPiece != NULL) {
                return false; // Path is obstructed
            }
        }
    }

    // Ensure the target square is empty or contains an opponent's piece
    return gameBoard[destRow][destCol].currPiece == NULL ||
           gameBoard[destRow][destCol].currPiece->color != piece->color;
}
bool isValidQueenMove(struct Square gameBoard[8][8], const struct Move *move) {
    int startRow = move->initialRow;
    int startCol = move->initialCol;
    int destRow = move->destinationRow;
    int destCol = move->destinationCol;

    struct Piece *piece = gameBoard[startRow][startCol].currPiece;

    // Check if moving piece is a queen
    if (piece == NULL || piece->pieceType != 4) {
        return false;
    }

    // Check diagonal, horizontal, or vertical movement
    if (abs(destRow - startRow) != abs(destCol - startCol) &&
        startRow != destRow && startCol != destCol) {
        return false; // Invalid queen movement
    }

    // Check if path is clear (combines rook and bishop logic)
    int rowDir = (destRow > startRow) ? 1 : -1;
    int colDir = (destCol > startCol) ? 1 : -1;

    // Diagonal Checks
    if (abs(destRow - startRow) == abs(destCol - startCol)) {
        for (int i = startRow + rowDir, j = startCol + colDir;
               i != destRow;
               i += rowDir, j += colDir) {
            if (gameBoard[i][j].currPiece != NULL) {
                return false;
            }
        }
    } else if (startRow == destRow) { // Horizontal Movement
        for (int col = startCol + colDir; col != destCol; col += colDir) {
            if (gameBoard[startRow][col].currPiece != NULL) {
                return false;
            }
        }
    } else { // Vertical Movement
        for (int row = startRow + rowDir; row != destRow; row += rowDir) {
            if (gameBoard[row][startCol].currPiece != NULL) {
                return false;
            }
        }
    }

    // Ensure the target square is empty or contains an opponent's piece
    return gameBoard[destRow][destCol].currPiece == NULL ||
           gameBoard[destRow][destCol].currPiece->color != piece->color;
}

bool isValidPawnMove(struct Square gameBoard[8][8], const struct Move *move) {
    int startRow = move->initialRow;
    int startCol = move->initialCol;
    int destRow = move->destinationRow;
    int destCol = move->destinationCol;

    struct Piece *piece = gameBoard[startRow][startCol].currPiece;

    // Ensure moving piece is a pawn
    if (piece == NULL || piece->pieceType != 0) {
        return false;
    }

    int direction = (piece->color == 0) ? -1 : 1; // (White: -1, Black: 1)
    if(isEnPassant(gameBoard, move, gameBoard[startRow][startCol].currPiece->color)){
        return true;
    }
    // Normal forward moves
    if (destCol == startCol) {
        if (destRow == startRow + direction) {
            return gameBoard[destRow][destCol].currPiece == NULL; // One step
        } else if (destRow == startRow + 2 * direction && piece->firstMove)  {
            // Two steps on first move
            return gameBoard[startRow + direction][startCol].currPiece == NULL && // First step empty
                   gameBoard[destRow][destCol].currPiece == NULL;        // Destination empty
        }
    }

    // Diagonal capture
    if (abs(destCol - startCol) == 1 && destRow == startRow + direction) {
        return gameBoard[destRow][destCol].currPiece != NULL &&
               gameBoard[destRow][destCol].currPiece->color != piece->color; // Opponent piece
    }
    //En passant support
    
    return false;
}
bool isEnPassant(struct Square gameBoard[8][8], const struct Move *move, int currColor) {
    if((gameBoard[move->initialRow][move->initialCol].currPiece->moveCount == 2) &&
       (abs(move->destinationCol - move->initialCol) == 1) &&
       (abs(move->destinationRow - move->initialRow) == 1)){
        
        struct Piece *targetPawn = gameBoard[move->initialRow][move->destinationCol].currPiece;
        if(targetPawn == NULL){
            return false;
        }
        else if((targetPawn->pieceType != 0) || (targetPawn->moveCount != 1)){ return false;}
        else{return true;}
    }
    return false;
}

bool possibleCheck(struct Square gameBoard[8][8], int currColor){
    
    int kingRow, kingCol;
    // Find the king!!!!
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (gameBoard[i][j].currPiece != NULL && gameBoard[i][j].currPiece->color == currColor &&  gameBoard[i][j].currPiece->pieceType == 5) {
                kingRow = i;
                kingCol = j;
                break; // You can break out of the loop once the king is found
            }
        }
    }
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            struct Piece *piece = gameBoard[i][j].currPiece;

            // Check for opponent's pieces
            if (piece != NULL && piece->color != currColor) {
                struct Move * move = malloc(sizeof(struct Move));
                move->initialRow = i;
                move->initialCol = j;
                move->destinationRow = kingRow;
                move->destinationCol = kingCol;
                if (isValidMove(gameBoard,move,piece->color)) {
                    free(move);
                    return true;
                }
                free(move);
            }
        }
    }

    return false;
}

bool isValidCastle(struct Square gameBoard[8][8], const struct Move *move){
    int col = gameBoard[move->initialRow][move->initialCol].currPiece->color;
    if(possibleCheck(gameBoard,col)){
        
        return false;
    }
    if(!gameBoard[move->initialRow][move->initialCol].currPiece->firstMove){
        
        return false;
    }
    if((move->destinationCol - move->initialCol) > 0){
        struct Piece* rook = gameBoard[move->initialRow][7].currPiece;
        if(rook == NULL || (rook->pieceType != 1)){return false;}
        if(!rook->firstMove){return false;}
        struct Square tempBoard[8][8];
        deepCopyBoard(gameBoard,tempBoard);
        for(int i = move->initialCol+1;i<=move->destinationCol;i++){
            if(gameBoard[move->initialRow][i].currPiece != NULL){
                freeBoard(tempBoard);
                return false;}
        }
        for(int i = (move->initialCol+1);i<=move->destinationCol;i++){
            struct Move *tempMove = malloc(sizeof(struct Move));
            tempMove->initialRow = move->initialRow;
            tempMove->destinationRow = move->initialRow;
            tempMove->initialCol = i-1;
            tempMove->destinationCol = i;
            makeMove_AI(tempBoard,tempMove,col);
            if(possibleCheck(tempBoard,col)){return false;}
        }
        return true;
    }
    else if((move->destinationCol - move->initialCol) < 0){
        struct Piece* rook = gameBoard[move->initialRow][0].currPiece;
        if(rook == NULL || (rook->pieceType != 1)){return false;}
        if(!rook->firstMove){return false;}
        struct Square tempBoard[8][8];
        deepCopyBoard(gameBoard,tempBoard);
        for(int i = move->initialCol-1;i<=move->destinationCol;i--){
            if(gameBoard[move->initialRow][i].currPiece != NULL){return false;}
        }
        for(int i = (move->initialCol-1);i<=move->destinationCol;i--){
            struct Move *tempMove = malloc(sizeof(struct Move));
            tempMove->initialRow = move->initialRow;
            tempMove->destinationRow = move->initialRow;
            tempMove->initialCol = i+1;
            tempMove->destinationCol = i;
            makeMove_AI(tempBoard,tempMove,col);
            if(possibleCheck(tempBoard,col)){return false;}
        }
        freeBoard(tempBoard);
        return true;
    }
    return false;
}
bool isValidKingMove(struct Square gameBoard[8][8], const struct Move *move) {
    int startRow = move->initialRow;
    int startCol = move->initialCol;
    int destRow = move->destinationRow;
    int destCol = move->destinationCol;
    struct Piece *piece = gameBoard[startRow][startCol].currPiece;

    // Check if moving piece is a king
    if (piece == NULL || piece->pieceType != 5) {
        return false;
    }

    // Check for basic one-square moves
    if (abs(destRow - startRow) <= 1 && abs(destCol - startCol) <= 1) {
        // Ensure the target square is empty or contains an opponent's piece
        return gameBoard[destRow][destCol].currPiece == NULL ||
               gameBoard[destRow][destCol].currPiece->color != piece->color;
    }

    // Check for castling
    if((abs(destRow - startRow)==0))
        if (isValidCastle(gameBoard, move)) {
            return true;
        }

    return false; // No other valid move patterns
}
bool isValidMove(struct Square gameBoard[8][8], const struct Move *move, int currColor) {
    if(move->initialRow < 0){return false;}
    if(move->initialCol < 0){return false;}
    if(move->destinationRow<0){return false;}
    if(move->destinationCol<0){return false;}
    if(move->initialRow > 7){return false;}
    if(move->initialCol > 7){return false;}
    if(move->destinationRow>7){return false;}
    if(move->destinationCol>7){return false;}
    if((move->destinationCol == move->initialCol) && (move->destinationRow == move->initialRow)){
        return false;
    };
    int startRow = move->initialRow;
    int startCol = move->initialCol;
    struct Piece *piece = gameBoard[startRow][startCol].currPiece;
    if (piece == NULL) {
        return false; // No piece to move
    }
    if(currColor != piece->color){
        return false;
    }
    // Call specialized functions based on piece type
    switch (piece->pieceType) {
        case 0: return isValidPawnMove(gameBoard, move);
        case 1: return isValidRookMove(gameBoard, move);
        case 2: return isValidBishopMove(gameBoard, move);
        case 3: return isValidKnightMove(gameBoard, move);
        case 4: return isValidQueenMove(gameBoard, move);
        case 5: return isValidKingMove(gameBoard, move);
        default:{return false;} // Unknown piece type
    }
}
int getPromotionChoice() {
    int choice;
    do {
        printf("Promote your pawn to:\n");
        printf("0 - Pawn\n");
        printf("1 - Rook\n");
        printf("2 - Bishop\n");
        printf("3 - Knight\n");
        printf("4 - Queen\n");
        printf("Enter your choice (0-4): ");
        scanf("%d", &choice);
    } while (choice < 0 || choice > 4);

    return choice;
}
void makeMove_AI(struct Square gameBoard[8][8], struct Move *move, int currColor){
    int startRow = move->initialRow;
    int startCol = move->initialCol;
    int destRow = move->destinationRow;
    int destCol = move->destinationCol;
    
    // Ensure the move is valid before execution
    if (!isValidMove(gameBoard, move, currColor)) {
        return;
    }
    if(isEnPassant(gameBoard,move,currColor)){
        gameBoard[startRow][startCol].currPiece->firstMove = false;
        gameBoard[startRow][startCol].currPiece->moveCount++;
        gameBoard[destRow][destCol].currPiece = gameBoard[startRow][startCol].currPiece;
        gameBoard[startRow][startCol].currPiece = NULL;
        if(currColor == 0){
            gameBoard[startRow][destCol].currPiece = NULL;
            return;
        }
        else{
            gameBoard[startRow][destCol].currPiece = NULL;
            return;
        }
    }
    if(gameBoard[startRow][startCol].currPiece->pieceType == 5 &&
    gameBoard[startRow][startCol].currPiece->firstMove && abs(startCol-destCol) == 2
    && abs(destRow-startRow) == 0){// We can be sure that the user would like to castle
        if(startRow == 7){
            if(destCol>startCol){
                
                gameBoard[startRow][startCol].currPiece->firstMove = false;
                gameBoard[startRow][startCol].currPiece->moveCount++;
                gameBoard[destRow][destCol].currPiece = gameBoard[startRow][startCol].currPiece;
                gameBoard[startRow][startCol].currPiece = NULL;
                gameBoard[7][7].currPiece->firstMove = false;
                gameBoard[7][5].currPiece = gameBoard[7][7].currPiece;
                gameBoard[7][7].currPiece = NULL;
            }
            else if(startCol>destCol){
                
                gameBoard[startRow][startCol].currPiece->firstMove = false;
                gameBoard[startRow][startCol].currPiece->moveCount++;
                gameBoard[destRow][destCol].currPiece = gameBoard[startRow][startCol].currPiece;
                gameBoard[startRow][startCol].currPiece = NULL;
                gameBoard[7][0].currPiece->firstMove = false;
                gameBoard[7][3].currPiece = gameBoard[7][0].currPiece;
                gameBoard[7][0].currPiece = NULL;
            }
        }
        else if(startRow == 0){
           if(destCol>startCol){
                
                gameBoard[startRow][startCol].currPiece->firstMove = false;
                gameBoard[startRow][startCol].currPiece->moveCount++;
                gameBoard[destRow][destCol].currPiece = gameBoard[startRow][startCol].currPiece;
                gameBoard[startRow][startCol].currPiece = NULL;
                gameBoard[0][7].currPiece->firstMove = false;
                gameBoard[0][5].currPiece = gameBoard[0][7].currPiece;
                gameBoard[0][7].currPiece = NULL;
            }
            else if(startCol>destCol){
                
                gameBoard[startRow][startCol].currPiece->firstMove = false;
                gameBoard[startRow][startCol].currPiece->moveCount++;
                gameBoard[destRow][destCol].currPiece = gameBoard[startRow][startCol].currPiece;
                gameBoard[startRow][startCol].currPiece = NULL;
                gameBoard[0][0].currPiece->firstMove = false;
                gameBoard[0][3].currPiece = gameBoard[0][0].currPiece;
                gameBoard[0][0].currPiece = NULL;
            }
        }
    }
    else if((destRow == 7 || destRow == 0) && gameBoard[startRow][startCol].currPiece->pieceType == 0){
        int promo;
        if(AI == true){promo = 4;}
        else{promo = getPromotionChoice();}
        if(promo == 0){
            gameBoard[startRow][startCol].currPiece->firstMove = false;
            gameBoard[startRow][startCol].currPiece->moveCount++;
            gameBoard[destRow][destCol].currPiece = gameBoard[startRow][startCol].currPiece;
            gameBoard[startRow][startCol].currPiece = NULL;
        }
        else if(promo == 1){
            struct Piece* newPiece = malloc(sizeof(struct Piece));
            newPiece->color = currColor;
            newPiece->pieceType = 1;
            newPiece->firstMove = false;
            newPiece->moveCount = gameBoard[startRow][startCol].currPiece->moveCount;
            gameBoard[destRow][destCol].currPiece = newPiece;
            gameBoard[startRow][startCol].currPiece = NULL;
        }
        else if(promo == 2){
            struct Piece* newPiece = malloc(sizeof(struct Piece));
            newPiece->color = currColor;
            newPiece->pieceType = 2;
            newPiece->firstMove = false;
            newPiece->moveCount = gameBoard[startRow][startCol].currPiece->moveCount;
            gameBoard[destRow][destCol].currPiece = newPiece;
            gameBoard[startRow][startCol].currPiece = NULL;
        }
        else if(promo == 3){
            struct Piece* newPiece = malloc(sizeof(struct Piece));
            newPiece->color = currColor;
            newPiece->pieceType = 3;
            newPiece->firstMove = false;
            newPiece->moveCount = gameBoard[startRow][startCol].currPiece->moveCount;
            gameBoard[destRow][destCol].currPiece = newPiece;
            gameBoard[startRow][startCol].currPiece = NULL;
        }
        else if(promo == 4){
            struct Piece* newPiece = malloc(sizeof(struct Piece));
            newPiece->color = currColor;
            newPiece->pieceType = 4;
            newPiece->firstMove = false;
            newPiece->moveCount = gameBoard[startRow][startCol].currPiece->moveCount;
            gameBoard[destRow][destCol].currPiece = newPiece;
            gameBoard[startRow][startCol].currPiece = NULL;
        }
    }
    else{
    // 1. Move the piece
        gameBoard[startRow][startCol].currPiece->firstMove = false;
        gameBoard[startRow][startCol].currPiece->moveCount++;
        gameBoard[destRow][destCol].currPiece = gameBoard[startRow][startCol].currPiece;
        gameBoard[startRow][startCol].currPiece = NULL; // Clear original square
    }
}
void makeMove(struct Square gameBoard[8][8], struct Move *move, int currColor) {
    int startRow = move->initialRow;
    int startCol = move->initialCol;
    int destRow = move->destinationRow;
    int destCol = move->destinationCol;
    
    int logType = 0;    // initialize log type to just moving
    int movedPiece;     // this will be the piece type for the log
    
    // Ensure the move is valid before execution
    if (!isValidMove(gameBoard, move, currColor)) {
        return;
    }
    if(isEnPassant(gameBoard,move,currColor)){
        gameBoard[startRow][startCol].currPiece->firstMove = false;
        gameBoard[startRow][startCol].currPiece->moveCount++;
        gameBoard[destRow][destCol].currPiece = gameBoard[startRow][startCol].currPiece;

        logType = 1;    // En Passant is also a kind of capture
        movedPiece = 0; // pawn
        addLog(logType, movedPiece, currColor, startRow, startCol, destRow, destCol);
        // the following statement returns so the global addLog will not work

        gameBoard[startRow][startCol].currPiece = NULL;
        if(currColor == 0){
            gameBoard[startRow][destCol].currPiece = NULL;
            return;
        }
        else{
            gameBoard[startRow][destCol].currPiece = NULL;
            return;
        }
    }
    if(gameBoard[startRow][startCol].currPiece->pieceType == 5 &&
    gameBoard[startRow][startCol].currPiece->firstMove && abs(startCol-destCol) == 2
    && abs(destRow-startRow) == 0){// We can be sure that the user would like to castle
        if(startRow == 7){
            if(destCol>startCol){   // white king side castling
                
                gameBoard[startRow][startCol].currPiece->firstMove = false;
                gameBoard[startRow][startCol].currPiece->moveCount++;
                gameBoard[destRow][destCol].currPiece = gameBoard[startRow][startCol].currPiece;
                gameBoard[startRow][startCol].currPiece = NULL;
                gameBoard[7][7].currPiece->firstMove = false;
                gameBoard[7][5].currPiece = gameBoard[7][7].currPiece;
                gameBoard[7][7].currPiece = NULL;

                logType = 2;    // set log type to castling
                movedPiece = 5; // king side castling
            }
            else if(startCol>destCol){  // white queen size castling
                
                gameBoard[startRow][startCol].currPiece->firstMove = false;
                gameBoard[startRow][startCol].currPiece->moveCount++;
                gameBoard[destRow][destCol].currPiece = gameBoard[startRow][startCol].currPiece;
                gameBoard[startRow][startCol].currPiece = NULL;
                gameBoard[7][0].currPiece->firstMove = false;
                gameBoard[7][3].currPiece = gameBoard[7][0].currPiece;
                gameBoard[7][0].currPiece = NULL;

                logType = 2;
                movedPiece = 4; // queen side castling
            }
        }
        else if(startRow == 0){
           if(destCol>startCol){    // black
                
                gameBoard[startRow][startCol].currPiece->firstMove = false;
                gameBoard[startRow][startCol].currPiece->moveCount++;
                gameBoard[destRow][destCol].currPiece = gameBoard[startRow][startCol].currPiece;
                gameBoard[startRow][startCol].currPiece = NULL;
                gameBoard[0][7].currPiece->firstMove = false;
                gameBoard[0][5].currPiece = gameBoard[0][7].currPiece;
                gameBoard[0][7].currPiece = NULL;

                logType = 2;
                movedPiece = 5;
            }
            else if(startCol>destCol){
                
                gameBoard[startRow][startCol].currPiece->firstMove = false;
                gameBoard[startRow][startCol].currPiece->moveCount++;
                gameBoard[destRow][destCol].currPiece = gameBoard[startRow][startCol].currPiece;
                gameBoard[startRow][startCol].currPiece = NULL;
                gameBoard[0][0].currPiece->firstMove = false;
                gameBoard[0][3].currPiece = gameBoard[0][0].currPiece;
                gameBoard[0][0].currPiece = NULL;

                logType = 2;
                movedPiece = 4;
            }
        }
    }
    else if((destRow == 7 || destRow == 0) && gameBoard[startRow][startCol].currPiece->pieceType == 0){
        int promo;
        if(AI == true){promo = 4;}
        else{promo = getPromotionChoice();}
        if(promo == 0){
            gameBoard[startRow][startCol].currPiece->firstMove = false;
            gameBoard[startRow][startCol].currPiece->moveCount++;
            gameBoard[destRow][destCol].currPiece = gameBoard[startRow][startCol].currPiece;
            gameBoard[startRow][startCol].currPiece = NULL;

            movedPiece = promo;     // not promoting, passing on as a regular move
        }
        else if(promo == 1){
            struct Piece* newPiece = malloc(sizeof(struct Piece));
            newPiece->color = currColor;
            newPiece->pieceType = 1;
            newPiece->firstMove = false;
            newPiece->moveCount = gameBoard[startRow][startCol].currPiece->moveCount;
            gameBoard[destRow][destCol].currPiece = newPiece;
            gameBoard[startRow][startCol].currPiece = NULL;

            logType = 3;    // set log type to promotion
            movedPiece = promo; // the piece promoted into
        }
        else if(promo == 2){
            struct Piece* newPiece = malloc(sizeof(struct Piece));
            newPiece->color = currColor;
            newPiece->pieceType = 2;
            newPiece->firstMove = false;
            newPiece->moveCount = gameBoard[startRow][startCol].currPiece->moveCount;
            gameBoard[destRow][destCol].currPiece = newPiece;
            gameBoard[startRow][startCol].currPiece = NULL;
            
            logType = 3;
            movedPiece = promo;
        }
        else if(promo == 3){
            struct Piece* newPiece = malloc(sizeof(struct Piece));
            newPiece->color = currColor;
            newPiece->pieceType = 3;
            newPiece->firstMove = false;
            newPiece->moveCount = gameBoard[startRow][startCol].currPiece->moveCount;
            gameBoard[destRow][destCol].currPiece = newPiece;
            gameBoard[startRow][startCol].currPiece = NULL;
            
            logType = 3;
            movedPiece = promo;
        }
        else if(promo == 4){
            struct Piece* newPiece = malloc(sizeof(struct Piece));
            newPiece->color = currColor;
            newPiece->pieceType = 4;
            newPiece->firstMove = false;
            newPiece->moveCount = gameBoard[startRow][startCol].currPiece->moveCount;
            gameBoard[destRow][destCol].currPiece = newPiece;
            gameBoard[startRow][startCol].currPiece = NULL;
            
            logType = 3;
            movedPiece = promo;
        }
    }
    else{
    // Move the piece / Capture a piece
        if(gameBoard[destRow][destCol].currPiece != NULL) logType = 1;  // set log type to capture

        gameBoard[startRow][startCol].currPiece->firstMove = false;
        gameBoard[startRow][startCol].currPiece->moveCount++;
        gameBoard[destRow][destCol].currPiece = gameBoard[startRow][startCol].currPiece;
        gameBoard[startRow][startCol].currPiece = NULL; // Clear original square

        movedPiece = gameBoard[destRow][destCol].currPiece->pieceType;  // set the piece just moved
    }
    addLog(logType, movedPiece, currColor, startRow, startCol, destRow, destCol);   // a new log is added after each move, no matter what kind
}

void freeBoard(struct Square gameBoard[8][8]){
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (gameBoard[i][j].currPiece != NULL) {
                free(gameBoard[i][j].currPiece);
            }
        }
    }
}
void deepCopyBoard(struct Square gameBoard[8][8], struct Square gameBoardCopy[8][8]) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            gameBoardCopy[i][j] = gameBoard[i][j];
            if (gameBoard[i][j].currPiece != NULL) {
                gameBoardCopy[i][j].currPiece = malloc(sizeof(struct Piece));
                *gameBoardCopy[i][j].currPiece = *gameBoard[i][j].currPiece; // Copy Piece data
            }
        }
    }
}
void generateRookMoves(struct Square gameBoard[8][8], int startRow, int startCol, struct Move moves[], int *numMoves, int currColor) {
    // Directions: Up, Right, Down, Left
    int rowDirections[] = { -1, 0, 1, 0 };
    int colDirections[] = { 0, 1, 0, -1 };

    for (int direction = 0; direction < 4; direction++) {
        int destRow = startRow + rowDirections[direction];
        int destCol = startCol + colDirections[direction];

        while (destRow >= 0 && destRow < 8 && destCol >= 0 && destCol < 8) {
            struct Move move;
            move.initialRow = startRow;
            move.initialCol = startCol;
            move.destinationRow = destRow;
            move.destinationCol = destCol;

            if (isValidMove(gameBoard, &move, currColor)) {
                moves[*numMoves] = move;
                (*numMoves)++;
            }

            // Check if path is blocked
            if (gameBoard[destRow][destCol].currPiece != NULL) {
                break; // Stop checking further in this direction
            }

            // Move to the next square in the direction
            destRow += rowDirections[direction];
            destCol += colDirections[direction];
        }
    }
}
void generateQueenMoves(struct Square gameBoard[8][8], int startRow, int startCol, struct Move moves[], int *numMoves, int currColor) {
    // Rook directions
    int rowDirections[] = { -1, 0, 1, 0 };
    int colDirections[] = { 0, 1, 0, -1 };

    // Bishop Directions
    int bishopRowDirections[] = { -1, 1, 1, -1 };
    int bishopColDirections[] = { 1, 1, -1, -1 };

    // Generate Rook-like moves
    for (int direction = 0; direction < 4; direction++) {
        int destRow = startRow + rowDirections[direction];
        int destCol = startCol + colDirections[direction];

        while (destRow >= 0 && destRow < 8 && destCol >= 0 && destCol < 8) {
            struct Move move;
            move.initialRow = startRow;
            move.initialCol = startCol;
            move.destinationRow = destRow;
            move.destinationCol = destCol;

            if (isValidMove(gameBoard, &move, currColor)) {
                moves[*numMoves] = move;
                (*numMoves)++;
            }

            if (gameBoard[destRow][destCol].currPiece != NULL) {
                break;
            }

            destRow += rowDirections[direction];
            destCol += colDirections[direction];
        }
    }

    // Generate Bishop-like moves
    for (int direction = 0; direction < 4; direction++) {
        int destRow = startRow + bishopRowDirections[direction];
        int destCol = startCol + bishopColDirections[direction];

        while (destRow >= 0 && destRow < 8 && destCol >= 0 && destCol < 8) {
            struct Move move;
            move.initialRow = startRow;
            move.initialCol = startCol;
            move.destinationRow = destRow;
            move.destinationCol = destCol;

            if (isValidMove(gameBoard, &move, currColor)) {
                moves[*numMoves] = move;
                (*numMoves)++;
            }

            if (gameBoard[destRow][destCol].currPiece != NULL) {
                break;
            }

            destRow += bishopRowDirections[direction];
            destCol += bishopColDirections[direction];
        }
    }
}
void generateKingMoves(struct Square gameBoard[8][8], int startRow, int startCol, struct Move moves[], int *numMoves, int currColor) {
    // Possible king moves (offsets)
    int rowDeltas[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
    int colDeltas[] = { -1, 0, 1, -1, 1, -1, 0, 1 };

    for (int i = 0; i < 8; i++) {
        int destRow = startRow + rowDeltas[i];
        int destCol = startCol + colDeltas[i];

        if (destRow >= 0 && destRow < 8 && destCol >= 0 && destCol < 8) {
            struct Move move;
            move.initialRow = startRow;
            move.initialCol = startCol;
            move.destinationRow = destRow;
            move.destinationCol = destCol;

            if (isValidMove(gameBoard, &move, currColor)) {
                moves[*numMoves] = move;
                (*numMoves)++;
            }
        }
    }
    struct Move move;
    move.initialRow = startRow;
    move.initialCol = startCol;
    move.destinationCol = startCol + 2;
    move.destinationRow = startRow;
    if(isValidCastle(gameBoard,&move)){
        moves[*numMoves] = move;
        (*numMoves)++;
    }
    move.destinationCol = startCol - 2;
    if(isValidCastle(gameBoard, &move)){
        moves[*numMoves] = move;
        (*numMoves)++;
    }
}
void generatePawnMoves(struct Square gameBoard[8][8], int startRow, int startCol, struct Move moves[], int *numMoves, int currColor) {
    *numMoves = 0;
    int direction = (currColor == 0) ? -1 : 1;
    // One-step forward
    struct Move move;
    move.initialRow = startRow;
    move.initialCol = startCol;
    move.destinationRow = startRow + direction;
    move.destinationCol = startCol;
    if (isValidMove(gameBoard, &move, currColor)) {
        moves[*numMoves] = move;
        (*numMoves)++;
    }
    // Two-step forward (if on starting rank)
    move.destinationRow = startRow + 2 * direction;
    if ((currColor == 0 && startRow == 6 && (gameBoard[startRow][startCol].currPiece->firstMove == 0)) || ((currColor == 1 && startRow == 1) && (gameBoard[startRow][startCol].currPiece->firstMove == true))) {
        if (gameBoard[startRow + direction][startCol].currPiece == NULL &&
            isValidMove(gameBoard, &move, currColor)) {

            moves[*numMoves] = move;
            (*numMoves)++;
        }
    }
    // Diagonal captures
    for (int deltaCol = -1; deltaCol <= 1; deltaCol += 2) {
        move.destinationRow = startRow + direction;
        move.destinationCol = startCol + deltaCol;
        if (startCol + deltaCol >= 0 && startCol + deltaCol < 8 &&
            gameBoard[move.destinationRow][move.destinationCol].currPiece != NULL &&
            gameBoard[move.destinationRow][move.destinationCol].currPiece->color != currColor &&
            isValidMove(gameBoard, &move, currColor)) {
            moves[*numMoves] = move;
            (*numMoves)++;
        }
    }
    for (int deltaCol = -1; deltaCol <= 1; deltaCol += 2) {
        move.destinationRow = startRow + direction;
        move.destinationCol = startCol + deltaCol;
        if (startCol + deltaCol >= 0 && startCol + deltaCol < 8 &&
            gameBoard[move.destinationRow][move.destinationCol].currPiece == NULL &&
            isValidMove(gameBoard, &move, currColor)) {
            moves[*numMoves] = move;
            (*numMoves)++;
        }
    }
    
}
void generateKnightMoves(struct Square gameBoard[8][8], int startRow, int startCol, struct Move moves[], int *numMoves, int currColor) {
    int possibleDeltas[][2] = { {1, 2}, {2, 1}, {2, -1}, {1, -2}, {-1, -2}, {-2, -1}, {-2, 1}, {-1, 2} };

    for (int i = 0; i < 8; i++) {
        int destRow = startRow + possibleDeltas[i][0];
        int destCol = startCol + possibleDeltas[i][1];

        // Check if the destination is on the board
        if (destRow >= 0 && destRow < 8 && destCol >= 0 && destCol < 8) {
            struct Move move;
            move.initialRow = startRow;
            move.initialCol = startCol;
            move.destinationRow = destRow;
            move.destinationCol = destCol;

            // Check legality and add to moves
            if (isValidMove(gameBoard, &move, currColor)) {
                moves[*numMoves] = move;
                (*numMoves)++;
            }
        }
    }
}
void generateBishopMoves(struct Square gameBoard[8][8], int startRow, int startCol, struct Move moves[], int *numMoves, int currColor) {
    // Directions: Up-Right, Down-Right, Down-Left, Up-Left
    int rowDirections[] = { -1, 1, 1, -1 };
    int colDirections[] = { 1, 1, -1, -1 };

    for (int direction = 0; direction < 4; direction++) {
        int destRow = startRow + rowDirections[direction];
        int destCol = startCol + colDirections[direction];

        while (destRow >= 0 && destRow < 8 && destCol >= 0 && destCol < 8) {
            struct Move move;
            move.initialRow = startRow;
            move.initialCol = startCol;
            move.destinationRow = destRow;
            move.destinationCol = destCol;

            if (isValidMove(gameBoard, &move, currColor)) {
                moves[*numMoves] = move;
                (*numMoves)++;
            }

            // Check if path is blocked
            if (gameBoard[destRow][destCol].currPiece != NULL) {
                break; // Stop checking further in this direction
            }

            // Move to the next diagonal square
            destRow += rowDirections[direction];
            destCol += colDirections[direction];
        }
    }
}

void generateLegalMoves(struct Square gameBoard[8][8], int startRow, int startCol, struct Move moves[], int *numMoves, int currColor) {
    struct Piece *piece = gameBoard[startRow][startCol].currPiece;
    if (piece == NULL || piece->color != currColor) {
        return;
    }

    *numMoves = 0;
    if(piece->pieceType == 0){generatePawnMoves(gameBoard, startRow, startCol, moves, numMoves, currColor);}
    else if(piece->pieceType == 1){generateRookMoves(gameBoard, startRow, startCol, moves, numMoves, currColor);}
    else if(piece->pieceType == 2){generateBishopMoves(gameBoard, startRow, startCol, moves, numMoves, currColor);}
    else if(piece->pieceType == 3){generateKnightMoves(gameBoard, startRow, startCol, moves, numMoves, currColor);}
    else if(piece->pieceType == 4){generateQueenMoves(gameBoard, startRow, startCol, moves, numMoves, currColor);}
    else if(piece->pieceType == 5){
        generateKingMoves(gameBoard, startRow, startCol, moves, numMoves, currColor);
    }
    /*switch(piece->pieceType) {
        case 0: generatePawnMoves(gameBoard, startRow, startCol, moves, numMoves, currColor);
        case 1: generateRookMoves(gameBoard, startRow, startCol, moves, numMoves, currColor);
        case 2: generateBishopMoves(gameBoard, startRow, startCol, moves, numMoves, currColor);
        case 3: generateKnightMoves(gameBoard, startRow, startCol, moves, numMoves, currColor);
        case 4: generateQueenMoves(gameBoard, startRow, startCol, moves, numMoves, currColor);
        case 5: generateKingMoves(gameBoard, startRow, startCol, moves, numMoves, currColor);
    }*/
}
bool canEscapeCheck(struct Square gameBoard[8][8], int currColor) {
    // 1. Ensure the player is actually in check
    if (!possibleCheck(gameBoard, currColor)) {
        return true; // Not in check, so 'escapable'
    }
    // 2. Iterate through the board
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            struct Piece *piece = gameBoard[i][j].currPiece;
            if (piece != NULL && piece->color == currColor) {
                // 3. Generate legal moves for the current piece
                struct Move moves[200];
                int numMoves = 0;
                generateLegalMoves(gameBoard, i, j, moves, &numMoves, currColor);
                
                // 4. Test each legal move
                for (int moveIndex = 0; moveIndex < numMoves; moveIndex++) {
                    struct Square gameBoardCopy[8][8];
                    deepCopyBoard(gameBoard, gameBoardCopy);
                    makeMove_AI(gameBoardCopy, &moves[moveIndex], currColor);

                    if (!possibleCheck(gameBoardCopy, currColor)) {
                        return true; // Found a move that escapes check
                    }
                }
            }
        }
    }

    // If we've tried all moves and none escape check:
    return false;
}
bool outOfCheck(struct Square gameBoard[8][8],struct Move *move,int currColor){
    struct Square gameBoardCopy[8][8];
    deepCopyBoard(gameBoard,gameBoardCopy);
    makeMove_AI(gameBoardCopy,move,currColor);
    if(possibleCheck(gameBoardCopy,currColor)){
        printf("Still in Check!\n");
        freeBoard(gameBoardCopy);
        return false;
    }
    else{
        freeBoard(gameBoardCopy);
        return true;}
}

/**
bool isStalemate(struct Square gameBoard[8][8], int currColor) {
    // Check if the current player is in check
    if (possibleCheck(gameBoard, currColor)) {
        return false; // Not a stalemate if in check
    }

    // Check if the current player has any legal moves
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            struct Piece *piece = gameBoard[i][j].currPiece;
            if (piece != NULL && piece->color == currColor) {
                struct Move moves[200];
                int numMoves = 0;
                generateLegalMoves(gameBoard, i, j, moves, &numMoves, currColor);

                // If we find at least one legal move, it's not stalemate
                if (numMoves > 0) {
                    return false;
                }
            }
        }
    }

    // If we've reached here, there are no legal moves and the player is not in check
    return true;
}
**/

bool isStalemate(struct Square gameBoard[8][8], int currColor) {
    // Check if the current player is in check
    if (possibleCheck(gameBoard, currColor)) {
        return false; // Not a stalemate if in check
    }
    struct Square tempBoard[8][8];
    // Check if the current player has any legal moves
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            struct Piece *piece = gameBoard[i][j].currPiece;
            if (piece != NULL && piece->color == currColor) {
                struct Move moves[200];
                int numMoves = 0;
                generateLegalMoves(gameBoard, i, j, moves, &numMoves, currColor);

                // If we find at least one legal move, it's not stalemate
                for(int i = 0;i<numMoves;i++){
                    deepCopyBoard(gameBoard, tempBoard);
                    makeMove_AI(tempBoard, &moves[i], currColor);
                    if(!possibleCheck(tempBoard, currColor)){return false;}
                }

            }
        }
    }

    // If we've reached here, there are no legal moves and the player is not in check
    return true;
}

bool isCapture(struct Square gameBoard[8][8],const struct Move* move){
    if(gameBoard[move->destinationRow][move->destinationCol].currPiece != NULL){
        return true;
    }
    else{return false;}
}
//AI Section
void verticalMirror(struct Square gameBoard[8][8],struct Square tempBoard[8][8]){
    for(int i = 0;i<7;i++){
        for(int j = 0;j<7;j++){
            tempBoard[i][j].currPiece = NULL;
        }
    }
    int k = 0;
    for(int i = 7;i>=0;i--){
        for(int j = 0;j<=7;j++){
            tempBoard[i][j] = gameBoard[k][j];
            if(gameBoard[k][j].currPiece!=NULL){
                tempBoard[i][j].currPiece = malloc(sizeof(struct Piece));
                *tempBoard[i][j].currPiece = *gameBoard[k][j].currPiece;
            }
        }
        k++;
    }
}
int evaluateBoard(struct Square gameBoard[8][8],int currColor){
    int pawntable[8][8] = {
        {0, 0, 0, 0, 0, 0, 0, 0},
        {5, 10, 10, -20, -20, 10, 10, 5},
        {5, -5, -10, 0, 0, -10, -5, 5},
        {0, 0, 0, 20, 20, 0, 0, 0},
        {5, 5, 10, 25, 25, 10, 5, 5},
        {10, 10, 20, 30, 30, 20, 10, 10},
        {50, 50, 50, 50, 50, 50, 50, 50},
        {0, 0, 0, 0, 0, 0, 0, 0}};
    int knightstable[8][8] = {
        {-50, -40, -30, -30, -30, -30, -40, -50},
        {-40, -20, 0, 5, 5, 0, -20, -40},
        {-30, 5, 10, 15, 15, 10, 5, -30},
        {-30, 0, 15, 20, 20, 15, 0, -30},
        {-30, 5, 15, 20, 20, 15, 5, -30},
        {-30, 0, 10, 15, 15, 10, 0, -30},
        {-40, -20, 0, 0, 0, 0, -20, -40},
        {-50, -40, -30, -30, -30, -30, -40, -50}};
    int bishopstable[8][8] = {
        {-20, -10, -10, -10, -10, -10, -10, -20},
        {-10, 5, 0, 0, 0, 0, 5, -10},
        {-10, 10, 10, 10, 10, 10, 10, -10},
        {-10, 0, 10, 10, 10, 10, 0, -10},
        {-10, 5, 5, 10, 10, 5, 5, -10},
        {-10, 0, 5, 10, 10, 5, 0, -10},
        {-10, 0, 0, 0, 0, 0, 0, -10},
        {-20, -10, -10, -10, -10, -10, -10, -20}};
    int rookstable[8][8] = {
        {0, 0, 0, 5, 5, 0, 0, 0},
        {-5, 0, 0, 0, 0, 0, 0, -5},
        {-5, 0, 0, 0, 0, 0, 0, -5},
        {-5, 0, 0, 0, 0, 0, 0, -5},
        {-5, 0, 0, 0, 0, 0, 0, -5},
        {-5, 0, 0, 0, 0, 0, 0, -5},
        {5, 10, 10, 10, 10, 10, 10, 5},
        {0, 0, 0, 0, 0, 0, 0, 0}};
    int queenstable[8][8] = {
        {-20, -10, -10, -5, -5, -10, -10, -20},
        {-10, 0, 0, 0, 0, 0, 0, -10},
        {-10, 5, 5, 5, 5, 5, 0, -10},
        {0, 0, 5, 5, 5, 5, 0, -5},
        {-5, 0, 5, 5, 5, 5, 0, -5},
        {-10, 0, 5, 5, 5, 5, 0, -10},
        {-10, 0, 0, 0, 0, 0, 0, -10},
        {-20, -10, -10, -5, -5, -10, -10, -20}};
    int kingstable[8][8] = {
        {20, 30, 10, 0, 0, 10, 30, 20},
        {20, 20, 0, 0, 0, 0, 20, 20},
        {-10, -20, -20, -20, -20, -20, -20, -10},
        {-20, -30, -30, -40, -40, -30, -30, -20},
        {-30, -40, -40, -50, -50, -40, -40, -30},
        {-30, -40, -40, -50, -50, -40, -40, -30},
        {-30, -40, -40, -50, -50, -40, -40, -30},
        {-30, -40, -40, -50, -50, -40, -40, -30}};
    int oppositeColor;
    if(currColor==0){
        oppositeColor = 1;
    }
    else{oppositeColor = 0;}
    if(possibleCheck(gameBoard, currColor)){
        return -9999;
    }
    else if(possibleCheck(gameBoard, oppositeColor)){
        return 9999;
    }
    if(isStalemate(gameBoard, currColor)){return 0;}
    else if(isStalemate(gameBoard, oppositeColor)){return 0;}
    int wp = pieceCounter(gameBoard, 0, 0);
    int bp = pieceCounter(gameBoard, 0, 1);
    int wr = pieceCounter(gameBoard, 1, 0);
    int br = pieceCounter(gameBoard, 1, 1);
    int wb = pieceCounter(gameBoard, 2, 0);
    int bb = pieceCounter(gameBoard, 2, 1);
    int wkn = pieceCounter(gameBoard, 3, 0);
    int bkn = pieceCounter(gameBoard, 3, 1);
    int wq = pieceCounter(gameBoard, 4, 0);
    int bq = pieceCounter(gameBoard, 4, 1);
    int material = 100 * (wp - bp) + 320 * (wkn - bkn) + 330 * (wb - bb) + 500 * (wr - br) + 900 * (wq - bq);
    struct Square tempBoard[8][8];
    verticalMirror(gameBoard, tempBoard);
    int pawnsq = pieceEval(gameBoard, pawntable, 0, 0);
    pawnsq = pawnsq - pieceEval(tempBoard, pawntable, 0, 1);
    freeBoard(tempBoard);
    verticalMirror(gameBoard, tempBoard);
    int rooksq = pieceEval(gameBoard, rookstable, 1, 0);
    rooksq = rooksq - pieceEval(tempBoard, rookstable, 1, 1);
    freeBoard(tempBoard);
    verticalMirror(gameBoard, tempBoard);
    int bishopsq = pieceEval(gameBoard, bishopstable, 2, 0);
    bishopsq = bishopsq - pieceEval(tempBoard, bishopstable, 2, 1);
    freeBoard(tempBoard);
    verticalMirror(gameBoard, tempBoard);
    int knightsq = pieceEval(gameBoard, knightstable, 3, 0);
    knightsq = knightsq - pieceEval(tempBoard, knightstable, 3, 1);
    freeBoard(tempBoard);
    verticalMirror(gameBoard, tempBoard);
    int queensq = pieceEval(gameBoard, queenstable, 4, 0);
    queensq = queensq - pieceEval(tempBoard, queenstable, 4, 1);
    freeBoard(tempBoard);
    verticalMirror(gameBoard, tempBoard);
    int kingsq = pieceEval(gameBoard, kingstable, 5, 0);
    kingsq = kingsq - pieceEval(tempBoard, kingstable, 5, 1);
    int eval = material + pawnsq + knightsq + bishopsq + rooksq + queensq + kingsq;
    freeBoard(tempBoard);
    return eval;
}
int pieceEval(struct Square gameBoard[8][8],int evalBoard[8][8],int pieceType,int currColor){
    int score = 0;
    for(int i = 0;i<8;i++){
        for(int j = 0;j<8;j++){
            if(gameBoard[i][j].currPiece == NULL){}
            else if(gameBoard[i][j].currPiece->pieceType == pieceType && gameBoard[i][j].currPiece->color == currColor){
                score = score + evalBoard[i][j];
            }
        }
    }
    return score;
}

int pieceCounter(struct Square gameBoard[8][8],int pieceType,int currColor){
    int count = 0;
    for(int i = 0;i<8;i++){
        for(int j = 0;j<8;j++){
            if(gameBoard[i][j].currPiece == NULL){}
            else if((gameBoard[i][j].currPiece->pieceType == pieceType) && (gameBoard[i][j].currPiece->color == currColor)){
                count++;
            }
        }
    }
    return count;
}

/**
int quiesce(struct Square gameBoard[8][8],int alpha, int beta,int currColor){
    int score;
    int stand_pat = evaluateBoard(gameBoard, currColor);
    if(stand_pat >= beta){
        return beta;
    }
    if(alpha < stand_pat){
        alpha = stand_pat;
    }
    struct Square tempBoard[8][8];
    struct Move moves[200];
    int numMoves;
    
    if(score >= beta){
        return beta;
    }
    if(score > alpha){
        alpha = score;
    }
    return alpha;
}
**/
void clearMoves(struct Move moves[],int *numMoves){
    for(int i = 0;i<*numMoves-1;i++){
        free(&moves[i]);
    }
}
int miniMax(struct Square gameBoard[8][8],int row,int col,int depth,int alpha, int beta,int currPlayer, bool maxPlayer){
/**   
 int oppositeColor = 0;
    if(currPlayer == 1){
        oppositeColor = 0;
    }
    else{oppositeColor = 1;}
**/
    if(depth == 0){
        return evaluateBoard(gameBoard, currPlayer);
    }
    if(maxPlayer){
        int maxEval = -9999;
        struct Square tempBoard[8][8];
        for(int i = 0;i<8;i++){
            for(int j = 0;j<8;j++){
                if(gameBoard[i][j].currPiece != NULL && gameBoard[i][j].currPiece->color == currPlayer){
                    struct Move moves[100];
                    int numMoves = 0;
                    generateLegalMoves(gameBoard,i,j,moves,&numMoves,currPlayer);
                    for(int r =0;r<numMoves;r++){
                        deepCopyBoard(gameBoard, tempBoard);
                        makeMove_AI(tempBoard, &moves[r], currPlayer);
                        //printf("r%d",moves[r].destinationRow);
                        //printf("c%d",moves[r].destinationCol);
                        int eval = miniMax(tempBoard, moves[r].destinationRow, moves[r].destinationCol,depth-1, alpha,beta, 1 - currPlayer, false);
                        //printf("e%de",eval);
                        freeBoard(tempBoard);
                        if(eval>maxEval){maxEval = eval;}
                        if(eval>alpha){alpha = eval;}
                        if(beta<=alpha){
                            break;
                        }

                    }
                }
            }
        }
        //printf("MAX%dMAX",maxEval);
        return maxEval;
    }
    else{
        int minEval = 9999;
        struct Square tempBoard[8][8];
        for(int i = 0;i<8;i++){
            for(int j = 0;j<8;j++){
                if(gameBoard[i][j].currPiece != NULL && gameBoard[i][j].currPiece->color == currPlayer){
                    struct Move moves[400];
                    int numMoves = 0;
                    generateLegalMoves(gameBoard,i,j,moves,&numMoves,currPlayer);
                    for(int r =0;r<numMoves;r++){
                        deepCopyBoard(gameBoard, tempBoard);
                        makeMove_AI(tempBoard, &moves[r], currPlayer);
                        //printf("r%d",moves[r].destinationRow);
                        //printf("c%d",moves[r].destinationCol);
                        int eval = miniMax(tempBoard, moves[r].destinationRow, moves[r].destinationCol,depth-1, alpha,beta, 1 - currPlayer, true);
                        //printf("e%de",eval);
                        freeBoard(tempBoard);
                        if(eval<minEval){minEval = eval;}
                        if(eval<beta){beta = eval;}
                        if(beta<=alpha){
                            break;
                        }
                    }
                }
            }
        }
        //printf("e%de",minEval);
        return minEval;
    }
}
struct Move* tryOpening(struct Square gameBoard[8][8],int currColor){
    if(currColor == 0){
        struct Move* rMove = malloc(sizeof(struct Move));
        rMove->initialRow = 6;
        rMove->initialCol = 3;
        rMove->destinationRow = 4;
        rMove->destinationCol = 3;
        if(isValidMove(gameBoard, rMove, currColor)){
            return rMove;
        }
        rMove->initialCol = 6;
        rMove->initialRow = 7;
        rMove->destinationRow = 5;
        rMove->destinationCol = 5;
        if(isValidMove(gameBoard, rMove, currColor)){
            return rMove;
        }
        rMove->initialCol = 4;
        rMove->initialRow = 6;
        rMove->destinationCol = 4;
        rMove->destinationRow = 5;
        if(isValidMove(gameBoard,rMove,currColor)){
            return rMove;
        }
        rMove->destinationCol = 4;
        rMove->destinationRow = 6;
        rMove->initialRow = 7;
        rMove->initialCol = 5;
        if(isValidMove(gameBoard,rMove,currColor)){
            return rMove;
        }
        free(rMove);
        return NULL;
    }
    else{
        struct Move* rMove = malloc(sizeof(struct Move));
        rMove->initialRow = 1;
        rMove->initialCol = 3;
        rMove->destinationRow = 3;
        rMove->destinationCol = 3;
        if(isValidMove(gameBoard, rMove, currColor)){
            return rMove;
        }
        rMove->initialCol = 6;
        rMove->initialRow = 0;
        rMove->destinationRow = 2;
        rMove->destinationCol = 5;
        if(isValidMove(gameBoard, rMove, currColor)){
            return rMove;
        }
        rMove->initialCol = 4;
        rMove->initialRow = 1;
        rMove->destinationCol = 4;
        rMove->destinationRow = 2;
        if(isValidMove(gameBoard,rMove,currColor)){
            return rMove;
        }
        rMove->destinationCol = 4;
        rMove->destinationRow = 1;
        rMove->initialRow = 0;
        rMove->initialCol = 5;
        if(isValidMove(gameBoard,rMove,currColor)){
            return rMove;
        }
        free(rMove);
        return NULL;
    }
}
struct Move* selectMove(struct Square gameBoard[8][8],int currColor){
    struct Move* rMove;
    if(possibleCheck(gameBoard, currColor)){
        rMove = NULL;
    }
    else{
        rMove = tryOpening(gameBoard, currColor);
    }
    if(rMove!=NULL){return rMove;}
    else{rMove = malloc(sizeof(struct Move));}
    int addition;
    int maxScore;
    if(currColor == 1){
        maxScore = 9999;
        addition = -1;
    }
    else{
        maxScore = -9999;
        addition = 1;
    }
    int totalEval;
    int numMoves;
    struct Square tempBoard[8][8];
    for(int i = 0;i<8;i++){
        for(int j = 0;j<8;j++){
            if(gameBoard[i][j].currPiece != NULL && gameBoard[i][j].currPiece->color == currColor){
                struct Move moves[400];
                numMoves = 0;
                generateLegalMoves(gameBoard,i,j,moves,&numMoves,currColor);
                //if(gameBoard[i][j].currPiece!=NULL){printf("-%d-/",gameBoard[i][j].currPiece->pieceType);}
                for(int r = 0;r<numMoves;r++){
                    deepCopyBoard(gameBoard, tempBoard);
                    makeMove_AI(tempBoard, &moves[r], currColor);
                    bool maxPlayer = false;
                    if(currColor == 1){maxPlayer = true;}
                    totalEval = miniMax(tempBoard, moves[r].destinationRow, moves[r].destinationCol,1, -9999,9999, 1 - currColor, maxPlayer);
                    if(isEnPassant(gameBoard, &moves[r], currColor)){
                        totalEval = totalEval + (addition*75);
                    }
                    if(isValidCastle(gameBoard, &moves[r])){
                        totalEval = totalEval + (addition*100);
                    }
                    if(currColor == 1){
                        if(totalEval<=maxScore && !possibleCheck(tempBoard, currColor)){
                            maxScore = totalEval;
                            rMove->initialCol = moves[r].initialCol;
                            rMove->initialRow = moves[r].initialRow;
                            rMove->destinationCol = moves[r].destinationCol;
                            rMove->destinationRow = moves[r].destinationRow;
                        }
                        freeBoard(tempBoard);
                    }
                    else{
                        if(totalEval>=maxScore && !possibleCheck(tempBoard,currColor)){
                            maxScore = totalEval;
                            rMove->initialCol = moves[r].initialCol;
                            rMove->initialRow = moves[r].initialRow;
                            rMove->destinationCol = moves[r].destinationCol;
                            rMove->destinationRow = moves[r].destinationRow;
                        }
                        freeBoard(tempBoard);
                    }
                    
                }
            }
        }
    }
    //printf("^%d^",maxScore);
    return rMove;
}

void makeMove_ORG(struct Square gameBoard[8][8], struct Move *move, int currColor) {
    int startRow = move->initialRow;
    int startCol = move->initialCol;
    int destRow = move->destinationRow;
    int destCol = move->destinationCol;
    
    // Ensure the move is valid before execution
    if (!isValidMove(gameBoard, move, currColor)) {
        return;
    }
    if(isEnPassant(gameBoard,move,currColor)){
        gameBoard[startRow][startCol].currPiece->firstMove = false;
        gameBoard[startRow][startCol].currPiece->moveCount++;
        gameBoard[destRow][destCol].currPiece = gameBoard[startRow][startCol].currPiece;
        gameBoard[startRow][startCol].currPiece = NULL;
        if(currColor == 0){
            gameBoard[startRow][destCol].currPiece = NULL;
            return;
        }
        else{
            gameBoard[startRow][destCol].currPiece = NULL;
            return;
        }
    }
    if(gameBoard[startRow][startCol].currPiece->pieceType == 5 &&
    gameBoard[startRow][startCol].currPiece->firstMove && abs(startCol-destCol) == 2
    && abs(destRow-startRow) == 0){// We can be sure that the user would like to castle
        if(startRow == 7){
            if(destCol>startCol){
                
                gameBoard[startRow][startCol].currPiece->firstMove = false;
                gameBoard[startRow][startCol].currPiece->moveCount++;
                gameBoard[destRow][destCol].currPiece = gameBoard[startRow][startCol].currPiece;
                gameBoard[startRow][startCol].currPiece = NULL;
                gameBoard[7][7].currPiece->firstMove = false;
                gameBoard[7][5].currPiece = gameBoard[7][7].currPiece;
                gameBoard[7][7].currPiece = NULL;
            }
            else if(startCol>destCol){
                
                gameBoard[startRow][startCol].currPiece->firstMove = false;
                gameBoard[startRow][startCol].currPiece->moveCount++;
                gameBoard[destRow][destCol].currPiece = gameBoard[startRow][startCol].currPiece;
                gameBoard[startRow][startCol].currPiece = NULL;
                gameBoard[7][0].currPiece->firstMove = false;
                gameBoard[7][3].currPiece = gameBoard[7][0].currPiece;
                gameBoard[7][0].currPiece = NULL;
            }
        }
        else if(startRow == 0){
           if(destCol>startCol){
                
                gameBoard[startRow][startCol].currPiece->firstMove = false;
                gameBoard[startRow][startCol].currPiece->moveCount++;
                gameBoard[destRow][destCol].currPiece = gameBoard[startRow][startCol].currPiece;
                gameBoard[startRow][startCol].currPiece = NULL;
                gameBoard[0][7].currPiece->firstMove = false;
                gameBoard[0][5].currPiece = gameBoard[0][7].currPiece;
                gameBoard[0][7].currPiece = NULL;
            }
            else if(startCol>destCol){
                
                gameBoard[startRow][startCol].currPiece->firstMove = false;
                gameBoard[startRow][startCol].currPiece->moveCount++;
                gameBoard[destRow][destCol].currPiece = gameBoard[startRow][startCol].currPiece;
                gameBoard[startRow][startCol].currPiece = NULL;
                gameBoard[0][0].currPiece->firstMove = false;
                gameBoard[0][3].currPiece = gameBoard[0][0].currPiece;
                gameBoard[0][0].currPiece = NULL;
            }
        }
    }
    else if((destRow == 7 || destRow == 0) && gameBoard[startRow][startCol].currPiece->pieceType == 0){
        int promo;
        if(AI == true){promo = 4;}
        else{promo = getPromotionChoice();}
        if(promo == 0){
            gameBoard[startRow][startCol].currPiece->firstMove = false;
            gameBoard[startRow][startCol].currPiece->moveCount++;
            gameBoard[destRow][destCol].currPiece = gameBoard[startRow][startCol].currPiece;
            gameBoard[startRow][startCol].currPiece = NULL;
        }
        else if(promo == 1){
            struct Piece* newPiece = malloc(sizeof(struct Piece));
            newPiece->color = currColor;
            newPiece->pieceType = 1;
            newPiece->firstMove = false;
            newPiece->moveCount = gameBoard[startRow][startCol].currPiece->moveCount;
            gameBoard[destRow][destCol].currPiece = newPiece;
            gameBoard[startRow][startCol].currPiece = NULL;
        }
        else if(promo == 2){
            struct Piece* newPiece = malloc(sizeof(struct Piece));
            newPiece->color = currColor;
            newPiece->pieceType = 2;
            newPiece->firstMove = false;
            newPiece->moveCount = gameBoard[startRow][startCol].currPiece->moveCount;
            gameBoard[destRow][destCol].currPiece = newPiece;
            gameBoard[startRow][startCol].currPiece = NULL;
        }
        else if(promo == 3){
            struct Piece* newPiece = malloc(sizeof(struct Piece));
            newPiece->color = currColor;
            newPiece->pieceType = 3;
            newPiece->firstMove = false;
            newPiece->moveCount = gameBoard[startRow][startCol].currPiece->moveCount;
            gameBoard[destRow][destCol].currPiece = newPiece;
            gameBoard[startRow][startCol].currPiece = NULL;
        }
        else if(promo == 4){
            struct Piece* newPiece = malloc(sizeof(struct Piece));
            newPiece->color = currColor;
            newPiece->pieceType = 4;
            newPiece->firstMove = false;
            newPiece->moveCount = gameBoard[startRow][startCol].currPiece->moveCount;
            gameBoard[destRow][destCol].currPiece = newPiece;
            gameBoard[startRow][startCol].currPiece = NULL;
        }
    }
    else{
    // 1. Move the piece
        gameBoard[startRow][startCol].currPiece->firstMove = false;
        gameBoard[startRow][startCol].currPiece->moveCount++;
        gameBoard[destRow][destCol].currPiece = gameBoard[startRow][startCol].currPiece;
        gameBoard[startRow][startCol].currPiece = NULL; // Clear original square
    }
    
}


