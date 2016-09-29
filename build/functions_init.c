#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <unistd.h>

#define coord_x 0
#define coord_y 1

int rows = 20;
int cols = 20;

void draw_cell(int x, int y, int cell_list[rows*cols][2], int *n_cells);
void create_object(int x, int y, int choice, int cell_list[rows*cols][2], int *n_cells);

int main() {
	return 0;
}


void draw_cell(int x, int y, int cell_list[rows*cols][2], int *n_cells)
{
    cell_list[*n_cells][0] = x;
    cell_list[*n_cells][1] = y;
    *n_cells++;

}

void create_object(int x, int y, int choice, int cell_list[rows*cols][2], int *n_cells)
{
    switch(choice)
    {
        case 1:
			// Glider
            draw_cell(x+1,   y, cell_list, n_cells);
            draw_cell(x+2, y+1, cell_list, n_cells);
            draw_cell(x,   y+2, cell_list, n_cells);
            draw_cell(x+1, y+2, cell_list, n_cells);
            draw_cell(x+2, y+2, cell_list, n_cells);

        break;

        case 2:
			// small_exploder
            draw_cell(x+1,   y, cell_list, n_cells);
            draw_cell(x,   y+1, cell_list, n_cells);
            draw_cell(x+1, y+1, cell_list, n_cells);
            draw_cell(x+2, y+1, cell_list, n_cells);
            draw_cell(x,   y+2, cell_list, n_cells);
            draw_cell(x+2, y+2, cell_list, n_cells);
            draw_cell(x+1, y+3, cell_list, n_cells);

        break;

        case 3:
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

        case 4:
			// 10_cell_row
            for (int k = 0; k < 10; k++)
            {
               draw_cell(x+k, y, cell_list, n_cells);
            }

        break;

        case 5:
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

        case 6:
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

        case 7:
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

        case 8:
			// r_pentomino
            draw_cell(x+1, y  , cell_list, n_cells);
            draw_cell(x+2, y  , cell_list, n_cells);
            draw_cell(x  , y+1, cell_list, n_cells);
            draw_cell(x+1, y+1, cell_list, n_cells);
            draw_cell(x+1, y+2, cell_list, n_cells);

        break;

        case 9:
			// diehard
            draw_cell(x  , y+1, cell_list, n_cells);
            draw_cell(x+1, y+1, cell_list, n_cells);
            draw_cell(x+1, y+2, cell_list, n_cells);

            draw_cell(x+6, y  , cell_list, n_cells);

            draw_cell(x+5, y+2, cell_list, n_cells);
            draw_cell(x+6, y+2, cell_list, n_cells);
            draw_cell(x+7, y+2, cell_list, n_cells);


        break;

        case 10:
			// acorn
            draw_cell(x+1, y  , cell_list, n_cells);
            draw_cell(x+3, y+1, cell_list, n_cells);

            draw_cell(x  , y+2, cell_list, n_cells);
            draw_cell(x+1, y+2, cell_list, n_cells);
            draw_cell(x+4, y+2, cell_list, n_cells);
            draw_cell(x+5, y+2, cell_list, n_cells);
            draw_cell(x+6, y+2, cell_list, n_cells);

        break;

        default:
        break;
    }
}




