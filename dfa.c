#define BUFSIZE 1000
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>


/* Forward declaration */
void print_transition();
void print_states();

char* file = NULL;

/* Current state in execution - Or is it? */
struct state_t *current_state;

struct transition_func_t {
    char input;                             /* on what input to transition */
    struct state_t *next;                   /* what state to transition to */
};

struct state_t {
    uint16_t number;                        /* number of this state */
    uint16_t number_out;                    /* number of outgoing arrows */
    struct transition_func_t *trns;         /* transition function */
};

struct state_t *init_states(uint16_t number_of_states) {
    struct state_t *start_state 
        = malloc(sizeof(struct state_t) * number_of_states);

    for(int i = 0; i < number_of_states; i++) {
        start_state[i].number = i;
            start_state[i].number_out = 0;
    }

    return start_state;
}

struct transition_func_t *init_transition(char input, struct state_t *next){
    struct transition_func_t *new
        = malloc(sizeof(struct transition_func_t));
    
    new->input = input;
    new->next = next;
    return new;
}

void set_transition(int q, int a, char tns) {
    struct transition_func_t *trans = init_transition(tns, &current_state[a]);

    int trans_index = current_state[q].number_out;
    if(trans_index == 0) {
        current_state[q].trns = trans;
    } else{
        struct transition_func_t *new 
            = realloc(current_state[q].trns, ( ((current_state[q].number_out) + 1) * sizeof(struct transition_func_t)) );
        if(new == NULL || trans_index < 0) {
            printf("Error with memory allocation!");
            exit(0);
        }

        current_state[q].trns = new;
        current_state[q].trns[trans_index] = *trans;

        free(trans);
    }

        current_state[q].number_out++;

        print_states(&current_state[q], 1);
        print_transition(current_state[q].trns, current_state[q].number_out);
        return;
}

void print_states(struct state_t *state, int number_of_states){
    for (int i = 0; i < number_of_states; i++) {
        printf("State q%d \
                number of outgoing states: %d \
                \n", state[i].number, state[i].number_out);
    }
}

void print_transition(struct transition_func_t *trans, int number_of_trans) {
    for (int i = 0; i < number_of_trans; i++) {
        printf("On char: %c \
                transition to: q%d \
                \n", trans[i].input, trans[i].next->number);
    }
}

void free_states() {
    free(current_state->trns);
    free(current_state);
}

void set_state(struct state_t *state){
    /* change to transition func later*/
    current_state = state;
}

void parse_file() {
    char buf[BUFSIZE];
    FILE* dfa_file = fopen(file, "r");
    char tns;
    uint16_t q = 0;
    uint16_t a = 0;

    if (!dfa_file) {
        fprintf(stderr, "%s: %s\n", file, strerror(errno));
        exit(1);
    }

    while (fgets(buf, BUFSIZE, dfa_file) != NULL) {
        if(buf[1] == '\0' || buf[1] == '\n'){
            continue;
        }

        /* dumbass parser */
        /* fix this shit */
        q = atoi(&buf[0]);
        tns = buf[1];
        a = atoi(&buf[2]);

        set_transition(q,a,tns);

        printf("q: %d\n", q);
        printf("trns: %c\n" ,tns);
        printf("a: %d\n\n", a);
    }

    fclose(dfa_file);
}

void display_help() {
    printf("HELP!");
}

int main(int argc, char *argv[]) {
    char c;
    uint16_t number_of_states = 0;

    while ((c = getopt(argc, argv, "f:s:h")) != -1) {
        switch (c) {
            case 'f': 
                file = optarg;
                break;
            case 's': 
                number_of_states = atoi(optarg);
                break;
            case 'h':
                display_help();
                exit(0);
        }
    }

    if(number_of_states == 0 || file == NULL) {
        exit(0);
    }

    /* Set up structure */
    struct state_t *start = init_states(number_of_states);
    set_state(start);
    parse_file();
    /* Compute */

    /* Cleanup */
    free_states();
    return 0;
}
