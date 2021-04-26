/* Includes */
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <errno.h>

/* Constants */
#define BUFSIZE 256
#define MAX_NAME_LENGTH 64

/* Globals */
char* file = NULL;
int token_id = 0;
struct token_t *tail; // Keep track of tail
struct token_t *head;

/* Forward declarations */
enum tokentype_t check_special_char(char c);
struct token_t *init_new_token();

void print_token(struct token_t *token);
void print_token_list(void);
void append_token_to_list(struct token_t *new);

enum tokentype_t{
    WHITESPACE,
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

void tokenize_line(char line[]) {
    struct token_t *new_token;
    enum tokentype_t tt;
    int l = 0;

    // Read chars
    for(int i = 0; line[i] != '\0'; i++) {
        tt = check_special_char(line[i]);
        new_token = init_new_token();

        switch (tt) {
            case WHITESPACE:
                break;

            case COMMENT_MARKER:
                return; //return?

            /* fallthrough */
            case ACCEPT_MARKER:
            case SEPARATOR:
            case END:
            case ESCAPE:
                new_token->ID = token_id;
                new_token->type = tt;
                strncpy(new_token->token_string, &line[i], 1);
                break;

            case NAME:
                new_token->ID = token_id;
                new_token->type = tt;
                for(l = i; check_special_char(line[l]) == NAME; l++ ){
                    /* printf("name: %c\n", line[l]); */
                    strncat(new_token->token_string, &line[l], 1);
                }
                i = --l;
                break;

            default:
                break;
        }

        token_id++;
        append_token_to_list(new_token);
    }

    print_token_list();
    return;
}

void print_token_list(void) {
    struct token_t *curr = head;

    for (int i = 0; curr != NULL; i++) {
        print_token(curr);
        curr = curr->next;
    }
    free(curr);
}

void append_token_to_list(struct token_t *new) {
    /* struct token_t *head = root; */

    print_token(new);
    if(head == NULL){
        printf("ALLOCDEBUG\n");
        head = new;
        tail = head;
        return;
    }

    // Append new
    tail->next = new;
    // Keep track of tail
    tail = tail->next;
}

// Lexer
// Reads text and returns token structs to the list of tokens.
void lex(void) {
    char line[BUFSIZE];
    FILE* dfa_file = fopen(file, "r");

    if (!dfa_file) {
        // Move this elsewhere
        fprintf(stderr, "%s: %s\n", file, strerror(errno));
        exit(1);
    }

    // Read lines
    while (fgets(line, BUFSIZE, dfa_file) != NULL) {
        printf("\nLine: %s",line); // debug
        tokenize_line(line);
    }
    fclose(dfa_file);
}

struct token_t *init_new_token(void){
    struct token_t *new_token = malloc(sizeof(struct token_t));
    return new_token;
}

void print_token(struct token_t *token){
    if(token->type == WHITESPACE) return;
    const char s[][16] = {
        "WHITESPACE",
        "NAME",
        "END",
        "SEPARATOR",
        "COMMENT_MARKER",
        "ACCEPT_MARKER",
        "ESCAPE",
    };
    printf("TOKEN:----------\n");
    printf("ID:     %d \n", token->ID);
    printf("TYPE::  %s \n", s[token->type]);
    printf("STRING: %s \n", token->token_string);
    printf("----------\n");
}


enum tokentype_t check_special_char(char c){
    switch (c) {
        case ' ':  // Space           (SPC)
        case '\t': // Horizontal tab  (TAB)
        case '\n': // Newline         (LF)
        case '\v': // Vertical tab    (VT)
        case '\f': // Feed            (FF)
        case '\r': // Carriage return (CR)
            return WHITESPACE;
        case '#':
            return COMMENT_MARKER;
        case '$':
            return ACCEPT_MARKER;
        case ',':
            return SEPARATOR;
        case ';':
            return END;
        case '\\':
            return ESCAPE;

        default:
            return NAME;
    }
}

// Remove later
int main(int argc, char *argv[]) {
    char c;

    while ((c = getopt(argc, argv, "f:s:h")) != -1) {
        switch (c) {
            case 'f':
                file = optarg;
                break;
        }
    }

    if(file == NULL) {
        exit(0);
    }
    lex();
    return 0;
}
