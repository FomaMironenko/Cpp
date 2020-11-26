//
//  Visitor.cpp
//  Compiler
//
//  Created by Foma Mironenko on 26.11.2020.
//

#include "Visitor.hpp"

std::string C_compiler::visit(Object *obj)
{
    assert(obj->langType == Object::CPasc);
    switch (obj->getType())
    {
        case(Object::Decl):
            return visitDecl((Declaration*)obj);
            
        case(Object::Assi):
            return visitAssi((Assignment*)obj);
            
        case(Object::Blck):
            return visitBlock((Block*)obj);
            
        case(Object::Cond):
            return visitCond((Conditional*)obj);
            
        case(Object::Loop):
            return visitLoop((Loop*)obj);
            
        case(Object::Const):
            return visitConst((Const*)obj);
            
        case(Object::Var):
            return visitVar((Var*)obj);
            
        case(Object::Sum):
            return visitSum((Sum*)obj);
            
        case(Object::Prod):
            return visitProd((Prod*)obj);
            
        case(Object::Less):
            return visitLess((Less*)obj);
            
        case(Object::And):
            return visitAnd((And*)obj);
            
        case(Object::Or):
            return visitOr((Or*)obj);
            
        case(Object::Not):
            return visitNot((Not*)obj);
    }
}


std::string C_compiler::visitDecl(Declaration * decl)
{
    return tabs() + decl->type + " " + decl->name + ";\n";
}

std::string C_compiler::visitAssi(Assignment * assi)
{
    assert(assi->Nchildren == 1 &&
           Expression::classof(assi->children[0]));
    return tabs() + assi->var + " = " + visit(assi->children[0]) + ";\n";
}

std::string C_compiler::visitBlock(Block * block)
{
    std::string res;
    res += tabs() + "{\n";
    
    Object **children = block->children;
    indent++;
    for(unsigned i = 0; i < block->Nchildren; i++)
    {
        res += visit(children[i]);
    }
    indent--;
    res += tabs() + "}\n";
    
    return std::move(res);
}

std::string C_compiler::visitCond(Conditional * cond)
{
    std::string res;
    assert(cond->Nchildren == 2 ||
           cond->Nchildren == 3);
    Object **children = cond->children;
    assert(Expression::classof(children[0]) &&
           Statement::classof(children[1]));
    if(cond->Nchildren == 3) {
        assert(Statement::classof(children[2]));
    }
    
    res += tabs() + "if( " + visit(children[0]) + " ) {\n";
    indent++;
    res += visit(children[1]);
    indent--;
    res += tabs() + "}\n";
    if(cond->Nchildren == 3) {
        res += tabs() + "else {\n";
        indent++;
        res += visit(children[2]);
        indent--;
        res += tabs() + "}\n";
    }
    
    return std::move(res);
}

std::string C_compiler::visitLoop(Loop * loop)
{
    std::string res;
    assert(loop->Nchildren == 2);
    Object **children = loop->children;
    assert(Expression::classof(children[0]) &&
           Statement::classof(children[1]));
    
    res += tabs() + "while( " + visit(children[0]) + " ) {\n";
    indent++;
    res += visit(children[1]);
    indent--;
    res += tabs() + "}\n";
    
    return std::move(res);
}

std::string C_compiler::visitConst(Const * cnst)
{
    return std::to_string(cnst->value);
}

std::string C_compiler::visitVar(Var *var)
{
    return var->name;
}

std::string C_compiler::visitBinExpr(BinaryExpr * binExp)
{
    assert(binExp->Nchildren == 2);
    Object **children = binExp->children;
    assert(Expression::classof(children[0]) &&
           Expression::classof(children[1]));
    
    return  std::string("( ") + visit(children[0]) + " ) " + binExp->getOperation() +
            " ( " + visit(children[1]) + " )";
}

std::string C_compiler::visitSum(Sum * sum)
{
    return visitBinExpr(sum);
}

std::string C_compiler::visitProd(Prod * prod)
{
    return visitBinExpr(prod);
}

std::string C_compiler::visitLess(Less * less)
{
    return visitBinExpr(less);
}

std::string C_compiler::visitAnd(And * _and)
{
    return visitBinExpr(_and);
}

std::string C_compiler::visitOr(Or * _or)
{
    return visitBinExpr(_or);
}

std::string C_compiler::visitNot(Not * _not)
{
    assert(_not->Nchildren == 1);
    assert(Expression::classof(_not->children[0]));
    
    return std::string("!( ") + visit(_not->children[0]) + " )";
}
