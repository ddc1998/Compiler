#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "node.h"
#include "symTab.h"
#include "Codegen.h"

struct RegTable RegTBL;
int label_no = 0;
int regT_no = 2;
int size_input = 0;

int printInt_flag = 0;
int printReal_flag = 0;
int printString_flag = 0;
int subprog_dec_flag = 0;
int func_call_flag = 0;
int variable_type = 0;
int variable_posi = 0;

char *stringconst;
int float_const[10];
int float_cnt = 0;

int is_printReal = 0;
int is_Real = 0;

int while_cnt = 0;
int if_cnt = 0;

int GetBinary32( float value ){
    union{
         float input;   // assumes sizeof(float) == sizeof(int)
         int   output;
    }    data;
    data.input = value;
    return data.output;
}


FILE *pFile;

int openfile(){
    pFile = fopen("hw4.s","w");
    if(NULL==pFile){
        printf("open failure\n");
    }
    return 0;
    
}

int regTab(){
    RegTBL.size = SymTBL.size;
    int posi_cnt = 0;
    for (int i = 0; i < SymTBL.size; i++){
        strcpy(RegTBL.Elemts[i].name, SymTBL.Elemts[i].name);
        RegTBL.Elemts[i].posi_no = posi_cnt;
        RegTBL.Elemts[i].dim = 0;
        RegTBL.Elemts[i].para_no = 0;
        //set type
        if(SymTBL.Elemts[i].type == TypeProc){
            RegTBL.Elemts[i].type = TypeProc;
            RegTBL.Elemts[i].posi_no = -2;
        }
        else if (SymTBL.Elemts[i].isFunction == 1){ //function no position
            RegTBL.Elemts[i].type = TypeFunc;
            RegTBL.Elemts[i].posi_no = -1;
        }
        else if (SymTBL.Elemts[i].arrDepth > 0){ //array ->modify type,arrdep,dim,start_index
            RegTBL.Elemts[i].type = TypeArr;
            RegTBL.Elemts[i].dim = SymTBL.Elemts[i].arrDepth;
            int cnt = 1;
            for (int k = 0; k < SymTBL.Elemts[i].arrDepth; k++){
                RegTBL.Elemts[i].arr_dep[k] = SymTBL.Elemts[i].arrRangeList[k*2+1] - SymTBL.Elemts[i].arrRangeList[k*2]+1;
                RegTBL.Elemts[i].start_index[k] = SymTBL.Elemts[i].arrRangeList[k*2];
                cnt *= (SymTBL.Elemts[i].arrRangeList[k*2+1] - SymTBL.Elemts[i].arrRangeList[k*2]+1);
            }
            posi_cnt += cnt;
        }
        else if(SymTBL.Elemts[i].type == TypeInt || SymTBL.Elemts[i].type == TypeReal || SymTBL.Elemts[i].type == TypeString){
            RegTBL.Elemts[i].type = SymTBL.Elemts[i].type;
            posi_cnt += 1;
        }
        else{
            printf("default: set regTab>type:%d", SymTBL.Elemts[i].type);
        }
        //print regTab
        printf("name:%s type:%d posi:%d dim:%d para_no:%d\n", RegTBL.Elemts[i].name, RegTBL.Elemts[i].type, RegTBL.Elemts[i].posi_no, RegTBL.Elemts[i].dim, RegTBL.Elemts[i].para_no);
        if (SymTBL.Elemts[i].arrDepth > 0){
            printf("       arrdep:");
            for (int k=0; k<SymTBL.Elemts[i].arrDepth; k++){
                printf("[%d]", RegTBL.Elemts[i].arr_dep[k]);
            }
            printf("\n");
        }
        if (SymTBL.Elemts[i].arrDepth > 0){
            printf("       start_index:");
            for (int k=0; k<SymTBL.Elemts[i].arrDepth; k++){
                printf("[%d]", RegTBL.Elemts[i].start_index[k]);
            }
            printf("\n");
        }
    }
    printf("posi_cnt:%d\n", posi_cnt);
    //set para type
    int cmp = 0;
    for (int i = 0; i < SymTBL.size; i++){
        if(RegTBL.Elemts[i].type == TypeFunc || RegTBL.Elemts[i].type == TypeProc){
            for(int k=0; k<SymTBL.Elemts[i].paramNo; k++){
                for (int p=i+1; p < SymTBL.size; p++){
                    if(strcmp(SymTBL.Elemts[SymTBL.Elemts[i].param_index_list[k]].name,RegTBL.Elemts[p].name)==0){
                        RegTBL.Elemts[p].para_no=k+1;
                        RegTBL.Elemts[p].type=TypePara;
                        printf("name:%s type:%d posi:%d dim:%d para_no:%d\n", RegTBL.Elemts[p].name, RegTBL.Elemts[p].type, RegTBL.Elemts[p].posi_no, RegTBL.Elemts[p].dim, RegTBL.Elemts[p].para_no);
                    }
                }
            }
        }
    }
    return 0;
}

int findsymPOSI(struct Node *node){
    for (int i = 0; i < RegTBL.size; i++){
        if (strcmp(RegTBL.Elemts[i].name, node->string) == 0){
            if (RegTBL.Elemts[i].type != TypeArr){
                return RegTBL.Elemts[i].posi_no;
            }
            else{ //array variable
                int _no[50];//the called parament, ex. a[5][2][1]=1;
                int dig = 1;
                int position = 0;
                if (RegTBL.Elemts[i].dim == 5){
                    _no[0] = node->child->child->iValue;
                    _no[1] = node->child->child->rsibling->child->iValue;
                    _no[2] = node->child->child->rsibling->child->rsibling->child->iValue;
                    _no[3] = node->child->child->rsibling->child->rsibling->child->rsibling->child->iValue;
                    _no[4] = node->child->child->rsibling->child->rsibling->child->rsibling->child->rsibling->child->iValue;
                }
                else if (RegTBL.Elemts[i].dim == 4){
                    _no[0] = node->child->child->iValue;
                    _no[1] = node->child->child->rsibling->child->iValue;
                    _no[2] = node->child->child->rsibling->child->rsibling->child->iValue;
                    _no[3] = node->child->child->rsibling->child->rsibling->child->rsibling->child->iValue;
                }
                else if (RegTBL.Elemts[i].dim == 3){
                    _no[0] = node->child->child->iValue;
                    _no[1] = node->child->child->rsibling->child->iValue;
                    _no[2] = node->child->child->rsibling->child->rsibling->child->iValue;
                }
                else if (RegTBL.Elemts[i].dim == 2){
                    _no[0] = node->child->child->iValue;
                    _no[1] = node->child->child->rsibling->child->iValue;
                }
                else if (RegTBL.Elemts[i].dim == 1){
                    _no[0] = node->child->child->iValue;
                }
                int in_arr_posi = 0;
                if (RegTBL.Elemts[i].dim == 1){
                    in_arr_posi = _no[0] - RegTBL.Elemts[i].start_index[0];
                    return in_arr_posi + RegTBL.Elemts[i].posi_no;
                }
                else{
                    int digit[10];
                    for (int k = 0; k < RegTBL.Elemts[i].dim; k++){
                        digit[k] = _no[k] - RegTBL.Elemts[i].start_index[k];
                    }
                    for (int k = 0; k + 1 < RegTBL.Elemts[i].dim; k++){
                        for (int p = k + 1; p < RegTBL.Elemts[i].dim; p++){
                            digit[k] *= RegTBL.Elemts[i].arr_dep[p];
                        }
                    }
                    for (int k = 0; k < RegTBL.Elemts[i].dim; k++){
                        in_arr_posi += digit[k];
                    }
                    return in_arr_posi + RegTBL.Elemts[i].posi_no;
                }
            }
        }
    }
    return -100;
}

int findsymTYPE(struct Node *node){
    for (int i = 0; i < RegTBL.size; i++){
        if (strcmp(RegTBL.Elemts[i].name, node->string) == 0){
            return RegTBL.Elemts[i].type;
        }
    }
    return -100;
}

int findsymParaNo(struct Node *node){
    for (int i = 0; i < RegTBL.size; i++){
        if (strcmp(RegTBL.Elemts[i].name, node->string) == 0){
            return RegTBL.Elemts[i].para_no;
        }
    }
    return -100;
}

void Code_CallFuncName(struct Node *node){
    fprintf(pFile,"\n");
    func_call_flag=1;
    Code_expression_list(node->child,1);
    
    fprintf(pFile,"    call %s\n", node->string);
    if(findsymTYPE(node)==TypeFunc){
        fprintf(pFile,"    mv a5,a0\n");
    }
    func_call_flag=0;
}

void Code_CallFuncName_no_par(struct Node *node){
    if(subprog_dec_flag){
        fprintf(pFile,"    call %s\n", node->string);
    }
    else{
        fprintf(pFile,"    call %s\n", node->string);
        if(findsymTYPE(node)==TypeFunc){
            fprintf(pFile,"    mv a5,a0\n");
        }
    }
}


int have_real_child(struct Node *node){
    if(node->nodeType==REALNUM||(node->nodeType==Node_variable&&findsymTYPE(node)==TypeReal)){
        return 1;
    }
    int realexist = 0;
    struct Node *fchild = node->child;
    if (fchild != NULL){
        do{
            realexist = have_real_child(fchild);
            if(realexist){
                break;
            }
            fchild = fchild->rsibling;
        } while (fchild != node->child);
    }
    if(realexist){
        return 1;
    }
    else{
        return 0;
    }
}


int op_cnt =0;

void Code_expression(struct Node *node,int tNum){
    int optree_flag =0;
    switch (node->nodeType){
        //condittion expression
        case Node_AND:
            op_cnt++;
            Code_expression(node->child, tNum);
            Code_expression(node->child->rsibling, tNum+1);
            fprintf(pFile,"    and t%d,t%d,t%d\n", tNum, tNum, tNum+1);
            fprintf(pFile,"\n");
            op_cnt--;
            break;
        case Node_OR:
            op_cnt++;
            Code_expression(node->child, tNum);
            Code_expression(node->child->rsibling, tNum+1);
            fprintf(pFile,"    or t%d,t%d,t%d\n", tNum, tNum, tNum+1);
            fprintf(pFile,"\n");
            op_cnt--;
            break;
        case Node_OP:
            op_cnt++;//the level of operator_tree
            printf("node:%d op_cnt%d\n",node->op,op_cnt);
            if(have_real_child(node)){
                Code_expression(node->child, 0);
                if((node->op == OP_PLUS || node->op == OP_MINUS)&&
                    node->child->rsibling->nodeType == Node_OP &&
                   (node->child->rsibling->op==OP_STAR||node->child->rsibling->op==OP_SLASH)){
                    fprintf(pFile,"    mv	t6,a0\n");
                    optree_flag=1;
                }
                Code_expression(node->child->rsibling, 1);
            }
            else{
                Code_expression(node->child, tNum);
                Code_expression(node->child->rsibling, tNum+1);
            }
            switch (node->op){
                case OP_PLUS:
                    //if(node->child->nodeType==REALNUM){
                    if(have_real_child(node)){
                        if(optree_flag){
                            fprintf(pFile,"    mv	a0,t6\n");
                        }
                        fprintf(pFile,"    call __addsf3\n");
                        if(op_cnt>1){
                            fprintf(pFile,"    mv	a1,a0\n");
                        }
                        else{
                            fprintf(pFile,"    mv	a5,a0\n");
                        }
                    }
                    else{
                        fprintf(pFile,"    add t%d,t%d,t%d\n", tNum, tNum, tNum+1);
                    }
                    break;
                case OP_MINUS:
                    if(have_real_child(node)){
                        if(optree_flag){
                            fprintf(pFile,"    mv	a0,t6\n");
                        }
                        fprintf(pFile,"    call __subsf3\n");
                        if(op_cnt>1){
                            fprintf(pFile,"    mv	a1,a0\n");
                        }
                        else{
                            fprintf(pFile,"    mv	a5,a0\n");
                        }
                    }
                    else{
                        fprintf(pFile,"    sub t%d,t%d,t%d\n", tNum, tNum, tNum+1);
                    }
                    break;
                case OP_STAR:
                    if(have_real_child(node)){
                        fprintf(pFile,"    call __mulsf3\n");
                        if(op_cnt>1){
                            fprintf(pFile,"what %d\n",op_cnt);
                            fprintf(pFile,"    mv	a1,a0\n");
                        }
                        else{
                            fprintf(pFile,"    mv	a5,a0\n");
                        }
                    }
                    else{
                        fprintf(pFile,"    mul t%d,t%d,t%d\n", tNum, tNum, tNum+1);
                    }
                    break;
                case OP_SLASH:
                    if(have_real_child(node)){
                        fprintf(pFile,"    call __divsf3\n");
                        if(op_cnt>1){
                            fprintf(pFile,"    mv	a1,a0\n");
                        }
                        else{
                            fprintf(pFile,"    mv	a5,a0\n");
                        }
                    }
                    else{
                        fprintf(pFile,"    div t%d,t%d,t%d\n", tNum, tNum, tNum+1);
                    }
                    break;
                case OP_GT://>
                    fprintf(pFile,"    sgt t%d,t%d,t%d\n", tNum, tNum, tNum+1);
                    break;
                case OP_LT://<
                    fprintf(pFile,"    slt t%d,t%d,t%d\n", tNum, tNum, tNum+1);
                    break;
                case OP_GE://>=
                    fprintf(pFile,"    slt t%d,t%d,t%d\n", tNum, tNum, tNum+1);
                    fprintf(pFile,"    xori t%d,t%d,1\n", tNum, tNum);
                    break;
                case OP_LE://<=
                    fprintf(pFile,"    sgt t%d,t%d,t%d\n", tNum, tNum, tNum+1);
                    fprintf(pFile,"    xori t%d,t%d,1\n", tNum, tNum);
                    break;
                case OP_EQUAL:
                    fprintf(pFile,"    sub t%d,t%d,t%d\n", tNum, tNum, tNum+1);
                    fprintf(pFile,"    seqz t%d,t%d\n", tNum, tNum);
                    break;
                case OP_NEQUAL:
                    fprintf(pFile,"    sub t%d,t%d,t%d\n", tNum, tNum, tNum+1);
                    fprintf(pFile,"    snez t%d,t%d\n", tNum, tNum);
                    break;
            }
            fprintf(pFile,"\n");
            op_cnt--;
            break;
        case Node_variable:
            variable_posi = findsymPOSI(node);
            variable_type = findsymTYPE(node);
            //printf("posi:%d, type:%d\n",variable_posi,variable_type);
    
            if(variable_posi== -100){
                printf("default: Code_expression> findsymPOSI can't find.\n");
            }
            if(variable_type== -100){
                printf("default: Code_expression> findsymTYPE can't find.\n");
            }
            switch(variable_type){
                case TypeInt:
                    fprintf(pFile,"    lw t%d,%d(s1)\n", tNum, 4*variable_posi);
                    
                    break;
                case TypeReal:
                    if(op_cnt){
                        fprintf(pFile,"    lw a%d,%d(s1)\n", tNum, 4*variable_posi);
                    }
                    else if(printReal_flag){
                        fprintf(pFile,"    lw a0,%d(s1)\n", 4*variable_posi);
                    }
                    else{
                        fprintf(pFile,"    lw a5,%d(s1)\n", 4*variable_posi);
                    }
                    break;
                case TypeString:
                    fprintf(pFile,"    lw in string variable\n");
                    break;
                case TypeFunc:
                    fprintf(pFile,"    mv t%d,a0\n", tNum);
                    fprintf(pFile,"\n");
                    break;
                case TypeProc:
                    fprintf(pFile,"    lw t%d,%d(s1)\n", tNum, 4*variable_posi);
                    fprintf(pFile,"\n");
                    break;
                case TypeArr:
                    fprintf(pFile,"    lw t%d,%d(s1)\n", tNum, 4*variable_posi);
                    break;
                case TypePara:
                    fprintf(pFile,"    mv t%d,a%d\n", tNum, findsymParaNo(node)+2);
                    break;
            }
            break;
        case DIGNUM:
            fprintf(pFile,"    li t%d,%d\n", tNum, node->iValue);
            break;
        case REALNUM:
            float_const[float_cnt] = GetBinary32(node->rValue);
             
            fprintf(pFile,"    lui	a5,%%hi(.LC%d)\n",float_cnt);
	        fprintf(pFile,"    lw	a5,%%lo(.LC%d)(a5)\n",float_cnt);
            if(op_cnt){
                fprintf(pFile,"    mv a%d,a5\n", tNum);
            }
            else if(printReal_flag){
                fprintf(pFile,"    mv a0,a5\n");
            }
            else if(node->parent->nodeType == Node_ASSIGNOP){ }
            else{
                fprintf(pFile,"    mv t1,a5\n");
            }
            
            float_cnt++;
            break;
        case Node_CallFuncName:
            Code_CallFuncName(node);
            break;
        case Node_UOP:
            if(node->child->nodeType==DIGNUM){
                fprintf(pFile,"    li t%d,-%d\n", tNum, node->child->iValue);
            }
            else if(node->child->nodeType==REALNUM){
                float_const[float_cnt] = GetBinary32(0-(node->rValue));
                fprintf(pFile,"    lui	a5,%%hi(.LC%d)",float_cnt);
                fprintf(pFile,"    lw	a5,%%lo(.LC%d)(a5)",float_cnt);
                fprintf(pFile,"    \n");
                float_cnt++;
            }
            break;
        case Node_STRINGCONST:
            //printf("Node_stringconst:%s",node->string);
            stringconst = (char*)malloc(strlen(node->string)+1);
            strcpy(stringconst, node->string);
            break;
        default:
            printf("    default: Code_expression:%d\n", node->nodeType);
            break;
    }
}

void Code_expression_list(struct Node *node,int tNum){
    if(node->child!=NULL){
        //printf("expression_list nodeType:%d\n",node->child->nodeType);
        Code_expression(node->child,tNum);
        if(func_call_flag){//in factor
            fprintf(pFile,"    mv a%d,t%d\n", tNum+2, tNum);
        }
        else{//in printInt/printReal/printString

        }
        struct Node *nnode = node->child->rsibling;
        if (nnode != node->child){
            tNum+=1;
            do{
                //printf("expression_list nodeType:%d\n",node->child->nodeType);
                Code_expression(nnode,tNum);
                if(func_call_flag){
                    fprintf(pFile,"    mv a%d,t%d\n", tNum+2, tNum);
                }
                nnode = nnode->rsibling;
            } while (nnode != node->child);
        }
    }
}

void Code_ASSIGNOP(struct Node *node){
    //left child: variable
    //printf("assignop\n");
    int variable_posi = findsymPOSI(node->child);
    int variable_type = findsymTYPE(node->child);
    int expression_type = node->child->nodeType;
    //printf("posi:%d, type:%d, expression:%d\n",variable_posi,variable_type,expression_type);
    if(variable_posi== -100){
        printf("default: Code_ASSIGNOP> findsymPOSI can't find.\n");
    }
    if(variable_type== -100){
        printf("default: Code_ASSIGNOP> findsymTYPE can't find.\n");
    }
    //right child: expression
    int right_child_type =  node->child->rsibling->nodeType;
    //printf("right child:%d\n",right_child_type);
    Code_expression(node->child->rsibling, 1); //-> 1: t1
    //assign to variable
    switch(variable_type){
        case TypeInt:
            if(expression_type == Node_CallFuncName){
                fprintf(pFile,"    mv a0,t1\n");
            }
            if(right_child_type == Node_CallFuncName){
                fprintf(pFile,"    mv t1, a0\n");
            }
            fprintf(pFile,"    sw t1,%d(s1)\n", 4*variable_posi);
            break;
        case TypeReal:
            fprintf(pFile,"    sw a5,%d(s1)\n", 4*variable_posi);
            break;
        case TypeString:
            fprintf(pFile,"    assign to string_variable\n");
            break;
        case TypeFunc:
            fprintf(pFile,"    mv a0,t1\n");
            break;
        case TypeProc:
            fprintf(pFile,"    assign to procedure\n");
            break;
        case TypeArr:
            fprintf(pFile,"    sw t1,%d(s1)\n", 4*variable_posi);
            break;
        case TypePara:
            fprintf(pFile,"    mv a%d,t1\n", findsymParaNo(node)+2);
            break;
    }
    fprintf(pFile,"\n");
}


void Code_IF(struct Node *node){
    if_cnt+=1;
    //int condition_label = label_no;
    int true_label = label_no;
    int false_label = label_no+1;
    int out_label = label_no+2;
    //Node_IF:
    label_no+=3;
    Code_expression(node->child, 1);
    fprintf(pFile,"    bne t1,zero,.L%d\n", true_label); //to true
    fprintf(pFile,"    j .L%d",false_label);//to false
    fprintf(pFile,"\n");
    //TRUE Body:
    fprintf(pFile,".L%d:\n", true_label);
    Code_statement(node->child->rsibling);
    fprintf(pFile,"    j .L%d\n\n", out_label);
    //FALSE Body:
    fprintf(pFile,".L%d:\n", false_label);
    Code_statement(node->child->rsibling->rsibling);
    fprintf(pFile,"    j .L%d\n\n", out_label);
    //out loop
    fprintf(pFile,".L%d:\n", out_label);
    if_cnt-=1;
}

void Code_WHILE(struct Node *node){
    while_cnt+=1;
    int condition_label = 0;
    int in_side_label = 0;
    int out_side_label = 0;
    //printf("lsibling:%d\n", node->lsibling->nodeType);
    if (while_cnt==1){
        fprintf(pFile,"    j .L%d\n\n", label_no);
        fprintf(pFile,".L%d:\n", label_no);
        condition_label = label_no;
        in_side_label = label_no+1;
        out_side_label = label_no+2;
        label_no+=3;
    }
    else{
        condition_label = label_no-2;
        in_side_label = label_no;
        out_side_label = label_no+1;
        label_no+=2;
    }
    //condition expression
    Code_expression(node->child, 1);
    fprintf(pFile,"    bne t1,zero,.L%d\n", in_side_label); //in loop
    fprintf(pFile,"    j .L%d\n\n", out_side_label);   //out loop
    fprintf(pFile,"\n");
    //WHILE_BODY:
    fprintf(pFile,".L%d:\n", in_side_label);
    Code_statement(node->child->rsibling);
    fprintf(pFile,"    j .L%d\n\n", condition_label);
    fprintf(pFile,".L%d:\n", out_side_label);
    while_cnt-=1;
}

void Code_PRINTINT(struct Node *node){
    printInt_flag = 1;
    Code_expression_list(node->child,1);
    fprintf(pFile,"    mv a5,t1\n");
    fprintf(pFile,"    mv a1,a5\n");
    fprintf(pFile,"    lui a5,%%");
    fprintf(pFile,"hi(.LC0)\n");
    fprintf(pFile,"    addi a0,a5,%%");
    fprintf(pFile,"lo(.LC0)\n");
    fprintf(pFile,"    call printf\n");
    fprintf(pFile,"    li	a5,0\n");
	fprintf(pFile,"    mv	a0,a5\n");
}

void Code_PRINTREAL(struct Node *node){
    printReal_flag = 1;
    //printf("find node_printreal\n");
    Code_expression_list(node->child,1);
    fprintf(pFile,"    call __extendsfdf2\n");
    fprintf(pFile,"    mv a5,a0\n");
    fprintf(pFile,"    mv a1,a5\n");
    fprintf(pFile,"    lui a5,%%hi(.LC%d)\n",float_cnt);
    fprintf(pFile,"    addi a0,a5,%%lo(.LC%d)\n",float_cnt);
    fprintf(pFile,"    call printf\n");
    fprintf(pFile,"    li	a5,0\n");
	fprintf(pFile,"    mv	a0,a5\n");
}

void Code_PRINTSTRING(struct Node *node){
    printString_flag = 1;
    Code_expression_list(node->child,1);
    fprintf(pFile,"    lui a5,%%");
    fprintf(pFile,"hi(.LC2)\n");
    fprintf(pFile,"    addi a0,a5,%%");
    fprintf(pFile,"lo(.LC2)\n");
    fprintf(pFile,"    call printf\n");
    fprintf(pFile,"    li	a5,0\n");
    fprintf(pFile,"    mv	a0,a5\n");
}

void Code_statement(struct Node *node){
    switch(node->nodeType){
        case Node_ASSIGNOP:
            Code_ASSIGNOP(node);
            break;
        case Node_PRINTINT:
            Code_PRINTINT(node);
            break;
        case Node_PRINTREAL:
            Code_PRINTREAL(node);
            break;
        case Node_PRINTSTRING:
            Code_PRINTSTRING(node);
            break;
        case Node_CallFuncName:
            Code_CallFuncName(node);
            break;
        case Node_procedure_statement:
            Code_CallFuncName_no_par(node);
            break;
        case Node_compound_statement:
            Code_compound_statement(node);
            break;
        case Node_IF:
            Code_IF(node);
            break;
        case Node_WHILE:
            Code_WHILE(node);
            break;
        default:
            printf("default: Code_statement:%d\n", node->nodeType);
            break;
    }
}

void Code_statement_list(struct Node *node){
    if(node->child!=NULL){
        //printf("statement_list nodeType:%d\n",node->child->nodeType);
        Code_statement(node->child);
        struct Node *nnode = node->child->rsibling;
        if (nnode != node->child){
            do{
                //printf("statement_list nodeType:%d\n",nnode->nodeType);
                Code_statement(nnode);
                nnode = nnode->rsibling;
            } while (nnode != node->child);
        }
    }
}

void Code_compound_statement(struct Node *node){
    Code_statement_list(node->child);
    if (subprog_dec_flag){
        subprog_dec_flag -= 1;
        fprintf(pFile,"    ld ra,1016(sp)\n");
        fprintf(pFile,"    ld s0,1008(sp)\n");
        fprintf(pFile,"    ld s1,1000(sp)\n");
        fprintf(pFile,"    addi sp,sp,1024\n");
        fprintf(pFile,"    jr ra\n");
        fprintf(pFile,"    .size %s, .-%s\n", node->parent->child->string, node->parent->child->string);
    }
}

void Code_subprog_dec(struct Node *node){
    switch(node->nodeType){
        case FUNCTION:
            fprintf(pFile,"    .text\n");
            fprintf(pFile,"    .align 1\n");
            fprintf(pFile,"    .globl %s\n", node->child->string);
            fprintf(pFile,"    .type %s, @function\n", node->child->string);
            fprintf(pFile,"%s:\n", node->child->string);
            fprintf(pFile,"    addi sp,sp,-1024\n");
            fprintf(pFile,"    sd ra,1016(sp)\n");
            fprintf(pFile,"    sd s0,1008(sp)\n");
            fprintf(pFile,"    sd s1,1000(sp)\n");
            fprintf(pFile,"    addi s0,sp,40\n");
            fprintf(pFile,"    addi s1,sp,0\n");
            subprog_dec_flag += 1;
             //Node_compound_statement
            Code_compound_statement(node->child->rsibling->rsibling->rsibling->rsibling->rsibling);
            //inner Node_subprog_declaration 
            Code_subprog_declaration(node->child->rsibling->rsibling->rsibling->rsibling);
            break;
        case PROCEDURE:
            fprintf(pFile,"    .text\n");
            fprintf(pFile,"    .align 1\n");
            fprintf(pFile,"    .globl %s\n", node->child->string);
            fprintf(pFile,"    .type %s, @function\n", node->child->string);
            fprintf(pFile,"%s:\n", node->child->string);
            fprintf(pFile,"    addi sp,sp,-1024\n");
            fprintf(pFile,"    sd ra,1016(sp)\n");
            fprintf(pFile,"    sd s0,1008(sp)\n");
            fprintf(pFile,"    sd s1,1000(sp)\n");
            fprintf(pFile,"    addi s0,sp,40\n");
            fprintf(pFile,"    addi s1,sp,0\n");
            subprog_dec_flag += 1;
            //Node_compound_statement
            Code_compound_statement(node->child->rsibling->rsibling->rsibling->rsibling);
            //inner Node_subprog_declaration 
            Code_subprog_declaration(node->child->rsibling->rsibling->rsibling);
            break;
    }
}

void Code_subprog_declaration(struct Node *node){
    if(node->child!=NULL){
        Code_subprog_dec(node->child);
        struct Node *nnode = node->child->rsibling;
        if (nnode != node->child){
            do{
                Code_subprog_dec(nnode);
                nnode = nnode->rsibling;
            } while (nnode != node->child);
        }
    }
}

void Codegen(struct Node *node){
    switch (node->nodeType){ 
        case Node_prog:
            openfile();
            regTab();
            fprintf(pFile,"\n    .text\n");
            fprintf(pFile,"    .section .rodata\n");
            fprintf(pFile,"    .align 3\n\n");
            if(is_Real){
                fprintf(pFile,"    .globl	__addsf3\n");
                fprintf(pFile,"    .globl	__subsf3\n");
                fprintf(pFile,"    .globl	__mulsf3\n");
                fprintf(pFile,"    .globl	__divsf3\n");
            }
            if((is_printReal)){
                fprintf(pFile,"    .globl __extendsfdf2\n");
            }

            fprintf(pFile,"    .text\n");
            fprintf(pFile,"    .align 1\n");
            fprintf(pFile,"    .globl main\n");
            fprintf(pFile,"    .type main, @function\n");
            fprintf(pFile,"\n");
            break;
        case Node_FUNC_DECL:
            Code_subprog_declaration(node->rsibling);
            break;
        case Node_MAIN_FUNC:
            fprintf(pFile,"main:\n");
            fprintf(pFile,"    addi sp,sp,-1024\n");
            fprintf(pFile,"    sd ra,1016(sp)\n");
            fprintf(pFile,"    sd s0,1008(sp)\n");
            fprintf(pFile,"    sd s1,1000(sp)\n");
            fprintf(pFile,"    addi s0,sp,40\n");
            fprintf(pFile,"    addi s1,sp,0\n");
            Code_statement_list(node->rsibling->child);//Node_MAIN_FUNC->Node_compound_statement->statement_list
            break;
        case Node_DOT:
            fprintf(pFile,"\n    li a5,0\n");
            fprintf(pFile,"    mv a0,a5\n");
            fprintf(pFile,"    ld ra,1016(sp)\n");
            fprintf(pFile,"    ld s0,1008(sp)\n");
            fprintf(pFile,"    ld s1,1000(sp)\n");
            fprintf(pFile,"    addi sp,sp,1024\n");
            fprintf(pFile,"    jr ra\n");
            if (printInt_flag){
                fprintf(pFile,".LC0:\n");
                fprintf(pFile,"    .string \"%%");
                fprintf(pFile,"d\\");
                fprintf(pFile,"n\"\n");
            }
            if (is_printReal){
                for(int k=0; k<float_cnt; k++){
                    fprintf(pFile,".LC%d:\n", k);
                    fprintf(pFile,"    .word %d\n",float_const[k]);
                }
                fprintf(pFile,".LC%d:\n", float_cnt);
                fprintf(pFile,"    .string \"%%f\\n\"\n");
            }
            if (printString_flag){
                fprintf(pFile,".LC2:\n");
                fprintf(pFile,"    .string ");
                fprintf(pFile,"%s\n",stringconst);
            }
            fprintf(pFile,"    .size main, .-main\n");
            fprintf(pFile,"\n");
            fclose(pFile);
            break;
        
        default:
            printf("    default: Codegen %d\n", node->nodeType);
            break;
    }
}

void printCode(struct Node *node, int ident){
    static char blank[1024];
    for (int i = 0; i < ident; i++)
        blank[i] = ' ';
    blank[ident] = 0;
    switch (node->nodeType){
    case Node_prog:
        //printf("%sPROGRAM\n", blank);
        Codegen(node);
        ident += 2;
        break;
    case Node_FUNC_DECL:
        Codegen(node);
        break;
    case Node_MAIN_FUNC:
        Codegen(node);
        break;
    case Node_DOT:
        Codegen(node);
        break;
    case FUNCTION:
        //printf("%sFUNCTION\n", blank);
        ident += 2;
        break;
    case PROCEDURE:
        //printf("%sPROCEDURE\n", blank);
        ident += 2;
        break;
    case Node_compound_statement:
        //printf("%sCOMPOUND_STMT\n", blank);
        ident += 2;
        break;
    case Node_statement_list:
        //printf("%sSTMT_LIST\n", blank);
        ident += 2;
        break;
    case Node_ASSIGNOP:
        //printf("%s:=\n", blank);
        ident += 2;
        break;
    case Node_IF:
        //printf("%sIF\n", blank);
        ident += 2;
        break;
    case Node_WHILE:
        //printf("%sWHILE\n", blank);
        break;
    case Node_expression_list:
        //printf("%sexpression_list\n", blank);
        ident += 2;
        break;
    case Node_CallFuncName:
        //printf("call function\n");
        break;
    case Node_PRINTINT:
        //printf("call printInt\n");
        break;
    case Node_PRINTREAL:
        //printf("call printREAL\n");
        break;
    case Node_PRINTSTRING:
        //printf("call printSTRING\n");
        break;
    case Node_subprog_declarations:
        break;
    /*
        default:
            //printf("%sdefault:printCode %d\n", blank, node->nodeType);
            break;*/
    case Node_arguments:
        //printf("%spara_list\n", blank);
        ident += 2;
        break;
    case Node_OP:
        switch (node->op)
        {
        case OP_PLUS:
            //printf("%s+\n", blank);
            break;
        case OP_MINUS:
            //printf("%s-\n", blank);
            break;
        case OP_STAR:
            //printf("%s*\n", blank);
            break;
        case OP_SLASH:
            //printf("%s/\n", blank);
            break;
        case OP_GT:
            //printf("%s>\n", blank);
            break;
        case OP_LT:
            //printf("%s<\n", blank);
            break;
        case OP_GE:
            //printf("%s>=\n", blank);
            break;
        case OP_LE:
            //printf("%s<=\n", blank);
            break;
        case OP_EQUAL:
            //printf("%s=\n", blank);
            break;
        case OP_NEQUAL:
            //printf("%s!=\n", blank);
            break;
        }
        ident += 2;
        break;
    case Node_id_list:
        break;
    case Node_declarations:
        break;
    case Node_UOP:
        //printf("%sUnary\n", blank);
        break;
    case Node_VAR:
        //printf("%sVAR_DECL\n", blank);
        ident += 2;
        break;
    case Node_INTEGER:
        //printf("%sTYPE_INT\n", blank);
        break;
    case Node_REAL:
        //printf("%sTYPE_REAL\n", blank);
        break;
    case Node_NOT:
        //printf("%sNOT\n", blank);
        break;
    case Node_STRING:
        //printf("%sTYPE_STRING\n", blank);
        break;
    case Node_AND:
        //printf("%sAND\n", blank);
        break;
    case Node_OR:
        //printf("%sOR\n", blank);
        break;
    case Node_ID:
        //printf("%s%s\n", blank, node->string);
        ident += 2;
        break;
    case DIGNUM:
        //printf("%s%d\n", blank, node->iValue);
        break;
    case REALNUM:
        //printf("%s%f\n", blank, node->rValue);
        break;
    case Node_num:
        break;
    case Node_STRINGCONST:
        //printf("%s%s\n", blank, node->string);
        break;
    case Node_VAR_DECL:
        //printf("%sVAR_DECL\n", blank);
        ident += 2;
        break;
    case Node_para_list:
        //printf("%spara_list\n", blank);
        ident += 2;
        break;
    case Node_optional_var:
        //printf("%soption_var\n", blank);
        ident += 2;
        break;
    case Node_tail:
        //printf("%stail\n", blank);
        ident += 2;
        break;
    case Node_variable:
        //printf("%svariable\n", blank);
        ident += 2;
        break;
    case Node_procedure_statement:
        //printf("%sprocedure_statement\n", blank);
        ident += 2;
        break;
    case Node_addop:
        //printf("%saddop\n", blank);
        ident += 2;
        break;
    case Node_mulop:
        //printf("%smulop\n", blank);
        ident += 2;
        break;
    case Node_relop:
        //printf("%srelop\n", blank);
        ident += 2;
        break;
    case ARRAY:
        //printf("%sARRAY\n", blank);
        ident += 2;
        break;
    case REAL:
        //printf("%sTYPE_REAL\n", blank);
        break;
    case INTEGER:
        //printf("%sTYPE_INT\n", blank);
        break;
    case STRING:
        //printf("%sTYPE_STR\n", blank);
        break;
    }
    struct Node *child = node->child;
    if (child != NULL){
        do{
            printCode(child, ident);
            child = child->rsibling;
        } while (child != node->child);
    }
}