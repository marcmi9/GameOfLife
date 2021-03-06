/*
 * This file contains all the functions related to the game logic itself, including the already commented optimizations.
 * Coded by "Coder3"
*/

#include <string.h> // used for the memset()

int count_cells(int x, int y, char live_cells[rows+margin][cols+margin])
{  // Function to count the number of cells in the neighbourhood.

    return  live_cells[x-1][y-1]    +
            live_cells[x][y-1]      +
            live_cells[x+1][y-1]    +

            live_cells[x-1][y]      +
            live_cells[x+1][y]      +

            live_cells[x-1][y+1]    +
            live_cells[x][y+1]      +
            live_cells[x+1][y+1];
}

void check_cell(int x, int y, char live_cells[rows+margin][cols+margin], char checked_cells[rows+margin][cols+margin], int cell_list_2[(rows+margin)*(cols+margin)][2], int * n_cells_2 ,char live_cells_2[rows+margin][cols+margin])
{  // Function that implements the main game logic that decides if a cell must live or die.

    int neighbours;

    if (!checked_cells[x][y]) // We assure that each cell is checked only once, for better performance
    {
        checked_cells[x][y] = 1;

        if ( x != 0 && y != 0 && x != rows + margin - 1 && y != cols + margin - 1 ) // Grid limits (including the margin)
        {
            neighbours = count_cells(x,y, live_cells);

            if (neighbours == 3 || (neighbours == 2 && live_cells[x][y]))
            {
                live_cells_2[x][y] = 1;

                cell_list_2[*n_cells_2][0] = x;
                cell_list_2[*n_cells_2][1] = y;

                *n_cells_2 = *n_cells_2 + 1;

            }

        }

    }

}


void next_state(int cell_list[(rows+margin)*(cols+margin)][2], int *n_cells, char live_cells[rows+margin][cols+margin],
               char checked_cells[rows+margin][cols+margin],
                int cell_list_2[(rows+margin)*(cols+margin)][2], int *n_cells_2, char live_cells_2[rows+margin][cols+margin])
{  // This function goes through all current live cells checking if the neighbours live or die and computes the next grid.
   // There is no need of evaluating the current live cells itself because if it has to live, then it must be checked as a neighbour of
   // other cell. If not, it will remain dead as default.

    int i = 0;

    for (i = 0; i < *n_cells; i++) // For each current live cell, evaluate the neighbours.

    {
        check_cell(cell_list[i][0]-1,   cell_list[i][1]-1,  live_cells, checked_cells, cell_list_2, n_cells_2, live_cells_2);
        check_cell(cell_list[i][0],     cell_list[i][1]-1,  live_cells, checked_cells, cell_list_2, n_cells_2, live_cells_2);
        check_cell(cell_list[i][0]+1,   cell_list[i][1]-1,  live_cells, checked_cells, cell_list_2, n_cells_2, live_cells_2);

        check_cell(cell_list[i][0]-1,   cell_list[i][1],    live_cells, checked_cells, cell_list_2, n_cells_2, live_cells_2);
        check_cell(cell_list[i][0]+1,   cell_list[i][1],    live_cells, checked_cells, cell_list_2, n_cells_2, live_cells_2);

        check_cell(cell_list[i][0]-1,   cell_list[i][1]+1,  live_cells, checked_cells, cell_list_2, n_cells_2, live_cells_2);
        check_cell(cell_list[i][0],     cell_list[i][1]+1,  live_cells, checked_cells, cell_list_2, n_cells_2, live_cells_2);
        check_cell(cell_list[i][0]+1,   cell_list[i][1]+1,  live_cells, checked_cells, cell_list_2, n_cells_2, live_cells_2);
    }

    memset(cell_list,0,sizeof(int)*(rows+margin)*(cols+margin)*2); // Initialize to value 0
    *n_cells = 0;

    memset(live_cells,0,sizeof(char)*(rows+margin)*(cols+margin));
    memset(checked_cells,0,sizeof(char)*(rows+margin)*(cols+margin));


}
