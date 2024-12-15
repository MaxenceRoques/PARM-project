#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include "traductor.h"

void print_binary2(int n, int bytes)
{
    if (bytes)
    {
        print_binary2(n >> 1, bytes - 1);
        fprintf(stderr, "%d", n & 1);
    }
}

int traductor(struct token ***directives, struct token **symbols, int **binary_directives, int *size)
{
    for (int i = 0; *(directives + i); i++)
    {
        (*size)++;
    }
    // fprintf(stderr, "%d", *size);
    *binary_directives = malloc((*size) * sizeof(int));

    int code = 0;
    for (int i = 0; i < *size; i++)
    {
        *(*binary_directives + i) = 0b0000000000000000;
        switch ((**(directives + i))->type)
        {
        case INSTRUCTION:
            switch ((**(directives + i))->value.instruction.grammar)
            {
            case REGISTER_REGISTER_IMMEDIATE:
                switch ((**(directives + i))->value.instruction.value)
                {
                case LSLS:
                    *(*binary_directives + i) |= 0b00000 << 11;

                    if (((*(*(directives + i) + 3))->value.immediate < 0) || (32 < (*(*(directives + i) + 3))->value.immediate))
                    {
                        fprintf(stderr, "Semantical error at line %d with ", (*(*(directives + i) + 3))->line);
                        print_token(stderr, **(*(directives + i) + 3));
                        fprintf(stderr, "\n");
                        code = 1;
                    }
                    *(*binary_directives + i) |= ((*(*(directives + i) + 3))->value.immediate) << 6;
                    *(*binary_directives + i) |= ((int)(*(*(directives + i) + 2))->value._register) << 3;
                    *(*binary_directives + i) |= (int)(*(*(directives + i) + 1))->value._register;

                    break;

                case LSRS:
                    *(*binary_directives + i) |= 0b00001 << 11;

                    if (((*(*(directives + i) + 3))->value.immediate < 0) || (32 < (*(*(directives + i) + 3))->value.immediate))
                    {
                        fprintf(stderr, "Semantical error at line %d with ", (*(*(directives + i) + 3))->line);
                        print_token(stderr, **(*(directives + i) + 3));
                        fprintf(stderr, "\n");
                        code = 1;
                    }
                    *(*binary_directives + i) |= ((*(*(directives + i) + 3))->value.immediate) << 6;
                    *(*binary_directives + i) |= ((int)(*(*(directives + i) + 2))->value._register) << 3;
                    *(*binary_directives + i) |= (int)(*(*(directives + i) + 1))->value._register;
                    break;

                case ASRS:
                    *(*binary_directives + i) |= 0b00010 << 11;

                    if (((*(*(directives + i) + 3))->value.immediate < 0) || (32 < (*(*(directives + i) + 3))->value.immediate))
                    {
                        fprintf(stderr, "Semantical error at line %d with ", (*(*(directives + i) + 3))->line);
                        print_token(stderr, **(*(directives + i) + 3));
                        fprintf(stderr, "\n");
                        code = 1;
                    }
                    *(*binary_directives + i) |= ((*(*(directives + i) + 3))->value.immediate) << 6;
                    *(*binary_directives + i) |= ((int)(*(*(directives + i) + 2))->value._register) << 3;
                    *(*binary_directives + i) |= (int)(*(*(directives + i) + 1))->value._register;
                    break;

                case ADDS:
                    *(*binary_directives + i) |= 0b0001110 << 9;

                    if (((*(*(directives + i) + 3))->value.immediate < 0) || (8 < (*(*(directives + i) + 3))->value.immediate))
                    {
                        fprintf(stderr, "Semantical error at line %d with ", (*(*(directives + i) + 3))->line);
                        print_token(stderr, **(*(directives + i) + 3));
                        fprintf(stderr, "\n");
                        code = 1;
                    }
                    *(*binary_directives + i) |= ((*(*(directives + i) + 3))->value.immediate) << 6;
                    *(*binary_directives + i) |= ((int)(*(*(directives + i) + 2))->value._register) << 3;
                    *(*binary_directives + i) |= (int)(*(*(directives + i) + 1))->value._register;
                    break;

                case SUBS:
                    *(*binary_directives + i) |= 0b0001111 << 9;

                    if (((*(*(directives + i) + 3))->value.immediate < 0) || (8 < (*(*(directives + i) + 3))->value.immediate))
                    {
                        fprintf(stderr, "Semantical error at line %d with ", (*(*(directives + i) + 3))->line);
                        print_token(stderr, **(*(directives + i) + 3));
                        fprintf(stderr, "\n");
                        code = 1;
                    }
                    *(*binary_directives + i) |= ((*(*(directives + i) + 3))->value.immediate) << 6;
                    *(*binary_directives + i) |= ((int)(*(*(directives + i) + 2))->value._register) << 3;
                    *(*binary_directives + i) |= (int)(*(*(directives + i) + 1))->value._register;
                    break;

                case RSBS:
                    *(*binary_directives + i) = 0b0100001001 << 6;

                    if ((*(*(directives + i) + 3))->value.immediate != 0)
                    {
                        fprintf(stderr, "Semantical error at line %d with ", (*(*(directives + i) + 3))->line);
                        print_token(stderr, **(*(directives + i) + 3));
                        fprintf(stderr, "\n");
                        code = 1;
                    }
                    *(*binary_directives + i) |= ((int)(*(*(directives + i) + 2))->value._register) << 3;
                    *(*binary_directives + i) |= (int)(*(*(directives + i) + 1))->value._register;
                    break;
                }
                break;

            case REGISTER_REGISTER_REGISTER:
                switch ((**(directives + i))->value.instruction.value)
                {
                case ADDS:
                    *(*binary_directives + i) |= 0b0001100 << 9;
                    *(*binary_directives + i) |= ((int)(*(*(directives + i) + 3))->value._register) << 6;
                    *(*binary_directives + i) |= ((int)(*(*(directives + i) + 2))->value._register) << 3;
                    *(*binary_directives + i) |= (int)(*(*(directives + i) + 1))->value._register;
                    break;

                case SUBS:
                    *(*binary_directives + i) |= 0b0001101 << 9;
                    *(*binary_directives + i) |= ((int)(*(*(directives + i) + 3))->value._register) << 6;
                    *(*binary_directives + i) |= ((int)(*(*(directives + i) + 2))->value._register) << 3;
                    *(*binary_directives + i) |= (int)(*(*(directives + i) + 1))->value._register;
                    break;

                case MULS:
                    *(*binary_directives + i) |= 0b0100001101 << 6;
                    *(*binary_directives + i) |= ((int)(*(*(directives + i) + 2))->value._register) << 3;
                    *(*binary_directives + i) |= (int)(*(*(directives + i) + 1))->value._register;
                    break;
                }
                break;

            case REGISTER_IMMEDIATE:
                switch ((**(directives + i))->value.instruction.value)
                {
                case MOVS:
                    *(*binary_directives + i) |= 0b00100 << 11;
                    break;

                case CMP:
                    *(*binary_directives + i) |= 0b00101 << 11;
                    break;

                case ADDS:
                    *(*binary_directives + i) |= 0b00110 << 11;
                    break;

                case SUBS:
                    *(*binary_directives + i) |= 0b00111 << 11;
                    break;
                }

                *(*binary_directives + i) |= ((int)(*(*(directives + i) + 1))->value._register) << 8;
                if (((*(*(directives + i) + 2))->value.immediate < 0) || (256 < (*(*(directives + i) + 2))->value.immediate))
                {
                    fprintf(stderr, "Semantical error at line %d with ", (*(*(directives + i) + 2))->line);
                    print_token(stderr, **(*(directives + i) + 2));
                    fprintf(stderr, "\n");
                    code = 1;
                }
                *(*binary_directives + i) |= (*(*(directives + i) + 2))->value.immediate;
                break;

            case REGISTER_REGISTER:
                *(*binary_directives + i) |= 0b010000 << 10;
                switch ((**(directives + i))->value.instruction.value)
                {

                case ANDS:
                    *(*binary_directives + i) |= 0b0000 << 6;
                    break;

                case EORS:
                    *(*binary_directives + i) |= 0b0001 << 6;
                    break;

                case LSLS:
                    *(*binary_directives + i) |= 0b0010 << 6;
                    break;

                case LSRS:
                    *(*binary_directives + i) |= 0b0011 << 6;
                    break;

                case ASRS:
                    *(*binary_directives + i) |= 0b0100 << 6;
                    break;

                case ADCS:
                    *(*binary_directives + i) |= 0b0101 << 6;
                    break;

                case SBCS:
                    *(*binary_directives + i) |= 0b0110 << 6;
                    break;

                case RORS:
                    *(*binary_directives + i) |= 0b0111 << 6;
                    break;

                case TST:
                    *(*binary_directives + i) |= 0b1000 << 6;
                    break;
                    //

                case CMP:
                    *(*binary_directives + i) |= 0b1010 << 6;
                    break;

                case CMN:
                    *(*binary_directives + i) |= 0b1011 << 6;
                    break;

                case ORRS:
                    *(*binary_directives + i) |= 0b1100 << 6;
                    break;
                    //

                case BICS:
                    *(*binary_directives + i) |= 0b1110 << 6;
                    break;

                case MVNS:
                    *(*binary_directives + i) |= 0b1111 << 6;
                    break;
                }

                *(*binary_directives + i) |= ((int)(*(*(directives + i) + 2))->value._register) << 3;
                *(*binary_directives + i) |= (int)(*(*(directives + i) + 1))->value._register;
                break;

            case REGISTER_ADDRESS_IMMEDIATE:
                switch ((**(directives + i))->value.instruction.value)
                {
                case STR:
                    *(*binary_directives + i) |= 0b10010 << 11;
                    break;

                case LDR:
                    *(*binary_directives + i) |= 0b10011 << 11;
                    break;
                }

                *(*binary_directives + i) |= ((int)(*(*(directives + i) + 1))->value._register) << 8;
                if ((((*(*(directives + i) + 3))->value.immediate) / 4 < 0) || (256 < ((*(*(directives + i) + 3))->value.immediate) / 4))
                {
                    fprintf(stderr, "Semantical error at line %d with ", (*(*(directives + i) + 3))->line);
                    print_token(stderr, **(*(directives + i) + 3));
                    fprintf(stderr, "\n");
                    code = 1;
                }
                *(*binary_directives + i) |= ((*(*(directives + i) + 3))->value.immediate) / 4;
                break;

            case REGISTER_ADDRESS:
                *(*binary_directives + i) |= 0b10011 << 11; // LDR
                *(*binary_directives + i) |= ((int)(*(*(directives + i) + 1))->value._register) << 8;
                break;

            case STACK_STACK_IMMEDIATE:
                switch ((**(directives + i))->value.instruction.value)
                {
                case ADD:
                    *(*binary_directives + i) |= 0b101100000 << 7;
                    break;

                case SUB:
                    *(*binary_directives + i) |= 0b101100001 << 7;
                    break;
                }
                if ((((*(*(directives + i) + 3))->value.immediate) / 4 < 0) || (128 < ((*(*(directives + i) + 3))->value.immediate / 4)))
                {
                    fprintf(stderr, "Semantical error at line %d with ", (*(*(directives + i) + 3))->line);
                    print_token(stderr, **(*(directives + i) + 3));
                    fprintf(stderr, "\n");
                    code = 1;
                }
                *(*binary_directives + i) |= ((*(*(directives + i) + 3))->value.immediate) / 4;
                break;

            case STACK_IMMEDIATE:
                switch ((**(directives + i))->value.instruction.value)
                {
                case ADD:
                    *(*binary_directives + i) |= 0b101100000 << 7;
                    break;
                case SUB:
                    *(*binary_directives + i) |= 0b101100001 << 7;
                    break;
                }
                if ((((*(*(directives + i) + 2))->value.immediate) / 4 < 0) || (128 < ((*(*(directives + i) + 2))->value.immediate) / 4))
                {
                    fprintf(stderr, "Semantical error at line %d with ", (*(*(directives + i) + 3))->line);
                    print_token(stderr, **(*(directives + i) + 2));
                    fprintf(stderr, "\n");
                    code = 1;
                }
                *(*binary_directives + i) |= ((*(*(directives + i) + 2))->value.immediate) / 4;
                break;
            }
            break;

        case BRANCH:
            // Vérification des labels
            bool label_exists = false;
            for (int j = 0; *(symbols + j); j++)
            {

                if (strcmp((*(*(directives + i) + 1))->value.label.name, (*(symbols + j))->value.label.name) == 0)
                {
                    label_exists = true;
                    int offset = (*(symbols + j))->value.label.address - (*(*(directives + i) + 1))->value.label.address - 3;
                    if (((**(directives + i))->value.branch != B) && (-128 <= offset) && (offset <= 127))
                    {
                        *(*binary_directives + i) = 0b1101 << 12;
                        *(*binary_directives + i) |= 0b0000000011111111 & offset;
                    }
                    else if (((**(directives + i))->value.branch == B) && (-1024 <= offset) && (offset <= 1023))
                    {
                        *(*binary_directives + i) |= 0b11100 << 11;
                        *(*binary_directives + i) |= offset & 0b0000011111111111;
                    }
                    else
                    {
                        label_exists = false;
                    }
                }
            }
            if (!label_exists)
            {
                fprintf(stderr, "Semantical error at line %d with ", (*(*(directives + i) + 1))->line);
                print_token(stderr, **(*(directives + i) + 1));
                fprintf(stderr, "\n");
                code = 1;
            }
            label_exists = false;

            switch ((**(directives + i))->value.branch)
            {
            case BEQ:
                *(*binary_directives + i) |= 0b0000 << 8;
                break;
            case BNE:
                *(*binary_directives + i) |= 0b0001 << 8;
                break;
            case BCS:
                *(*binary_directives + i) |= 0b0010 << 8;
                break;
            case BCC:
                *(*binary_directives + i) |= 0b0011 << 8;
                break;
            case BMI:
                *(*binary_directives + i) |= 0b0100 << 8;
                break;
            case BPL:
                *(*binary_directives + i) |= 0b0101 << 8;
                break;
            case BVS:
                *(*binary_directives + i) |= 0b0110 << 8;
                break;
            case BVC:
                *(*binary_directives + i) |= 0b0111 << 8;
                break;
            case BHI:
                *(*binary_directives + i) |= 0b1000 << 8;
                break;
            case BLS:
                *(*binary_directives + i) |= 0b1001 << 8;
                break;
            case BGE:
                *(*binary_directives + i) |= 0b1010 << 8;
                break;
            case BLT:
                *(*binary_directives + i) |= 0b1011 << 8;
                break;
            case BGT:
                *(*binary_directives + i) |= 0b1100 << 8;
                break;
            case BLE:
                *(*binary_directives + i) |= 0b1101 << 8;
                break;
            case BAL:
                *(*binary_directives + i) |= 0b1110 << 8;
                break;
            case BIT:
                *(*binary_directives + i) |= 0b1111 << 8;
                break;
            }
            break;
        }
    }

    return code;
}