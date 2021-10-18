#ifndef _SNAKE_HEADER_
#define _SNAKE_HEADER_

#include <assert.h>
#include <curses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ACTIONS
#define NONE -1
#define UP 0
#define LEFT 1
#define RIGHT 2
#define DOWN 3
#define ENTER 4
#define QUIT 5

// GAME STATES
#define MENU 0
#define GAME 1
#define ARCHIVE 2 // Archived snakes menu
#define EXIT 3

// OBJECTS
#define HEAD_NONE 's' // The head of a snake that is not moving
#define HEAD_UP '^'
#define HEAD_LEFT '<'
#define HEAD_RIGHT '>'
#define HEAD_DOWN 'v'
#define BODY 'x'

#define WALL '#'  // The snake dies if it touches a wall
#define EMPTY ' ' // An empty space over which the snake can move

#define FOOD1 '*'
#define FOOD2 'i'
#define FOOD3 '0'
#define FOOD4 '@'

typedef struct snake {
    int **location;
    int length;
    int direction;
    char *stomach;
} snake;

typedef struct archived_snake {
    char *player_name;
    snake *snake;
    int score;
} archived_snake;

typedef struct game {
    char **board;
    int width;
    int height;
    snake *snake;
    int score;
    bool found;
    int state;
    int menu_index;
    archived_snake *archived_snakes;
    int num_archived_snakes;
} game;

/* All the functions of the game. For more info, see the function definition in `snake.c`. */
void display_menu(game *g);
void display_game(game *g);
void display_archive(game *g);

void update_menu(game *g, int direction);
bool update_game(game *g);

game *new_game(int height, int width);
char **new_board(int height, int width);
snake *new_snake();

void reset_game(game *g);

void delete_game(game *g);
void delete_snake(snake *s);

void move_snake(snake *s, int new_y, int new_x);
void resize_snake(snake *s, int new_length);
void grow_snake(snake *s, int new_y, int new_x, char food);

void draw_snake(snake *s, char **board);
void undraw_snake(snake *s, char **board);
void generate_food(game *g);

snake *copy_snake(snake *s);
void archive_snake(game *g, char *name);

#endif
