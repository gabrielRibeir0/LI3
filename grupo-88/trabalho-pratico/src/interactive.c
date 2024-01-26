#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif

#include "interactive.h"
#include "interpreter.h"
#include "parser.h"
#include <stdio.h>
#include <ncurses.h>
#include <string.h>

void backspace() {
    int r, c;
    noecho();
    nocbreak();
    getyx(stdscr, r, c);
    move(r, c - 1);
    delch();
    cbreak();
    refresh();
}

void continue_menu(DATA_USERS *data_users, DATA_FLIGHTS *data_flights, DATA_RESERVATIONS *data_reservations, DATA_STATISTICS *data_statistics, char *caminho){
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
        
            mvwprintw(menuwin, 1, 2, "Deseja continuar com mais alguma Query?");
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
    if (highlight == 0){
        erase();
        second_menu(data_users, data_flights, data_reservations, data_statistics, caminho);
    }
    else{
        erase();
        int ret;
        ret = system("clear");
        if(ret == -1){
            exit(0);
        }
        exit(0);
    }

    erase();
    endwin();
}

void third_menu(DATA_USERS *data_users, DATA_FLIGHTS *data_flights, DATA_RESERVATIONS *data_reservations, DATA_STATISTICS *data_statistics, char *caminho, int n_Query){
    initscr();
    cbreak();
    keypad(stdscr, TRUE);

    int xMax;
    xMax = getmaxx (stdscr);

    int max_length = 64;
    char input[max_length + 1];
    memset(input, 0, 65);

    WINDOW *menuwin = newwin(4, xMax - 15, 0, 8);
    box(menuwin, 0, 0);
    refresh();
    wrefresh(menuwin);
    mvprintw(1, 10, "Escreva o comando (press Enter para enviar):");

    int ch;
    int position, init_position;

    if(n_Query == 10){
        input[0] = '1';
        input[1] = '0';
        input[2] = ' ';
        position = 3;
        init_position = 3;
    }
    else{
        input[0] = (char)(n_Query + 48);
        input[1] = ' ';
        position = 2;
        init_position = 2;
    }

    while (1) {
        ch = getch();

        switch (ch) {
            case KEY_BACKSPACE: //apagar
                if (position > 0) {
                    position--;
                    input[position] = '\0';
                    backspace();
                }
                break;
            case 10:
                break;
            default:
                if (position < max_length) {
                    input[position] = ch;
                    mvprintw(2, 10, "Você digitou: %s", input + init_position);
                    position ++;
                }
                else {
                    ch = 10;
                }
                break;
        }
        if (ch == 10) {
            break;
        }
    }
    static int n_command = 1;
    Cmd *command = parse_command(input);
    execute_command(command, n_command, data_users, data_flights, data_reservations, data_statistics, INTERACTIVE);
    //aparecer output no terminal
    //perguntar se quer mais queries (voltar ao segundo menu)
    n_command++;
    continue_menu(data_users, data_flights, data_reservations, data_statistics, caminho);
    endwin();
}

void instructions_menu (DATA_USERS *data_users, DATA_FLIGHTS *data_flights, DATA_RESERVATIONS *data_reservations, DATA_STATISTICS *data_statistics, char *caminho){
    clear();
    initscr();
    cbreak();
    keypad(stdscr, TRUE);

    int xMax;
    xMax = getmaxx (stdscr);

    WINDOW *menuwin = newwin(26, xMax - 13, 0, 6);
    refresh();
    wrefresh(menuwin);

    mvprintw(1, 8, "Especificações de cada Query:");
    mvprintw(3, 8, "Query 1 -> Lista o resumo de um utilizador, voo, ou reserva, consoante o identificador recebido por argumento.");
    mvprintw(4, 8, " Comando : <ID>");
    mvprintw(5, 8, "Query 2 -> : Lista os voos ou reservas de um utilizador ordenados por data.");
    mvprintw(6, 8, " Comando : <ID> [flights|reservations]");
    mvprintw(7, 8, "Query 3 -> Apresenta a classificação media de um hotel, a partir do seu identificador.");
    mvprintw(8, 8, " Comando : <ID>");
    mvprintw(9, 8, "Query 4 ->  Lista as reservas de um hotel, ordenadas por data de inicio. Usa-se o identificador como criterio de desempate.");
    mvprintw(10, 8, " Comando : <ID>");
    mvprintw(11, 8, "Query 5-> Lista os voos com origem num dado aeroporto, ordenados por data de partida estimada. Usa-se o identificador como desempate.");
    mvprintw(12, 8, " Comando : <Name> <Begin_date> <End_date>");
    mvprintw(13, 8, "Query 6 -> Lista o top N aeroportos com mais passageiros, para um dado ano. Usa-se o nome do aeroporto como criterio de desempate.");
    mvprintw(14, 8, " Comando : <Year> <N>");
    mvprintw(15, 8, "Query 7 -> Lista o top N aeroportos com a maior mediana de atrasos. Usa-se o nome do aeroporto como criterio de desempate.");
    mvprintw(16, 8, " Comando : <N>");
    mvprintw(17, 8, "Query 8 -> Apresenta a receita total de um hotel entre duas datas, a partir do seu utilizador.");
    mvprintw(18, 8, " Comando : <Id> <Begin_date> <End_date>");
    mvprintw(19, 8, "Query 9 -> Lista todos os utilizadores cujo nome começa com o prefixo passado por argumento, ordenados por nome");
    mvprintw(20, 8, " Comando : <Prefix>");
    mvprintw(21, 8, "Query 10 -> Apresenta metricas gerais: nº de novos utilizadores; nº de voos; nº de passageiros; nº de passageiros únicos; nº de reservas.");
    mvprintw(22, 8, " Comando :  [year [month]]");
    mvprintw(24,8, " Legenda: <x> delimita valores obrigatórios; [x] delimita campos opcionais; x|y|z delimita possíveis alternativas. ");


    int ch;

    while (1) {
        ch = getch();

        if(ch == ' '){
            break;
        }
    }
    erase();
    second_menu(data_users, data_flights, data_reservations, data_statistics, caminho);
    endwin();
}


void second_menu(DATA_USERS *data_users, DATA_FLIGHTS *data_flights, DATA_RESERVATIONS *data_reservations, DATA_STATISTICS *data_statistics, char *caminho){
    initscr();
    cbreak();
    noecho();

    int xMax, n_Query;
    xMax = getmaxx (stdscr);

    WINDOW * menuwin = newwin(14, xMax - 15, 0, 8);
    box(menuwin, 0, 0);
    refresh();
    wrefresh(menuwin);

    keypad(menuwin, true);

    char choices [10][30] = {"Query 1","Query 2","Query 3","Query 4","Query 5","Query 6","Query 7","Query 8","Query 9","Query 10"};
    int choice;
    int highlight = 0;

    while(1){
        for(int i = 0; i < 10; i++){
            mvwprintw(menuwin, 1, 2, "Escolha a Query (press Enter para selecionar)");
            mvwprintw(menuwin, 2, 2, "Press Espaço para aceder ás especificações");
            if (i == highlight)
                wattron(menuwin, A_REVERSE);
            mvwprintw (menuwin, i+3, 2,"%s", choices[i]);
            wattroff(menuwin, A_REVERSE);
        }
        choice = wgetch(menuwin);

        switch(choice){
            case KEY_UP:
                highlight --;
                if(highlight == -1) highlight = 0;
                break;
            case KEY_DOWN:
                highlight ++;
                if (highlight == 10) highlight = 9;
                break;
            case ' ':
                clear();
                instructions_menu(data_users, data_flights, data_reservations, data_statistics, caminho);
                erase();
                break;
        }

        if (choice == 10){ //enter
            switch (highlight) {
                case 0:
                    n_Query = 1;
                    break;
                case 1:
                    n_Query = 2;
                    break;
                case 2:
                    n_Query = 3;
                    break;
                case 3:
                    n_Query = 4;
                    break;
                case 4:
                    n_Query = 5;
                    break;
                case 5:
                    n_Query = 6;
                    break;
                case 6:
                    n_Query = 7;
                    break;
                case 7:
                    n_Query = 8;
                    break;
                case 8:
                    n_Query = 9;
                    break;
                default:
                    n_Query = 10;
                    break;
            }
            break;
        }
        erase();
    }
    erase();
    third_menu(data_users, data_flights, data_reservations, data_statistics, caminho, n_Query);
    getch();
    endwin();
}


void initInteractiveMode(DATA_USERS *data_users, DATA_FLIGHTS *data_flights, DATA_RESERVATIONS *data_reservations, DATA_STATISTICS *data_statistics){
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    int xMax;
    xMax = getmaxx (stdscr);

    int max_length = 64;
    char dataset_path_input[max_length + 1];
    memset(dataset_path_input, 0, 65);

    WINDOW *menuwin = newwin(4, xMax - 15, 0, 8);
    box(menuwin, 0, 0);
    refresh();
    wrefresh(menuwin);
    mvprintw(1, 10, "Escreva o caminho do Dataset (press Enter para enviar):");

    int ch;
    int position = 0;

    while (1) {
        ch = getch();

        switch (ch) {
            case KEY_BACKSPACE: //apagar
                if (position > 0) {
                    position--;
                    dataset_path_input[position] = '\0';
                    backspace();
                }
                break;
            case 10:
                break;
            default:
                if (position < max_length) {
                    dataset_path_input[position] = ch;
                    mvprintw(2, 10, "Você digitou: %s", dataset_path_input);
                    position ++;
                }
                else {
                    ch = 10;
                }
                break;
        }
        if (ch == 10)
            break;
    }
    erase();

    char *test_filepath;
    int size;
    size = asprintf(&test_filepath, "%s/users.csv", dataset_path_input);
    int acc = access(test_filepath, F_OK);

    while(size == -1 || acc == -1){
        free(test_filepath);
        box(menuwin, 0, 0);
        refresh();
        wrefresh(menuwin);
        mvprintw(1, 10, "Caminho inválido! Escreva o caminho do Dataset (press Enter para enviar):");
        memset(dataset_path_input, 0, 65);
        position = 0;
        while (1) {
            ch = getch();

            switch (ch) {
                case KEY_BACKSPACE: //apagar
                    if (position > 0) {
                        position--;
                        dataset_path_input[position] = '\0';
                        backspace();
                    }
                    break;
                case 10:
                    break;
                default:
                    if (position < max_length) {
                        dataset_path_input[position] = ch;
                        mvprintw(2, 10, "Você digitou: %s", dataset_path_input);
                        position ++;
                    }
                    else {
                        ch = 10;
                    }
                    break;
            }
            if (ch == 10)
                break;
        }

        erase();
        size = asprintf(&test_filepath, "%s/users.csv", dataset_path_input);
        acc = access(test_filepath, F_OK);
    }
    openDirectory(dataset_path_input, data_flights, data_reservations, data_users, data_statistics);
    second_menu (data_users, data_flights, data_reservations, data_statistics, dataset_path_input);
    endwin();
}


