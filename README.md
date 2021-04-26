 # DFA - A Deterministic Finite Automaton Simulator in C

 ### Build instructions
Setup build dir:

    meson build
compile with ninja:

    ninja -C build

### Syntax:

Make a file containing the states and their transition.
Syntax example:
    
    0y1 <- Defining state 0 that transitions to state 1 on char 'y'
    1'w0 <- Defining an accepting state 1 that transitions to state 0 on char 'w'
    Mark a state with ' to indicate an accepting state.
...And so forth

 ##### Example:
 ###### TODO: Put example DFA here
 
    
        
    
 ### Run DFA simulator with: 

    ./dfa -s [number of states] -f [file to read] || -h [help]
After which input can be entered, pressing enter twice indicates end of input.

# TODO:
- Update readme for new syntax
- Make parser proper
- Fix the tests
- Fix build file

/* Copyright (C) 2019, Hrafnkell Sigurðarson <hrs70@hi.is> */

