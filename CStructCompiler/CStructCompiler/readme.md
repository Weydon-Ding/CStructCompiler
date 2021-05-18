A grammar describes the syntax of a programming language, and might be defined in Backus-Naur form (BNF). 
A lexer performs lexical analysis, turning text into tokens. 
A parser takes tokens and builds a data structure like an abstract syntax tree (AST). The parser is concerned with context: does the sequence of tokens fit the grammar? 
A compiler is a combined lexer and parser, built for a specific grammar.