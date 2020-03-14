#ifndef _NODE_H__
#define _NODE_H__

#include "y.tab.h"
#include "symTab.h"

struct Node;

#define VALUE_inVALID  0
#define VALUE_I  1
#define VALUE_R  2

//operation
#define OP_EQUAL   21
#define OP_NEQUAL  22
#define OP_GE      23
#define OP_GT      24
#define OP_LE      25
#define OP_LT      26
#define OP_STAR    27
#define OP_SLASH   28
#define OP_PLUS    39
#define OP_MINUS   30

enum ntype
{
    Node_VAR,
    Node_INTEGER,
    Node_REAL,
    Node_FUNCTION,
    Node_PROCEDURE,
    Node_ASSIGNOP,
    Node_IF,
    Node_WHILE,
    Node_NOT,
    Node_STRING,
    Node_AND,
    Node_OR,
    Node_ID,
    Node_STRINGCONST,
    Node_OP,
    Node_UOP,
    Node_VAR_DECL,
    Node_FUNC_DECL,
    Node_MAIN_FUNC,
    Node_DOT,
    Node_prog,
    Node_id_list,
    Node_declarations,
    Node_subprog_declarations,
    Node_arguments,
    Node_para_list,
    Node_optional_var,
    Node_compound_statement,
    Node_statement_list,
    Node_tail,
    Node_variable,
    Node_CallFuncName,
    Node_PRINTINT,
    Node_PRINTREAL,
    Node_PRINTSTRING,
    Node_procedure_statement,
    Node_expression_list,
    Node_addop,
    Node_mulop,
    Node_relop,
    Node_num
};

struct Node {
    int nodeType;
    
    struct Node *parent;
    struct Node *child;
    struct Node *lsibling;
    struct Node *rsibling;
    int tokenType;
    
    int indexStart;
    int indexEnd;
    int arrDepth;
    struct Node *ref;

    int iValue;
    double rValue;
    char *string;

    char valueValid;
    char op;
    int line_no;
    int scope;
    enum StdType valueType;
    struct TabElemt *element;

    int isFunction;
    int paramNo;
    int param_sym_index[100];
};



struct Node *MakeNode(int type);
void AdoptChildren(struct Node *node, struct Node *child);
void deleteNode(struct Node *node);

void printTree(struct Node *node, int ident);

#endif