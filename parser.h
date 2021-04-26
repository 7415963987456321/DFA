#ifndef  _PARSER_H
#define _PARSER_H

/* Constants */
#define MAX_NAME_LENGTH 64
#define BUFSIZE         256

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

void print_token(struct token_t *token);
void print_token_list(void);
void append_token_to_list(struct token_t *new);


#endif
