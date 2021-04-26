project(
  'DFA',
  'c',
  version: '0.0.1',
  license: 'MIT',
  default_options: ['c_std=c99']
)

cc = meson.get_compiler('c')

inc = include_directories('include')

sources = ['dfa.c', 'parser.c']

dfa = executable(
  'DFA',
  [sources],
  include_directories : inc)

# Tests
# test('dfatest', dfa)
test('parsertest', dfa, args : ['-f test/test2.dfa'] )

