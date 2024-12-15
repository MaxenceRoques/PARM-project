#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "token.h"
#include "lexer.h"
#include "parser.h"
#include "traductor.h"

void print_binary(int n, int bytes)
{
    if (bytes)
    {
        print_binary(n >> 1, bytes - 1);
        fprintf(stdout, "%d", n & 1);
    }
}

//

int main(int argc, char **argv)
{
    /* Reading */
    if (argc != 2)
    {
        fprintf(stderr, "Wrong number of argument.\n");
        exit(1);
    }

    FILE *src = NULL;
    if (!(src = fopen(*(++argv), "r")))
    {
        fprintf(stderr, "The file %s does not exists.\n", *argv);
        exit(1);
    }

    struct token **tokens = NULL;
    struct token **symbols = NULL;
    if (lexer(src, &tokens, &symbols))
    {
        exit(1);
    }
    fclose(src);

    print_token_array(stdout, tokens);

    fprintf(stdout, "\n");
    print_token_array(stdout, symbols);

    struct token ***directives = NULL;
    if (parser(tokens, &directives))
    {
        exit(1);
    }

    fprintf(stdout, "\n[\n");
    for (int i = 0; *(directives + i); i++)
    {
        fprintf(stdout, "\t");
        print_token_array(stdout, *(directives + i));
    }
    fprintf(stdout, "]\n");

    fprintf(stdout, "\n");
    print_token_array(stdout, symbols);

    int *binary_directives = NULL;
    int size = 0;

    if (traductor(directives, symbols, &binary_directives, &size))
    {
        exit(1);
    }

    fprintf(stdout, "\n[ ");
    for (int i = 0; i < size; i++)
    {
        print_binary(*(binary_directives + i), 16);
        fprintf(stdout, " ");
    }
    fprintf(stdout, "]\n");

    free_token_array(&tokens);
    for (int i = 0; *(directives + i); i++)
    {
        free(*(directives + i));
    }
    free(directives);
    free(symbols);

    /* Writing */
    FILE *dest = NULL;

    char *file_base = strrchr(*argv, '/');
    int len_name = strlen(file_base);
    char file_name[len_name + 12];
    strcpy(file_name, "generated");
    strcat(file_name, file_base);
    file_name[len_name + 8] = '\0';
    strcat(file_name, "bin");

    if (!(dest = fopen(file_name, "w")))
    {
        fprintf(stderr, "Error when creating the file.\n");
        exit(1);
    }
    fputs("v2.0 raw\n", dest);

    for (int i = 0; i < size; i++)
    {
        fprintf(dest, "%04x ", *(binary_directives + i));
    }
    fputc('\n', dest);

    fclose(dest);

    return 0;
}