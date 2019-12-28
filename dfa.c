#define BUFSIZE 1000
#define INPUT_ALPHABET_BUFSIZE 200

#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <errno.h>

/* Forward declaration */
void print_transition();
void print_states();

/* Globals */
char* file = NULL;
char* input_file = NULL;
struct state_t *states;

struct {
    char input_alphabet[INPUT_ALPHABET_BUFSIZE];
    enum {
        RUNNING,
        STOPPED
    } running_state;

} global_state;

enum state_type {
    NORMAL,
    ACCEPT,
};

struct transition_func_t {
    char input;                             /* on what input to transition */
    struct state_t *next;                   /* what state to transition to */
};

struct state_t {
    int number;                             /* number of this state */
    int number_out;                         /* number of outgoing arrows */
    enum state_type type;                   /* Normal or accept state */
    struct transition_func_t *trns;         /* transition function */
};

struct state_t *init_states(uint16_t number_of_states) {
    struct state_t *start_state 
        = malloc(sizeof(struct state_t) * number_of_states);

    for(int i = 0; i < number_of_states; i++) {
        start_state[i].type = NORMAL;
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

void set_transition(int q, int a, char tns, bool accept) {
    struct transition_func_t *trans = init_transition(tns, &states[a]);

    int trans_index = states[q].number_out;
    if(trans_index == 0) {
        states[q].trns = trans;
    } else{
        struct transition_func_t *new 
            = realloc(states[q].trns, ( ((states[q].number_out) + 1) * sizeof(struct transition_func_t)) );
        if(new == NULL || trans_index < 0) {
            printf("Error with memory allocation!");
            exit(0);
        }

        states[q].trns = new;
        states[q].trns[trans_index] = *trans;
        states[q].type = accept;

        free(trans);
    }

        states[q].number_out++;

        print_states(&states[q], 1);
        print_transition(states[q].trns, states[q].number_out);
        return;
}

void print_states(struct state_t *state, int number_of_states){
    for (int i = 0; i < number_of_states; i++) {
        printf("State q%d \
                number of outgoing states: %d \
                \n", state[i].number, state[i].number_out);
        if(state[i].type == ACCEPT){
            printf("Is accept state\n");
        }
    }
}

void print_transition(struct transition_func_t *trans, int number_of_trans) {
    for (int i = 0; i < number_of_trans; i++) {
        printf("On char: %c \
                transition to: q%d \
                \n", trans[i].input, trans[i].next->number);
    }
}

void check_state(struct state_t *current_state) {
    if(current_state->type == ACCEPT){
        printf("ACCEPT!\n");
    }else {
        printf("HALT/REJECT\n");
    }
}

void free_states(void) {
    free(states);
}

void add_to_alphabet(char input) {
    int len = strlen(global_state.input_alphabet);
    for(int i = 0; i < INPUT_ALPHABET_BUFSIZE; i++){
        if(input == global_state.input_alphabet[i]){
            return;
        }
    }
    global_state.input_alphabet[len] = input;
}

bool check_alphabet(char input) {
    int len = strlen(global_state.input_alphabet);
    if(input == '\n'){ return true; }

    for(int i = 0; i < len; i++){
        if(input == global_state.input_alphabet[i]){
            return true;
        }
    }
    return false;
}

void parse_file(void) {
    char buf[BUFSIZE];
    FILE* dfa_file = fopen(file, "r");
    char tns;
    char mark;
    bool accept = false;
    uint16_t q = 0;
    uint16_t a = 0;

    if (!dfa_file) {
        fprintf(stderr, "%s: %s\n", file, strerror(errno));
        exit(1);
    }

    while (fgets(buf, BUFSIZE, dfa_file) != NULL) {
        accept = false;
        if (sscanf(buf, "%hu%c,%c,%hu,", &q, &mark, &tns, &a) == 4){
            if (mark == '\'') {
                accept = true;
            }
        }else if (sscanf(buf, "%hu,%c,%hu,", &q, &tns, &a) != 3){
            continue;
        }

        set_transition(q,a,tns,accept);

        printf("q: %d\n", q);
        printf("trns: %c\n" ,tns);
        printf("a: %d\n\n", a);

        add_to_alphabet(tns);
    }

    fclose(dfa_file);
}

void display_help(void) {
    printf("Deterministic Finite Automaton Simulator: \n");
    printf("========================================================\n");
    printf("Make a file containing the states and their transition. \n");
    printf("Example: \t 0y1 <- Defining state 0 that transitions to state 1 on char 'y'  \n \
            1w0 <- Defining state 1 that transitions to state 0 on char 'w' \n \
            And so forth....\n ");
    printf("Mark a state with ' to indicate an accepting state.\n");
    printf("Run with ./dfa -s [number of states] -f [file to read] || -h [help]\n");
}
  

void run_input(void) {
    bool running = true;
    char line[BUFSIZE];
    char input_char;

    struct state_t * current_state = states; 

    printf("Enter input to machine:\n");
    while(running && fgets(line, BUFSIZE, stdin) != NULL){
        if(sscanf(line, "%c", &input_char) != 1){
            printf("Invalid input, please try again! \n");
            continue;
        }

        if(input_char == '\n'){
            printf(".:End of input:.\n");
            running = false;
        }

        if(!check_alphabet(input_char)){
            printf(".:Invalid input!:.\n");
            continue;
        }

       for (int i = 0; i < current_state->number_out; i++) {
            if(current_state->trns[i].input == input_char){
                printf("Current state: q%d\n", current_state->number);
                current_state = current_state->trns[i].next;
                printf("Transition to: q%d\n", current_state->number);
                break;
            }
       } 
    }
    check_state(current_state);
}

int main(int argc, char *argv[]) {
    char c;
    int number_of_states = 0;

    while ((c = getopt(argc, argv, "f:s:h")) != -1) {
        switch (c) {
            case 'i':
                input_file = optarg;
                break;
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
    states = init_states(number_of_states);
    parse_file();

    /* Compute */
    run_input();

    /* Cleanup */
    free_states();
    return 0;
}
