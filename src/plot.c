#include <ncurses.h>

#define DELAY 200  // Delay between plots

//------------------------------------GLOBAL VARIABLES----------------------------------------

WINDOW *window;
int ch; // Character to read from keyboard

//---------------------------------------FUNCTIONS--------------------------------------------

int starting_menu() // Function that creates the starting menu
{
    int choice = -1;

    WINDOW *w;
    char list[12][18] = { "SELECT (WITH ->):", "The R-Pentomino", "Diehard", "Acorn", "Glider", "Small Exploder", "Exploder", "10_cell_row", "Spaceship", "Tumbler", "Glider Gun", "QUIT" };
    char item[18];
    int i = 0;

    w = newwin( 14, 22, 1, 40 ); // Create a new window
    box( w, 0, 0 );              // Sets default borders for the window
    
    for(i = 0; i < 12; i++ ) {
        if (i == 1)
            wattron(w, A_STANDOUT | A_BOLD);
        else if( i == 0 ) 
            wattron( w, A_BOLD ); // Highlights the first item.
        else
            wattroff( w, A_STANDOUT | A_BOLD );

        sprintf(item, "%-15s",  list[i]);
        mvwprintw( w, i+1, 2, "%s", item );
    }
 
    wrefresh( w ); // Update the terminal screen
 
    i = 1;
    keypad( w, TRUE ); // Enable keyboard input for the window.
    curs_set( 0 );     // Hide the default screen cursor.
     

    while (choice == -1){ // Get the input
			
            ch = wgetch(w);
                // right pad with spaces to make the items appear with even width.
            sprintf(item, "%-15s",  list[i]); 
            mvwprintw( w, i+1, 2, "%s", item ); 
               // use a variable to increment or decrement the value based on the input.
            switch( ch ) {
                case KEY_UP:
                    i--;
                    if (i < 1) i = 11;
                    break;
                case KEY_DOWN:
                    i++;
                    if (i > 11) i = 1;
                    break;
                case KEY_RIGHT:
                    choice = i;
                    break;
            }

            // Now highlight the next item in the list.

            wattron( w, A_STANDOUT | A_BOLD);
             
            sprintf(item, "%-15s",  list[i]);
            mvwprintw( w, i+1, 2, "%s", item);
            wattroff( w, A_STANDOUT | A_BOLD );
    }
 
    delwin( w );
    endwin();
    
    return choice;
	
}

void hints() // Function that creates the help box
{
    WINDOW *w;

    w = newwin( 4, 30, 1, cols + 10 ); // create a new window
    box( w, 0, 0 );                    // sets default borders for the window
    
    mvwprintw(w, 1, 2, "Press 'RIGHT' to advance");
    mvwprintw(w, 2, 2, "Press 'Q' to quit");
	
    wrefresh(w);
    refresh();
	
}

void print_iter(unsigned long iter) // Function that creates the box and prints the number of iterations
{
    WINDOW *w;

    w = newwin( 3, 30, 5, cols + 10 ); // create a new window
    box( w, 0, 0 ); // sets default borders for the window

    mvwprintw(w, 1, 1, "Iterations: %lu",iter);

    wrefresh(w);
    refresh();

}


WINDOW *init_window(int height, int width)  // Function that creates a new window for plotting
{
    WINDOW *wind;
    wind = newwin(height, width,0,0);

    wrefresh(wind);

    return wind;
}

void plot_cells(char live_cells[rows+margin][cols+margin]) // Function that plots the current grid
{
    wclear(window);
    box(window,0,0);
    wborder(window, '|', '|', '~', '~', '+', '+', '+', '+');
    hints();

    int i,j;
    for (i = 1; i < rows + margin; i++)
    {
        for (j = 1; j < cols + margin; j++)
        {
            if (live_cells[i][j] == 1 && (i > margin/2 && i < rows + margin/2) && (j > margin/2 && j < cols + margin/2))
                    mvwprintw(window,i - margin/2,j - margin/2,"O");

        }
    }

    wrefresh(window);
    usleep(DELAY); 
}

void wait_for_next_key(int key, char * exit_flag) // Function that waits for a certain key
{
    //keypad(window, TRUE);
    while ((ch = wgetch(window)) != key) {
        if (ch == 'q') {
            *exit_flag = 1;
            break;
        }
    }
}




