
#include <gtk/gtk.h>
#include "chess_gui.h"
#include "struct.h"
#include "rules.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

GtkWidget *window ; 
GtkWidget *chess_icon ; 
GtkWidget *table ;

GtkWidget *label_grid;
GtkWidget *label_piece;
GtkWidget *label_status;
GtkWidget *label_firstP;
GtkWidget *label_secondP;
GtkWidget *label_move;
GtkWidget *label_round;
GtkWidget *label_player;
GtkWidget *label_valid;

int status = 0;
int firstP_x;
int firstP_y;
int secondP_x;
int secondP_y;
int rounds_n = 0;
char valid[20]="valid?:xxx";



gint  delete_event (GtkWidget *widget,GdkEvent  *event, gpointer   data);
gboolean  button_press_event (GtkWidget *widget,GdkEventButton  *event, /*gpointer   data*/ /*struct Square data[8][8]*/struct Move *move);
void redisplay();
void CoordToGrid(int c_x, int c_y, int *g_x, int *g_y);
void DrawBoard(struct Square gameboard[8][8]);

int currColor = 0;
struct Square board[8][8];


//struct Square board[8][8];
gint  delete_event (GtkWidget *widget,GdkEvent  *event, gpointer   data)
{
  	
	g_print ("delete event occurred\n"); 
  	gtk_main_quit();
  	return FALSE;
}

gboolean  button_press_event (GtkWidget *widget,GdkEventButton  *event, /*gpointer data*/ /*struct Square  data[8][8]*/struct Move *move){
	if (event->button == 1) {
        // Retrieve mouse coordinates
        int c_x = (int)event->x;
        int c_y = (int)event->y;
	
	int g_x, g_y;
	CoordToGrid(c_x,c_y,&g_x,&g_y);
        // Print mouse coordinates
        g_print("Clicked, c_x=%d, c_y=%d, g_x=%d, g_y=%d\n", c_x, c_y,g_x,g_y);
	gchar *text_grid;
	gchar *text_piece;
	gchar *text_status;
	gchar *text_firstP;
	gchar *text_secondP;
	gchar *text_move;
	gchar *text_round;
	gchar *text_player;
	gchar *text_valid;

		strcpy(valid,"valid?:xxx");
		if(g_x==-1 || g_y==-1){
			text_grid = g_strdup_printf("MouseP[x,x]");
			text_piece = g_strdup_printf("Piece:XX");
			text_move = g_strdup_printf("move:xxxx");
			if(status==0){
                        text_firstP = g_strdup_printf("firstP[x,x]");
                        text_secondP = g_strdup_printf("secondP[x,x]");
                        firstP_x = g_x;
                        firstP_y = g_y;
                        }
                        else {
                        text_firstP = g_strdup_printf("firstP[%d,%d]",firstP_x,firstP_y);
                        text_secondP = g_strdup_printf("secondP[x,x]");
                        secondP_x = g_x;
                        secondP_y = g_y;
                        }

		}
		else{
			text_grid = g_strdup_printf("MouseP[%d,%d]", g_x, g_y);
			struct Piece *piece = board[g_y][g_x].currPiece;
			int piece_color;
			int piece_role;
			char piece_color_c;
			char piece_role_c;
			char color_array[3]="WB";
			char role_array[7]="PRBNQK";
			if(piece==NULL){
				piece_color_c = 'X';
                                piece_role_c = 'X';
				text_piece = g_strdup_printf("Piece:XX");
			}
			else{
				//text_piece = g_strdup_printf("Piece:%c%c",piece->color,piece->pieceType);
				piece_color = piece->color;
				piece_role = piece->pieceType;
				piece_color_c = color_array[piece_color];
				piece_role_c = role_array[piece_role];
				text_piece = g_strdup_printf("Piece:%c%c",piece_color_c,piece_role_c);

			}
			if(status==0){
			text_firstP = g_strdup_printf("MouseP[%d,%d]",g_x,g_y);
			text_secondP = g_strdup_printf("MouseP[x,x]");
			text_move = g_strdup_printf("move:xxxx");
			firstP_x = g_x;
			firstP_y = g_y;
			status++;
			}
			else {
			text_firstP = g_strdup_printf("MouseP[%d,%d]",firstP_x,firstP_y);
			text_secondP = g_strdup_printf("MouseP[%d,%d]",g_x,g_y);
			secondP_x = g_x;
			secondP_y = g_y;
			char Move_1 = (firstP_x)+97;
			char Move_2 = (8-firstP_y)+48;
			char Move_3 = (secondP_x)+97;
			char Move_4 = (8-secondP_y)+48;
			g_print("Making move:%c%c%c%c\n",Move_1,Move_2,Move_3,Move_4);
			text_move = g_strdup_printf("move:%c%c%c%c",Move_1,Move_2,Move_3,Move_4);
			//char input[5];
			//input[0]=Move_1;
			//input[1]=Move_2;
			//input[2]=Move_3;
			//input[3]=Move_4;
			//getPlayerMove_rint_TEST(move,input);
			move->initialCol=firstP_x;
			move->initialRow=firstP_y;
			move->destinationCol=secondP_x;
			move->destinationRow=secondP_y;
			if(isValidMove(board, move, currColor)){
				rounds_n++;
				g_print("Valid move\n");
				strcpy(valid,"valid?:valid");
				makeMove_ORG(board,move,currColor);
				redisplay();
				if(currColor==0){
					currColor=1;
				}
				else{
					currColor=0;
				}

				if(isStalemate(board,currColor)){
					printf("Stalemate! Ending Game.\n");
					gtk_main_quit();
				}
				if(possibleCheck(board, currColor)){
					printf("Player is in Check!\n");
					if(!canEscapeCheck(board, currColor)){ // No escape; game over
                    				printf("Checkmate!! Game Over!\n");
                    				gtk_main_quit();
                			}
				}

			}
			else{
				g_print("Invalid move\n");
				strcpy(valid,"valid?:invalid");
			}
                        status--;
                        }
		}
		if(currColor==0){
		text_player=g_strdup_printf("Player:White");
		}
		else{
		text_player=g_strdup_printf("Player:Black");
		}
		text_status = g_strdup_printf("Status:%d",status);
		text_round = g_strdup_printf("round:%d",rounds_n);
		text_valid = g_strdup_printf(valid);

		// Update label text
        	gtk_label_set_text(GTK_LABEL(label_grid), text_grid);
		gtk_label_set_text(GTK_LABEL(label_piece), text_piece);
		gtk_label_set_text(GTK_LABEL(label_status),text_status);
		gtk_label_set_text(GTK_LABEL(label_firstP),text_firstP);
		gtk_label_set_text(GTK_LABEL(label_secondP),text_secondP);
		gtk_label_set_text(GTK_LABEL(label_move),text_move);
		gtk_label_set_text(GTK_LABEL(label_round),text_round);
		gtk_label_set_text(GTK_LABEL(label_player),text_player);
		gtk_label_set_text(GTK_LABEL(label_valid),text_valid);

        	// Free memory
        	g_free(text_grid);
		g_free(text_piece);
		g_free(text_status);
		g_free(text_firstP);
		g_free(text_secondP);
		g_free(text_move);
		g_free(text_round);
		g_free(text_player);
		g_free(text_valid);
	}
	//gtk_main_quit();
	return TRUE;

}
void redisplay(struct Square gameboard[8][8]){
	gtk_container_remove(GTK_CONTAINER(window),table);
	table = gtk_table_new (dim, dim, TRUE) ;
 	gtk_widget_set_size_request (table, BOARD_WIDTH, BOARD_HEIGHT) ;
  	DrawBoard(gameboard);
	label_grid = gtk_label_new("MouseP[x,x]");
  	gtk_table_attach(GTK_TABLE(table),label_grid,9,13,0,1,GTK_FILL,GTK_FILL,0,0);

  	label_piece = gtk_label_new("Piece:XX");
  	gtk_table_attach(GTK_TABLE(table),label_piece,9,13,1,2,GTK_FILL,GTK_FILL,0,0);

  	label_status = gtk_label_new("Status:0");
  	gtk_table_attach(GTK_TABLE(table),label_status,9,13,2,3,GTK_FILL,GTK_FILL,0,0);

	label_firstP = gtk_label_new("firstP[x,x]");
        gtk_table_attach(GTK_TABLE(table),label_firstP,9,13,3,4,GTK_FILL,GTK_FILL,0,0);

	label_secondP = gtk_label_new("secondP[x,x]");
        gtk_table_attach(GTK_TABLE(table),label_secondP,9,13,4,5,GTK_FILL,GTK_FILL,0,0);

	label_move = gtk_label_new("move:xxxx");
        gtk_table_attach(GTK_TABLE(table),label_move,9,13,5,6,GTK_FILL,GTK_FILL,0,0);

	char input_message[10]="round: ";
	input_message[6]=rounds_n+48;
	label_round = gtk_label_new(input_message);
        gtk_table_attach(GTK_TABLE(table),label_round,9,13,6,7,GTK_FILL,GTK_FILL,0,0);

	if(currColor==0){
		label_player = gtk_label_new("Player:White");
	}
	else{
		label_player = gtk_label_new("Player:Black");
	}
	gtk_table_attach(GTK_TABLE(table),label_player,9,13,7,8,GTK_FILL,GTK_FILL,0,0);

	label_valid = gtk_label_new(valid);
        gtk_table_attach(GTK_TABLE(table),label_valid,9,13,8,9,GTK_FILL,GTK_FILL,0,0);

	gtk_container_add(GTK_CONTAINER(window),table);
	gtk_widget_show_all(window) ;
}

void CoordToGrid(int c_x, int c_y, int *g_x, int *g_y)
{
        // *g_x = (c_x - BOARD_BORDER) / SQUARE_SIZE;
        // *g_y = (c_y - BOARD_BORDER) / SQUARE_SIZE;
	 int g_x_temp = (c_x-40)/SQUARE_SIZE;
         int g_y_temp = c_y/SQUARE_SIZE;
	 if(c_x<40 || g_x_temp<0 || g_x_temp>7){
		*g_x=-1;
                *g_y=-1;

	}
	else if(g_y_temp<0 || g_y_temp>7){
		*g_x=-1;
                *g_y=-1;
	}
	else{
		*g_x=g_x_temp;
		*g_y=g_y_temp;
	}

}

void DrawBoard(struct Square gameboard[8][8])
{

	char color;
	char role;
	char background;
	char input2[5];
	char input1[14]="./chess_icon/";
        char input3[5] = ".png";
        char filepath[24];

	gtk_table_attach(GTK_TABLE(table), gtk_image_new_from_file("./chess_icon/Chess_8.png") , 0, 1, 0, 1, GTK_FILL, GTK_FILL, 0, 0) ;
	gtk_table_attach(GTK_TABLE(table), gtk_image_new_from_file("./chess_icon/Chess_7.png") , 0, 1, 1, 2, GTK_FILL, GTK_FILL, 0, 0) ;
	gtk_table_attach(GTK_TABLE(table), gtk_image_new_from_file("./chess_icon/Chess_6.png") , 0, 1, 2, 3, GTK_FILL, GTK_FILL, 0, 0) ;
	gtk_table_attach(GTK_TABLE(table), gtk_image_new_from_file("./chess_icon/Chess_5.png") , 0, 1, 3, 4, GTK_FILL, GTK_FILL, 0, 0) ;
	gtk_table_attach(GTK_TABLE(table), gtk_image_new_from_file("./chess_icon/Chess_4.png") , 0, 1, 4, 5, GTK_FILL, GTK_FILL, 0, 0) ;
	gtk_table_attach(GTK_TABLE(table), gtk_image_new_from_file("./chess_icon/Chess_3.png") , 0, 1, 5, 6, GTK_FILL, GTK_FILL, 0, 0) ;
	gtk_table_attach(GTK_TABLE(table), gtk_image_new_from_file("./chess_icon/Chess_2.png") , 0, 1, 6, 7, GTK_FILL, GTK_FILL, 0, 0) ;
	gtk_table_attach(GTK_TABLE(table), gtk_image_new_from_file("./chess_icon/Chess_1.png") , 0, 1, 7, 8, GTK_FILL, GTK_FILL, 0, 0) ;

	gtk_table_attach(GTK_TABLE(table), gtk_image_new_from_file("./chess_icon/Chess_a.png") , 1, 2, 8, 9, GTK_FILL, GTK_FILL, 0, 0) ;
	gtk_table_attach(GTK_TABLE(table), gtk_image_new_from_file("./chess_icon/Chess_b.png") , 2, 3, 8, 9, GTK_FILL, GTK_FILL, 0, 0) ;
	gtk_table_attach(GTK_TABLE(table), gtk_image_new_from_file("./chess_icon/Chess_c.png") , 3, 4, 8, 9, GTK_FILL, GTK_FILL, 0, 0) ;
	gtk_table_attach(GTK_TABLE(table), gtk_image_new_from_file("./chess_icon/Chess_d.png") , 4, 5, 8, 9, GTK_FILL, GTK_FILL, 0, 0) ;
	gtk_table_attach(GTK_TABLE(table), gtk_image_new_from_file("./chess_icon/Chess_e.png") , 5, 6, 8, 9, GTK_FILL, GTK_FILL, 0, 0) ;
	gtk_table_attach(GTK_TABLE(table), gtk_image_new_from_file("./chess_icon/Chess_f.png") , 6, 7, 8, 9, GTK_FILL, GTK_FILL, 0, 0) ;
	gtk_table_attach(GTK_TABLE(table), gtk_image_new_from_file("./chess_icon/Chess_g.png") , 7, 8, 8, 9, GTK_FILL, GTK_FILL, 0, 0) ;
	gtk_table_attach(GTK_TABLE(table), gtk_image_new_from_file("./chess_icon/Chess_h.png") , 8, 9, 8, 9, GTK_FILL, GTK_FILL, 0, 0) ;
    for(int x = 0; x < dim; x ++)
	{
		for(int y = 0; y < dim; y ++)
		{
			if(x%2==1){
        
                        if(y%2==0){
                                        background = 'W';
                                }
                                else{
                                     	background = 'B';
                                }
                        }
                        if(x%2==0){ 
                                if(y%2==0){   
                                        background = 'B';
                                }
                                else{ 
                                      	background = 'W';
                                }
                        }
			struct Piece *piece = gameboard[y][x].currPiece;
			if(piece==NULL){
				color = 'X';
				role = 'X';
			}
			else{
			//piece = gameboard[x][y].currPiece;
			//printf("%d\n",piece->color);
				if(piece->color == 0){
					color = 'W';
				}
				else{
					color = 'B';
				}
				switch(piece->pieceType){
					case 0: role = 'P'; break;
                        		case 1: role = 'R'; break;
                        		case 2: role = 'B'; break;
                        		case 3: role = 'N'; break;
                        		case 4: role = 'Q'; break;
                        		case 5: role = 'K'; break;
				}
			}
			//printf("at posisition %d%d, Piece is %c%c\n",x,y,color,role);
			//printf("at posisition %d%d, Piece is %cB%c%c\n",x,y,background,color,role);
			//char input2[5];
			input2[0]=background;
			input2[1]='B';
			input2[2]=color;
			input2[3]=role;
			//char input1[13]="./chess_icon/";
			//char input3[4] = ".png";
			//char filepath[21];
                        sprintf(filepath, "%s%s%s", input1, input2, input3);
			//printf("%s\n",filepath);
			chess_icon = gtk_image_new_from_file(filepath) ;

			gtk_table_attach(GTK_TABLE(table), chess_icon, x+1, x + 2, y, y + 1, GTK_FILL, GTK_FILL, 0, 0) ;
		}
	}

}


int main(int   argc, char *argv[]){
	//struct Square board[8][8];
	initializeBoard(board);
	//printBoard(board);

	struct Move *move1 = malloc(sizeof(struct Move));
	
	gtk_init(&argc, &argv) ;
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL) ;
 	gtk_widget_set_size_request(window, WINDOW_WIDTH, WINDOW_HEIGHT) ; 
  	gtk_container_set_border_width (GTK_CONTAINER(window), WINDOW_BORDER) ; 
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER) ;
  	//gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_MOUSE) ; 
  	gtk_window_set_title(GTK_WINDOW(window), "chess_gui") ; 
  	gtk_window_set_resizable(GTK_WINDOW(window), FALSE) ; 


	g_signal_connect(window, "delete_event", G_CALLBACK(delete_event), NULL) ;

	g_signal_connect(G_OBJECT(window), "button_press_event",G_CALLBACK(button_press_event),/*board*/move1);
  	gtk_widget_add_events(window,GDK_BUTTON_PRESS_MASK); 

	table = gtk_table_new (dim, dim, TRUE) ; 
	gtk_widget_set_size_request (table, BOARD_WIDTH, BOARD_HEIGHT) ; 
  	
	DrawBoard(board);

	label_grid = gtk_label_new("MouseP[x,x]");
  gtk_table_attach(GTK_TABLE(table),label_grid,9,13,0,1,GTK_FILL,GTK_FILL,0,0);

  label_piece = gtk_label_new("Piece:XX");
  gtk_table_attach(GTK_TABLE(table),label_piece,9,13,1,2,GTK_FILL,GTK_FILL,0,0);

  label_status = gtk_label_new("Status:0");
  gtk_table_attach(GTK_TABLE(table),label_status,9,13,2,3,GTK_FILL,GTK_FILL,0,0);

	label_firstP = gtk_label_new("firstP[x,x]");
        gtk_table_attach(GTK_TABLE(table),label_firstP,9,13,3,4,GTK_FILL,GTK_FILL,0,0);

	label_secondP = gtk_label_new("secondP[x,x]");
        gtk_table_attach(GTK_TABLE(table),label_secondP,9,13,4,5,GTK_FILL,GTK_FILL,0,0);

	label_move = gtk_label_new("move:xxxx");
        gtk_table_attach(GTK_TABLE(table),label_move,9,13,5,6,GTK_FILL,GTK_FILL,0,0);

	label_round = gtk_label_new("round:0");
        gtk_table_attach(GTK_TABLE(table),label_round,9,13,6,7,GTK_FILL,GTK_FILL,0,0);

        label_player = gtk_label_new("Player:White");
        gtk_table_attach(GTK_TABLE(table),label_player,9,13,7,8,GTK_FILL,GTK_FILL,0,0);

	label_valid = gtk_label_new(valid);
        gtk_table_attach(GTK_TABLE(table),label_valid,9,13,8,9,GTK_FILL,GTK_FILL,0,0);


	gtk_container_add(GTK_CONTAINER(window),table);
	gtk_widget_show_all(window);
	gtk_main();
	//printf("%d",board[1][1].currPiece->color);

}
