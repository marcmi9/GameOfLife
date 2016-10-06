//--------------------------------------------ABSTRACT------------------------------------------------

/* Embedded and Real-Time Systems
 * Master's Degree in Automatic Control & Robotics
 * October 2016
 *
 * This is an implementation in C of the John Conway's Game of Life.
 *
 * The program is composed by some files that groups all the functions of the same utility, for improved clearance.
 * The main executes the whole game accessing to the needed functions.
 *
 * The basic idea of the game is to check each cell looking for their neighbours and decide if the cell must live or die
 * in the next iteration. The easiest way to implement it is to access each position in a grid using 2 for statements (rows and cols).
 * However, this is not efficient because usually most of the grid is empty. For this reason, we propose some optimizations to compute
 * it faster and be able to play in much bigger grids.
 *
 * Our optimization consists in a list containing only the current live cells (cell_list). The only cells that will live in the next
 * iteration must be close to the current live ones, so we only check the neighbours of the live cells in cell_list. In addition, we
 * assure checking the same position only once when there are a lot of close live cells.
 *
 * Finally, we provide an user interface to play with the most common initial conditions.
 *
*/

//-------------------------------------------TEAM WORK--------------------------------------------------
/*
 * First of all, the 3 coders of this project contributed to the design of this code. All 3 thought about how to program and optimize the
 * game. Then, we divided the tasks to develop and then we all implemented the program together.
 * Working in this manner has led us to have better ideas with the brainstorming and then a fast implementation through the work division.
 * In addition, the resulting code is well structured, coherent and easy to follow.
 *
 *
*/

#include "create.c"    // Contains the functions to create the initial common structures.
#include "algorithm.c" // Contains all the logics of the game.
#include "plot.c"      // Contains all the functions related to the visualization with ncurses.

//--------------------------------------------MAIN PROGRAM------------------------------------------------

int main()
{    

    // Variable inicialization

    int cell_list[(rows+margin)*(cols+margin)][2];                  // list with current live cells coordinates
    memset(cell_list,0,sizeof(int)*(rows+margin)*(cols+margin)*2);  // Setting to 0 value
    int n_cells = 0;                                                // Number of current live cells, has the same size of cell_list

    int cell_list_2[(rows+margin)*(cols+margin)][2];                // Complementary list to store the live cells coordinates of the next state
    memset(cell_list_2,0,sizeof(int)*(rows+margin)*(cols+margin)*2);
    int n_cells_2 = 0;

    char live_cells[rows+margin][cols+margin];                      // This is the main 2D grid (0 -> dead, 1 -> alive)
    memset(live_cells,0,sizeof(char)*(rows+margin)*(cols+margin));  // Setting to 0 value

    char live_cells_2[rows+margin][cols+margin];                    // Complementary grid to store the next iteration
    memset(live_cells_2,0,sizeof(char)*(rows+margin)*(cols+margin));

    char checked_cells[rows+margin][cols+margin];                     // this matrix stores the cells that have been checked, dead or alive ones
    memset(checked_cells,0,sizeof(char)*(rows+margin)*(cols+margin)); // Setting to 0 value
	
    // Plot inicialization

    initscr();
    noecho();
    cbreak();
    curs_set(FALSE);

    window = init_window(rows,cols);


    int choice = starting_menu(); // The selection start menu

    if (choice != 11) {
    //----------------------------------------------- initialization --------------------------------------------------
        unsigned long iter = 0;

        create_object(init_center_x, init_center_y, choice, cell_list, &n_cells, live_cells); // Create certain initial structure of cells

        hints();                // Creates the help dialog box

        print_iter(iter);       // Creates the box that indicates the number of iterations

        plot_cells(live_cells); // Plots the current grid

    //----------------------------------------------- main algorithm ---------------------------------------------------


        char exit_flag = 0; // Variable for exiting the program

        while(!exit_flag)   // Our algorithm switches between cell_list and cell_list_2.
            // This is more efficient than copying the next state to the same variable at each iteration
        {
            keypad(window, TRUE);

            wait_for_next_key(KEY_RIGHT, &exit_flag);

            if (exit_flag) break;

            next_state(cell_list,   &n_cells,   live_cells, checked_cells,
                       cell_list_2, &n_cells_2, live_cells_2);

            print_iter(++iter);

            plot_cells(live_cells_2); // The same, but with cell_list_2

            wait_for_next_key(KEY_RIGHT, &exit_flag);

            next_state(cell_list_2, &n_cells_2, live_cells_2, checked_cells,
                       cell_list,   &n_cells,   live_cells);

            print_iter(++iter);

            plot_cells(live_cells);
        }

        delwin(window);
        endwin(); // For closing ncurses when quit

    }

    return 0;
}
