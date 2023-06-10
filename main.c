#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

typedef struct tictactoe{
    char board[3][3];
    int num_X; //number of X on board
    int num_O; //number of O on board
}tictactoe;

//global section
tictactoe game;

//helper functions
void make_board(){ //ptr to a tictactoe object
    //fills each section in board with a dash
    //use this functions for reset cmd to kernel
    game.num_X = 0; //initialize to zero
    game.num_O = 0; //initialize to zero
    int i;
    for (i = 0; i < 3; i++){
        int z;
        for (z = 0; z < 3; z++){
            game.board[i][z] = '-';
        }
    }
}

void print_board(){
    //prints out board
    printf("- - A B C \n");
    printf("  -------- \n");
    int i;
    for (i = 0; i < 3; i++){
        printf("%d | ", i + 1);
        int z;
        for (z = 0; z < 3; z++){
            printf("%c ", game.board[i][z]);
        }
        printf("\n");
    }
}

int validate_move(char column, char row, char turn){
    int m1;
    int m2;
    if (row != '1' && row != '2' && row != '3'){
        printf("tictactoe: invalid row, out of bounds \n");
        return 0;
    }
    if (column != 'A' && column != 'B' && column != 'C'){
        printf("tictactoe: invalid column, out of bounds \n");
        return 0;
    }

    //adjust m1, m2 for the index with arrays
    m1 = row - '1'; //ASCII Math
    m2 = column - 'A'; //A - Ascii A = 0, B - Ascii B = 1, etc
    //function takes board ptr, move 1, move 2, char to know whose turn
    if (turn != 'X' && turn != 'O'){
        //invalid turn, not X or O
        printf("ticactoe: invalid move \n");
        return 0;
    }
    if (turn == 'X' && game.num_X != game.num_O){
        //X can only move if both players have done same num of moves
        //Since X always goes first X and O should be equal
        printf("tictactoe: dev error \n");
        return 0;
    }
    if (turn == 'O' && game.num_X != game.num_O + 1){
        //X can only move if both players have done same num of moves
        //if O turn shoud always be one more X then O
        printf("tictactoe: dev error \n");
        return 0;
    }
    if (game.board[m1][m2] != '-'){
        printf("tictactoe: occupied cell \n");
        return 0;
    }
    game.board[m1][m2] = turn; //implements turn
    if (turn == 'X'){
        game.num_X ++;
        //incrememnts X counter
    }
    else{
        game.num_O ++;
        //increments O counter
    }
    return 1;
}

int parse_command(char* instruction){
    int i;
    for (i = 0; instruction[i] != '\0'; i++){ // '\0' is null char
        instruction[i] = toupper(instruction[i]); //moves to capital chars
    }
    int temp;
    temp = strcmp("RESET", instruction); //return zero if same
    if (temp == 0){
        //resets the board and restarts the game
        make_board();
        return 1;
    }
    temp = strcmp("BOARD", instruction);
    if (temp == 0){
        //prints out current state of board
        print_board();
        return 1;
    }
    temp = strncmp("TURN", instruction, 4);
    int len = strlen(instruction);
    //strncmp("Phrase 1", "Phrase 2", Max num of letters to compare);
    if (temp != 0){
        printf("tictactoe: invalid command \n");
        return 0;
    }
    if (len < 8){
        //min len is 8 following command structure
        //"TURN A 3"
        printf("tictactoe: invalid command \n");
        return 0;
    }
    int res = validate_move(instruction[5], instruction[7], 'X');
    if (res == 0){
        //was invalid move
        return 0;
    }
    //function call to AI turn here
    return 2; //2 indicates command was to make a turn, AI stil needs to go, game over not checked
}

int game_over(){
    //o is game not done
    //1 is won
    //2 is tie
    if (game.board[0][0] != '-' && game.board[0][0] == game.board[0][1] && game.board[0][1] == game.board[0][2]){
        return 1;
    }
    if (game.board[1][0] != '-' && game.board[1][0] == game.board[1][1] && game.board[1][1] == game.board[1][2]){
       return 1;
    }
    if (game.board[2][0] != '-' && game.board[2][0] == game.board[2][1] && game.board[2][1] == game.board[2][2]){
        return 1;
    }
    if (game.board[0][0] != '-' && game.board[0][0] == game.board[1][0] && game.board[1][0] == game.board[2][0]){
        return 1;
    }
    if (game.board[0][1] != '-' && game.board[0][1] == game.board[1][1] && game.board[1][1] == game.board[2][1]){
        return 1;
    }
    if (game.board[0][2] != '-' && game.board[0][2] == game.board[1][2] && game.board[1][2] == game.board[2][2]){
        return 1;
    }
    if (game.board[0][0] != '-' && game.board[0][0] == game.board[1][1] && game.board[1][1] == game.board[2][2]){
        return 1;
    }
    if (game.board[0][2] != '-' && game.board[0][2] == game.board[1][1] && game.board[1][1] == game.board[2][0]){
        return 1;
    }
    if (game.num_X == 5){
        //tie
        return 2;
    }
    //game still going
    return 0;
}

void move_AI(){
    int m1;
    int m2;
    while (1){
        m1 = rand() % 3;
        m2 = rand() % 3;
        if (game.board[m1][m2] == '-'){
            break;
        }
    }
    game.board[m1][m2] = 'O';
    game.num_O++;
}

int main(){
    srand(time(0));
    make_board();
    print_board();
    char instruction[100];
    while (1){
        printf("tictactoe: Human Turn: \n");
        fgets(instruction, 100, stdin);
        int len = strlen(instruction);
        //deals with if the input is taking the new \n with it
        if (len > 0 && instruction[len - 1] == '\n'){
            instruction[len -1 ] = '\0';
        }
        int p_return = parse_command(instruction);
        if (p_return == 0 || p_return == 1){
            continue;
        }
        print_board();
        int g_return = game_over();
        if (g_return == 1){
            //X won game
            make_board();
            printf("tictactoe: human won");
            break;
        }
        if (g_return == 2){
            //tie won game
            make_board();
            printf("tictactoe: tie");
            break;
        }
        //let AI move
        move_AI();
        print_board();
        g_return = game_over();
        if (g_return == 1){
            //O won game
            make_board();
            printf("tictactoe: AI Won");
            break;
        }
    }
//    validate_move('D', '3', 'X'); //invalid test
//    validate_move('A', '1', 'Z'); //invalid test
//    validate_move('C', '2', 'O'); //invalid test
//    validate_move('C', '3', 'X'); //valid test
    return 0;
}

