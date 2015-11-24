#define _BSD_SOURCE

#include "library.h"

int verify_cpf_on_database(char *token_cpf, char *name, char *cpf, char *phone)
{
	int line_number;
	FILE *file;
	file = fopen(PATH, "r");
	
	if (file == NULL) {
		perror("fopen: %s");
		exit(EXIT_FAILURE);
	}
	
	if (get_size(PATH) == 0) {
		fprintf(stderr, "O arquivo está vazio!\n");
		return EXIT_FAILURE;
	}
	
	if (token_cpf == NULL)
		return 2;
	
	line_number = 1;
	
	while(!feof(file)) {

		if (fscanf(file, "%" STRINGIFY(SIZE_NAME) "[^\n]\n", name) < 1)
			goto parse_error;
		line_number++;
		
		printf("LI NOME DO ARQUIVO: %s\n", name);
		
		strcat(name, "\n");

		if (fscanf(file, "%" STRINGIFY(SIZE_CPF) "[^\n]\n", cpf) < 1)
			goto parse_error;
		line_number++;
			
		if (fscanf(file, "%" STRINGIFY(SIZE_PHONE) "[^\n]\n", phone) < 1)
			goto parse_error;
		line_number++;

		if(strncmp(token_cpf, cpf, SIZE_CPF) == 0)
			return 1;
	}
	
	return 2;
	
	fclose(file);
	
parse_error:
	fclose(file);
	fprintf(stderr, "Erro na leitura do arquivo na linha %u!\n", line_number);
	return 3;

}
