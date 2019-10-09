%{
/*
 * scan.l
 *
 * lex input file for pascal scanner
 *
 * extensions: two ways to spell "external" and "->" ok for "^".
 */

 #include <stdio.h>

#define   true    1
#define   false   0

#define   AND      1
#define   ARRAY      2
#define   CASE      3
#define   CONST      4
#define   DIV      5
#define   DO      6
#define   DOWNTO      7
#define   ELSE      8
#define   END      9

#define   EXTERNAL      10
#define   FOR      11
#define   FORWARD      12
#define   FUNCTION      13
#define   GOTO      14
#define   IF      15
#define   IN      16
#define   LABEL      17
#define   MOD      18
#define   NIL      19
#define   NOT      20
#define   OF       21
#define   OR      22
#define   OTHERWISE      23
#define   PACKED      24
#define   PBEGIN      25
#define   PFILE      26
#define   PROCEDURE      27
#define   PROGRAM      28
#define   RECORD      29
#define   REPEAT      30
#define   SET      31
#define   THEN      32
#define   TO      33
#define   TYPE      34
#define   UNTIL      35
#define   VAR      36
#define   WHILE      37
#define   WITH      38
#define   ID       39

#define   ASSIGNOP      40
#define   CHARACTER_STRING      41
#define   COLON      42
#define   COMMA      43
#define   INTEGER      44
#define   DOT      45
#define   DOTDOT      46
#define   EQUAL      47
#define   GE      48
#define   GT      49
#define   LBRAC      50
#define   LE      51
#define   LPAREN      52
#define   LT      53
#define   MINUS      54
#define   NOTEQUAL      55
#define   PLUS      56
#define   RBRAC      57
#define   REAL      58
#define   RPAREN      59
#define   SEMICOLON      60
#define   SLASH      61
#define   STAR      62
#define   STARSTAR      63
#define   UPARROW      64


#define   NUM         66
#define   EFLOAT      67
#define   FLOAT       68
#define   ERROR       70
#define   DOWNARROW   71
#define   STRING      100
#define   STRG1       101
#define   COMMENT     200


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
Num      [+-]?({int}|{Expo}|{Float})
Strg1    ["][^\n"]*
Strg2    ["][^\n"]*["]

%x CMMT1
%x CMMT2
%x NUM_ST
%x NUM_ST2

%%

{A}{R}{R}{A}{Y}              return(ARRAY);
{D}{O}                       return(DO);
{E}{L}{S}{E}                 return(ELSE);
{E}{N}{D}                    return(END);
{F}{U}{N}{C}{T}{I}{O}{N}     return(FUNCTION);
{I}{F}                       return(IF);
{I}{N}{T}{E}{G}{E}{R}        return(INTEGER);
{O}{F}                       return(OF); 
{B}{E}{G}{I}{N}              return(PBEGIN);
{N}{O}{T}                    return(NOT);  
{P}{R}{O}{C}{E}{D}{U}{R}{E}  return(PROCEDURE);
{P}{R}{O}{G}{R}{A}{M}        return(PROGRAM);
{R}{E}{A}{L}                 return(REAL);
{T}{H}{E}{N}                 return(THEN);
{V}{A}{R}                    return(VAR);
{W}{H}{I}{L}{E}              return(WHILE);
{id}                         return(ID);
{Num}                        { BEGIN NUM_ST; fprintf(stderr, "%3d-th token(type:%d) on line %d : %s\n", i, NUM,line_no, yytext); i++;}

<NUM_ST>"+"                  { fprintf(stderr, "%3d-th token(type:%d) on line %d : +\n",  i, PLUS, line_no); i++; BEGIN NUM_ST2;}
<NUM_ST>"-"                  { fprintf(stderr, "%3d-th token(type:%d) on line %d : -\n",  i, MINUS,line_no); i++; BEGIN NUM_ST2;}
<NUM_ST2>{Num}               { fprintf(stderr, "%3d-th token(type:%d) on line %d : %s\n", i, NUM,  line_no, yytext); i++; BEGIN NUM_ST;}

<NUM_ST>[ \t\f]                      { }
<NUM_ST>\r                           { }  
<NUM_ST>\n                           { BEGIN 0; line_no++;}
<NUM_ST>{A}{R}{R}{A}{Y}              { BEGIN 0; return(ARRAY);}
<NUM_ST>{D}{O}                       { BEGIN 0; return(DO);}
<NUM_ST>{E}{L}{S}{E}                 { BEGIN 0; return(ELSE);}
<NUM_ST>{E}{N}{D}                    { BEGIN 0; return(END);}
<NUM_ST>{F}{U}{N}{C}{T}{I}{O}{N}     { BEGIN 0; return(FUNCTION);}
<NUM_ST>{I}{F}                       { BEGIN 0; return(IF);}
<NUM_ST>{I}{N}{T}{E}{G}{E}{R}        { BEGIN 0; return(INTEGER);}
<NUM_ST>{O}{F}                       { BEGIN 0; return(OF); }
<NUM_ST>{B}{E}{G}{I}{N}              { BEGIN 0; return(PBEGIN);}
<NUM_ST>{N}{O}{T}                    { BEGIN 0; return(NOT);  }
<NUM_ST>{P}{R}{O}{C}{E}{D}{U}{R}{E}  { BEGIN 0; return(PROCEDURE);}
<NUM_ST>{P}{R}{O}{G}{R}{A}{M}        { BEGIN 0; return(PROGRAM);}
<NUM_ST>{R}{E}{A}{L}                 { BEGIN 0; return(REAL);}
<NUM_ST>{T}{H}{E}{N}                 { BEGIN 0; return(THEN);}
<NUM_ST>{V}{A}{R}                    { BEGIN 0; return(VAR);}
<NUM_ST>{W}{H}{I}{L}{E}              { BEGIN 0; return(WHILE);}
<NUM_ST>{id}                         { BEGIN 0; return(ID);}
<NUM_ST>":="                         { BEGIN 0; return(ASSIGNOP);}
<NUM_ST>":"                          { BEGIN 0; return(COLON);}
<NUM_ST>","                          { BEGIN 0; return(COMMA);}
<NUM_ST>"."                          { BEGIN 0; return(DOT);}
<NUM_ST>".."                         { BEGIN 0; return(DOTDOT);}
<NUM_ST>"="                          { BEGIN 0; return(EQUAL);}
<NUM_ST>"!="                         { BEGIN 0; return(NOTEQUAL);}
<NUM_ST>">="                         { BEGIN 0; return(GE);}
<NUM_ST>">"                          { BEGIN 0; return(GT);}
<NUM_ST>"["                          { BEGIN 0; return(LBRAC);}
<NUM_ST>"<="                         { BEGIN 0; return(LE);}
<NUM_ST>"("                          { BEGIN 0; return(LPAREN);}
<NUM_ST>"<"                          { BEGIN 0; return(LT);}
<NUM_ST>"]"                          { BEGIN 0; return(RBRAC);}
<NUM_ST>")"                          { BEGIN 0; return(RPAREN);}
<NUM_ST>";"                          { BEGIN 0; return(SEMICOLON);}
<NUM_ST>"/"                          { BEGIN 0; return(SLASH);}
<NUM_ST>"*"                          { BEGIN 0; return(STAR);}
<NUM_ST>{Strg1}                      { BEGIN 0; return(STRG1);}
<NUM_ST>{Strg2}                      { BEGIN 0; return(STRING);}
<NUM_ST>"/*"                         { BEGIN CMMT1;  fprintf(stderr, "%3d-th token(type:%d) on line %d : %s", i, COMMENT,line_no, yytext);}
<NUM_ST>"//"                         { BEGIN CMMT2;  fprintf(stderr, "%3d-th token(type:%d) on line %d : %s", i, COMMENT,line_no, yytext);}
<NUM_ST>.                            {  fprintf(stderr,"Lexical analyzer error at line %d : %s\n",line_no,yytext);}


":="                         return(ASSIGNOP);
":"                          return(COLON);
","                          return(COMMA);
"."                          return(DOT);
".."                         return(DOTDOT);
"="                          return(EQUAL);
"!="                         return(NOTEQUAL);
">="                         return(GE);
">"                          return(GT);
"["                          return(LBRAC);
"<="                         return(LE);
"("                          return(LPAREN);
"<"                          return(LT);
"-"                          return(MINUS);
"+"                          return(PLUS);
"]"                          return(RBRAC);
")"                          return(RPAREN);
";"                          return(SEMICOLON);
"/"                          return(SLASH);
"*"                          return(STAR);

{Strg1}                     { return(STRG1);}
{Strg2}                     { return(STRING);}
"/*"                        { BEGIN CMMT1;  fprintf(stderr, "%3d-th token(type:%d) on line %d : %s", i, COMMENT,line_no, yytext);}
<CMMT1>"*/"                 { BEGIN 0;      fprintf(stderr, "%s\n", yytext); i++; }
<CMMT1>\n                   { line_no++;    fprintf(stderr, "%s", yytext);}
<CMMT1>.                    { fprintf(stderr, "%s", yytext); }        

"//"                        { BEGIN CMMT2;  fprintf(stderr, "%3d-th token(type:%d) on line %d : %s", i, COMMENT,line_no, yytext);}
<CMMT2>\n                   { line_no++;    fprintf(stderr, "%s", yytext); i++; BEGIN 0;}
<CMMT2>.                    { fprintf(stderr, "%s", yytext); }

[ \t\f]    ;

\r         ;
\n   { line_no++;}

.   {  fprintf(stderr,"Lexical analyzer error at line %d : %s\n",line_no,yytext);}

%%


int main(int argc, char** argv) {
  yyin = fopen("test.txt", "r");
  for (i = 0; 1==1; i ++) {
    tok = yylex();
    if(tok==STRG1){
        fprintf(stderr, "Find a error string on line %d : %s\n",line_no, yytext);
        i--;
    }
    else{
        fprintf(stderr, "%3d-th token(type:%d) on line %d : %s\n", i, tok,line_no, yytext);
    }
  }
  return(0);
} /* end of main */

yywrap() {
  fclose(yyin);
  exit(1);
}
