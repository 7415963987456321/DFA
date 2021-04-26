#ifndef  PARSER_H
#define PARSER_H

/* Includes */
#include <stdio.h>

/* Constants */
#define MAX_NAME_LENGTH 64
#define PARSER_BUFSIZE  256

/* Names of token types */
static const char token_names[][16] = {
    "WHITESPACE",
    "NAME",
    "END",
    "SEPARATOR",
    "comment_marKER",
    "ACCEPT_MARKER",
    "ESCAPE",
};

enum tokentype_t{
    WHITESPACE,     // 
    NAME,           // Some string
    END,            // ';'
    SEPARATOR,      // ','
    COMMENT_MARKER, // '#'
    ACCEPT_MARKER,  // '$'
    ESCAPE,         // '\'
};

struct token_t {
    int ID;
    enum tokentype_t type;
    char token_string[MAX_NAME_LENGTH];
    struct token_t *next;   // Linked list of tokens
};

void parse(FILE* dfa_file);
void print_token(struct token_t *token);
void print_token_list(void);
void append_token_to_list(struct token_t *new);

#endif
