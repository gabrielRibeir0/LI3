#ifndef OUTPUT_H
#define OUTPUT_H

// Função para adicionar uma linha de uma entidade com erros ao respetivo ficheiro de erros
int addToErrorsFile(char *filepath, char *header, char *line, int header_written);

// Função para adicionar os campos de um output de um comando ao respetivo ficheiro
int addToOutputFile(char **fields, int fields_num, char input_type, int query_id, int flag, int cmd_number, int line_number);

#endif