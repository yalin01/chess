#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"
#include "rules.h"
#include <ctype.h>
#include <stdbool.h>

int main(int argc, const char * argv[]) {

	printf("testing chess program rulecheck with a classic chess game.\n");
	printf("\n\nTesting PVP mode: Fastest checkMate.\n\n");
        int counter_PVP = 0;
        char input_string_PVP[100][10];
        counter_PVP = addMove(input_string_PVP,counter_PVP,"f2f3");
        counter_PVP = addMove(input_string_PVP,counter_PVP,"no");
	counter_PVP = addMove(input_string_PVP,counter_PVP,"e7e5");
        counter_PVP = addMove(input_string_PVP,counter_PVP,"no");
	counter_PVP = addMove(input_string_PVP,counter_PVP,"g2g4");
        counter_PVP = addMove(input_string_PVP,counter_PVP,"no");
	counter_PVP = addMove(input_string_PVP,counter_PVP,"d8h4");
        counter_PVP = addMove(input_string_PVP,counter_PVP,"no");
	//counter_PVP = addMove(input_string_PVP,counter_PVP,"exit");
	PlayerMode_1_TEST(input_string_PVP);



}
