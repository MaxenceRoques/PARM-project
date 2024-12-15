#ifndef TOKEN
#define TOKEN

enum token_type
{
    SEPARATOR,
    ADDRESS,
    INSTRUCTION,
    REGISTER,
    IMMEDIATE,
    STACK_POINTER,
    BRANCH,
    LABEL,
    UNDEFINED
};

//

enum address
{
    OPEN,
    CLOSE
};

//

enum grammar
{
    REGISTER_REGISTER_IMMEDIATE,
    REGISTER_REGISTER_REGISTER,
    REGISTER_IMMEDIATE,
    REGISTER_REGISTER,
    REGISTER_ADDRESS_IMMEDIATE,
    REGISTER_ADDRESS,
    STACK_STACK_IMMEDIATE,
    STACK_IMMEDIATE,
    UNRECOGNISED
};

extern char string_grammar[9][4];

enum instruction_value
{
    LSLS,
    LSRS,
    ASRS,
    ADDS,
    SUBS,
    MOVS,
    ANDS,
    EORS,
    ADCS,
    SBCS,
    RORS,
    TST,
    RSBS,
    CMP,
    CMN,
    ORRS,
    MULS,
    BICS,
    MVNS,
    STR,
    LDR,
    ADD,
    SUB
};

extern char string_instruction[23][5];

struct instruction
{
    enum instruction_value value;
    enum grammar grammar;
};

//

enum _register
{
    R0,
    R1,
    R2,
    R3,
    R4,
    R5,
    R6,
    R7
};

extern char string_register[8][3];

//

enum branch
{
    BEQ,
    BNE,
    BCS, // = BHS
    BCC, // = BLO
    BMI,
    BPL,
    BVS,
    BVC,
    BHI,
    BLS,
    BGE,
    BLT,
    BGT,
    BLE,
    BAL,
    BIT,
    B
};

extern char string_branch[17][4];

//

enum label_type
{
    DEFINITION,
    REFERENCE
};

struct label
{
    enum label_type type;
    char *name;
    int address;
};

//

union token_value
{
    // void separator;
    enum address address;
    struct instruction instruction;
    enum _register _register;
    int immediate;
    // void stack_pointer;
    enum branch branch;
    struct label label;
};

struct token
{
    enum token_type type;
    union token_value value;
    int line;
};

void print_token(FILE *stream, struct token token);

void print_token_array(FILE *stream, struct token **tab);

void free_token(struct token *token);

void free_token_array(struct token ***tab);

#endif