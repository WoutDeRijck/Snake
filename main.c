#include <curses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "snake.h"

/*
[GIVEN] Sleep for x milliseconds.
*/
void sleep_ms(int milliseconds) {
    usleep(milliseconds * 1000);
}

/*
[GIVEN] Print given string and wait for `y` or `n` input. Returns true if input was `y`.
*/
bool ask(char *question) {
    printw("\n\t\t%s (y/n):", question);
    refresh();
    int c = getch();
    while (c != 'y' && c != 'n') {
        c = getch();
        sleep_ms(100);
    }
    return (c == 'y');
}

/*
[GIVEN] Play a game.
*/
int main(int argc, char *argv[]) {
    initscr();            // Initialise a default curses screen
    noecho();             // Don't echo any keypresses
    curs_set(FALSE);      // Don't display a cursor
    timeout(0);           // Non-blocking input (getchar() doesn't wait until user presses key)
    keypad(stdscr, TRUE); // Enable arrow keys
    cbreak();             // Disable line buffering (don't wait until newline to read a character)

    int action = NONE;
    bool exit = false;
    game *g = new_game(30, 30);

    // Game loop
    while (g && !exit) {
        if (g->state == GAME) {
            sleep_ms(100);
        } else {
            sleep_ms(100);
        }
        // Get input and translate into an `action`
        switch (getch()) {
        case 'a':
        case 'q':
        case KEY_LEFT:
            action = LEFT;
            break;
        case 'd':
        case KEY_RIGHT:
            action = RIGHT;
            break;
        case 'w':
        case 'z':
        case KEY_UP:
            action = UP;
            break;
        case 's':
        case KEY_DOWN:
            action = DOWN;
            break;
        case '\r':
        case '\n':
        case KEY_ENTER:
            action = ENTER;
            break;
        case 'm':
            action = QUIT;
            break;
        default:
            // Don't do anything if the key isn't recognised
            action = NONE;
        }
        switch (g->state) {
        case MENU:
            update_menu(g, action);
            display_menu(g);
            action = NONE;
            break;
        case GAME:
            if (action == QUIT && ask("Back to menu?")) {
                reset_game(g);
                g->state = MENU;
                continue;
            }
            if (action == UP || action == DOWN || action == LEFT || action == RIGHT) {
                g->snake->direction = action;
            }
            if (!update_game(g)) {
                // Move failed; the game is over
                printw("\n\tGAME OVER!         \n");
                if (ask("Do you want to archive your snake?")) {
                    printw("\n\t\tPlease insert your name (max. 10 characters): ");
                    refresh();
                    timeout(-1); // Switch to blocking input so we get the entire name
                    echo();      // Echo keypresses
                    char name[11] = {'\0'};
                    getnstr(name, 10);
                    timeout(0); // Go back to non-blocking input
                    noecho();   // Don't echo keypresses
                    archive_snake(g, name);
                }
                // Clear game
                reset_game(g);
                g->state = MENU;
            } else {
                // Move succeeded, so draw game and continue
                display_game(g);
            }
            break;
        case ARCHIVE:
            if ((action == QUIT || action == LEFT) && ask("Back to menu?")) {
                g->state = MENU;
                continue;
            }
            display_archive(g);
            break;
        case EXIT:
            if (ask("QUIT?")) {
                delete_game(g);
                exit = 1;
                continue;
            }
            g->state = MENU;
            break;
        }
    }
    printw("\n\n\nExiting...");
    refresh();
    endwin();
    return EXIT_SUCCESS;
}
