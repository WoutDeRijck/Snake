#include "snake.h"

/*
[GIVEN] Displays the menu screen and highlights the active menu entry.
*/
void display_menu(game *g) {
    char *menu_text[4] = {"MENU", "Start", "Archived snakes", "Exit"};
    assert(g);
    clear();
    printw("SNAKE\n\n");
    for (int i = 0; i < 4; i++) {
        if (i == g->menu_index) {
            printw("\t***%s***\n", menu_text[i]);
        } else {
            printw("\t   %s\n", menu_text[i]);
        }
    }
    refresh();
}

/*
[GIVEN] Displays the game screen including the board and snake.
*/
void display_game(game *g) {
    // Assert game
    assert(g);
    // Clear what was previously on the screen
    clear();
    // Show the score
    printw("Score: %d\n", g->score);
    // Top wall
    for (int x = 0; x < g->width + 2; x++) {
        addch(WALL);
    }
    printw("\n");
    // Playing field
    for (int y = 0; y < g->height; y++) {
        addch(WALL);
        for (int x = 0; x < g->width; x++) {
            addch(g->board[y][x]);
        }
        addch(WALL);
        printw("\n");
    }
    // Bottom wall
    for (int x = 0; x < g->width + 2; x++) {
        addch(WALL);
    }
    // Show start message if snake is not moving
    if (g->snake->direction == -1) {
        printw("\nPress any key to start!\n");
    }
    // Display the new screen
    refresh();
    return;
}

/*
Displays the snake archive.
*/
void display_archive(game *g) {
    assert(g);
    clear();

    printw("ARCHIVED SNAKES:\n");
    for (int i=0; i<g->num_archived_snakes; i++){
        printw("%s ate %d things: %s",
            g->archived_snakes[i].player_name, 
            g->archived_snakes[i].score, 
            g->archived_snakes[i].snake->stomach);
    }
    refresh();
}

/*
[GIVEN] Updates the menu position and game state based on the action.
*/
void update_menu(game *g, int action) {
    assert(g);
    if (action == UP && g->menu_index > 0) {
        g->menu_index--;
    } else if (action == DOWN && g->menu_index < 3) {
        g->menu_index++;
    } else if (action == ENTER) {
        g->state = g->menu_index;
    }
}

/*
Moves the snake in its direction and checks if there are collisions. Returns false if there are collisions.
Note: this function does not display anything.
*/
bool update_game(game *g) {
    assert(g);
    int y_kop = g->snake->location[0][0];
    int x_kop = g->snake->location[0][1];
    generate_food(g);

    switch (g->snake->direction){
    case UP:
        y_kop--;
        break;
    
    case DOWN:
        y_kop++;
        break;
    
    case LEFT:
        x_kop--;
        break;
    
    case RIGHT:
        x_kop++;
        break;

    default:
        break;
    }

    if((x_kop < g->width && y_kop < g->height && x_kop >=0 && y_kop >= 0) && g->board[y_kop][x_kop] != BODY){
        undraw_snake(g->snake, g->board);
        if((g->board[y_kop][x_kop] == FOOD1) | (g->board[y_kop][x_kop] == FOOD2) | (g->board[y_kop][x_kop] == FOOD3) | (g->board[y_kop][x_kop] == FOOD4)){
            g->found = true;
            grow_snake(g->snake, y_kop, x_kop, g->board[y_kop][x_kop]);
        }else{
            move_snake(g->snake, y_kop, x_kop);
        }
        draw_snake(g->snake,g->board);
        return true;
    }else{
        return false;
    }
}

/*
Initializes the game with an empty board and a snake of length 1 in the top left corner.
*/
game *new_game(int width, int height) {
    game *g = malloc(sizeof(game));
    if(g == NULL){
        exit(EXIT_FAILURE);
    }
    g->found = true;
    g->archived_snakes = NULL;
    g->board = new_board(width, height);
    g->height = height;
    g->width = width;
    g->snake = new_snake();
    g->state = GAME;
    g->menu_index = MENU;
    g->num_archived_snakes = 0;
    g->score = -1;
    return g;
}

/*
Creates an empty board (used by NewGame).
*/
char **new_board(int width, int height) {
    char **board = malloc(height * sizeof(char *));
    if(board == NULL){
        exit(EXIT_FAILURE);
    }
    for(int y = 0; y < height; y++){
        int x = 0;
        board[y] = malloc((width+1)*sizeof(char));
        if(board == NULL){
            exit(EXIT_FAILURE);
        }
        for(x = 0; x < width; x++){
            board[y][x] = ' ';
        }
        board[y][x] = '\0';
    }
    return board;
}

/*
Create snake in starter position with empty stomach (used by init_game and reset_game).
*/
snake *new_snake() {
    snake *s = malloc(sizeof(snake));
    if(s == NULL){
        exit(EXIT_FAILURE);
    }
    s->length = 1;
    s->direction = NONE;
    s->location =  malloc(sizeof(int*));
    if(s->location == NULL){
        exit(EXIT_FAILURE);
    }
    s->location[0] = calloc(2, sizeof(int));
    if(s->location[0] == NULL){
        exit(EXIT_FAILURE);
    }
    s->stomach = malloc(sizeof(char));
    if(s->stomach == NULL){
        exit(EXIT_FAILURE);
    }
    s->stomach[0] = '\0';
    return s;
}

/*
Resets the game so a new game can be played.
*/
void reset_game(game *g) {
    assert(g);
    undraw_snake(g->snake,g->board);
    g->score = 1;
    if(g->snake){
        delete_snake(g->snake);
    }
    g->snake = new_snake();
}

/*
Deletes all the memory used by the game.
*/
void delete_game(game *g) {
    if(!g){
        return;
    }
    if(g->board){
        for(int i = 0; i<g->width; i++){
            free(g->board[i]);
        }
        free(g->board);
    }
    delete_snake(g->snake);
    if(g->num_archived_snakes && g->archived_snakes){
        for(int i = 0; i<g->num_archived_snakes; i++){
            delete_snake(g->archived_snakes[i].snake);
            free(g->archived_snakes[i].player_name);
        }
        free(g->archived_snakes);
    }
    free(g);
}

/*
Deletes/frees the memory of the snake. (used by delete_game)
*/
void delete_snake(snake *s) {
    if(!s){
        return;
    }
    if(s->location){
        for(int i = 0; i<s->length;i++){
            if(s->location[i]){
                free(s->location[i]);
            }
        }
        free(s->location);
    }

    if(s->stomach){
        free(s->stomach);
    }
    free(s);
}

/*
Moves the snake towards the new coordinates.
*/
void move_snake(snake *s, int new_y, int new_x) {
    for(int i  = s->length - 2; i>=0; i--){
        s->location[i+1][0] = s->location[i][0];
        s->location[i+1][1] = s->location[i][1];
    }
    s->location[0][1] = new_x;
    s->location[0][0] = new_y;
}

/*
Resizes the snake. (used by grow_snake)
 - If the snake grows, this function adds empty segments at the back of the snake and its stomach.
 - If the snake shrinks, this function removes segments from the back of the snake and its stomach.
*/
void resize_snake(snake *s, int new_length) {
    s->location = realloc(s->location, new_length * sizeof(int*));
    if(s->location == NULL){
        exit(EXIT_FAILURE);
    }
    if(s->length < new_length){
        for(int i = 0; i<new_length-s->length;i++){
            s->location[s->length+i] = calloc(2,sizeof(int));
            if(s->location[s->length+i] == NULL){
                exit(EXIT_FAILURE);
            }
        }
    }
    s->stomach = realloc(s->stomach, new_length * sizeof(char));
    if(s->stomach == NULL){
        exit(EXIT_FAILURE);
    }
    s->length = new_length;
}

/*
Grows the snake towards specified coordinates and adds the food to the stomach.
*/
void grow_snake(snake *s, int new_y, int new_x, char food) {
    resize_snake(s, s->length + 1);
    s->stomach[s->length-2] = food;
    s->stomach[s->length-1] = '\0';
    move_snake(s, new_y, new_x);
}

/*
Draws the snake onto the board.
Note that this does not display the new board or snake yet.
*/
void draw_snake(snake *s, char **board) {
    assert(s);
    assert(board);
    
    int head = NONE;
    if(s->direction == UP){
        head = HEAD_UP;
    }else if(s->direction == DOWN){
        head = HEAD_DOWN;
    }else if(s->direction == LEFT){
        head = HEAD_LEFT;
    }else if(s->direction == RIGHT){
        head = HEAD_RIGHT;
    }

    board[s->location[0][0]][s->location[0][1]] = head;
    for(int i = 1; i < s->length; i++){
        board[s->location[i][0]][s->location[i][1]] = BODY;
    }
}

/*
Clears the snake from the board: set all places where the snake is drawn to ' '.
Note: this leaves the food on the board.
*/
void undraw_snake(snake *s, char **board) {
    for(int i = 0; i < s->length; i++){
        board[s->location[i][0]][s->location[i][1]] = ' ';
    }
}

/*
Generates new food on the board if previous one was found.
*/
void generate_food(game *g) {
    if(!g->found){
        return;
    }
    g->score++;
    g->found = false;

    int x = 0;
    int y = 0;
    int new = 0;
    srand((unsigned int)time(NULL));

    while(!new){
       x = rand() % g->width;
       y = rand() % g->height;
       if(!(g->board[y][x] == BODY || (g->snake->location[0][0] == y && g->snake->location[0][1] == x))){
           new = 1;
       }
    }

    char food = FOOD1;
    switch (rand() % 4){
    case 0:
        food = FOOD1;
        break;
    case 1:
        food = FOOD2;
        break;
    case 2:
        food = FOOD3;
        break;
    case 3:
        food = FOOD4;
        break;
    }
    g->board[y][x] = food;
}


/*
Returns a deep copy of the snake (used by archive_snake).
*/
snake *copy_snake(snake *s) {
    snake *new = malloc(sizeof(snake));
    if(new == NULL){
        exit(EXIT_FAILURE);
    }

    new->location = malloc(s->length * sizeof(int*));
    if(new->location == NULL){
        exit(EXIT_FAILURE);
    }
    for (int i =0; i<s->length; i++){
        new->location[i] = malloc(2 * sizeof(int));
        if(new->location[i] == NULL){
            exit(EXIT_FAILURE);
        }

        new->location[i][0] = s->location[i][0];
        new->location[i][1] = s->location[i][1];
    }
    new->direction = s->direction;
    new->length = s->length;

    new->stomach = malloc(s->length * sizeof(char));
    if(new->stomach == NULL){
        exit(EXIT_FAILURE);
    }

    for(int i =0; i<s->length;i++){
        new->stomach[i] = s->stomach[i];
    }

    return new;
}

/*
Adds the snake to the archived snakes list.
*/
void archive_snake(game *g, char *name) {
    if (g->num_archived_snakes == 0) {
        g->archived_snakes = malloc(1 * sizeof(archived_snake));
        if (g->archived_snakes == NULL) {
            exit(EXIT_FAILURE);
        }
        g->num_archived_snakes++;
    } else {
        g->num_archived_snakes++;
        g->archived_snakes = realloc(g->archived_snakes, g->num_archived_snakes * sizeof(archived_snake));
        if (g->archived_snakes == NULL) {
            exit(EXIT_FAILURE);
        }
    }
    g->archived_snakes[g->num_archived_snakes - 1].player_name = (char *)malloc((strlen(name) + 1) * sizeof(char));
    if (g->archived_snakes[g->num_archived_snakes - 1].player_name == NULL) {
        exit(EXIT_FAILURE);
    }
    strcpy(g->archived_snakes[g->num_archived_snakes - 1].player_name, name);
    g->archived_snakes[g->num_archived_snakes - 1].score = g->score;
    g->archived_snakes[g->num_archived_snakes - 1].snake = copy_snake(g->snake);
}
