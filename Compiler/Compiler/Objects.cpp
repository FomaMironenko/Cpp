//
//  Objects.cpp
//  Compiler
//
//  Created by Foma Mironenko on 17.12.2020.
//

#include <stdio.h>
#include "Objects.hpp"

std::map<Object*, unsigned> Object::refs;
bool Object::quiet = true;
unsigned Object::globCacheLevel = 0;
unsigned Object::allocated = 0;
unsigned Object::deallocated = 0;


void Program::addInstr(Statement * stat)
{
    assert(stat);
    Object::refs[stat]++;
    instructions.push_back(stat);
}

Block *Program::toBlock()
{
    std::vector<Statement *> tmp(instructions.size());
    auto cur = instructions.begin();
    for (int i = 0; i < instructions.size(); i++)
    {
        tmp[i] = *cur;
        cur = ++cur;
    }
    // will automatically update Object::refs
    return new Block(tmp);
}

Program::~Program()
{
    auto cur = instructions.begin();
    auto end = instructions.end();
    for( ; cur != end; ++cur)
    {
        auto ref = Object::refs.find(*cur);
        assert(ref != Object::refs.end() &&
               ref->second != 0);
        ref->second--;
        if(ref->second == 0) {
            delete ref->first;
        }
    }
}



Object::Object(unsigned Nch) :
    Nchildren(Nch),
    children(new Object*[Nch]),
    cacheLevel(globCacheLevel)
{
    allocated++;
    if(!quiet) {
        std::cout << "allocated " << this << "\n";
    }
    refs[this] = 0;
}

Object::~Object()
{
    for(int i = 0; i < Nchildren; i++)
    {
        auto ref = refs.find(children[i]);
        assert(ref != refs.end() &&
               ref->second != 0);
        ref->second--;
        if(ref->second == 0) {
            delete ref->first;
            refs.erase(ref);
        }
    }
    delete [] children;
    deallocated++;
    if(!quiet){
        std::cout << "deallocated " << this << "\n";
    }
}

void Object::assignChild(unsigned i, Object *child)
{
    assert(child &&
           i < Nchildren);
    children[i] = child;
    refs[child]++;
}

Declaration::Declaration(::Var *var, Expression *expr): Statement(expr ? 2 : 1)
{
    assignChild(0, var);
    if(expr) {
        assignChild(1, expr);
    }
}

Assignment::Assignment(::Var *var, Expression *expr): Statement(2)
{
    assignChild(0, var);
    assignChild(1, expr);
}

Block::Block(std::vector<Statement*> stats): Statement(stats.size())
{
    for(int i = 0; i < Nchildren; i++)
    {
        assignChild(i, stats[i]);
    }
}

Conditional::Conditional(Expression *cond, Statement *_if, Statement *_else):
    Statement(_else ? 3 : 2)
{
    assignChild(0, cond);
    assignChild(1, _if);
    if(_else) {
        assignChild(2, _else);
    }
}

Loop::Loop(Expression * expr, Statement * stat): Statement(2)
{
    assignChild(0, expr);
    assignChild(1, stat);
}

Const::Const(int val): Expression(0), value(val)
{   }

Var::Var(std::string name, std::string type): Expression(0), name(name), type(type)
{
    assert((type == "int") || (type == "bool"));
}

Not::Not(Expression *expr): Expression(1)
{
    assignChild(0, expr);
}

BinaryExpr::BinaryExpr(Expression *expr1, Expression *expr2):
        Expression(2)
{
    assignChild(0, expr1);
    assignChild(1, expr2);
}

#define defBinExpr(Type) \
Type::Type(Expression *expr1, Expression *expr2): BinaryExpr(expr1, expr2) \
{   }

defBinExpr(Sum)

defBinExpr(Prod)

defBinExpr(Less)

defBinExpr(And)

defBinExpr(Or)

