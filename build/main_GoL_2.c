#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <unistd.h>

// X CAP A BAIX, Y CAP A LA DRETA!




#define rows 40
#define cols 80	
#define DELAY 5000 //que es¿?¿? --> UN DELAY PERQUÈ NO S'EXECUTI TOT INSTANTÀNIAMENT
#define margin 1000

void store_cells(int cell_list[rows*cols][2], int * n_cells, char live_cells[rows+margin][cols+margin]);
void check_cell(int x, int y, char live_cells[rows+margin][cols+margin], char checked_cells[rows+margin][cols+margin], int cell_list_2[rows*cols][2], int * n_cells_2 ,char live_cells_2[rows+margin][cols+margin]);
int count_cells(int x, int y, char live_cells[rows+margin][cols+margin]);

void update_grid(cell_list, live_cells, checked_cells, cell_list_2, *n_cells_2, live_cells_2);

void draw_cell(int x, int y, int cell_list[rows*cols][2], int *n_cells);
void create_object(int x, int y, int choice, int cell_list[rows*cols][2], int *n_cells);

void plot_cells(char live_cells[rows+margin][cols+margin]);

int starting_menu();
void hints();



WINDOW *init_window(int height, int width);

//------GLOBAL VARIABLES------

WINDOW *window;
int init_center_x = (rows+margin)/2;
int init_center_y = (cols+margin)/2;
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

    char live_cells[rows+margin][cols+margin]; // matriu on es mostren les cells vives, la grid
    memset(live_cells,0,sizeof(char)*(rows+margin)*(cols+margin)); //inicialitzem a 0

    char live_cells_2[rows+margin][cols+margin]; // matriu on es mostren les cells vives
    memset(live_cells_2,0,sizeof(char)*(rows+margin)*(cols+margin)); //inicialitzem a 0

    char checked_cells[rows+margin][cols+margin]; // matriu on es mostren les cells comprobades, siguin vives o mortes
    memset(checked_cells,0,sizeof(char)*(rows+margin)*(cols+margin)); //inicialitzem a 0
	
	initscr();
	noecho();
	cbreak();
	curs_set(FALSE);
	
	window = init_window(rows,cols);

    int choice = starting_menu();

	if (choice != 11) {
    //------------------------------------ init -----------------------------------------	
	
    create_object(init_center_x, init_center_y, choice, cell_list, &n_cells);

	
	hints();
    
    //---------------------------------------------------------------------------------------------
	
    store_cells(cell_list,&n_cells,live_cells); // funcio per a passar la llista de cells vives a la matriu binaria

    plot_cells(live_cells);
    
    //-------------------------------------- programa principal ---------------------------------------------------

    int l, i;
    for(l = 0; true; l++)
    {
		keypad( window, TRUE );
        
        while ((ch = wgetch(window)) != KEY_RIGHT) { 
			if (ch == 'q') {
				endwin();
				return 0;
			}
		}

        for (i = 0; i < n_cells; i++) //Per a cada cell viva s'evaluen els neighbours

        {   //funcio per evaluar si la cell ha de morir o viure, actualitza les matrius
			
			update_grid(cell_list, live_cells, checked_cells, cell_list_2, *n_cells_2, live_cells_2);

        }
	
        memset(cell_list,0,sizeof(int)*(rows)*(cols)*2); //inicialitzem a 0 
        n_cells = 0;

        memset(live_cells,0,sizeof(char)*(rows+margin)*(cols+margin)); //inicialitzem a 0
        memset(checked_cells,0,sizeof(char)*(rows+margin)*(cols+margin)); //inicialitzem a 0



        plot_cells(live_cells_2);
        
        while ((ch = wgetch(window)) != KEY_RIGHT) { 
			if (ch == 'q') {
				endwin();
				return 0;
			}
		}


        for (i = 0; i < n_cells_2; i++) //Per a cada cell viva s'evaluen els neighbours

        {   //funcio per evaluar si la cell ha de morir o viure, actualitza les matrius
			
			update_grid(cell_list_2, live_cells_2, checked_cells, cell_list, *n_cells, live_cells);

        }

        memset(cell_list_2,0,sizeof(int)*(rows)*(cols)*2); //inicialitzem a 0
        n_cells_2 = 0;

        memset(live_cells_2,0,sizeof(char)*(rows+margin)*(cols+margin)); //inicialitzem a 0
        memset(checked_cells,0,sizeof(char)*(rows+margin)*(cols+margin)); //inicialitzem a 0

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
    char list[12][18] = { "SELECT (WITH ->):", "The R-Pentomino", "Diehard", "Acorn", "Glider", "Small Exploder", "Exploder", "10_cell_row", "Spaceship", "Tumbler", "Glider Gun", "QUIT" };
    char item[18];
    int ch, i = 0;

    w = newwin( 14, 22, 1, 40 ); // create a new window
    box( w, 0, 0 ); // sets default borders for the window
    
    for(i = 0; i < 12; i++ ) {
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
    while (choice == -1){ 
			
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
            // now highlight the next item in the list.
            wattron( w, A_STANDOUT | A_BOLD);
             
            sprintf(item, "%-15s",  list[i]);
            mvwprintw( w, i+1, 2, "%s", item);
            wattroff( w, A_STANDOUT | A_BOLD );
    }
 
    delwin( w );
    endwin();
    
    return choice;
	
}

void hints() {
	
    WINDOW *w;

    w = newwin( 4, 30, 1, cols + 10 ); // create a new window
    box( w, 0, 0 ); // sets default borders for the window
    
    mvwprintw(w, 1, 2, "Press 'RIGHT' to advance");
    mvwprintw(w, 2, 2, "Press 'Q' to quit");
    //mvwprintw(w, 3, 2, "Press 'R' to return");
	
	wrefresh(w);
	refresh();
	
}


WINDOW *init_window(int height, int width) {
	
	WINDOW *wind;
	wind = newwin(height, width,0,0);
	
	wrefresh(wind);
	
	return wind;
}

void update_grid(cell_list, live_cells, checked_cells, cell_list_2, *n_cells_2, live_cells_2) {
	
	check_cell(cell_list[i][0]-1,   cell_list[i][1]-1,  live_cells, checked_cells, cell_list_2, n_cells_2, live_cells_2);
	check_cell(cell_list[i][0],     cell_list[i][1]-1,  live_cells, checked_cells, cell_list_2, n_cells_2, live_cells_2);
	check_cell(cell_list[i][0]+1,   cell_list[i][1]-1,  live_cells, checked_cells, cell_list_2, n_cells_2, live_cells_2);

	check_cell(cell_list[i][0]-1,   cell_list[i][1],    live_cells, checked_cells, cell_list_2, n_cells_2, live_cells_2);
	check_cell(cell_list[i][0]+1,   cell_list[i][1],    live_cells, checked_cells, cell_list_2, n_cells_2, live_cells_2);

	check_cell(cell_list[i][0]-1,   cell_list[i][1]+1,  live_cells, checked_cells, cell_list_2, n_cells_2, live_cells_2);
	check_cell(cell_list[i][0],     cell_list[i][1]+1,  live_cells, checked_cells, cell_list_2, n_cells_2, live_cells_2);
	check_cell(cell_list[i][0]+1,   cell_list[i][1]+1,  live_cells, checked_cells, cell_list_2, n_cells_2, live_cells_2);
	
}

void store_cells(int cell_list[rows*cols][2], int * n_cells, char live_cells[rows+margin][cols+margin])
{ 
	//Passa de cell_list a live_cells
    int i;
    for (i = 0; i < *n_cells; i++)
    {
        live_cells[cell_list[i][0]][cell_list[i][1]] = 1;
    }
}

void check_cell(int x, int y, char live_cells[rows+margin][cols+margin], char checked_cells[rows+margin][cols+margin], int cell_list_2[rows*cols][2], int * n_cells_2 ,char live_cells_2[rows+margin][cols+margin])
{


    int neighbours;

    if (!checked_cells[x][y])
    {
        checked_cells[x][y] = 1;

        if ( x != 0 && y != 0 && x != rows + margin - 1 && y != cols + margin - 1 ) //Limits de la quadricula
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

void plot_cells(char live_cells[rows+margin][cols+margin])
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
            if (live_cells[i][j] == 1 && (i > margin/2 && i < rows + margin/2) && (j > margin/2 && j < cols + margin/2)) {
				mvwprintw(window,i - margin/2,j - margin/2,"O"); 
				
			}
        }
    }
    //mvwprintw(window, 10, cols - 10, "Hola");
    wrefresh(window);
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
            draw_cell(x  , y  , cell_list, n_cells);
            draw_cell(x+1, y  , cell_list, n_cells);
            draw_cell(x  , y-1, cell_list, n_cells);
            draw_cell(x-1, y  , cell_list, n_cells);
            draw_cell(x-1, y+1, cell_list, n_cells);

        break;

        case 2:
			// diehard
            draw_cell(x  , y-2, cell_list, n_cells);
            draw_cell(x  , y-3, cell_list, n_cells);
            draw_cell(x+1, y-2, cell_list, n_cells);

            draw_cell(x-1, y+3, cell_list, n_cells);

            draw_cell(x+1, y+2, cell_list, n_cells);
            draw_cell(x+1, y+3, cell_list, n_cells);
            draw_cell(x+1, y+4, cell_list, n_cells);


        break;

        case 3:
			// acorn
            draw_cell(x  , y  , cell_list, n_cells);
            draw_cell(x  , y+1, cell_list, n_cells);

            draw_cell(x-2, y+1, cell_list, n_cells);
            draw_cell(x-1, y+3, cell_list, n_cells);
            draw_cell(x  , y+4, cell_list, n_cells);
            draw_cell(x  , y+5, cell_list, n_cells);
            draw_cell(x  , y+6, cell_list, n_cells);

        break;
        case 4:
			// glider
            draw_cell(x-1, y  , cell_list, n_cells);
            draw_cell(x  , y+1, cell_list, n_cells);
            draw_cell(x+1, y+1, cell_list, n_cells);
            draw_cell(x+1, y  , cell_list, n_cells);
            draw_cell(x+1, y-1, cell_list, n_cells);

        break;

        case 5:
			// small_exploder
            draw_cell(x+1, y  , cell_list, n_cells);
            draw_cell(x  , y-1, cell_list, n_cells);
            draw_cell(x-1, y-1, cell_list, n_cells);
            draw_cell(x-1, y  , cell_list, n_cells);
            draw_cell(x-2, y  , cell_list, n_cells);
            draw_cell(x-1, y+1, cell_list, n_cells);
            draw_cell(x  , y+1, cell_list, n_cells);

        break;

        case 6:
			// exploder
            draw_cell(x-2, y  , cell_list, n_cells);
            draw_cell(x-2, y+2, cell_list, n_cells);
            draw_cell(x-1, y+2, cell_list, n_cells);
            draw_cell(x  , y+2, cell_list, n_cells);
            draw_cell(x+1, y+2, cell_list, n_cells);
            draw_cell(x+2, y+2, cell_list, n_cells);
            draw_cell(x+2, y  , cell_list, n_cells);
            draw_cell(x+2, y-2, cell_list, n_cells);
            draw_cell(x+1, y-2, cell_list, n_cells);
            draw_cell(x  , y-2, cell_list, n_cells);
            draw_cell(x-1, y-2, cell_list, n_cells);
            draw_cell(x-2, y-2, cell_list, n_cells);

        break;

        case 7:
			// 10_cell_row
            for (k = -5; k < 5; k++)
            {
               draw_cell(x, y + k, cell_list, n_cells);
            }

        break;

        case 8:
			// spaceship
            draw_cell(x+1, y+1, cell_list, n_cells);
            draw_cell(x  , y+2, cell_list, n_cells);
            draw_cell(x-1, y+2, cell_list, n_cells);
            draw_cell(x-2, y+2, cell_list, n_cells);
            draw_cell(x-2, y+1, cell_list, n_cells);
            draw_cell(x-2, y  , cell_list, n_cells);
            draw_cell(x-2, y-1, cell_list, n_cells);
            draw_cell(x-1, y-2, cell_list, n_cells);
            draw_cell(x+1, y-2, cell_list, n_cells);

        break;

        case 9:
			// tumbler
            draw_cell(x-2, y+1, cell_list, n_cells);
            draw_cell(x-2, y+2, cell_list, n_cells);
            draw_cell(x-1, y+1, cell_list, n_cells);
            draw_cell(x-1, y+2, cell_list, n_cells);
            draw_cell(x  , y+1, cell_list, n_cells);
            draw_cell(x+1, y+1, cell_list, n_cells);
            draw_cell(x+2, y+1, cell_list, n_cells);
            draw_cell(x+3, y+2, cell_list, n_cells);
            draw_cell(x+3, y+3, cell_list, n_cells);
            draw_cell(x+2, y+3, cell_list, n_cells);
            draw_cell(x+1, y+3, cell_list, n_cells);
            
            draw_cell(x-2, y-1, cell_list, n_cells);
            draw_cell(x-2, y-2, cell_list, n_cells);
            draw_cell(x-1, y-1, cell_list, n_cells);
            draw_cell(x-1, y-2, cell_list, n_cells);
            draw_cell(x  , y-1, cell_list, n_cells);
            draw_cell(x+1, y-1, cell_list, n_cells);
            draw_cell(x+2, y-1, cell_list, n_cells);
            draw_cell(x+3, y-2, cell_list, n_cells);
            draw_cell(x+3, y-3, cell_list, n_cells);
            draw_cell(x+2, y-3, cell_list, n_cells);
            draw_cell(x+1, y-3, cell_list, n_cells);


        break;

        case 10:
        
			// glider_gun
            draw_cell(x-3, y-19, cell_list, n_cells);
            draw_cell(x-3, y-18, cell_list, n_cells);
            draw_cell(x-2, y-19, cell_list, n_cells);
            draw_cell(x-2, y-18, cell_list, n_cells);

            draw_cell(x-3, y-10, cell_list, n_cells);
            draw_cell(x-3, y-9, cell_list, n_cells);
            draw_cell(x-2, y-11, cell_list, n_cells);
            draw_cell(x-2, y-9, cell_list, n_cells);
            draw_cell(x-1, y-11, cell_list, n_cells);
            draw_cell(x-1, y-10, cell_list, n_cells);

            draw_cell(x-1, y-3, cell_list, n_cells);
            draw_cell(x-1, y-2, cell_list, n_cells);
            draw_cell(x  , y-3, cell_list, n_cells);
            draw_cell(x  , y-1, cell_list, n_cells);
            draw_cell(x+1, y-3, cell_list, n_cells);

            draw_cell(x-5, y+4, cell_list, n_cells);
            draw_cell(x-5, y+5, cell_list, n_cells);
            draw_cell(x-4, y+3, cell_list, n_cells);
            draw_cell(x-4, y+5, cell_list, n_cells);
            draw_cell(x-3, y+3, cell_list, n_cells);
            draw_cell(x-3, y+4, cell_list, n_cells);

            draw_cell(x+7, y+5, cell_list, n_cells);
            draw_cell(x+7, y+6, cell_list, n_cells);
            draw_cell(x+7, y+7, cell_list, n_cells);
            draw_cell(x+8, y+5, cell_list, n_cells);
            draw_cell(x+9, y+6, cell_list, n_cells);

            draw_cell(x-5, y+15, cell_list, n_cells);
            draw_cell(x-5, y+16, cell_list, n_cells);
            draw_cell(x-4, y+15, cell_list, n_cells);
            draw_cell(x-4, y+16, cell_list, n_cells);

            draw_cell(x+2, y+16, cell_list, n_cells);
            draw_cell(x+2, y+17, cell_list, n_cells);
            draw_cell(x+3, y+16, cell_list, n_cells);
            draw_cell(x+3, y+18, cell_list, n_cells);
            draw_cell(x+4, y+16, cell_list, n_cells);
		
        break;
        

        default:
        break;
    }
}









