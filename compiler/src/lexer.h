#ifndef LEXER
#define LEXER

/**
 * Convert a lexeme into an allocated token.
 */
struct token *tokenize(char *lexeme);

//

/**
 * Convert a text into an allocated array of token.
 */
int lexer(FILE *src, struct token ***tokens, struct token ***symbols);

#endif