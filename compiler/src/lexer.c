#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "token.h"
#include "lexer.h"

//

struct token *tokenize(char *lexeme)
{
    struct token *token = malloc(sizeof(struct token));
    int length = strlen(lexeme);

    if (length == 1)
    {
        /* Separator */
        if (*lexeme == ',')
        {
            token->type = SEPARATOR;
            return token; // No value to stock
        }

        /* Address */
        else if ((*lexeme == '[') || (*lexeme == ']'))
        {
            token->type = ADDRESS;
            token->value.address = (*lexeme == '[') ? OPEN : CLOSE;
            return token;
        }

        /* Branch (simple) */
        if (*lexeme == 'b')
        {
            token->type = BRANCH;
            token->value.branch = B;
            return token;
        }

        token->type = UNDEFINED;
        return token;
    }

    if (length == 2)
    {
        /* Stack Pointer */
        if ((*lexeme == 's') && (*(lexeme + 1) == 'p'))
        {
            token->type = STACK_POINTER;
            return token; // No value to stock
        }

        /* Register */
        for (int i = 0; i < 8; i++)
        {
            if (strcmp(lexeme, string_register[i]) == 0)
            {
                token->type = REGISTER;
                token->value._register = (enum _register)(i);
                return token;
            }
        }

        /* Immediate (small) */
        if ((*lexeme == '#') && ('0' <= *(lexeme + 1)) && (*(lexeme + 1) <= '9'))
        {
            token->type = IMMEDIATE;
            token->value.immediate = *(lexeme + 1) - '0';
            return token;
        }

        /* Label (small) */
        if ((*lexeme == '.') && (*(lexeme + 1) != ':'))
        {
            token->type = LABEL;
            token->value.label.type = REFERENCE;
            token->value.label.name = malloc(2 * sizeof(char));
            *(token->value.label.name) = *(lexeme + 1);
            *(token->value.label.name + 1) = '\0';
            token->value.label.address = -1; // undefined
            return token;
        }

        token->type = UNDEFINED;
        return token;
    }

    /* Immediate */
    if (*lexeme == '#')
    {
        for (int i = 1; *(lexeme + i); i++)
        {
            if (!(('0' <= *(lexeme + i)) && (*(lexeme + i) <= '9')))
            {
                token->type = UNDEFINED;
                return token;
            }
        }
        token->type = IMMEDIATE;
        token->value.immediate = atoi(lexeme + 1);
        return token;
    }

    /* Label */
    if (*lexeme == '.')
    {
        token->type = LABEL;
        if (*(lexeme + length - 1) == ':')
        {
            token->value.label.type = DEFINITION;
            token->value.label.name = strncpy(malloc((length - 2) * sizeof(char)), lexeme + 1, length - 2);
            // end string
            *(token->value.label.name + length - 2) = '\0';
        }
        else
        {
            token->value.label.type = REFERENCE;
            token->value.label.name = strncpy(malloc((length - 1) * sizeof(char)), lexeme + 1, length - 1);
            // end string
            *(token->value.label.name + length - 1) = '\0';
        }
        token->value.label.address = -1; // undefined
        return token;
    }

    /* Branch */
    for (int i = 0; i < 16; i++)
    {
        if (strcmp(lexeme, string_branch[i]) == 0)
        {
            token->type = BRANCH;
            token->value.branch = (enum branch)(i);
            return token;
        }
    }
    if (strcmp(lexeme, "bHS") == 0)
    {
        token->type = BRANCH;
        token->value.branch = BCS; // Synonym of branch BCS
        return token;
    }
    if (strcmp(lexeme, "bLO") == 0)
    {
        token->type = BRANCH;
        token->value.branch = BCC; // Synonym of branch BCC
        return token;
    }

    /* Instruction */
    for (int i = 0; i < 23; i++)
    {
        if (strcmp(lexeme, string_instruction[i]) == 0)
        {
            token->type = INSTRUCTION;
            token->value.instruction.value = (enum instruction_value)(i);
            token->value.instruction.grammar = UNRECOGNISED;
            return token;
        }
    }

    token->type = UNDEFINED;
    return token;
}

//

int lexer(FILE *src, struct token ***tokens, struct token ***symbols)
{
    int allocated_size = 50;
    *tokens = malloc(allocated_size * sizeof(struct token *));
    int current_tokens_size = 0;

    int current_symbols_size = 0;

    int c;
    bool comment_line = false;
    int line = 1;
    long start_lexeme_position;
    long end_lexeme_position;
    char *lexeme = NULL;
    struct token *token = NULL;
    bool bad_first_character = false;

    int code = 0;

    while (true)
    {
        /* Ignoring comment line, space and line break */
        while (((c = fgetc(src)) != EOF) && ((c == '@') || (c <= ' ') || comment_line))
        {
            comment_line = (c == '@') ? true : ((c == '\n') ? false : comment_line);
            if (c == '\n')
            {
                line++;
            }
        }

        if (c == EOF)
        {
            break;
        }

        /* Reading the lexeme  */
        start_lexeme_position = ftell(src) - 1;
        bad_first_character = (c == ',') || (c == '[');

        while (((c = fgetc(src)) != EOF) && !bad_first_character && (c != ',') && (c != '[') && (c != ']') && (c > ' '))
        {
            if (c == '\n')
            {
                line++;
            }
        }

        end_lexeme_position = (c != EOF) ? ftell(src) : ftell(src) + 1;

        /* Copying the lexeme and tokenizying it */
        fseek(src, start_lexeme_position, SEEK_SET);

        lexeme = malloc((end_lexeme_position - start_lexeme_position) * sizeof(char));
        for (int i = 0; i < end_lexeme_position - start_lexeme_position - 1; i++)
        {
            *(lexeme + i) = fgetc(src);
        }
        *(lexeme + end_lexeme_position - start_lexeme_position - 1) = '\0';

        token = tokenize(lexeme);

        /* Send error */
        if (token->type == UNDEFINED)
        {
            fprintf(stderr, "Lexical error at line %d with \"%s\"\n", line, lexeme);
            code = 1;
        }
        token->line = line;
        free(lexeme);

        /* Building the symbol table */
        if ((token->type == LABEL) && (token->value.label.type == DEFINITION))
        {
            *symbols = realloc(*symbols, (current_symbols_size + 1) * sizeof(struct token *));
            *(*symbols + (current_symbols_size++)) = token;
        }

        /* Building the token table */
        if (current_tokens_size == allocated_size)
        {
            *tokens = realloc(*tokens, (allocated_size *= 2) * sizeof(struct token *));
        }
        *(*tokens + (current_tokens_size++)) = token;
    }

    if ((current_tokens_size + 1) != allocated_size)
    {
        *tokens = realloc(*tokens, (current_tokens_size + 1) * sizeof(struct token *));
    }
    *(*tokens + current_tokens_size) = NULL;

    *symbols = realloc(*symbols, (current_symbols_size + 1) * sizeof(char *));
    *(*symbols + (current_symbols_size++)) = NULL;

    return code;
}