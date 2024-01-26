#ifndef VALIDATION_H
#define VALIDATION_H

// Função para verificar se várias partes numa string têm o comprimento desejado
// É passado o conjunto dos delimitadores que separam as várias partes, assim como os comprimentos esperados
int checkFieldsLen(char *field, char *delims, int *lengths, int n_fields);

// Função para verificar se uma string representa uma data válida
int validateDate(char *field, int has_time);

// Função para verificar se uma string tem comprimento maior que 0
int validatePositiveLen(char *field);

// Função para verificar se uma string representa um inteiro entre 1 e 5
int validateInt1to5(char *field);

// Função para verificar se uma string representa um rating válido (1 a 5)
int validateRating(char *field);

// Função para verificar se uma string representa um inteiro positivo
int validatePosInt(char *field);

// Função para verificar se uma string representa o campo 'includes_breakfast' válido
int validateInclBreakfast(char *field, char *inc_breakfast);

// Função para verificar se uma string representa um aeroporto válido
int validateAirport(char *field);

// Função para verificar se um email é válido
int validateEmail(char *field);

// Função para verificar se um 'Country Code' é válido
int validateCountryCode(char *field);

// Função para verificar se um estado é válido
int validateAccountStatus(char *field, char *status);
#endif