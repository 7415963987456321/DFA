/* Includes */
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>

#include "parser.h"

/* Globals */
int token_id = 0;

// Linked list of tokens
struct token_t *token_head;
struct token_t *token_tail; // Keep track of tail

/* Forward declarations */
enum tokentype_t check_special_char(char c);
struct token_t *init_new_token();
void lex(FILE* dfa_file);

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
}



void print_token_list(void) {
    struct token_t *curr = token_head;

    while (curr != NULL) {
        print_token(curr);
        curr = curr->next;
    }
    free(curr);
}

void append_token_to_list(struct token_t *new) {
    if(token_head == NULL){
        token_head = new;
        token_tail = token_head;
        return;
    }

    // Append new
    token_tail->next = new;
    // Keep track of tail
    token_tail = token_tail->next;
}

// Lexer
// Reads text and returns token structs to the list of tokens.
void lex(FILE* dfa_file) {
    printf("æklajsdfælkajsdf\n");
    char line[PARSER_BUFSIZE];

    // Read lines
    while (fgets(line, PARSER_BUFSIZE, dfa_file) != NULL) {
        /* printf("\nLine: %s",line); // debug */
        tokenize_line(line);
    }
    fclose(dfa_file);

    /* Debug */
    print_token_list();
}

struct token_t *init_new_token(void){
    struct token_t *new_token = malloc(sizeof(struct token_t));
    return new_token;
}

void print_token(struct token_t *token){
    if(token->type == WHITESPACE) return;
    printf("TOKEN:----------\n");
    printf("ID:     %d \n", token->ID);
    printf("TYPE::  %s \n", token_names[token->type]);
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

// Parse and return states?
void parse(FILE* dfa_file) {
    lex(dfa_file);
}
