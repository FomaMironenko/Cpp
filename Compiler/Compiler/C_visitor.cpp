
#include "Visitor.hpp"

//-----C_compiler-----//

std::string C_compiler::visitDecl(Declaration * decl)
{
    assert(decl->Nchildren == 1 ||
           decl->Nchildren == 2);
    assert(decl->children[0]->ofclass<Var*>());
    Var *var = static_cast<Var*>(decl->children[0]);
    
    std::string res;
    res += tabs() + var->type + " " + var->name + ";\n";
    if(decl->Nchildren == 2) {
        assert(decl->children[1]->ofclass<Expression*>());
        res += tabs() + var->name + " = " + visit(decl->children[1]) + ";\n";
    }
    return res;
}

std::string C_compiler::visitAssi(Assignment * assi)
{
    assert(assi->Nchildren == 2 &&
           assi->children[0]->ofclass<Var*>() &&
           assi->children[1]->ofclass<Expression*>());
    Var *var = static_cast<Var*>(assi->children[0]);
    return tabs() + var->name + " = " + visit(assi->children[1]) + ";\n";
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
    
    return res;
}

std::string C_compiler::visitCond(Conditional * cond)
{
    std::string res;
    assert(cond->Nchildren == 2 ||
           cond->Nchildren == 3);
    Object **children = cond->children;
    assert(children[0]->ofclass<Expression*>() &&
           children[1]->ofclass<Statement*>());
    
    res += tabs() + "if( " + visit(children[0]) + " ) {\n";
    indent++;
    res += visit(children[1]);
    indent--;
    res += tabs() + "}\n";
    if(cond->Nchildren == 3) {
        assert(children[2]->ofclass<Statement*>());
        res += tabs() + "else {\n";
        indent++;
        res += visit(children[2]);
        indent--;
        res += tabs() + "}\n";
    }
    
    return res;
}

std::string C_compiler::visitLoop(Loop * loop)
{
    std::string res;
    assert(loop->Nchildren == 2);
    Object **children = loop->children;
    assert(children[0]->ofclass<Expression*>() &&
           children[1]->ofclass<Statement*>());
    
    res += tabs() + "while( " + visit(children[0]) + " ) {\n";
    indent++;
    res += visit(children[1]);
    indent--;
    res += tabs() + "}\n";
    
    return res;
}


std::string C_compiler::visitConst(Const * cnst)
{
    return std::to_string(cnst->value);
}

std::string C_compiler::visitVar(Var *var)
{
    return var->name;
}

std::string C_compiler::visitNot(Not * _not)
{
    assert(_not->Nchildren == 1);
    assert(_not->children[0]->ofclass<Expression*>());
    
    return std::string("!( ") + visit(_not->children[0]) + " )";
}


std::string C_compiler::visitBinExpr(BinaryExpr * binExp)
{
    assert(binExp->Nchildren == 2);
    Object **children = binExp->children;
    assert(children[0]->ofclass<Expression*>() &&
           children[1]->ofclass<Expression*>());
    
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


//~~~~~C_compiler~~~~~//

