#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Our reference coordinate frame is located at the left top corner of the window.
// X points down, Y points to right

// Game window dimensions
#define rows 40
#define cols 80

#define margin 100  //MUST BE AN EVEN NUMBER!
        // This margin is used to create a bigger grid than the window. This assures that a moving group of cells
        // will not collide to a border and change its behaviour, unless is deliverately wanted (margin 0).

        // We decided not to use dynamic memory because we consider it useless in a limited plot window and dangerous for moving cells
        // like a glider or a spaceship, that create an infinite grid through time.

#define init_center_x (rows+margin)/2 // Center positions of the window
#define init_center_y (cols+margin)/2


void create_cell(int x, int y, int cell_list[(rows+margin)*(cols+margin)][2], int *n_cells, char live_cells[rows+margin][cols+margin])
{  // Function to create a cell in a certain coordinates

    cell_list[*n_cells][0] = x;
    cell_list[*n_cells][1] = y;
    live_cells[x][y] = 1;
    (*n_cells)++;

}

void create_object(int x, int y, int choice, int cell_list[(rows+margin)*(cols+margin)][2], int *n_cells, char live_cells[rows+margin][cols+margin])
{  // Function to create a desired object in a certain coordinates. Is used to create the initial conditions.

    int k;
    switch(choice)
    {
		
        case 1:
			// r_pentomino
            create_cell(x  , y  , cell_list, n_cells, live_cells);
            create_cell(x+1, y  , cell_list, n_cells, live_cells);
            create_cell(x  , y-1, cell_list, n_cells, live_cells);
            create_cell(x-1, y  , cell_list, n_cells, live_cells);
            create_cell(x-1, y+1, cell_list, n_cells, live_cells);

        break;

        case 2:
			// diehard
            create_cell(x  , y-2, cell_list, n_cells, live_cells);
            create_cell(x  , y-3, cell_list, n_cells, live_cells);
            create_cell(x+1, y-2, cell_list, n_cells, live_cells);

            create_cell(x-1, y+3, cell_list, n_cells, live_cells);

            create_cell(x+1, y+2, cell_list, n_cells, live_cells);
            create_cell(x+1, y+3, cell_list, n_cells, live_cells);
            create_cell(x+1, y+4, cell_list, n_cells, live_cells);


        break;

        case 3:
			// acorn
            create_cell(x  , y  , cell_list, n_cells, live_cells);
            create_cell(x  , y+1, cell_list, n_cells, live_cells);

            create_cell(x-2, y+1, cell_list, n_cells, live_cells);
            create_cell(x-1, y+3, cell_list, n_cells, live_cells);
            create_cell(x  , y+4, cell_list, n_cells, live_cells);
            create_cell(x  , y+5, cell_list, n_cells, live_cells);
            create_cell(x  , y+6, cell_list, n_cells, live_cells);

        break;
        case 4:
			// glider
            create_cell(x-1, y  , cell_list, n_cells, live_cells);
            create_cell(x  , y+1, cell_list, n_cells, live_cells);
            create_cell(x+1, y+1, cell_list, n_cells, live_cells);
            create_cell(x+1, y  , cell_list, n_cells, live_cells);
            create_cell(x+1, y-1, cell_list, n_cells, live_cells);

        break;

        case 5:
			// small_exploder
            create_cell(x+1, y  , cell_list, n_cells, live_cells);
            create_cell(x  , y-1, cell_list, n_cells, live_cells);
            create_cell(x-1, y-1, cell_list, n_cells, live_cells);
            create_cell(x-1, y  , cell_list, n_cells, live_cells);
            create_cell(x-2, y  , cell_list, n_cells, live_cells);
            create_cell(x-1, y+1, cell_list, n_cells, live_cells);
            create_cell(x  , y+1, cell_list, n_cells, live_cells);

        break;

        case 6:
			// exploder
            create_cell(x-2, y  , cell_list, n_cells, live_cells);
            create_cell(x-2, y+2, cell_list, n_cells, live_cells);
            create_cell(x-1, y+2, cell_list, n_cells, live_cells);
            create_cell(x  , y+2, cell_list, n_cells, live_cells);
            create_cell(x+1, y+2, cell_list, n_cells, live_cells);
            create_cell(x+2, y+2, cell_list, n_cells, live_cells);
            create_cell(x+2, y  , cell_list, n_cells, live_cells);
            create_cell(x+2, y-2, cell_list, n_cells, live_cells);
            create_cell(x+1, y-2, cell_list, n_cells, live_cells);
            create_cell(x  , y-2, cell_list, n_cells, live_cells);
            create_cell(x-1, y-2, cell_list, n_cells, live_cells);
            create_cell(x-2, y-2, cell_list, n_cells, live_cells);

        break;

        case 7:
			// 10_cell_row
            for (k = -5; k < 5; k++)
            {
               create_cell(x, y + k, cell_list, n_cells, live_cells);
            }

        break;

        case 8:
			// spaceship
            create_cell(x+1, y+1, cell_list, n_cells, live_cells);
            create_cell(x  , y+2, cell_list, n_cells, live_cells);
            create_cell(x-1, y+2, cell_list, n_cells, live_cells);
            create_cell(x-2, y+2, cell_list, n_cells, live_cells);
            create_cell(x-2, y+1, cell_list, n_cells, live_cells);
            create_cell(x-2, y  , cell_list, n_cells, live_cells);
            create_cell(x-2, y-1, cell_list, n_cells, live_cells);
            create_cell(x-1, y-2, cell_list, n_cells, live_cells);
            create_cell(x+1, y-2, cell_list, n_cells, live_cells);

        break;

        case 9:
			// tumbler
            create_cell(x-2, y+1, cell_list, n_cells, live_cells);
            create_cell(x-2, y+2, cell_list, n_cells, live_cells);
            create_cell(x-1, y+1, cell_list, n_cells, live_cells);
            create_cell(x-1, y+2, cell_list, n_cells, live_cells);
            create_cell(x  , y+1, cell_list, n_cells, live_cells);
            create_cell(x+1, y+1, cell_list, n_cells, live_cells);
            create_cell(x+2, y+1, cell_list, n_cells, live_cells);
            create_cell(x+3, y+2, cell_list, n_cells, live_cells);
            create_cell(x+3, y+3, cell_list, n_cells, live_cells);
            create_cell(x+2, y+3, cell_list, n_cells, live_cells);
            create_cell(x+1, y+3, cell_list, n_cells, live_cells);
            
            create_cell(x-2, y-1, cell_list, n_cells, live_cells);
            create_cell(x-2, y-2, cell_list, n_cells, live_cells);
            create_cell(x-1, y-1, cell_list, n_cells, live_cells);
            create_cell(x-1, y-2, cell_list, n_cells, live_cells);
            create_cell(x  , y-1, cell_list, n_cells, live_cells);
            create_cell(x+1, y-1, cell_list, n_cells, live_cells);
            create_cell(x+2, y-1, cell_list, n_cells, live_cells);
            create_cell(x+3, y-2, cell_list, n_cells, live_cells);
            create_cell(x+3, y-3, cell_list, n_cells, live_cells);
            create_cell(x+2, y-3, cell_list, n_cells, live_cells);
            create_cell(x+1, y-3, cell_list, n_cells, live_cells);


        break;

        case 10:
        
			// glider_gun
            create_cell(x-3, y-19, cell_list, n_cells, live_cells);
            create_cell(x-3, y-18, cell_list, n_cells, live_cells);
            create_cell(x-2, y-19, cell_list, n_cells, live_cells);
            create_cell(x-2, y-18, cell_list, n_cells, live_cells);

            create_cell(x-3, y-10, cell_list, n_cells, live_cells);
            create_cell(x-3, y-9,  cell_list, n_cells, live_cells);
            create_cell(x-2, y-11, cell_list, n_cells, live_cells);
            create_cell(x-2, y-9,  cell_list, n_cells, live_cells);
            create_cell(x-1, y-11, cell_list, n_cells, live_cells);
            create_cell(x-1, y-10, cell_list, n_cells, live_cells);

            create_cell(x-1, y-3, cell_list, n_cells, live_cells);
            create_cell(x-1, y-2, cell_list, n_cells, live_cells);
            create_cell(x  , y-3, cell_list, n_cells, live_cells);
            create_cell(x  , y-1, cell_list, n_cells, live_cells);
            create_cell(x+1, y-3, cell_list, n_cells, live_cells);

            create_cell(x-5, y+4, cell_list, n_cells, live_cells);
            create_cell(x-5, y+5, cell_list, n_cells, live_cells);
            create_cell(x-4, y+3, cell_list, n_cells, live_cells);
            create_cell(x-4, y+5, cell_list, n_cells, live_cells);
            create_cell(x-3, y+3, cell_list, n_cells, live_cells);
            create_cell(x-3, y+4, cell_list, n_cells, live_cells);

            create_cell(x+7, y+5, cell_list, n_cells, live_cells);
            create_cell(x+7, y+6, cell_list, n_cells, live_cells);
            create_cell(x+7, y+7, cell_list, n_cells, live_cells);
            create_cell(x+8, y+5, cell_list, n_cells, live_cells);
            create_cell(x+9, y+6, cell_list, n_cells, live_cells);

            create_cell(x-5, y+15, cell_list, n_cells, live_cells);
            create_cell(x-5, y+16, cell_list, n_cells, live_cells);
            create_cell(x-4, y+15, cell_list, n_cells, live_cells);
            create_cell(x-4, y+16, cell_list, n_cells, live_cells);

            create_cell(x+2, y+16, cell_list, n_cells, live_cells);
            create_cell(x+2, y+17, cell_list, n_cells, live_cells);
            create_cell(x+3, y+16, cell_list, n_cells, live_cells);
            create_cell(x+3, y+18, cell_list, n_cells, live_cells);
            create_cell(x+4, y+16, cell_list, n_cells, live_cells);
		
        break;
        

        default:
        break;
    }
}









