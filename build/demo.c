#include <ncurses.h>
#include <unistd.h>

#define DELAY 10000

// A ball that bounces back and forth around the screen!

int main(int argc, char *argv[]) {
	int x = 0, y = 0;
	int max_y = 0, max_x = 0;
	int next_x = 0, next_y = 0;
	int direction = 1;

	initscr();
	noecho();
	curs_set(FALSE);

	// Global var `stdscr` is created by the call to `initscr()`
	getmaxyx(stdscr, max_y, max_x); // Assigns to max_x and max_y the size of the screen.

	while(1) {
		clear();
		mvprintw(y, x, "o");
		refresh();

		usleep(DELAY);

		next_x = x + direction;
		//next_y = y + direction;

		if (next_x >= max_x || next_x < 0 || next_y >= max_y || next_y < 0) {
			direction *= -1;

		} else {
			x += direction;
			//y += direction;

		}
	}

	endwin();
}
