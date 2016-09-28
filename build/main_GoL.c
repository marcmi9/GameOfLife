#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <unistd.h>
//#include <stdbool.h>

//X cap a la dreta, Y cap abaix

#define rows 20
#define cols 20
//#define init_center 10  //referencia inicial
#define DELAY 1000 //que es¿?¿? --> UN DELAY PERQUÈ NO S'EXECUTI TOT INSTANTÀNIAMENT

void store_cells(int cell_list[rows*cols][2], int * n_cells, char live_cells[rows+2][cols+2]);
void check_cell(int x, int y, char live_cells[rows+2][cols+2], char checked_cells[rows+2][cols+2], int cell_list_2[rows*cols][2], int * n_cells_2 ,char live_cells_2[rows+2][cols+2]);
int count_cells(int x, int y, char live_cells[rows+2][cols+2]);

void plot_cells(char live_cells[rows+2][cols+2]);

WINDOW *init_window(int height, int width);

WINDOW *my_win;

//int cols = 2*rows;
int init_center_x = cols/2;
int init_center_y = rows/2;

int ch;

int main()
{    
	
	initscr();
	noecho();
	cbreak();
	curs_set(FALSE);
	
	my_win = init_window(rows,cols);
	
    int cell_list[rows*cols][2]; // llista amb les coordenades de les cells vives
    memset(cell_list,0,sizeof(int)*(rows)*(cols)*2); //inicialitzem a 0. 
    //ROWS I COLS +2 perque afegim uns contorns al nostre mapa

    int cell_list_2[rows*cols][2]; // llista amb les coordenades de les cells vives
    memset(cell_list_2,0,sizeof(int)*(rows)*(cols)*2); //inicialitzem a 0

    int n_cells = 0; //Numero de celules vives, es el tamany de cell_list
    int n_cells_2 = 0;

    char live_cells[rows+2][cols+2]; // matriu on es mostren les cells vives, la grid
    memset(live_cells,0,sizeof(char)*(rows+2)*(cols+2)); //inicialitzem a 0

    char live_cells_2[rows+2][cols+2]; // matriu on es mostren les cells vives
    memset(live_cells_2,0,sizeof(char)*(rows+2)*(cols+2)); //inicialitzem a 0

    char checked_cells[rows+2][cols+2]; // matriu on es mostren les cells comprobades, siguin vives o mortes
    memset(checked_cells,0,sizeof(char)*(rows+2)*(cols+2)); //inicialitzem a 0

    //------------------------------------ init -----------------------------------------
    cell_list[0][0] = init_center_x;
    cell_list[0][1] = init_center_y;

    n_cells++;

    cell_list[1][0] = init_center_x;
    cell_list[1][1] = init_center_y+1;

    n_cells++;

    cell_list[2][0] = init_center_x;
    cell_list[2][1] = init_center_y-1;
	
    n_cells++;

    cell_list[3][0] = init_center_x-1;
    cell_list[3][1] = init_center_y;

    n_cells++;

    cell_list[4][0] = init_center_x+1;
    cell_list[4][1] = init_center_y;

    n_cells++;
    //---------------------------------------------------------------------------------------------
	
    store_cells(cell_list,&n_cells,live_cells); // funcio per a passar la llista de cells vives a la matriu binaria

    plot_cells(live_cells);
    
    //-------------------------------------- programa principal ---------------------------------------------------
    for(int l = 0; true; l++)
    {
		while (ch = wgetch(my_win) != 'q') {  }

        for (int i = 0; i < n_cells; i++) //Per a cada cell viva s'evaluen els neighbours

        {   //funcio per evaluar si la cell ha de morir o viure, actualitza les matrius

            check_cell(cell_list[i][0]-1,   cell_list[i][1]-1,  live_cells, checked_cells, cell_list_2, &n_cells_2, live_cells_2);
            check_cell(cell_list[i][0],     cell_list[i][1]-1,  live_cells, checked_cells, cell_list_2, &n_cells_2, live_cells_2);
            check_cell(cell_list[i][0]+1,   cell_list[i][1]-1,  live_cells, checked_cells, cell_list_2, &n_cells_2, live_cells_2);

            check_cell(cell_list[i][0]-1,   cell_list[i][1],    live_cells, checked_cells, cell_list_2, &n_cells_2, live_cells_2);
            check_cell(cell_list[i][0]+1,   cell_list[i][1],    live_cells, checked_cells, cell_list_2, &n_cells_2, live_cells_2);

            check_cell(cell_list[i][0]-1,   cell_list[i][1]+1,  live_cells, checked_cells, cell_list_2, &n_cells_2, live_cells_2);
            check_cell(cell_list[i][0],     cell_list[i][1]+1,  live_cells, checked_cells, cell_list_2, &n_cells_2, live_cells_2);
            check_cell(cell_list[i][0]+1,   cell_list[i][1]+1,  live_cells, checked_cells, cell_list_2, &n_cells_2, live_cells_2);
        }
	
        memset(cell_list,0,sizeof(int)*(rows)*(cols)*2); //inicialitzem a 0 --> PER QUÈ NO TÉ EL *2 COM A DALT DE TOT?
        n_cells = 0;

        memset(live_cells,0,sizeof(char)*(rows+2)*(cols+2)); //inicialitzem a 0
        memset(checked_cells,0,sizeof(char)*(rows+2)*(cols+2)); //inicialitzem a 0



        plot_cells(live_cells_2);
        
        while (ch = wgetch(my_win) != 'q') {  }


        for (int i = 0; i < n_cells_2; i++) //Per a cada cell viva s'evaluen els neighbours

        {   //funcio per evaluar si la cell ha de morir o viure, actualitza les matrius

            check_cell(cell_list_2[i][0]-1,   cell_list_2[i][1]-1,  live_cells_2, checked_cells, cell_list, &n_cells, live_cells);
            check_cell(cell_list_2[i][0],     cell_list_2[i][1]-1,  live_cells_2, checked_cells, cell_list, &n_cells, live_cells);
            check_cell(cell_list_2[i][0]+1,   cell_list_2[i][1]-1,  live_cells_2, checked_cells, cell_list, &n_cells, live_cells);

            check_cell(cell_list_2[i][0]-1,   cell_list_2[i][1],    live_cells_2, checked_cells, cell_list, &n_cells, live_cells);
            check_cell(cell_list_2[i][0]+1,   cell_list_2[i][1],    live_cells_2, checked_cells, cell_list, &n_cells, live_cells);

            check_cell(cell_list_2[i][0]-1,   cell_list_2[i][1]+1,  live_cells_2, checked_cells, cell_list, &n_cells, live_cells);
            check_cell(cell_list_2[i][0],     cell_list_2[i][1]+1,  live_cells_2, checked_cells, cell_list, &n_cells, live_cells);
            check_cell(cell_list_2[i][0]+1,   cell_list_2[i][1]+1,  live_cells_2, checked_cells, cell_list, &n_cells, live_cells);
        }

        memset(cell_list_2,0,sizeof(int)*(rows)*(cols)*2); //inicialitzem a 0
        n_cells_2 = 0;

        memset(live_cells_2,0,sizeof(char)*(rows+2)*(cols+2)); //inicialitzem a 0
        memset(checked_cells,0,sizeof(char)*(rows+2)*(cols+2)); //inicialitzem a 0

        plot_cells(live_cells);
    }
    
    delwin(my_win);
    endwin(); //que es¿?¿? PER TANCAR EL NCURSES QUAN ACABA TOTA L'EXECUCIÓ

    return 0;
}

WINDOW *init_window(int height, int width) {
	
	WINDOW *wind;
	wind = newwin(height, width,0,0);
	
	wrefresh(wind);
	
	return wind;
}

void store_cells(int cell_list[rows*cols][2], int * n_cells, char live_cells[rows+2][cols+2])
{ 
	//Passa de cell_list a live_cells
    for (int i = 0; i < *n_cells; i++)
    {
        live_cells[cell_list[i][1]][cell_list[i][0]] = 1;
    }
}

void check_cell(int x, int y, char live_cells[rows+2][cols+2], char checked_cells[rows+2][cols+2], int cell_list_2[rows*cols][2], int * n_cells_2 ,char live_cells_2[rows+2][cols+2])
{


    int neighbours;

    if (!checked_cells[y][x])
    {
        checked_cells[y][x] = 1;

        if ( x != 0 && y != 0 && x != cols + 1 && y != rows + 1) //Limits de la quadricula
        {
            neighbours = count_cells(x,y, live_cells);

            if (neighbours == 3 || (neighbours == 2 && live_cells[y][x]))
            {
                live_cells_2[y][x] = 1;

                cell_list_2[*n_cells_2][0] = x;
                cell_list_2[*n_cells_2][1] = y;

                *n_cells_2 = *n_cells_2 + 1;

            }

        }

    }

}

int count_cells(int x, int y, char live_cells[rows+2][cols+2])
{
    return  live_cells[y-1][x-1]    +
            live_cells[y-1][x]      +
            live_cells[y-1][x+1]    +

            live_cells[y][x-1]      +
            live_cells[y][x+1]      +

            live_cells[y+1][x-1]    +
            live_cells[y+1][x]      +
            live_cells[y+1][x+1];
}

void plot_cells(char live_cells[rows+2][cols+2])
{
	wclear(my_win);
	box(my_win,0,0);
	wborder(my_win, '|', '|', '~', '~', '+', '+', '+', '+');
    for (int i = 1; i <= rows; i++)
    {
        for (int j = 1; j <= cols; j++)
        {
            if (live_cells[j][i] == 1) {
				mvwprintw(my_win,j,i,"O"); //Not working....
			}
        }
    }
    wrefresh(my_win);
    usleep(DELAY); 
}










