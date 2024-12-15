#include <stdio.h>
#include <stdlib.h>
#include "token.h"

//

char string_grammar[9][4] = {"rri", "rrr", "ri", "rr", "rai", "ra", "ssi", "si", "?"};
char string_instruction[23][5] = {"lsls", "lsrs", "asrs", "adds", "subs", "movs", "ands", "eors", "adcs", "sbcs", "rors", "tst", "rsbs", "cmp", "cmn", "orrs", "muls", "bics", "mvns", "str", "ldr", "add", "sub"};
char string_register[8][3] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};
char string_branch[17][4] = {"bEQ", "bNE", "bCS", "bCC", "bMI", "bPL", "bVS", "bVC", "bHI", "bLS", "bGE", "bLT", "bGT", "bLE", "bAL", "bIT", "b"}; // CS = HS, CC = LO

//

void print_token(FILE *stream, struct token token)
{
    fprintf(stream, "(");
    switch (token.type)
    {
    case SEPARATOR:
        fprintf(stream, "SEPARATOR");
        break;

    case ADDRESS:
        fprintf(stream, "ADDRESS %s", (token.value.address == OPEN) ? "[" : "]");
        break;

    case INSTRUCTION:
        fprintf(stream, "INSTRUCTION %s %s", string_instruction[(int)(token.value.instruction.value)], string_grammar[(int)(token.value.instruction.grammar)]);
        break;

    case REGISTER:
        fprintf(stream, "REGISTER %s", string_register[(int)(token.value._register)]);
        break;

    case IMMEDIATE:
        fprintf(stream, "IMMEDIATE %d", token.value.immediate);
        break;

    case STACK_POINTER:
        fprintf(stream, "STACK_POINTER");
        break;

    case BRANCH:
        fprintf(stream, "BRANCH %s", string_branch[(int)(token.value.branch)]);
        break;

    case LABEL:
        fprintf(stream, "LABEL %s %s %d", (token.value.label.type == DEFINITION) ? "definition" : "reference", token.value.label.name, token.value.label.address);
        break;

    case UNDEFINED:
        fprintf(stream, "?");
        break;
    }
    fprintf(stream, ")");
}

//

void print_token_array(FILE *stream, struct token **tab)
{
    fprintf(stream, "[ ");
    for (int i = 0; *(tab + i); i++)
    {
        print_token(stream, **(tab + i));
        fprintf(stream, " ");
    }
    fprintf(stream, "]\n");
}

//

void free_token(struct token *token)
{
    if (token->type == LABEL)
    {
        free(token->value.label.name);
    }
    free(token);
}

//

void free_token_array(struct token ***tab)
{
    for (int i = 0; *(*tab + i); i++)
    {
        free_token(*(*tab + i));
    }
    free(*tab);
}