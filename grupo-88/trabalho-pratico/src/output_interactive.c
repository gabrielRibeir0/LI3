#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif

#include <ncurses.h>
#include "stdlib.h"
#include "output_interactive.h"

void output_menu(int cmd_number){
    FILE *fp;
    int row, y, size;
    char *line = NULL;
    size_t len;
    keypad(stdscr, TRUE);
    row = getmaxy(stdscr);
    long offset, *pos = NULL, lineno = 0;
    TABSIZE = 1;
    char *filepath;

    size = asprintf(&filepath, "./Resultados/command%d_output.txt", cmd_number);
    if(size == -1){
        perror("Error");
        return;
    }

    clear();
    fp = fopen(filepath, "r");
    fseek (fp, 0, SEEK_END);
    long file_size = ftell(fp);

    if (0 != file_size) {
        while (1) {
            long topline = lineno;
            while ((getline(&line, &len, fp)) != -1) {
                offset = ftell(fp);
                pos = realloc(pos, (lineno + 1) * sizeof *pos);
                if (!pos) exit(1);
                pos[lineno++] = offset;
                addstr(line);
                getyx(stdscr, y, size);
                if (y == row - 1)
                    break;
            }
            printw("Pressione seta para cima e seta para baixo para se movimentar e N para continuar...");
            int c = getch();
            if (c == KEY_UP)    // Up arrow
                fseek(fp, pos[lineno = topline > 1 ? topline - 1 : 0], SEEK_SET);
            else if (c == KEY_PPAGE) // Previous page
                fseek(fp, pos[lineno = topline >= row ? topline - row + 1 : 0], SEEK_SET);
            else if (c == 'n' || !line)
                break;

            clear();
            move(0, 0);
        }
    }
    else{
        printw("Output da query vazio, pressione alguma tecla para continuar...");
        getch();
    }


    fclose(fp);
    free(pos);
    if(line != NULL){
        free(line);
    }
    clear();

    textfile_menu(filepath);
    erase();
}

void textfile_menu(char *filepath){
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    int xMax;
    xMax = getmaxx (stdscr);

    WINDOW *menuwin = newwin(7, xMax - 15, 0, 8);
    box(menuwin, 0, 0);
    refresh();
    wrefresh(menuwin);

    keypad(menuwin, true);

    char choices [2][6] = {"Sim", "Não"};
    int choice;
    int highlight = 0;

    while (1) {

        mvwprintw(menuwin, 1, 2, "Deseja ter o output num ficheiro de texto? (Pasta Resultados)");
        for (int i = 0; i < 2; i++) {
            if (i == highlight)
                wattron(menuwin, A_REVERSE);
            mvwprintw(menuwin, i + 3, 2, "%-5s", choices[i]);
            if (i == highlight)
                wattroff(menuwin, A_REVERSE);
        }

        wrefresh(menuwin);
        choice = wgetch(menuwin);

        switch (choice) {
            case KEY_UP:
                highlight --;
                if (highlight == -1)
                    highlight = 0;
                break;
            case KEY_DOWN:
                highlight ++;
                if (highlight == 2)
                    highlight = 1;
                break;
            case 10:
                break;
            default:
                break;
        }
        if (choice == 10)
            break;
    }
    if (highlight == 1){
        remove(filepath);
    }

    erase();
    endwin();
}