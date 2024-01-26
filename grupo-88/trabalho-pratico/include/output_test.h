#ifndef OUTPUT_TEST_H
#define OUTPUT_TEST_H

// Função para imprimir o tempo total e o máximo de memória utilizada
void outputResources(double total_time, long int max_memory);

// Função para imprimir os tempos de duração das queries
void outputQueriesTime(double times[10]);

// Função para comparar os ficheiros de resultados obtidos com as soluções esperadas
void compareOutputFiles(const char* outputs_filepath, const char *input_filepath);

#endif