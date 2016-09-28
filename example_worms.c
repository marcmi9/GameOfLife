/*

  MAIN.C
  ======
  (c) Copyright Paul Griffiths 2000
  Email: mail@paulgriffiths.net

  Worms game using ncurses.

*/


#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <signal.h>
#include <sys/time.h>
#include <curses.h>

#include "worms.h"
#include "helper.h"


#define TIMESTEP 200000


void SetTimer(void);
void SetSignals(void);


WINDOW * mainwin;
int oldcur;


int main(void) {

    /*  Seed RNG, set timer and register signal handlers  */

    srand( (unsigned) time(NULL) );
    SetTimer();
    SetSignals();
    

    /*  Initialize ncurses  */

    if ( (mainwin = initscr()) == NULL ) {
        perror("error initialising ncurses");
        exit(EXIT_FAILURE);
    }

    noecho();
    keypad(mainwin, TRUE);
    oldcur = curs_set(0);


    /*  Initialize worm and draw it  */

    InitWorm();
    Draw();


    /*  Loop and get user input  */

    while ( 1 ) {
        int key = getch();

        switch ( key ) {

        case KEY_UP:
        case 'Y':
        case 'y':
            ChangeDir(UP);
            break;

        case KEY_DOWN:
        case 'N':
        case 'n':
            ChangeDir(DOWN);
            break;

        case KEY_LEFT:
        case 'G':
        case 'g':
            ChangeDir(LEFT);
            break;

        case KEY_RIGHT:
        case 'J':
        case 'j':
            ChangeDir(RIGHT);
            break;

        case 'Q':
        case 'q':
            Quit(USER_QUIT);
            break;
            
        }
    }


    /*  We never get here  */

    return EXIT_SUCCESS;
}


/*  Sets up the game timer  */

void SetTimer(void) {

    struct itimerval it;

    
    /*  Clear itimerval struct members  */

    timerclear(&it.it_interval);
    timerclear(&it.it_value);


    /*  Set timer  */

    it.it_interval.tv_usec = TIMESTEP;
    it.it_value.tv_usec    = TIMESTEP;
    setitimer(ITIMER_REAL, &it, NULL);
}


/*  Sets up signal handlers we need  */

void SetSignals(void) {

    struct sigaction sa;


    /*  Fill in sigaction struct  */

    sa.sa_handler = handler;
    sa.sa_flags   = 0;
    sigemptyset(&sa.sa_mask);


    /*  Set signal handlers  */

    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGINT,  &sa, NULL);
    sigaction(SIGALRM, &sa, NULL);


    /*  Ignore SIGTSTP  */

    sa.sa_handler = SIG_IGN;
    sigaction(SIGTSTP, &sa, NULL);
}



worms.h

/*

  WORMS.H
  =======
  (c) Copyright Paul Griffiths 2000
  Email: mail@paulgriffiths.net

  Interface to worms functions

*/


#ifndef PG_WORMS_H
#define PG_WORMS_H


/*  Structures and typedefs  */

struct worm_part {
    struct worm_part * next;
    int x;
    int y;
};

typedef struct worm_part WORM;


/*  Macros  */

#define DOWN  1
#define UP    2
#define LEFT  3
#define RIGHT 4

#define WORMBIT  'O'
#define EMPTY    ' '
#define WORMFOOD 'X'


/*  Function prototypes  */

void InitWorm(void);
void MoveWorm(void);
void Draw(void);
void FreeWorm(void);


#endif  /* PG_WORMS_H  */



worms.c

/*

  WORMS.C
  =======
  (c) Copyright Paul Griffiths 2000
  Email: mail@paulgriffiths.net

  Implementation of worms functions.

*/


#include <stdlib.h>

#include <curses.h>

#include "worms.h"
#include "helper.h"


#define MOVESCORE  1
#define FOODSCORE 10
#define WORMSIZE   8

static WORM * worm;
static int dir = DOWN;
static int rows, cols;
int score = 0;

void PlaceFood(void);
void Draw(void);


/*  Initialises the worm, and draws it for the first time  */

void InitWorm(void) {

    WORM * temp;
    int x = 1, y = 1, i;


    /*  Allocate memory for worm  */

    for ( i = 0; i < WORMSIZE; i++ ) {
        if ( i == 0 ) {

            /*  If this is the first worm part  */
            
            if ( (worm = malloc(sizeof(WORM))) == NULL )
                Error_Quit("couldn't allocate memory for worm");
            temp = worm;
        }
        else {

            /*  If this is not the first worm part  */

            if ( (temp->next = malloc(sizeof(WORM))) == NULL )
                Error_Quit("couldn't allocate memory for worm");
            temp = temp->next;
        }


        /*  Set coordinates for this worm part  */

        temp->x = x;
        temp->y = y++;
    }
    temp->next = NULL;


    /* Get window size */

    GetTermSize(&rows, &cols);  
}


/*  Draws game arena  */

void Draw(void) {

    WORM * temp = worm;


    /*  Draw border around arena  */

    box(stdscr, 0, 0);


    /*  Draw the worm  */

    while ( temp ) {
        move(temp->y, temp->x);
        addch(WORMBIT);
        temp = temp->next;
    }

    
    /*  Draw initial worm food  */

    PlaceFood();
}


/*  Moves the worm  */
    
void MoveWorm(void) {

    WORM * temp = worm;
    int x, y, ch;


    /*  Move to last worm bit  */

    while ( temp->next != NULL )
        temp = temp->next;


    /*  Make a new worm bit  */

    if ( (temp->next = malloc(sizeof(WORM))) == NULL )
        Error_Quit("couldn't allocate memory in MoveWorm()");
    

    /*  Determine position of new worm bit  */

    switch ( dir ) {
    case DOWN:
        x = temp->x;
        y = temp->y + 1;
        break;
        
    case UP:
        x = temp->x;
        y = temp->y - 1;
        break;
        
    case LEFT:
        x = temp->x - 1;
        y = temp->y;
        break;
        
    case RIGHT:
        x = temp->x + 1;
        y = temp->y;
        break;
    }


    /*  Fill in new worm bit structure  */
    
    temp       = temp->next;
    temp->next = NULL;
    temp->x    = x;
    temp->y    = y;


    /*  See if we hit anything  */

    move(y, x);
    switch ( (ch = inch()) ) {
    case EMPTY:

        score += MOVESCORE;

        /*  Delete first worm bit  */

        temp = worm->next;
        move(worm->y, worm->x);
        addch(EMPTY);
        free(worm);
        worm = temp;    /*  Fallthrough  */
        
    case WORMFOOD:

        /*  Add new wormbit  */

        move(y, x);
        addch(WORMBIT);
        if ( ch == WORMFOOD ) {

            /*  Place some new food  */
            
            PlaceFood();

            score += FOODSCORE;
        }

        refresh();
        break;

    case WORMBIT:
        Quit(HITSELF);

    default:
        Quit(HITWALL);
    }
}


/*  Place food in the arena  */

void PlaceFood(void) {
    
    int x, y;

    do {
        x = rand() % (cols - 3) + 1;
        y = rand() % (rows - 3) + 1;
        move(y, x);
    } while ( inch() != EMPTY );

    addch(WORMFOOD);
}


/*  Changes the direction of travel  */

void ChangeDir(int d) {

    WORM * temp = worm;


    /*  Move to last worm bit  */

    while ( temp->next != NULL )
        temp = temp->next;


    /*  Determine next position  */

    switch ( d ) {
    case LEFT:
        if ( dir == RIGHT )
            return;
        move(temp->y, temp->x - 1);
        break;
      
    case RIGHT:
        if ( dir == LEFT )
            return;
        move(temp->y, temp->x + 1);
        break;

    case UP:
        if ( dir == DOWN )
            return;
        move(temp->y - 1, temp->x);
        break;

    case DOWN:
        if ( dir == UP )
            return;
        move(temp->y + 1, temp->x);
        break;
    }

    dir = d;
}


/*  Releases memory used by worm  */

void FreeWorm(void) {
    
    WORM * temp = worm;

    while ( worm ) {
        temp = worm->next;
        free(worm);
        worm = temp;
    }
}



helper.h

/*

  HELPER.H
  ========
  (c) Copyright Paul Griffiths 2000
  Email: mail@paulgriffiths.net

  Interface to helper functions.

*/


#ifndef PG_HELPER_H
#define PG_HELPER_H


/*  Macros  */

#define USER_QUIT    1
#define HITSELF      2
#define HITWALL      3


/*  Function prototypes  */

void Error_Quit(char * msg);
void Quit(int reason);
void GetTermSize(int * rows, int * cols);
void handler(int signum);
void ChangeDir(int d);


#endif  /*  PG_HELPER_H  */




helper.c

/*

  HELPER.C
  ========
  (c) Copyright Paul Griffiths 2000
  Email: mail@paulgriffiths.net
  
  Helper functions.

*/


#include <stdlib.h>

#include <sys/ioctl.h>
#include <signal.h>
#include <curses.h>

#include "helper.h"
#include "worms.h"


/*  Quit on error  */

void Error_Quit(char * msg) {

    extern WINDOW * mainwin;
    extern int oldcur;


    /*  Clean up nicely  */

    delwin(mainwin);
    curs_set(oldcur);
    endwin();
    refresh();
    FreeWorm();


    /*  Output error message and exit  */

    perror(msg);
    exit(EXIT_FAILURE);
}


/*  Quit successfully  */

void Quit(int reason) {

    extern WINDOW * mainwin;
    extern int oldcur;
    extern int score;


    /*  Clean up nicely  */

    delwin(mainwin);
    curs_set(oldcur);
    endwin();
    refresh();
    FreeWorm();


    /*  Output farewell message  */

    switch ( reason ) {
    case HITWALL:
        printf("\nYou hit a wall!\n");
        printf("Your score is %d\n", score);
        break;

    case HITSELF:
        printf("\nYou ran into yourself!\n");
        printf("Your score is %d\n", score);
        break;

    default:
        printf("\nGoodbye!\n");
        break;
    }

    exit(EXIT_SUCCESS);
}


/*  Returns the x-y size of the terminal  */

void GetTermSize(int * rows, int * cols) {

    struct winsize ws;


    /*  Get terminal size  */

    if ( ioctl(0, TIOCGWINSZ, &ws) < 0 ) {
        perror("couldn't get window size");
        exit(EXIT_FAILURE);
    }
    

    /*  Update globals  */

    *rows = ws.ws_row;
    *cols = ws.ws_col;
}


/*  Signal handler  */

void handler(int signum) {

    extern WINDOW * mainwin;
    extern int oldcur;


    /*  Switch on signal number  */

    switch ( signum ) {

    case SIGALRM:

        /*  Received from the timer  */

        MoveWorm();
        return;

    case SIGTERM:
    case SIGINT:

        /*  Clean up nicely  */

        delwin(mainwin);
        curs_set(oldcur);
        endwin();
        refresh();
        FreeWorm();
        exit(EXIT_SUCCESS);

    }
}

