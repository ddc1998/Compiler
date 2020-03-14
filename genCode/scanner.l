%{
/*
 * scan.l
 *
 * lex input file for Mini-pascal scanner
 *
 */

#include <stdio.h>
#include <string.h>
#include "y.tab.h"
#include "node.h"

#define   true    1
#define   false   0
/*
#define   PROGRAM      1
#define   ID           2
#define   LPAREN       3
#define   RPAREN       4
#define   SEMICOLON    5
#define   DOT          6
#define   COMMA        7
#define   VAR          8
#define   COLON        9
#define   ARRAY       10
#define   LBRAC       11
#define   RBRAC       12
#define   NUM         13
#define   STRINGCONST 14
#define   OF          15
#define   INTEGER     16
#define   REAL        17
#define   FUNCTION    18
#define   PROCEDURE   19
#define   PBEGIN      20
#define   END         21
#define   ASSIGNOP    22
#define   IF          23
#define   THEN        24
#define   ELSE        25
#define   WHILE       26
#define   DO          27
#define   LT          28
#define   GT          29
#define   LE          30
#define   GE          31
#define   EQUAL       32
#define   NOTEQUAL    33
#define   PLUS        34
#define   MINUS       35
#define   STAR        36
#define   SLASH       37
#define   NOT         38
#define   DOTDOT      39
#defint   PRINTINT    44

#define   STRING      40

#define   AND         41
#define   OR          42
*/

#define   EFLOAT      67
#define   FLOAT       68
#define   ERROR       70
#define   STRG1       101
#define   COMMENT     200

struct Node *newTokenNode(int tokenType);
int fileno(FILE *stream);
int line_no = 1;
int tmp = 0;
int i=0,tok=0;
%}

A [aA]
B [bB]
C [cC]
D [dD]
E [eE]
F [fF]
G [gG]
H [hH]
I [iI]
J [jJ]
K [kK]
L [lL]
M [mM]
N [nN]
O [oO]
P [pP]
Q [qQ]
R [rR]
S [sS]
T [tT]
U [uU]
V [vV]
W [wW]
X [xX]
Y [yY]
Z [zZ]
NQUOTE [^']

id       ([a-zA-Z]([a-zA-Z0-9_])*)|([_]([a-zA-Z0-9_])+)
int      [0-9]+
Float    {int}+\.{int}+
Expo     ({Float}|{int}){E}[+-]?{int}
Num      {int}|{Expo}|{Float}
Strg1    ["][^\n"]*
Strg2    ["][^\n"]*["]





%%

{A}{R}{R}{A}{Y}              { yylval.node = newTokenNode(ARRAY); return(ARRAY);}
{D}{O}                       { return(DO);}
{E}{L}{S}{E}                 { return(ELSE);}
{E}{N}{D}                    { return(END);}
{F}{U}{N}{C}{T}{I}{O}{N}     { return(FUNCTION);}
{I}{F}                       { return(IF);}
{I}{N}{T}{E}{G}{E}{R}        { yylval.node = newTokenNode(INTEGER); return(INTEGER);}
{O}{F}                       { return(OF);}
{B}{E}{G}{I}{N}              { return(PBEGIN);}
{N}{O}{T}                    { return(NOT);}
{P}{R}{O}{C}{E}{D}{U}{R}{E}  { return(PROCEDURE);}
{P}{R}{O}{G}{R}{A}{M}        { return(PROGRAM);}
{R}{E}{A}{L}                 { yylval.node = newTokenNode(REAL); return(REAL);}
{T}{H}{E}{N}                 { return(THEN);}
{V}{A}{R}                    { return(VAR);}
{W}{H}{I}{L}{E}              { return(WHILE);}
{S}{T}{R}{I}{N}{G}           { yylval.node = newTokenNode(STRING); return(STRING);}
{A}{N}{D}                    { return(AND);}
{O}{R}                       { return(OR);}
{P}{R}{I}{N}{T}{I}{N}{T}     { return(PRINTINT);}
{P}{R}{I}{N}{T}{R}{E}{A}{L}  { return(PRINTREAL);}
{P}{R}{I}{N}{T}{S}{T}{R}{I}{N}{G}  { return(PRINTSTRING);}
{id}                         { yylval.string = yytext;  return(ID);}
{int}                        { yylval.number = atoi(yytext);  return(DIGNUM);}
{Float}                      { yylval.string = yytext;  return(REALNUM);}
{Expo}                       { yylval.string = yytext;  return(REALNUM);}

":="                         { yylval.line_no = line_no; return(ASSIGNOP);}
":"                          { return(COLON);}
","                          { return(COMMA);}
"."                          { return(DOT);}
".."                         { return(DOTDOT);}
"="                          { yylval.line_no = line_no; return(EQUAL);}
"!="                         { yylval.line_no = line_no; return(NOTEQUAL);}
">="                         { yylval.line_no = line_no; return(GE);}
">"                          { yylval.line_no = line_no; return(GT);}
"["                          { return(LBRAC);}
"<="                         { yylval.line_no = line_no; return(LE);}
"("                          { return(LPAREN);}
"<"                          { yylval.line_no = line_no; return(LT);}
"-"                          { yylval.line_no = line_no; return(MINUS);}
"+"                          { yylval.line_no = line_no; return(PLUS);}
"]"                          { return(RBRAC);}
")"                          { return(RPAREN);}
";"                          { return(SEMICOLON);}
"/"                          { yylval.line_no = line_no; return(SLASH);}
"*"                          { yylval.line_no = line_no; return(STAR);}

{Strg1}                     { fprintf(stderr, "Find a error string on line %d : %s\n",line_no, yytext);}
{Strg2}                     { yylval.node = newTokenNode(Node_STRINGCONST); yylval.string = yytext; return(STRINGCONST);}
      

"/*"                        { register int c;
                              while ((c = input())){
                                if (c == '*') {
                                  if ((c = input()) == '/') {
                                    i++;
                                    break;
                                  }
                                  else unput(c);
                                }
                                else if (c == '\n') line_no++;
                              }
                            }


"//".*[\r\n]                { line_no++; i++; BEGIN 0;}


[ \t\f]    ;

\r         ;
\n   { line_no++;}

.   {  fprintf(stderr,"Lexical analyzer error at line %d : %s\n",line_no,yytext);}

%%

struct Node *newTokenNode(int tokenType){
    struct Node *node = MakeNode(Node_TOKEN); //all nodes' default type
    node->tokenType = tokenType;
    return node;
}

