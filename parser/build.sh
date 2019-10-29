#!/bin/bash
yacc -d minipas.y 
lex scanner.l 
gcc y.tab.c lex.yy.c -ly -o out 

exit 0