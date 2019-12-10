#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "node.h"
#include "symTab.h"

struct SymTable SymTBL;
static int scope = 1;
static int mScope = 1;
char *curfunc = NULL;
int semantic_error = 0;


struct TabElemt* findSymbol(char *s, int itsscope){
    for(int i=0; i<SymTBL.size; i++) {
        struct TabElemt Elemt = SymTBL.Elemts[i];
        // Find a specific symbol
        if(strcmp(s, Elemt.name) == 0 && itsscope == Elemt.scope) { 
            return &SymTBL.Elemts[i]; 
        }
    }
    return 0;
}


int findSymbolIndex(char *s, int itsscope){
    for(int i=0; i<SymTBL.size; i++) {
        struct TabElemt Elemt = SymTBL.Elemts[i];
        // Find a specific symbol
        if(strcmp(s, Elemt.name) == 0){// && itsscope == Elemt.scope) { 
            //printf("find symbol index %s %d\n", Elemt.name, i);
            return i; 

        }
    }
    return 0;
}

int setParamIndex(char *s,int k, int SymTabIndex){
    for(int i=0; i<SymTBL.size; i++) {
        if(strcmp(s, SymTBL.Elemts[i].name) == 0){ 
            SymTBL.Elemts[i].param_index_list[k] = SymTabIndex; 
            //printf("setParam %s : No.%d index %d\n", s, k, SymTabIndex);
        }
    }
    return 0;
}

struct TabElemt* findSymbol_noscope(char *s){
    for(int i=0; i<SymTBL.size; i++) {
        if(strcmp(s, SymTBL.Elemts[i].name) == 0) { 
            return &SymTBL.Elemts[i]; 
        }
    }
    return 0;
}

void setParamNo(char *s, int paramNo){
    for(int i=0; i<SymTBL.size; i++) {
        if(strcmp(s, SymTBL.Elemts[i].name) == 0){ 
            SymTBL.Elemts[i].paramNo = paramNo; 
            //printf("setParamNo %s : %d\n", s, paramNo);
        }
    }
}


int setValueValid(char *s, int _scope, char valueValid) {
    for(int i=0; i<SymTBL.size; i++){
        struct TabElemt Elemt = SymTBL.Elemts[i];
        if(strcmp(s, Elemt.name) == 0 && _scope == Elemt.scope) {
            SymTBL.Elemts[i].valueValid = valueValid;
            return 0;
        }
    }
    return -1;
}


struct TabElemt *addVariable(   char *s,
                                enum StdType type,
                                int arrDepth,
                                int indexStart,
                                int indexEnd,
                                int isFunction,
                                int itsscope,
                                int line_no ) {
    if(findSymbol(s, itsscope) != 0) {
        printf("Error: duplicate declaration of variable %s\n", s);
        semantic_error = 1;
        return 0;
    }
    int index = SymTBL.size;
    SymTBL.size++;

    strcpy(SymTBL.Elemts[index].name, s);
    SymTBL.Elemts[index].type = type;
    SymTBL.Elemts[index].arrDepth = arrDepth;
    SymTBL.Elemts[index].indexStart = indexStart;
    SymTBL.Elemts[index].indexEnd = indexEnd;
    SymTBL.Elemts[index].isFunction = isFunction;
    SymTBL.Elemts[index].scope = itsscope;
    SymTBL.Elemts[index].paramNo = 0;
    SymTBL.Elemts[index].valueValid = VALUE_inVALID;

    printf("Add %s symbol into symbol table \n", s);
    
    return &SymTBL.Elemts[index];    
}

struct Node *nthChild(int n, struct Node *node){
    struct Node *child = node->child;
    for(int i=1; i<n; i++)child = child->rsibling;

    return child;
}

int setArrRange(char *s, int _scope, struct Node *node){
    for(int i=0; i<SymTBL.size; i++){
        struct TabElemt Elemt = SymTBL.Elemts[i];
        if(strcmp(s, Elemt.name) == 0 && _scope == Elemt.scope) {
            int k =0;
            int size = (node->arrDepth)*2;
            SymTBL.Elemts[i].arrRangeList[k] = node->indexStart;
            k++;
            SymTBL.Elemts[i].arrRangeList[k] = node->indexEnd;
            k++;
            while(k<size){
                node = nthChild(3, node);
                SymTBL.Elemts[i].arrRangeList[k] = node->indexStart;
                k++;
                SymTBL.Elemts[i].arrRangeList[k] = node->indexEnd;
                k++;
            }
            int m = 0;
            /*while(m<size){
                printf("(%d, %d) \n", SymTBL.Elemts[i].arrRangeList[m], SymTBL.Elemts[i].arrRangeList[m+1]);
                m+=2;
            }*/
            
        }
    }
    return 0;
}




void semanticCheck(struct Node *node){
//	printf("Debug:%d %s\n", node->nodeType, node->string);
    switch(node->nodeType){
		case FUNCTION:
        {
  			scope = mScope;
  			struct Node *idNode = nthChild(1, node);
  			struct Node *typeNode = nthChild(3, node);

            enum StdType valueType;
            if(typeNode->nodeType == INTEGER)valueType = TypeInt;
            else if(typeNode->nodeType == REAL)valueType = TypeReal;
            else if(typeNode->nodeType == STRING)valueType = TypeString;
            else valueType = TypeReal;

            idNode->isFunction = 1;
            addVariable(idNode->string, valueType, 0, 0, 0, 1, 1, node->line_no);
            mScope++;
            scope = mScope;
            curfunc = (char*)malloc(strlen(idNode->string)+1);
            strcpy(curfunc, idNode->string);
            semanticCheck(nthChild(2, node));
            setParamNo(idNode->string, idNode->rsibling->paramNo);
            for(int k=0; k<idNode->rsibling->paramNo; k++){
                idNode->param_sym_index[k] = idNode->rsibling->param_sym_index[k];
                //printf("Func child1 idNode Paramlist %s %d %d\n", idNode->string, k, idNode->param_sym_index[k]);
                setParamIndex(idNode->string, k, idNode->param_sym_index[k]);
            }
            semanticCheck(nthChild(4, node));
            semanticCheck(nthChild(5, node));
            free(curfunc);
            curfunc = NULL;
            scope = 1;

            return;
        }

		case PROCEDURE:
        {
            scope = mScope;
            struct Node *idNode = nthChild(1, node);
            addVariable(idNode->string, TypeProc, 0, 0, 0, 1, 1, node->line_no);
            mScope++;
            scope = mScope;
            semanticCheck(nthChild(2, node));
            setParamNo(idNode->string, idNode->rsibling->paramNo);
            for(int k=0; k<idNode->rsibling->paramNo; k++){
                idNode->param_sym_index[k] = idNode->rsibling->param_sym_index[k];
                //printf("Proc child1 idNode Paramlist %s %d %d\n", idNode->string, k, idNode->param_sym_index[k]);
                setParamIndex(idNode->string, k, idNode->param_sym_index[k]);
            }
            semanticCheck(nthChild(3, node));
            semanticCheck(nthChild(4, node));
            scope = 1;

            return;
		}
        
        case Node_arguments: {
            if(node->child != NULL){
                struct Node *child = nthChild(1, node);
                semanticCheck(child);
                node->paramNo = child->paramNo;
                for(int k=0; k<node->paramNo; k++){
                    node->param_sym_index[k] = child->param_sym_index[k];
                    //printf("argument Paralist %d : %d\n", k, node->param_sym_index[k]);
                }
                //printf("argument ParamNo %s : %d\n", node->string, node->paramNo);
            }
            return;
        }

        case ARRAY:{
            struct Node *start = nthChild(1, node);
            struct Node *end = nthChild(2, node);
            struct Node *typeNode = nthChild(3, node);

            if (start->nodeType != DIGNUM || end->nodeType != DIGNUM){
                printf("Error: Array declaration error\n");
                semantic_error = 1;
            }
            node->indexStart = start->iValue;
            node->indexEnd = end->iValue;
            //printf("indexStart %d indexEnd %d \n", node->indexStart, node->indexEnd);
            semanticCheck(typeNode);	
            node->arrDepth = typeNode->arrDepth + 1;
            break;
		}

		case Node_para_list:{
            struct Node *TYPE = nthChild(2, node); //type
            //printf("Debug:%d %s\n", TYPE->nodeType, TYPE->string);
            
            enum StdType valueType;
            int arrDepth = 0;
            int indexStart = 0;
            int indexEnd = 0;

            if(TYPE->nodeType == INTEGER)valueType = TypeInt;
            else if(TYPE->nodeType == REAL)valueType = TypeReal;
            else if(TYPE->nodeType == STRING)valueType = TypeString;
            else if (TYPE->nodeType == ARRAY)
            {
                semanticCheck(TYPE);
                arrDepth = TYPE->arrDepth;
                indexStart = TYPE->indexStart;
                indexEnd = TYPE->indexEnd;
                //printf("TYPE indexStart %d indexEnd %d arrDepth %d\n", indexStart, indexEnd, arrDepth);

                struct Node *arrayType = TYPE->ref;
                while(arrayType->nodeType == ARRAY)arrayType = arrayType->ref;

                switch( arrayType->nodeType )
                {
                    case INTEGER:
                    valueType = TypeInt;
                    break;
                    case REAL:
                    valueType = TypeReal;
                    break;
                    case STRING:
                    valueType = TypeString;
                    break;
                    default:
                        printf("Error: unidentified type %d\n", arrayType->nodeType);
                        valueType = TypeInt;
                        semantic_error = 1;
                }
            }
            else valueType = TypeReal;

            struct Node *idList = nthChild(1, node);
            struct Node *idNode = idList->child;
            node->paramNo = 0;

            do{
                if (TYPE->nodeType == ARRAY)    {
                    idNode->indexStart = TYPE->indexStart;
                    idNode->indexEnd = TYPE->indexEnd;
                    idNode->arrDepth = TYPE->arrDepth;
                }
                //printf("para_list1  ParamNo %s : %d\n", node->string, node->paramNo);

                //printf("Para_list1  child1 idNode %s type %d\n", idNode->string, valueType);
                addVariable(idNode->string, valueType, arrDepth, indexStart, indexEnd, 0, scope, node->line_no);
                
                char valueValid = (valueType == TypeInt)? VALUE_I : VALUE_R;
                setValueValid(idNode->string, scope, valueValid);
                if(TYPE->nodeType == ARRAY){
                    setArrRange(idNode->string, scope, TYPE);
                }
                //printf("para_list2 ParamNo %s : %d\n", node->string, node->paramNo);

                node->param_sym_index[node->paramNo] = findSymbolIndex(idNode->string, scope);

                idNode = idNode->rsibling;
                node->paramNo ++;
                //printf("para_list4 ParamNo %s : %d\n", node->string, node->paramNo);
            } while(idNode != idList->child);

            if (TYPE->rsibling != NULL){
                struct Node *sublist = nthChild(3, node);
                semanticCheck(sublist);
                for(int k=node->paramNo; k<(node->paramNo + sublist->paramNo); k++){
                    node->param_sym_index[k] = sublist->param_sym_index[k - node->paramNo];
                }
                node->paramNo += sublist->paramNo;
            }
            //printf("para_list ParamNo %s : %d\n", node->string, node->paramNo);
            return;
		}

        
        case Node_VAR_DECL: {
            if (node->child == NULL)break;

            struct Node *TYPE = nthChild(2, node); //Type
            //printf("Debug:%d %s\n", TYPE->nodeType, TYPE->string);
            enum StdType valueType;
            int arrDepth = 0;
            int indexStart = 0;
            int indexEnd = 0;

            if(TYPE->nodeType == INTEGER) valueType = TypeInt;
            else if(TYPE->nodeType == REAL) valueType = TypeReal;
            else if(TYPE->nodeType == STRING) valueType = TypeString;
            else if (TYPE->nodeType == ARRAY){
                semanticCheck(TYPE);
                arrDepth = TYPE->arrDepth;
                indexStart = TYPE->indexStart;
                indexEnd = TYPE->indexEnd;
                /*
                struct Node *TTYPE = nthChild(3, TYPE);
                printf("TTYPE indexStart %d TTYPE indexEnd %d TTYPE arrDepth %d\n", TTYPE->indexStart, TTYPE->indexEnd, TTYPE->arrDepth);
                struct Node *TTTYPE = nthChild(3, TTYPE);
                printf("TTTYPE indexStart %d TTTYPE indexEnd %d TTTYPE arrDepth %d\n", TTTYPE->indexStart, TTTYPE->indexEnd, TTTYPE->arrDepth);
                */
                struct Node *arrayType = TYPE->ref;
                while(arrayType->nodeType == ARRAY)arrayType = arrayType->ref;

                switch( arrayType->nodeType )
                {
                    case INTEGER:
                        valueType = TypeInt;
                        break;
                    case REAL:
                        valueType = TypeReal;
                        break;
                    case STRING:
                        valueType = TypeString;
                        break;
                    default:
                        printf("Error: unidentified type %d", arrayType->nodeType);
                        semantic_error = 1;
                        valueType = TypeInt;
                }
            }
            else valueType = TypeReal;

            struct Node *idList = nthChild(1, node); //id_list
            struct Node *idNode = idList->child;
            do {
                if (TYPE->nodeType == ARRAY) {
                    idNode->indexStart = TYPE->indexStart;
                    idNode->indexEnd = TYPE->indexEnd;
                    idNode->arrDepth = TYPE->arrDepth;
                }
                addVariable(idNode->string, valueType, arrDepth, indexStart, indexEnd, 0, scope, node->line_no);
                //printf("idNode %s valueType %d scope %d indexStart %d indexEnd %d TTTYPE arrDepth %d\n", idNode->string, valueType, scope, indexStart, indexEnd, arrDepth);
                if(TYPE->nodeType == ARRAY){
                    setArrRange(idNode->string, scope, TYPE);
                }
                idNode = idNode->rsibling;
            } while(idNode != idList->child);
            
            return;
        }
        //Node_ID->nodetype
        case Node_procedure_statement:{
            struct TabElemt *Elemt = findSymbol_noscope(node->string);
            if(Elemt == 0){
                printf("Error: undeclared procedure \"%s\"\n", node->string);
                semantic_error = 1;
                //exit(0);
            }
            //make sure the amount of parameter in the procedure right
            node->paramNo = Elemt->paramNo;
            struct Node *paramlist = nthChild(1, node);
            if (paramlist != NULL) {
                semanticCheck(paramlist);
                if (paramlist->paramNo != node->paramNo) {
                    printf("Error: wrong number of parameter in \"%s\"\n", node->string);
                    semantic_error = 1;
                    //exit(0);
                }
            }
            else{
                if (node->paramNo != 0) {
                    printf("Error: wrong number of parameter in \"%s\"\n", node->string);
                    semantic_error = 1;
                    
                }
            }

            return;
        }
        //Node_ID->nodetype
        case Node_CallFuncName: {
            struct TabElemt *Elemt = findSymbol_noscope(node->string);

            if(Elemt == 0) {
                printf("Error: undeclared function \"%s\"\n", node->string);
                semantic_error = 1;
                //exit(0);
            }

            node->element = Elemt;
            node->valueType = Elemt->type;
            node->isFunction = 1;
            node->paramNo = Elemt->paramNo;

            struct Node *paramlist = nthChild(1, node);
            semanticCheck(paramlist);

            if (paramlist->paramNo != node->paramNo) {
                printf("Error: wrong number of parameter in \"%s\"\n", node->string);
                semantic_error = 1;
                exit(0);
            }

            return;
        }

        //Node_ID->nodeType 
        case Node_variable:{
            //printf("  Debug:%d %s\n", node->nodeType, node->string);
            struct TabElemt *Elemt = findSymbol(node->string, scope);
            
            if(Elemt == 0){
                Elemt = findSymbol(node->string, 1);
                if(Elemt == 0){
                    printf("Error: undeclared variable \"%s\"\n", node->string);
                    semantic_error = 1;
                }

                node->isFunction = Elemt->isFunction;
                node->paramNo = Elemt->paramNo;

                //printf("%s func:%d\n", node->string, Elemt->isFunction);
            }

            node->element = Elemt;
            node->valueType = Elemt->type;
            node->arrDepth = Elemt->arrDepth;
            node->indexStart = Elemt->indexStart;
            node->indexEnd = Elemt->indexEnd;
            node->valueValid = Elemt->valueValid;
            //printf("  valueType %d arrDepth %d indexStart %d indexEnd %d valueValid %d\n", node->valueType, node->arrDepth, node->indexStart, node->indexEnd, node->valueValid);
            //printf("%s valueType:%d\n", node->string, node->valueType);

            if (node->arrDepth > 0){
                struct Node *tail = nthChild(1, node);
                struct Node *expr = tail;
                //printf("    semanticCheck_tail_BEGIN\n");
                //semanticCheck(tail);//nodeType = Node_tail
                //printf("    semanticCheck_tail_END\n");
                int tmpArrDepth = node->arrDepth;
                int exprValue = 0;
                int out_of_arrRange = 0;
                int start = 0;
                int end = 0;
                int k = 0;
                while(tail->child != NULL){
                    
                    tmpArrDepth--;
                    tail = nthChild(2, tail);
                    start = Elemt->arrRangeList[k];
                    end = Elemt->arrRangeList[k+1];
                    semanticCheck(expr);
                    exprValue=expr->iValue ;
                    if (tmpArrDepth < 0){
                        printf("Error: wrong array dimension of \"%s\"\n", node->string);
                        semantic_error = 1;
                    }
                    else if (exprValue < start || exprValue > end){
                        out_of_arrRange = 1;
                        printf("Error: wrong array index of \"%s\"\n", node->string);
                        semantic_error = 1;
                    }
                    k+=2;
                    expr = nthChild(1, tail);
                }
            }
            
            break;
        }
        

        case DIGNUM:{
            node->valueType = TypeInt;
            break;
        }

        case REALNUM:{
            node->valueType = TypeReal;
            break;
        }

        /* Unary operator */
        case Node_UOP:{
            struct Node *child = nthChild(1, node);
            semanticCheck(child);
            node->valueValid = child->valueValid;
            node->valueType = child->valueType;

            // node->valueType is TypeInt means that it's a integer
            if (node->valueType == TypeInt)node->iValue = child->iValue*(-1);
            else node->rValue = child->rValue*(-1);
            
            break;
        }

        case Node_OP: {
            struct Node *child1 = nthChild(1, node);
            struct Node *child2 = nthChild(2, node);
            semanticCheck(child1);
            semanticCheck(child2);
            if(child1->valueValid == VALUE_I && child2->valueValid == VALUE_I){
                node->valueValid = VALUE_I;
            }
            else{
                node->valueValid = VALUE_R;
            }            

            if(child1->valueType == TypeInt && child2->valueType == TypeInt){
                node->valueType = TypeInt;
            }
            else{
                node->valueType = TypeReal;
            }
            // node->valueType is TypeInt means that it's a integer
            if (node->valueType == TypeInt) node->iValue = child1->iValue*child2->iValue;
            else node->rValue = child1->rValue*child2->rValue;
            
            break;
        }

        case Node_ASSIGNOP:{
            //printf("Debug:%d %s\n", node->nodeType, node->string);
            struct Node *childVari = nthChild(1, node); //variable
            struct Node *childExpr = nthChild(2, node); //expression
            semanticCheck(childVari);
            //printf("Debug:childVari\n");
            semanticCheck(childExpr);
            //printf("Debug:childExpr\n");
            if (node->nodeType == ASSIGNOP && childVari->isFunction && curfunc != NULL){
            //This is return value
                if(strcmp(childVari->string, curfunc) == 0) {
                    //printf("Current func:%s %d\n", childVari->string, childExpr->valueValid);
                    setValueValid(childVari->string, 1, childExpr->valueValid);
                }
                else {
                    printf("Error: left hand side \"%s\" is not assignable\n", childVari->string);
                    semantic_error = 1;
                }
            }
            //printf("%s %d %d %d\n", childVari->string, childVari->valueType, childVari->nodeType, childVari->valueValid);
            //printf("%s %d %d %d\n", childExpr->string, childExpr->valueType, childExpr->nodeType, childExpr->valueValid);

            /* We only implement the checking for integer and real types
            you should implement the checking for array type by yourself */
            //printf("ValueT Vari %d Expr %d arrDepth Vari %d Expr %d\n", childVari->valueType, childExpr->valueType, childVari->arrDepth, childExpr->arrDepth);
            if(childVari->valueType != childExpr->valueType){
                printf("Error: type mismatch for assignment.\n");
                semantic_error = 1;
                return;
            }
                
            //Check if oprands have been initialized
            if (childVari->valueValid == VALUE_inVALID && childVari->nodeType == Node_variable)
            if (node->nodeType == Node_OP){
                printf("Error: use \"%s\" before initialization\n", childVari->string);
                semantic_error = 1;
            }
            if (childExpr->valueValid == VALUE_inVALID && childExpr->nodeType == Node_variable){
                printf("Error: use \"%s\" before initialization\n", childExpr->string);
                semantic_error = 1;
            }
            node->valueType = childVari->valueType;
            
            if (node->nodeType == ASSIGNOP && childVari->nodeType == Node_variable)
            setValueValid(childVari->string, scope, childExpr->valueValid);

            if (node->nodeType == Node_OP)
            node->valueValid = childVari->valueValid;

            return;
        }

        case Node_expression_list:{
            struct Node *child = nthChild(1, node);
            node->paramNo = 0;
            do{
                semanticCheck(child);
                node->paramNo++;
                child = child->rsibling;
            } while(child != node->child);
            return;
        }

        //case NODE_STMTLIST:
    }

        /* Default action for other nodes not listed in the switch-case */
    struct Node *child = node->child;
    if(child != 0){
        do{
            semanticCheck(child);
            child = child->rsibling;
        } while(child != node->child);
    }
}

void printTable(){
    for(int tableno = 0; tableno < mScope; tableno++){
        if(tableno!=0){
            printf("********************************\n"
                   "*          OPEN SCOPE          *\n"
                   "********************************\n");
        }
        printf("+----------------------------------------------------------------------------------+\n");
        printf("   name   |    type    | scope | return |    parameter     | dim | Array range\n");
        printf("+----------------------------------------------------------------------------------+\n");

        for (int i=0; i<SymTBL.size; i++){
            //print symbol name
            if(SymTBL.Elemts[i].scope == tableno+1){

                printf("%8s  ", SymTBL.Elemts[i].name);
                
                //print type
                if (SymTBL.Elemts[i].type == TypeProc)    printf("   Procedure  ");
                else if(SymTBL.Elemts[i].isFunction == 1) printf("    Function  ");
                //else if(SymTBL.Elemts[i].arrDepth > 0)    printf("       Array  ");
                else if(SymTBL.Elemts[i].type == TypeInt) printf("     Integer  ");
                else if(SymTBL.Elemts[i].type == TypeReal)printf("  Realnumber  ");
                else if(SymTBL.Elemts[i].type == TypeString)printf("      String  ");
                //print scope
                printf(" %3d    ", SymTBL.Elemts[i].scope);

                // printf return 
                if(SymTBL.Elemts[i].isFunction == 1) {
                    if (SymTBL.Elemts[i].type == TypeInt)         printf("  Int    ");
                    else if (SymTBL.Elemts[i].type == TypeReal)   printf("  Real   ");
                    else if (SymTBL.Elemts[i].type == TypeString) printf(" String  ");
                    else printf("         ");
                }
                else printf("         ");
                
                //////////// printf parameter /////////////////
                if (SymTBL.Elemts[i].isFunction == 1){
                    printf("    (");

                    for(int k=0; k<SymTBL.Elemts[i].paramNo; k++){
                        if(k!=0) printf(",");
                        printf("%s",SymTBL.Elemts[SymTBL.Elemts[i].param_index_list[k]].name);
                    }
                    printf(")");
                    for(int p=0; p<13-2*SymTBL.Elemts[i].paramNo; p++){
                        printf(" ");
                    }
                }
                else printf("                  ");
                //printf("        %d         ", SymTBL.Elemts[i].paramNo);
                //printf(", dim: ");
                if(SymTBL.Elemts[i].arrDepth > 0)
                    printf(" %3d  ", SymTBL.Elemts[i].arrDepth);
                else printf("      ");
                //printf(", Array range: ");
                if (SymTBL.Elemts[i].arrDepth > 0){
                    for(int k=0; k<SymTBL.Elemts[i].arrDepth ; k++) {
                        printf(" (%2d,%2d)", SymTBL.Elemts[i].arrRangeList[k*2], SymTBL.Elemts[i].arrRangeList[k*2+1]);
                    }
                }
                else printf("             ");
                //printf("%d  \n", SymTBL.Elemts[i].paramNo);
                printf("\n");
            }

        }
        if(tableno!=0){
            printf("********************************\n"
                   "*          CLOSE SCOPE         *\n"
                   "********************************\n");
        }
    }
    if(semantic_error == 0){
        printf("********************************\n"
               "*      No semantic error!      *\n"
               "********************************\n");
    }
}