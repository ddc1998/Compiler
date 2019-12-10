#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "node.h"

struct Node *MakeNode(int type) {
    struct Node *node = (struct Node*)malloc(sizeof(struct Node));
    node->nodeType = type;
    node->arrDepth = 0;
    node->valueValid = VALUE_inVALID;
    node->string = NULL;
    node->parent = NULL;
    node->child = NULL;
    node->lsibling = node;
    node->rsibling = node;
    return node;
}

void AdoptChildren(struct Node *node, struct Node *child) {
    
    child->parent = node;

    if(node->child == NULL){
        node->child = child;
    }
    else{
        child->lsibling = node->child->lsibling;
        child->rsibling = node->child;
        node->child->lsibling->rsibling = child;
        node->child->lsibling = child;
    }
}

void deleteNode(struct Node *node) {
    if(node->string != NULL) free(node->string);
    free(node);
}

void printTree(struct Node *node, int ident) {
    static char blank[1024];

    for (int i = 0; i < ident; i++)
        blank[i] = ' ';
    blank[ident] = 0;

    switch (node->nodeType){
        case Node_UOP:
            printf("%sUnary\n", blank);
        break;
        case Node_OP:
            switch(node->op)
            {
                case OP_PLUS:
                    printf("%s+\n", blank);
                    break;
                case OP_MINUS:
                    printf("%s-\n", blank);
                    break;
                case OP_STAR:
                    printf("%s*\n", blank);
                    break;
                case OP_SLASH:
                    printf("%s/\n", blank);
                    break;
                case OP_GT:
                    printf("%s>\n", blank);
                    break;
                case OP_LT:
                    printf("%s<\n", blank);
                    break;
                case OP_GE:
                    printf("%s>=\n", blank);
                    break;
                case OP_LE:
                    printf("%s<=\n", blank);
                    break;
                case OP_EQUAL:
                    printf("%s=\n", blank);
                    break;
                case OP_NEQUAL:
                    printf("%s!=\n", blank);
                    break;
            }
            ident += 2;
            break;
        case Node_VAR:
            printf("%sVAR_DECL\n", blank);
            ident += 4;
            break;
        case Node_INTEGER:
            printf("%sTYPE_INT\n", blank);
            break;
        case Node_REAL:
            printf("%sTYPE_REAL\n", blank);
            break;
        case Node_FUNCTION:
            printf("%sFUNCTION\n", blank);
            ident += 4;
            break;
        case Node_PROCEDURE:
            printf("%sPROCEDURE\n", blank);
            ident += 4;
            break;
        case Node_ASSIGNOP:
            printf("%s:=\n", blank);
            ident += 2;
            break;
        case Node_IF:
            printf("%sIF\n", blank);
            break;
        case Node_WHILE:
            printf("%sWHILE\n", blank);
            break;
        case Node_NOT:
            printf("%sNOT\n", blank);
            break;
        case Node_STRING:
            printf("%sTYPE_STRING\n", blank);
            break;
        case Node_AND:
            printf("%sAND\n", blank);
            break;
        case Node_OR:
            printf("%sOR\n", blank);
            break;
        case Node_ID:
            printf("%s%s\n", blank, node->string);
            ident += 4;
            break;
        case DIGNUM:
            printf("%s%d\n", blank, node->iValue);
            break;
        case REALNUM:
            printf("%s%f\n", blank, node->rValue);
            break;
        case Node_num:
            break;
        case Node_STRINGCONST:
            printf("%s%s\n", blank, node->string);
            break;
        case Node_VAR_DECL:
            printf("%sVAR_DECL\n", blank);
            ident += 4;
            break;
        case Node_prog:
            printf("%sPROGRAM\n", blank);
            ident += 4;
            break;
        case Node_id_list:
            break;
        case Node_declarations:
            break;
        case Node_subprog_declarations:
            break;
        case Node_subprog_dec:
            printf("%ssubprog_dec\n", blank);
            ident += 4;
            break;
        case Node_arguments:
            printf("%spara_list\n", blank);
            ident += 4;
            break;
        case Node_para_list:
            printf("%spara_list\n", blank);
            ident += 4;
            break;
        case Node_para:
            printf("%spara\n", blank);
            ident += 4;
            break;
        case Node_optional_var:
            printf("%soption_var\n", blank);
            ident += 4;
            break;
        case Node_compound_statement:
            printf("%sCOMPOUND_STMT\n", blank);
            ident += 4;
            break;
        case Node_statement_list:
            printf("%sSTMT_LIST\n", blank);
            ident += 4;
            break;
        case Node_statement:
            printf("%sSTMT\n", blank);
            ident += 4;
            break;
        case Node_tail:
            printf("%stail\n", blank);
            ident += 4;
            break;
        case Node_variable:
            printf("%svariable\n", blank);
            ident += 4;
            break;
        case Node_procedure_statement:
            printf("%sprocedure_statement\n", blank);
            ident += 4;
            break;
        case Node_expression_list:
            printf("%sexpression_list\n", blank);
            ident += 4;
            break;
        case Node_addop:
            printf("%saddop\n", blank);
            ident += 4;
            break;
        case Node_mulop:
            printf("%smulop\n", blank);
            ident += 4;
            break;
        case Node_relop:
            printf("%srelop\n", blank);
            ident += 4;
            break;
        case ARRAY:
            printf("%sARRAY\n", blank);
            ident += 4;
            break;
        case REAL:
            printf("%sTYPE_REAL\n", blank);
            break;
        case INTEGER:
            printf("%sTYPE_INT\n", blank);
            break;
        case STRING:
            printf("%sTYPE_STR\n", blank);
            break;
        case FUNCTION:
            printf("%sFUNCTION\n", blank);
            ident += 4;
            break;
        case PROCEDURE:
            printf("%sPROCEDURE\n", blank);
            ident += 4;
            break;
        default : 
            printf("%sdefault:%d\n",blank, node->nodeType); break;
   }
   struct Node *child = node->child;
   if (child != NULL)
   {
       do
       {
           printTree(child, ident);
           child = child->rsibling;
       } while (child != node->child);
   }
}
