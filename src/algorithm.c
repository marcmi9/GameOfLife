#include <string.h>

int count_cells(int x, int y, char live_cells[rows+margin][cols+margin])
{
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
{


    int neighbours;

    if (!checked_cells[x][y])
    {
        checked_cells[x][y] = 1;

        if ( x != 0 && y != 0 && x != rows + margin - 1 && y != cols + margin - 1 ) //Limits de la quadricula (incloent el margin)
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
{
    int i = 0;

    for (i = 0; i < *n_cells; i++) //Per a cada cell viva s'evaluen els neighbours

    {   //funcio per evaluar si la cell ha de morir o viure, actualitza les matrius

        check_cell(cell_list[i][0]-1,   cell_list[i][1]-1,  live_cells, checked_cells, cell_list_2, n_cells_2, live_cells_2);
        check_cell(cell_list[i][0],     cell_list[i][1]-1,  live_cells, checked_cells, cell_list_2, n_cells_2, live_cells_2);
        check_cell(cell_list[i][0]+1,   cell_list[i][1]-1,  live_cells, checked_cells, cell_list_2, n_cells_2, live_cells_2);

        check_cell(cell_list[i][0]-1,   cell_list[i][1],    live_cells, checked_cells, cell_list_2, n_cells_2, live_cells_2);
        check_cell(cell_list[i][0]+1,   cell_list[i][1],    live_cells, checked_cells, cell_list_2, n_cells_2, live_cells_2);

        check_cell(cell_list[i][0]-1,   cell_list[i][1]+1,  live_cells, checked_cells, cell_list_2, n_cells_2, live_cells_2);
        check_cell(cell_list[i][0],     cell_list[i][1]+1,  live_cells, checked_cells, cell_list_2, n_cells_2, live_cells_2);
        check_cell(cell_list[i][0]+1,   cell_list[i][1]+1,  live_cells, checked_cells, cell_list_2, n_cells_2, live_cells_2);
    }

    memset(cell_list,0,sizeof(int)*(rows+margin)*(cols+margin)*2); //inicialitzem a 0
    *n_cells = 0;

    memset(live_cells,0,sizeof(char)*(rows+margin)*(cols+margin)); //inicialitzem a 0
    memset(checked_cells,0,sizeof(char)*(rows+margin)*(cols+margin)); //inicialitzem a 0


}
