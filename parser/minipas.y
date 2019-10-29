%{
/*
 * minipas.y
 *
 * Based on the file "MINI-PASCAL-GRAMMAR.txt"
 */
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    int yyparse();
    int yylex();

    void yyerror(const char *str)
    {
    	extern char *yytext;
	    extern int line_no;
    	fprintf (stderr, "line %d : error token '%s'\n",line_no,yytext);
    }

%}

%union{
    struct Node* node;
    char *string;
}

%token <node> PROGRAM LPAREN RPAREN SEMICOLON DOT COMMA VAR COLON
%token <node> ARRAY LBRAC RBRAC OF INTEGER REAL FUNCTION PROCEDURE
%token <node> PBEGIN END ASSIGNOP IF THEN ELSE WHILE DO LT GT LE GE
%token <node> EQUAL NOTEQUAL PLUS MINUS STAR SLASH NOT DOTDOT
%token <node> STRING AND OR

%token <string> ID
%token <string> NUM
%token <string> STRINGCONST

%type <node>    id_list declarations type standard_type subprog_declarations subprog_dec
%type <node>    subprog_head arguments para_list optional_var compound_statement 
%type <node>    optional_statements statement_list statement tail variable 
%type <node>    procedure_statement expression_list expression simple_expression
%type <node>    term factor addop mulop relop semicolon comma num optional_sign
%type <string>  id

%%
prog : PROGRAM id LPAREN id_list RPAREN semicolon
  declarations
  subprog_declarations
  compound_statement
  DOT { fprintf(stderr, "Parse finish. \n");}
 ;

id_list : 
    id {}
 |  id_list comma id {}
 ;

declarations : 
    declarations VAR id_list COLON type semicolon {}
 |  {}
 ;

type :  
    standard_type {}
 |  ARRAY LBRAC num DOTDOT num RBRAC OF type {}
 ;

standard_type :
    INTEGER  {}
 |  REAL  {}
 |  STRING  {}
 ;

subprog_declarations : 
    subprog_declarations subprog_dec semicolon {}
 |  {}
 ;

subprog_dec : 
    subprog_head  declarations subprog_declarations compound_statement {}
 |  {}
 ;

subprog_head : 
    FUNCTION id arguments COLON standard_type semicolon {}
 |  PROCEDURE id arguments semicolon {}
 ;

arguments : 
    LPAREN para_list RPAREN {}
 |  {}
 ;

para_list : 
    optional_var id_list COLON type {}
 |  optional_var id_list COLON type semicolon para_list {}
 ;

optional_var : 
    VAR {}
 | {}
 ;

compound_statement : 
    PBEGIN optional_statements END {}
 ;

optional_statements :  
    statement_list {}
    | {}
 ;

statement_list : 
    statement {}
 |  statement_list semicolon statement {}
 ;

statement : 
    variable ASSIGNOP expression {}
 |  procedure_statement {}
 |  compound_statement {}
 |  IF expression THEN statement ELSE statement {}
 |  WHILE expression DO statement {}
 |  {}
 ;


variable : 
    id tail {}
 ;

tail : 
    LBRAC expression RBRAC tail {}
 |  {}
 ;

procedure_statement : 
    id {}
 |  id LPAREN expression_list RPAREN {}
 ;

expression_list : 
    expression {}
 |  expression_list comma expression {}
 ;

expression : 
    boolexpression {}
 |  boolexpression AND boolexpression {}
 |  boolexpression OR  boolexpression {}
 ;

boolexpression :
    simple_expression {}
 |  simple_expression relop simple_expression {}
 ;

simple_expression : 
    term {}
 |  simple_expression addop term {}
 ;

term : 
    factor {}
 |  term mulop factor {}
 ;

factor : 
    id tail {}
 |  id LPAREN expression_list RPAREN {}
 |  num {}
 |  STRINGCONST {}
 |  LPAREN expression RPAREN {}
 |  NOT factor {}
 ;

num : 
    optional_sign NUM {}
 ;

optional_sign : 
    addop {}
 |  {}
 ; 

addop: 
    PLUS {}
 |  MINUS {}
 ;

mulop : 
    STAR {}
 |  SLASH {}
 ;

relop : 
    EQUAL {}
 |  NOTEQUAL {}
 |  LT    {}
 |  GT    {}
 |  LE    {}
 |  GE    {}
 ;

id : ID {}
 ;
semicolon : SEMICOLON {}
 ;
comma : COMMA {}
 ;


%%

int yywrap() { return 1; }

int main(int argc, char **argv){
    return yyparse();
}