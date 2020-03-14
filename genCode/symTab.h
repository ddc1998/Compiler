#ifndef _SYMTAB_H__
#define _SYMTAB_H__

enum StdType
{
    TypeInt,
    TypeReal,
    TypeString,
    TypeFunc,
    TypeProc,
    TypeArr,
    TypePara
};




struct TabElemt
{
    char name[100];
    enum StdType type;
    int arrDepth;
    int indexStart;
    int indexEnd;
    int isFunction;
    int scope;
    int paramNo;
    char valueValid;
    int arrRangeList[100];
    int param_index_list[100];
};

struct SymTable
{
    int size;
    struct TabElemt Elemts[100];
};



extern struct SymTable SymTBL;

#include "node.h"

void semanticCheck(struct Node *node);
void printTable();

#endif