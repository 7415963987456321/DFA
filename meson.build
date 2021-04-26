project('DFA', 'c')

dfa    = executable('DFA',    'dfa.c')
parser = executable('Parser', 'parser.c')

# Tests
# test('dfatest', dfa)
test('parsertest', dfa, args : ['-f test/test2.dfa'] )

