#ifndef _CODEGEN_H__
#define _CODEGEN_H__

#include "y.tab.h"
#include "symTab.h"
#include "node.h"
struct RegTabElemt
{
    char name[100];
    enum StdType type; //TypeInt TypeReal TypeString TypeFunc TypeProc TypeArr
    int posi_no;       //TypeFunc:-1, TypeProc:-2
    int dim;
    int arr_dep[10];
    int start_index[10];
    int para_no;
};

struct RegTable
{
    int size;
    struct RegTabElemt Elemts[100];
};

extern int is_printReal;
extern int is_Real;

extern struct RegTable RegTBL;
int regTab();
int findsymPOSI(struct Node *node);
int findsymTYPE(struct Node *node);
void Code_CallFuncName(struct Node *node);
void Code_expression(struct Node *node,int tNum);
void Code_expression_list(struct Node *node,int tNum);
void Code_ASSIGNOP(struct Node *node);
void Code_PRINTINT(struct Node *node);
void Code_PRINTREAL(struct Node *node);
void Code_PRINTSTRING(struct Node *node);
void Code_IF(struct Node *node);
void Code_WHILE(struct Node *node);
void Code_statement(struct Node *node);
void Code_statement_list(struct Node *node);
void Code_compound_statement(struct Node *node);
void Code_subprog_dec(struct Node *node);
void Code_subprog_declaration(struct Node *node);
void Codegen(struct Node *node);
void printCode(struct Node *node, int ident);


#endif