#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "token.h"
#include "parser.h"

// void print_operation(struct token **tab, int end)
// {
//     for (int i = 0; *(tab + i) && (i <= end); i++)
//     {
//         print_token(stderr, **(tab + i));
//     }
// }

//

void array_copy(struct token ***dst, struct token ***src, int end)
{
    *dst = malloc((end + 2) * sizeof(struct token *));
    int j = 0;
    for (int i = 0; *(*src + i + j) && (i + j <= end); i++)
    {
        /* Ignoring separator and address */
        if (((*(*src + i + j))->type == SEPARATOR) || ((*(*src + i + j))->type == ADDRESS))
        {
            j++;
            i--;
            continue;
        }
        *(*dst + i) = *(*src + i + j);
    }
    *dst = realloc(*dst, (end + 2 - j) * sizeof(struct token *));
    *(*dst + end + 1 - j) = NULL;
}

//

bool rule_register_register_immediate(struct token **tab, int *index, struct token ****directives, int *size)
{
    bool res = true;

    res = res && (*(tab + 1)) && ((*(tab + 1))->type == REGISTER);
    res = res && (*(tab + 2)) && ((*(tab + 2))->type == SEPARATOR);
    res = res && (*(tab + 3)) && ((*(tab + 3))->type == REGISTER);
    res = res && (*(tab + 4)) && ((*(tab + 4))->type == SEPARATOR);
    res = res && (*(tab + 5)) && ((*(tab + 5))->type == IMMEDIATE);

    if (res)
    {
        (*tab)->value.instruction.grammar = REGISTER_REGISTER_IMMEDIATE;
        *directives = realloc(*directives, (*size + 1) * sizeof(struct token **));
        array_copy(*directives + ((*size)++), &tab, 5);
        *index += 5;
    }

    return res;
}

//

bool rule_register_register_register(struct token **tab, int *index, struct token ****directives, int *size)
{
    bool res = true;

    res = res && (*(tab + 1)) && ((*(tab + 1))->type == REGISTER);
    res = res && (*(tab + 2)) && ((*(tab + 2))->type == SEPARATOR);
    res = res && (*(tab + 3)) && ((*(tab + 3))->type == REGISTER);
    res = res && (*(tab + 4)) && ((*(tab + 4))->type == SEPARATOR);
    res = res && (*(tab + 5)) && ((*(tab + 5))->type == REGISTER);

    if (res)
    {
        (*tab)->value.instruction.grammar = REGISTER_REGISTER_REGISTER;
        *directives = realloc(*directives, (*size + 1) * sizeof(struct token **));
        array_copy(*directives + ((*size)++), &tab, 5);
        *index += 5;
    }

    return res;
}

//

bool rule_register_immediate(struct token **tab, int *index, struct token ****directives, int *size)
{
    bool res = true;

    res = res && (*(tab + 1)) && ((*(tab + 1))->type == REGISTER);
    res = res && (*(tab + 2)) && ((*(tab + 2))->type == SEPARATOR);
    res = res && (*(tab + 3)) && ((*(tab + 3))->type == IMMEDIATE);

    if (res)
    {
        (*tab)->value.instruction.grammar = REGISTER_IMMEDIATE;
        *directives = realloc(*directives, (*size + 1) * sizeof(struct token **));
        array_copy(*directives + ((*size)++), &tab, 3);
        *index += 3;
    }

    return res;
}

//

bool rule_register_register(struct token **tab, int *index, struct token ****directives, int *size)
{
    bool res = true;

    res = res && (*(tab + 1)) && ((*(tab + 1))->type == REGISTER);
    res = res && (*(tab + 2)) && ((*(tab + 2))->type == SEPARATOR);
    res = res && (*(tab + 3)) && ((*(tab + 3))->type == REGISTER);

    if (res)
    {
        (*tab)->value.instruction.grammar = REGISTER_REGISTER;
        *directives = realloc(*directives, (*size + 1) * sizeof(struct token **));
        array_copy(*directives + ((*size)++), &tab, 3);
        *index += 3;
    }

    return res;
}

//

bool rule_register_address_immediate(struct token **tab, int *index, struct token ****directives, int *size)
{
    bool res = true;

    res = res && (*(tab + 1)) && ((*(tab + 1))->type == REGISTER);
    res = res && (*(tab + 2)) && ((*(tab + 2))->type == SEPARATOR);
    res = res && (*(tab + 3)) && ((*(tab + 3))->type == ADDRESS) && ((*(tab + 3))->value.address == OPEN);
    res = res && (*(tab + 4)) && ((*(tab + 4))->type == STACK_POINTER);
    res = res && (*(tab + 5)) && ((*(tab + 5))->type == SEPARATOR);
    res = res && (*(tab + 6)) && ((*(tab + 6))->type == IMMEDIATE);
    res = res && (*(tab + 7)) && ((*(tab + 7))->type == ADDRESS) && ((*(tab + 7))->value.address == CLOSE);

    if (res)
    {
        (*tab)->value.instruction.grammar = REGISTER_ADDRESS_IMMEDIATE;
        *directives = realloc(*directives, (*size + 1) * sizeof(struct token **));
        array_copy(*directives + ((*size)++), &tab, 7);
        *index += 7;
    }

    return res;
}

//

bool rule_register_address(struct token **tab, int *index, struct token ****directives, int *size)
{
    bool res = true;

    res = res && (*(tab + 1)) && ((*(tab + 1))->type == REGISTER);
    res = res && (*(tab + 2)) && ((*(tab + 2))->type == SEPARATOR);
    res = res && (*(tab + 3)) && ((*(tab + 3))->type == ADDRESS) && ((*(tab + 3))->value.address == OPEN);
    res = res && (*(tab + 4)) && ((*(tab + 4))->type == STACK_POINTER);
    res = res && (*(tab + 5)) && ((*(tab + 5))->type == ADDRESS) && ((*(tab + 5))->value.address == CLOSE);

    if (res)
    {
        (*tab)->value.instruction.grammar = REGISTER_ADDRESS;
        *directives = realloc(*directives, (*size + 1) * sizeof(struct token **));
        array_copy(*directives + ((*size)++), &tab, 5);
        *index += 5;
    }

    return res;
}

//

bool rule_stack_stack_immediate(struct token **tab, int *index, struct token ****directives, int *size)
{
    bool res = true;

    res = res && (*(tab + 1)) && ((*(tab + 1))->type == STACK_POINTER);
    res = res && (*(tab + 2)) && ((*(tab + 2))->type == SEPARATOR);
    res = res && (*(tab + 3)) && ((*(tab + 3))->type == STACK_POINTER);
    res = res && (*(tab + 4)) && ((*(tab + 4))->type == SEPARATOR);
    res = res && (*(tab + 5)) && ((*(tab + 5))->type == IMMEDIATE);

    if (res)
    {
        (*tab)->value.instruction.grammar = STACK_STACK_IMMEDIATE;
        *directives = realloc(*directives, (*size + 1) * sizeof(struct token **));
        array_copy(*directives + ((*size)++), &tab, 5);
        *index += 5;
    }

    return res;
}

//

bool rule_stack_immediate(struct token **tab, int *index, struct token ****directives, int *size)
{
    bool res = true;

    res = res && (*(tab + 1)) && ((*(tab + 1))->type == STACK_POINTER);
    res = res && (*(tab + 2)) && ((*(tab + 2))->type == SEPARATOR);
    res = res && (*(tab + 3)) && ((*(tab + 3))->type == IMMEDIATE);

    if (res)
    {
        (*tab)->value.instruction.grammar = STACK_IMMEDIATE;
        *directives = realloc(*directives, (*size + 1) * sizeof(struct token **));
        array_copy(*directives + ((*size)++), &tab, 3);
        *index += 3;
    }

    return res;
}

//

bool rule_label(struct token **tab, int *index, struct token ****directives, int *size, int directive_number)
{
    bool res = true;

    res = res && (*(tab + 1)) && ((*(tab + 1))->type == LABEL) && ((*(tab + 1))->value.label.type == REFERENCE);

    if (res)
    {
        (*(tab + 1))->value.label.address = directive_number;
        *directives = realloc(*directives, (*size + 1) * sizeof(struct token **));
        array_copy(*directives + ((*size)++), &tab, 1);
        *index += 1;
    }

    return res;
}

//

int parser(struct token **tokens, struct token ****directives)
{
    int size = 0;
    bool is_recognised = false;
    int directive_number = 0;

    int code = 0;

    for (int i = 0; *(tokens + i); i++)
    {
        is_recognised = false;
        switch ((*(tokens + i))->type)
        {
        case INSTRUCTION:
            switch ((*(tokens + i))->value.instruction.value)
            {
            case LSLS: // same
            case LSRS: // same
            case ASRS: // same
                is_recognised = is_recognised || rule_register_register_immediate(tokens + i, &i, directives, &size);
                is_recognised = is_recognised || rule_register_register(tokens + i, &i, directives, &size);
                break;

            case ADDS: // same
            case SUBS:
                is_recognised = is_recognised || rule_register_immediate(tokens + i, &i, directives, &size);
                is_recognised = is_recognised || rule_register_register_register(tokens + i, &i, directives, &size);
                is_recognised = is_recognised || rule_register_register_immediate(tokens + i, &i, directives, &size);
                break;

            case CMP:
                is_recognised = is_recognised || rule_register_register(tokens + i, &i, directives, &size);
            case MOVS:
                is_recognised = is_recognised || rule_register_immediate(tokens + i, &i, directives, &size);
                break;

            case ANDS: // same
            case EORS: // same
            case ADCS: // same
            case SBCS: // same
            case RORS: // same
            case TST:  // same
            case CMN:  // same
            case ORRS: // same
            case BICS: // same
            case MVNS:
                is_recognised = is_recognised || rule_register_register(tokens + i, &i, directives, &size);
                break;

            case RSBS:
                is_recognised = is_recognised || rule_register_register_immediate(tokens + i, &i, directives, &size);
                break;

            case MULS:
                is_recognised = is_recognised || rule_register_register_register(tokens + i, &i, directives, &size);
                break;

            case STR:
                is_recognised = is_recognised || rule_register_address_immediate(tokens + i, &i, directives, &size);
                break;

            case LDR:
                is_recognised = is_recognised || rule_register_address_immediate(tokens + i, &i, directives, &size);
                is_recognised = is_recognised || rule_register_address(tokens + i, &i, directives, &size);
                break;

            case ADD: // Same
            case SUB:
                is_recognised = is_recognised || rule_stack_stack_immediate(tokens + i, &i, directives, &size);
                is_recognised = is_recognised || rule_stack_immediate(tokens + i, &i, directives, &size);
                break;
            }

            if (is_recognised)
            {
                directive_number++;
                break;
            }

        case BRANCH:
            if (rule_label(tokens + i, &i, directives, &size, directive_number))
            {
                directive_number++;
                break;
            }

        case LABEL:
            if ((*(tokens + i))->value.label.type == DEFINITION)
            {
                (*(tokens + i))->value.label.address = directive_number;
                break;
            }

        default:
            fprintf(stderr, "Syntax error at line %d with ", (*(tokens + i))->line);
            print_token(stderr, **(tokens + i));
            fprintf(stderr, "\n");
            code = 1;
            break;
        }
    }

    *directives = realloc(*directives, (size + 1) * sizeof(struct token **));
    *(*directives + (size++)) = NULL;

    return code;
}