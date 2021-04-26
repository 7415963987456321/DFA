project(
  'DFA',
  'c'
  version: '0.0.1',
  license: 'MIT',
  default_options: ['c_std=c99']
  )

cc = meson.get_compiler('c')

dfa    = executable('DFA',    'dfa.c')
parser = executable('Parser', 'parser.c')

# Tests
# test('dfatest', dfa)
test('parsertest', dfa, args : ['-f test/test2.dfa'] )

