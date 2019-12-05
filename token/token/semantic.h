
#pragma once
#include "Utilize.h"

void executePro(treeNode* s);
void executeStmt(treeNode* node);
void executeDeclar(treeNode* node, int lev);
value* executeExpr(treeNode* node);
void executeAssign(treeNode* node);
void executeIf(treeNode* node);
void executeWhile(treeNode* node, int lev);
void executeIn(treeNode* node, int lev);
void executeOut(treeNode* node, int lev);
void executeStateB(treeNode* node, int lev);
symbol* executeValue(treeNode* node, int level);
value* executeOp(tokenType opera, value* op1, value*  op2);
