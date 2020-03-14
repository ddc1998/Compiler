#!/bin/bash
yacc -d minipas.y 
lex scanner.l 
gcc -c -o lex.yy.o lex.yy.c 
gcc -c -o y.tab.o y.tab.c
gcc -c -o node.o node.c -std=gnu99
gcc -c -o symTab.o symTab.c -std=gnu99
gcc -c -o Codegen.o Codegen.c -std=gnu99
g++ -lcrypt -o out lex.yy.o y.tab.o node.o symTab.o Codegen.o -ll

exit 0
