#ifndef UTILITY_H
#define UTILITY_H

//Enum para indicar qual o modo do programa usado
typedef enum program_mode{
    BATCH,
    INTERACTIVE
} PROGRAM_MODE;

// A função 'str_upper' recebe uma string 'str' e modifica essa mesma string de forma a ficar com as letras todas maiúsculas.
void str_upper(char *str);

// A função 'str_isAlpha' verifica se uma string é formada apenas por letras ou não.
// Devolve 1 caso a string seja inteiramente feita de letras ou 0 caso haja algum caracter que não é uma letra.
// Parâmetros: string 'str', comprimento da string 'len'
int str_isAlpha(char *str, int len);

#endif