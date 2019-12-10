%{
/*
 * minipas.y
 *
 * Based on the file "MINI-PASCAL-GRAMMAR.txt"
 */
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "node.h"
    #include "symTab.h"

    int yyparse();
    int yylex();
    struct Node* newOpNode(int op);
    struct Node* newUOpNode(int op);
    struct Node *ASThead;

    void yyerror(const char *str)
    {
    	extern char *yytext;
	    extern int line_no;
    	fprintf (stderr, "line %d : error token '%s'\n",line_no,yytext);
    }

%}

%union{
    struct Node *node;
    int number;
    double realnum;
    char *string;
    int line_no;
}

%token <node> PROGRAM LPAREN RPAREN SEMICOLON DOT COMMA VAR COLON
%token <node> ARRAY LBRAC RBRAC OF INTEGER REAL FUNCTION PROCEDURE
%token <node> PBEGIN END ASSIGNOP IF THEN ELSE WHILE DO LT GT LE GE
%token <node> EQUAL NOTEQUAL PLUS MINUS STAR SLASH NOT DOTDOT
%token <node> STRING AND OR Node_TOKEN 

%token <number> DIGNUM 
%token <string> ID
%token <string> REALNUM
%token <string> STRINGCONST

%type <node>    prog id id_list declarations type standard_type subprog_declarations subprog_dec 
%type <node>    subprog_head arguments para_list optional_var compound_statement 
%type <node>    statement_list statement tail variable 
%type <node>    procedure_statement expression_list expression boolexpression
%type <node>    simple_expression term factor addop mulop relop num  var_decl

%%
prog : PROGRAM id LPAREN id_list RPAREN SEMICOLON
  declarations
  subprog_declarations
  compound_statement
  DOT { 
            $$ = MakeNode(Node_prog);
            ASThead = $$;
            
            AdoptChildren($$, $2);
            AdoptChildren($$, $4);
            AdoptChildren($$, $7);
            AdoptChildren($$, $8);
            AdoptChildren($$, $9);
        }
 ;

id_list : 
    id  {
            $$ = MakeNode(Node_id_list);
            AdoptChildren($$, $1);
        }
 |  id_list COMMA id {
            $$ = $1;
            AdoptChildren($1, $3);
        }
 ;

declarations : 
    declarations var_decl {                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
            $$ = $1;
            AdoptChildren($$, $2);
            $$->line_no = $2->line_no;
        }
 |      {
            $$ = MakeNode(Node_declarations);
        }
 ;

var_decl :
    VAR id_list COLON type SEMICOLON {
            $$ = MakeNode(Node_VAR_DECL);
            $$->line_no = $4->line_no;
            AdoptChildren($$, $2);
            AdoptChildren($$, $4);
        }
;

type :  
    standard_type {
            $$ = $1;
        }
 |  ARRAY LBRAC num DOTDOT num RBRAC OF type {
            $$ = $1;
            $$->nodeType = ARRAY;
            $$->ref = $8;
            AdoptChildren($$, $3);
            AdoptChildren($$, $5);
            AdoptChildren($$, $8);
        }
 ;

standard_type :
    INTEGER  {
            $$ = MakeNode(Node_INTEGER);
            $$->nodeType = INTEGER;
        }
 |  REAL  {
            $$ = MakeNode(Node_REAL);
            $$->nodeType = REAL;
        }
 |  STRING  {
            $$ = MakeNode(Node_STRING);
            $$->nodeType = STRING;
        }
 ;

subprog_declarations : 
    subprog_declarations subprog_dec {
            $$ = $1;
            AdoptChildren($$, $2);
        }
 |      {
            $$ = MakeNode(Node_subprog_declarations);
        }
 ;

subprog_dec : 
    subprog_head  declarations subprog_declarations compound_statement SEMICOLON{
            $$ = $1;
            AdoptChildren($$, $2);
            AdoptChildren($$, $3);
            AdoptChildren($$, $4);
        }
 ;

subprog_head : 
    FUNCTION id arguments COLON standard_type SEMICOLON {
            $$ = MakeNode(Node_FUNCTION);
            $$->nodeType = FUNCTION;
            AdoptChildren($$, $2);
            AdoptChildren($$, $3);
            AdoptChildren($$, $5);
        }
 |  PROCEDURE id arguments SEMICOLON {
            $$ = MakeNode(Node_PROCEDURE);
            $$->nodeType = PROCEDURE;
            AdoptChildren($$, $2);
            AdoptChildren($$, $3);
        }
 ;

arguments : 
    LPAREN para_list RPAREN {
            $$ = MakeNode(Node_arguments);
            AdoptChildren($$,$2);
        }
 |      {
            $$ = MakeNode(Node_arguments);
        }
 ;

para_list : 
    optional_var id_list COLON type {
            $$ = MakeNode(Node_para_list);
            AdoptChildren($$, $2);
            AdoptChildren($$, $4);
            deleteNode($1);
        }
 |  optional_var id_list COLON type SEMICOLON para_list {
            $$ = MakeNode(Node_para_list);
            AdoptChildren($$, $2);
            AdoptChildren($$, $4);
            AdoptChildren($$, $6);
            deleteNode($1);
        }
 ;



optional_var : 
    VAR {
            $$ = MakeNode(Node_VAR);
        }
 |      {
            $$ = MakeNode(Node_optional_var);
        }
 ;

compound_statement : 
    PBEGIN statement_list END {
            $$ = MakeNode(Node_compound_statement);
            AdoptChildren($$, $2);
		}
;

statement_list : 
    statement {
            $$ = MakeNode(Node_statement_list);
            AdoptChildren($$, $1);
        }
 |  statement_list SEMICOLON statement {
            $$ = $1;
            AdoptChildren($$, $3);
        }
 |      {
            $$ = MakeNode(Node_statement_list);
        }
 ;

statement : 
    variable ASSIGNOP expression {
            $$ = MakeNode(Node_ASSIGNOP);
            AdoptChildren($$, $1);
            AdoptChildren($$, $3);
        }
 |  procedure_statement {
            $$ = $1;
        }
 |  compound_statement {
            $$ = $1;
        }
 |  IF expression THEN statement ELSE statement {
            $$ = MakeNode(Node_IF);
			AdoptChildren($$, $2);
			AdoptChildren($$, $4);
			AdoptChildren($$, $6);
		
        }
 |  WHILE expression DO statement {
            $$ = MakeNode(Node_WHILE);
			AdoptChildren($$, $2);
			AdoptChildren($$, $4);
        }
 ;


variable : 
    id tail {
            $$ = $1;
            $1->nodeType = Node_variable;
            AdoptChildren($$, $2);
        }
 ;

tail : 
    LBRAC expression RBRAC tail {
            $$ = MakeNode(Node_tail);
            AdoptChildren($$, $2);
            AdoptChildren($$, $4);
        }
 |      {
            $$ = MakeNode(Node_tail);
        }
 ;

procedure_statement : 
    id {
            $$ = $1;
            $$->nodeType = Node_procedure_statement;
        }
 |  id LPAREN expression_list RPAREN {
            $$ = $1;
            $$->nodeType = Node_procedure_statement;
            AdoptChildren($$, $3);
        }
 ;

expression_list : 
    expression {
            $$ = MakeNode(Node_expression_list);
            AdoptChildren($$, $1);
        }
 |  expression_list COMMA expression {
            $$ = $1;
            AdoptChildren($$, $3);
        }
 ;

expression : 
    boolexpression {
            $$ = $1;
        }
 |  boolexpression AND boolexpression {
            $$ = MakeNode(Node_AND);
            AdoptChildren($$, $1);
            AdoptChildren($$, $3);
        }
 |  boolexpression OR boolexpression {
            $$ = MakeNode(Node_OR);
            AdoptChildren($$, $1);
            AdoptChildren($$, $3);
        }
 ;

boolexpression :
    simple_expression {
            $$ = $1;
        }
 |  simple_expression relop simple_expression {
            $$ = $2;
            AdoptChildren($$, $1);
            AdoptChildren($$, $3);
        }
 ;

simple_expression : 
    term { 
            $$ = $1;
        }
 |  simple_expression addop term {
            $$ = $2;
            AdoptChildren($$, $1);
            AdoptChildren($$, $3);
        }
 ;

term : 
    factor {
            $$ = $1;
        }
 |  term mulop factor {
            $$ = $2;
            AdoptChildren($$, $1);
            AdoptChildren($$, $3);
        }
 ;

factor : 
    id tail {
            $$ = $1;
            $1->nodeType = Node_variable;
            AdoptChildren($$, $2);
        }
 |  id LPAREN expression_list RPAREN {
            $$ = $1;
            $$->nodeType = Node_CallFuncName;
            AdoptChildren($$, $3);
        }
 |  num {
            $$ = $1;
        }
 |  STRINGCONST {
            $$ = MakeNode(Node_STRINGCONST);
            $$->string = (char*)malloc(strlen($1)+1);
            strcpy($$->string, $1);
        }
 |  LPAREN expression RPAREN {
            $$ = $2;
        }
 |  NOT factor {
            $$ = MakeNode(Node_NOT);
            AdoptChildren($$, $2);
        }
 ;

num : 
    MINUS num {
            $$ = newUOpNode(OP_MINUS);
            AdoptChildren($$, $2);
        }
 |  REALNUM {
            $$ = MakeNode(Node_num);
            $$->valueValid = VALUE_R;
            $$->rValue = atof($1);
            $$->nodeType = REALNUM;
        }

 |  DIGNUM {
            $$ = MakeNode(Node_num);
            $$->valueValid = VALUE_I;
            $$->iValue = $1;
            $$->nodeType = DIGNUM;
            
        }
 ;

addop: 
    PLUS {
            $$ = newOpNode(OP_PLUS);
        }
 |  MINUS {
            $$ = newOpNode(OP_MINUS);
        }
 ;

mulop : 
    STAR {
            $$ = newOpNode(OP_STAR);
        }
 |  SLASH {
            $$ = newOpNode(OP_SLASH);
        }
 ;

relop : 
    EQUAL {
            $$ = newOpNode(OP_EQUAL);  
        }
 |  NOTEQUAL {
            $$ = newOpNode(OP_NEQUAL);
        }
 |  LT  {
            $$ = newOpNode(OP_LT);
        }
 |  GT  {
            $$ = newOpNode(OP_GT);
        }
 |  LE  {
            $$ = newOpNode(OP_LE);
        }
 |  GE  {
            $$ = newOpNode(OP_GE);
        }
 ;

id : ID {
        $$ = MakeNode(Node_ID);
        $$->string = (char*)malloc(strlen($1)+1);
        strcpy($$->string, $1);
        }
 ;



%%

// Construct a new node for normal operator 
struct Node* newOpNode(int op) {
    struct Node *node = MakeNode(Node_OP);
    node->op = op;
    return node;
}

// Construct a new node for unary operator 
struct Node *newUOpNode(int op) {
    struct Node *node = MakeNode(Node_UOP);
    node->op = op;
    return node;
}

int yywrap() { return 1; }

int main(int argc, char **argv){
    int res;
    if(argc>1 && freopen(argv[1],"r",stdin)==NULL){exit(1);}
    fprintf(stderr, "Open file.\n");
    fprintf(stderr, "call yyparse\n");
    res = yyparse();
    fprintf(stderr, "after call yyparse, res = %d.\n", res);
    if(res > 0)
    {
        return 0;
    }
    fprintf(stderr, "SUCCESS\n");
    
    printTree(ASThead, 0);

    printf("********************************\n"
            "*       No syntax error!       *\n"
            "********************************\n");


    SymTBL.size = 0;
    semanticCheck(ASThead);
    
    printTable();
    
    
 
    return 0;
}