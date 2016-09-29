#include "init.c"
#include "algorithm.c"
#include "plot.c"

//-----------------------------------MAIN PROGRAM-------------------------------------

int main()
{    

    // variable inicialization

    int cell_list[(rows+margin)*(cols+margin)][2]; // llista amb les coordenades de les cells vives
    memset(cell_list,0,sizeof(int)*(rows+margin)*(cols+margin)*2); //inicialitzem a 0.
    //ROWS I COLS +2 perque afegim uns contorns al nostre mapa

    int cell_list_2[(rows+margin)*(cols+margin)][2]; // llista amb les coordenades de les cells vives
    memset(cell_list_2,0,sizeof(int)*(rows+margin)*(cols+margin)*2); //inicialitzem a 0

    int n_cells = 0; //Numero de celules vives, es el tamany de cell_list
    int n_cells_2 = 0;

    char live_cells[rows+margin][cols+margin]; // matriu on es mostren les cells vives, la grid
    memset(live_cells,0,sizeof(char)*(rows+margin)*(cols+margin)); //inicialitzem a 0

    char live_cells_2[rows+margin][cols+margin]; // matriu on es mostren les cells vives
    memset(live_cells_2,0,sizeof(char)*(rows+margin)*(cols+margin)); //inicialitzem a 0

    char checked_cells[rows+margin][cols+margin]; // matriu on es mostren les cells comprobades, siguin vives o mortes
    memset(checked_cells,0,sizeof(char)*(rows+margin)*(cols+margin)); //inicialitzem a 0
	
    // plot inicialization
    initscr();
    noecho();
    cbreak();
    curs_set(FALSE);

    window = init_window(rows,cols);


    int choice = starting_menu();

    if (choice != 11) {
    //------------------------------------ init -----------------------------------------	
	
        create_object(init_center_x, init_center_y, choice, cell_list, &n_cells, live_cells);

        hints();

        plot_cells(live_cells);

        //----------------------------------------------- programa principal ---------------------------------------------------

        int l;
        for(l = 0; true; l++)
        {

            keypad( window, TRUE ); // Aixo ha de estar dins el for?

            wait_for_next_key(KEY_RIGHT);

            next_state(cell_list,   &n_cells,   live_cells, checked_cells,
                       cell_list_2, &n_cells_2, live_cells_2);


            plot_cells(live_cells_2);

            wait_for_next_key(KEY_RIGHT);

            next_state(cell_list_2, &n_cells_2, live_cells_2, checked_cells,
                       cell_list,   &n_cells,   live_cells);


            plot_cells(live_cells);
        }

        delwin(window);
        endwin(); //PER TANCAR EL NCURSES QUAN ACABA TOTA L'EXECUCIÓ

    }

    return 0;
}
