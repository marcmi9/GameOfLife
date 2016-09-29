#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <unistd.h>

//X cap a la dreta, Y cap abaix --> ncurses funciona al revés...

/* COSES QUE FALTEN:
 * - Canviar eixos X per Y.
 * - Si canviem numero de cols de 20 a 40 (per exemple) la meitat del dibuix no es veu! per què?
 * - Arreglar les funcions_init del Jordan (les he posat en aquest arxiu, per no liar-la)
 */


#define rows 40
#define cols 40
//#define init_center 10  //referencia inicial
#define DELAY 5000 //que es¿?¿? --> UN DELAY PERQUÈ NO S'EXECUTI TOT INSTANTÀNIAMENT

void store_cells(int cell_list[rows*cols][2], int * n_cells, char live_cells[rows+2][cols+2]);
void check_cell(int x, int y, char live_cells[rows+2][cols+2], char checked_cells[rows+2][cols+2], int cell_list_2[rows*cols][2], int * n_cells_2 ,char live_cells_2[rows+2][cols+2]);
int count_cells(int x, int y, char live_cells[rows+2][cols+2]);
void draw_cell(int x, int y, int cell_list[rows*cols][2], int *n_cells);
void create_object(int x, int y, int choice, int cell_list[rows*cols][2], int *n_cells);

void plot_cells(char live_cells[rows+2][cols+2]);

int starting_menu();
void hints();



WINDOW *init_window(int height, int width);

//------GLOBAL VARIABLES------

WINDOW *window;
//int choice;
int init_center_x = cols/2;
int init_center_y = rows/2;
int ch; // Character to read from keyboard

//------MAIN PROGRAM---------

int main()
{    
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
	
	initscr();
	noecho();
	cbreak();
	curs_set(FALSE);
	
	window = init_window(rows,cols);

    int choice = starting_menu();

	if (choice != 4) {
    //------------------------------------ init -----------------------------------------
    
//    cell_list[0][0] = init_center_x;
//    cell_list[0][1] = init_center_y;

//    n_cells++;

//    cell_list[1][0] = init_center_x;
//    cell_list[1][1] = init_center_y+1;

//    n_cells++;

//    cell_list[2][0] = init_center_x;
//    cell_list[2][1] = init_center_y-1;
	
//    n_cells++;

//    cell_list[3][0] = init_center_x-1;
//    cell_list[3][1] = init_center_y;

//    n_cells++;

//    cell_list[4][0] = init_center_x+1;
//    cell_list[4][1] = init_center_y;

//    n_cells++;
	
	
    create_object(init_center_x, init_center_y, choice, cell_list, &n_cells);

	
	hints();
    
    //---------------------------------------------------------------------------------------------
	
    store_cells(cell_list,&n_cells,live_cells); // funcio per a passar la llista de cells vives a la matriu binaria

    plot_cells(live_cells);
    
    //-------------------------------------- programa principal ---------------------------------------------------

    int l, i;
    for(l = 0; true; l++)
    {
        
        while ((ch = wgetch(window)) != 'd') { 
			if (ch == 'q') {
				endwin();
				return 0;
				//break;
			}
		}

        for (i = 0; i < n_cells; i++) //Per a cada cell viva s'evaluen els neighbours

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
	
        memset(cell_list,0,sizeof(int)*(rows)*(cols)*2); //inicialitzem a 0 
        n_cells = 0;

        memset(live_cells,0,sizeof(char)*(rows+2)*(cols+2)); //inicialitzem a 0
        memset(checked_cells,0,sizeof(char)*(rows+2)*(cols+2)); //inicialitzem a 0



        plot_cells(live_cells_2);
        
        while ((ch = wgetch(window)) != 'd') { 
			if (ch == 'q') {
				endwin();
				return 0;
			}
		}


        for (i = 0; i < n_cells_2; i++) //Per a cada cell viva s'evaluen els neighbours

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
    
    delwin(window);
    endwin(); //que es¿?¿? PER TANCAR EL NCURSES QUAN ACABA TOTA L'EXECUCIÓ
	
	}

    return 0;
}


//-------------------------FUNCTIONS--------------------------//

int starting_menu() {
	
    int choice = -1;

    WINDOW *w;
    char list[5][18] = { "Choose a game:", "The R-Pentomino", "Diehard", "Acorn", "Quit" };
    char item[18];
    int ch, i = 0;

    w = newwin( 8, 20, 1, cols + 10 ); // create a new window
    box( w, 0, 0 ); // sets default borders for the window
    
    for(i = 0; i < 5; i++ ) {
		if (i == 1) wattron(w, A_STANDOUT | A_BOLD);
        else if( i == 0 ) 
            wattron( w, A_BOLD ); // highlights the first item.
        else
            wattroff( w, A_STANDOUT | A_BOLD );
        sprintf(item, "%-15s",  list[i]);
        mvwprintw( w, i+1, 2, "%s", item );
    }
 
    wrefresh( w ); // update the terminal screen
 
    i = 1;
    keypad( w, TRUE ); // enable keyboard input for the window.
    curs_set( 0 ); // hide the default screen cursor.
     
       // get the input
    while(choice == -1 && ( ch = wgetch(w)) != 'q'){ 
         
                // right pad with spaces to make the items appear with even width.
            sprintf(item, "%-15s",  list[i]); 
            mvwprintw( w, i+1, 2, "%s", item ); 
              // use a variable to increment or decrement the value based on the input.
            switch( ch ) {
                case KEY_UP:
					i--;
					if (i < 1) i = 4;
					break;
                case KEY_DOWN:
					i++;
					if (i > 4) i = 1;
					break;
				case KEY_RIGHT:
					choice = i;
					break;
            }
            // now highlight the next item in the list.
            wattron( w, A_STANDOUT );
             
            sprintf(item, "%-15s",  list[i]);
            mvwprintw( w, i+1, 2, "%s", item);
            wattroff( w, A_STANDOUT );
    }
 
    delwin( w );
    endwin();
    return choice;
	
}

void hints() {
	
    WINDOW *w;

    w = newwin( 4, 25, 1, cols + 10 ); // create a new window
    box( w, 0, 0 ); // sets default borders for the window
    
    mvwprintw(w, 1, 2, "Press 'D' to advance");
    mvwprintw(w, 2, 2, "Press 'Q' to quit");
	
	wrefresh(w);
	refresh();
	
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
    int i;
    for (i = 0; i < *n_cells; i++)
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
	wclear(window);
	box(window,0,0);
	wborder(window, '|', '|', '~', '~', '+', '+', '+', '+');
	hints();
    int i,j;
    for (i = 1; i <= rows; i++)
    {
        for (j = 1; j <= cols; j++)
        {
            if (live_cells[j][i] == 1) {
				mvwprintw(window,j,i,"O"); 
			}
        }
    }
    refresh();
    usleep(DELAY); 
}

void draw_cell(int x, int y, int cell_list[rows*cols][2], int *n_cells)
{
    cell_list[*n_cells][0] = x;
    cell_list[*n_cells][1] = y;
    (*n_cells)++;
}

void create_object(int x, int y, int choice, int cell_list[rows*cols][2], int *n_cells)
{
    int k;
    switch(choice)
    {
		
		case 1:
			// r_pentomino
            draw_cell(x+1, y  , cell_list, n_cells);
            draw_cell(x+2, y  , cell_list, n_cells);
            draw_cell(x  , y+1, cell_list, n_cells);
            draw_cell(x+1, y+1, cell_list, n_cells);
            draw_cell(x+1, y+2, cell_list, n_cells);

        break;

        case 2:
			// diehard
            draw_cell(x  , y+1, cell_list, n_cells);
            draw_cell(x+1, y+1, cell_list, n_cells);
            draw_cell(x+1, y+2, cell_list, n_cells);

            draw_cell(x+6, y  , cell_list, n_cells);

            draw_cell(x+5, y+2, cell_list, n_cells);
            draw_cell(x+6, y+2, cell_list, n_cells);
            draw_cell(x+7, y+2, cell_list, n_cells);


        break;

        case 3:
			// acorn
            draw_cell(x+1, y  , cell_list, n_cells);
            draw_cell(x+3, y+1, cell_list, n_cells);

            draw_cell(x  , y+2, cell_list, n_cells);
            draw_cell(x+1, y+2, cell_list, n_cells);
            draw_cell(x+4, y+2, cell_list, n_cells);
            draw_cell(x+5, y+2, cell_list, n_cells);
            draw_cell(x+6, y+2, cell_list, n_cells);

        break;
        case 4:
			// glider
            draw_cell(x+1,   y, cell_list, n_cells);
            draw_cell(x+2, y+1, cell_list, n_cells);
            draw_cell(x,   y+2, cell_list, n_cells);
            draw_cell(x+1, y+2, cell_list, n_cells);
            draw_cell(x+2, y+2, cell_list, n_cells);

        break;

        case 5:
			// small_exploder
            draw_cell(x+1,   y, cell_list, n_cells);
            draw_cell(x,   y+1, cell_list, n_cells);
            draw_cell(x+1, y+1, cell_list, n_cells);
            draw_cell(x+2, y+1, cell_list, n_cells);
            draw_cell(x,   y+2, cell_list, n_cells);
            draw_cell(x+2, y+2, cell_list, n_cells);
            draw_cell(x+1, y+3, cell_list, n_cells);

        break;

        case 6:
			// exploder
            draw_cell(x  , y  , cell_list, n_cells);
            draw_cell(x+2, y  , cell_list, n_cells);
            draw_cell(x+4, y  , cell_list, n_cells);
            draw_cell(x  , y+1, cell_list, n_cells);
            draw_cell(x  , y+2, cell_list, n_cells);
            draw_cell(x  , y+3, cell_list, n_cells);
            draw_cell(x  , y+4, cell_list, n_cells);
            draw_cell(x+4, y+1, cell_list, n_cells);
            draw_cell(x+4, y+2, cell_list, n_cells);
            draw_cell(x+4, y+3, cell_list, n_cells);
            draw_cell(x+4, y+4, cell_list, n_cells);

        break;

        case 7:
			// 10_cell_row
	    
            for (k = 0; k < 10; k++)
            {
               draw_cell(x+k, y, cell_list, n_cells);
            }

        break;

        case 8:
			// spaceship
            draw_cell(x+1, y  , cell_list, n_cells);
            draw_cell(x+2, y  , cell_list, n_cells);
            draw_cell(x+3, y  , cell_list, n_cells);
            draw_cell(x+4, y  , cell_list, n_cells);
            draw_cell(x  , y+1, cell_list, n_cells);
            draw_cell(x+4, y+1, cell_list, n_cells);
            draw_cell(x+4, y+2, cell_list, n_cells);
            draw_cell(x  , y+3, cell_list, n_cells);
            draw_cell(x+3, y+3, cell_list, n_cells);

        break;

        case 9:
			// tumbler
            draw_cell(x+1, y  , cell_list, n_cells);
            draw_cell(x+2, y  , cell_list, n_cells);
            draw_cell(x+4, y  , cell_list, n_cells);
            draw_cell(x+5, y  , cell_list, n_cells);
            draw_cell(x+1, y+1, cell_list, n_cells);
            draw_cell(x+2, y+1, cell_list, n_cells);
            draw_cell(x+4, y+1, cell_list, n_cells);
            draw_cell(x+5, y+1, cell_list, n_cells);
            draw_cell(x+2, y+2, cell_list, n_cells);
            draw_cell(x+4, y+2, cell_list, n_cells);
            draw_cell(x+2, y+3, cell_list, n_cells);
            draw_cell(x+4, y+3, cell_list, n_cells);
            draw_cell(x+2, y+4, cell_list, n_cells);
            draw_cell(x+4, y+4, cell_list, n_cells);

            draw_cell(x  , y+3, cell_list, n_cells);
            draw_cell(x+6, y+3, cell_list, n_cells);
            draw_cell(x  , y+4, cell_list, n_cells);
            draw_cell(x+6, y+4, cell_list, n_cells);
            draw_cell(x  , y+5, cell_list, n_cells);
            draw_cell(x+6, y+5, cell_list, n_cells);

            draw_cell(x+1, y+5, cell_list, n_cells);
            draw_cell(x+5, y+5, cell_list, n_cells);


        break;

        case 10:
			// glider_gun
            draw_cell(x  , y+2, cell_list, n_cells);
            draw_cell(x  , y+2, cell_list, n_cells);
            draw_cell(x+1, y+3, cell_list, n_cells);
            draw_cell(x+1, y+3, cell_list, n_cells);

            draw_cell(x+9 , y+2, cell_list, n_cells);
            draw_cell(x+10, y+2, cell_list, n_cells);
            draw_cell(x+8, y+3, cell_list, n_cells);
            draw_cell(x+10, y+3, cell_list, n_cells);
            draw_cell(x+8, y+4, cell_list, n_cells);
            draw_cell(x+9, y+4, cell_list, n_cells);

            draw_cell(x+16, y+4, cell_list, n_cells);
            draw_cell(x+17, y+4, cell_list, n_cells);
            draw_cell(x+16, y+5, cell_list, n_cells);
            draw_cell(x+18, y+5, cell_list, n_cells);
            draw_cell(x+16, y+6, cell_list, n_cells);

            draw_cell(x+23 , y, cell_list, n_cells);
            draw_cell(x+24, y, cell_list, n_cells);
            draw_cell(x+22, y+1, cell_list, n_cells);
            draw_cell(x+24, y+1, cell_list, n_cells);
            draw_cell(x+22, y+2, cell_list, n_cells);
            draw_cell(x+23, y+2, cell_list, n_cells);

            draw_cell(x+24, y+12, cell_list, n_cells);
            draw_cell(x+25, y+12, cell_list, n_cells);
            draw_cell(x+26, y+12, cell_list, n_cells);
            draw_cell(x+24, y+13, cell_list, n_cells);
            draw_cell(x+25, y+14, cell_list, n_cells);

            draw_cell(x+34, y  , cell_list, n_cells);
            draw_cell(x+34, y  , cell_list, n_cells);
            draw_cell(x+35, y+1, cell_list, n_cells);
            draw_cell(x+35, y+1, cell_list, n_cells);

            draw_cell(x+35, y+7, cell_list, n_cells);
            draw_cell(x+36, y+7, cell_list, n_cells);
            draw_cell(x+35, y+8, cell_list, n_cells);
            draw_cell(x+37, y+8, cell_list, n_cells);
            draw_cell(x+35, y+9, cell_list, n_cells);

        break;

        default:
        break;
    }
}









