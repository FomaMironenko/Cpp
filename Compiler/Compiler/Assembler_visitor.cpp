
#include "Visitor.hpp"

//-----Assembl_compiler-----//

/* LOGIC OF VISITING EXPRESSIONS
 *
 * Each object of Assembl_compiler has a conunter Ntmp which counts
 * temporary variables. Each call of visit(Expression *expr) creates
 * some lines of assebler code which contain some declarations of
 * temporary variables. When a tmp var is created, Ntmp is incremented.
 * In the end of string returned from call visit(Expression *expr) there
 * is a new variable tmp{Ntmp} which actually equals the argument expression.
 */

std::string Assembl_compiler::compile(Program * prog)
{
    assert(prog);
    Object::globCacheLevel++;
    Assembl_compiler compiler;
    Block *body = prog->toBlock();
    std::string res = compiler.visit(body);
    delete body;
    return res;
}

std::string Assembl_compiler::visitDecl(Declaration *decl)
{
    assert(decl->Nchildren == 1 ||
           decl->Nchildren == 2);
    assert(decl->children[0]->ofclass<Var*>());
    
    Var *var = static_cast<Var*>(decl->children[0]);
    std::string res;
    res += var->type + " " + var->name + ";\n";
    if (decl->Nchildren == 2) {
        assert(decl->children[1]->ofclass<Expression*>());
        res += visit(decl->children[1]);
        // now the last tmp variable contains the expresion
        std::string var1 = "tmp" + std::to_string(Ntmp);
        res += std::string("MOV ") + var->name + " " + var1 + ";\n";
    }
    return res;
}

std::string Assembl_compiler::visitAssi(Assignment *assi)
{
    assert(assi->Nchildren == 2 &&
           assi->children[0]->ofclass<Var*>() &&
           assi->children[1]->ofclass<Expression*>());
    
    Var *var = static_cast<Var*>(assi->children[0]);
    std::string res;
    
    res += visit(assi->children[1]);
    // now "tmp{Ntmp}" contains the expression
    std::string var1 = std::string("tmp") + std::to_string(Ntmp);
    res += "MOV " + var->name + " " + var1 + ";\n";
    return res;
}

std::string Assembl_compiler::visitBlock(Block *blck)
{
    // cut block with empty lines
    std::string res;
    Object **children = blck->children;
    for(unsigned i = 0; i < blck->Nchildren; i++)
    {
        assert(children[i]->ofclass<Statement*>());
        res += visit(children[i]);
    }
    return res;
}

std::string Assembl_compiler::visitCond(Conditional *cond)
{
    assert(cond->Nchildren == 2 ||
           cond->Nchildren == 3);
    Object **children = cond->children;
    assert(children[0]->ofclass<Expression*>() &&
           children[1]->ofclass<Statement*>());
    
    std::string res = "\n";
    uint64_t curLabel = ++Nlab;
    std::string elseBr = "else" + std::to_string(curLabel);
    std::string end   = "end" + std::to_string(curLabel);
    
    res += visit(children[0]);
    std::string ifCond = std::string("tmp") + std::to_string(Ntmp);
    std::string doElse = std::string("tmp") + std::to_string(++Ntmp);
    res += "bool " + doElse + ";\n";
    res += "MOV " + doElse + " " + ifCond + ";\n";
    res += "NOT " + doElse + ";\n";
    
    res += "\nGOTOIF " + doElse + " " + elseBr + ";\n";
    res += visit(children[1]);
    res += "GOTOIF true " + end + "\n";
    res += "\nLABEL " + elseBr + ";\n";
    if(cond->Nchildren == 3) {
        assert(children[2]->ofclass<Statement*>());
        res += visit(children[2]);
    }
    res += "LABEL " + end + ";\n";
    return res;
}

std::string Assembl_compiler::visitLoop(Loop *loop)
{
    assert(loop->Nchildren == 2);
    Object **children = loop->children;
    assert(children[0]->ofclass<Expression*>() &&
           children[1]->ofclass<Statement*>());
    
    std::string res;
    uint64_t curLabel = ++Nlab;
    std::string begin = "begin" + std::to_string(curLabel);
    std::string end   = "end" + std::to_string(curLabel);
    
    res += "\nLABEL " + begin + ";\n";
    res += visit(children[0]);
    std::string loopCond  = std::string("tmp") + std::to_string(Ntmp);
    std::string terminate = std::string("tmp") + std::to_string(++Ntmp);
    res += "bool " + terminate + ";\n";
    res += "MOV " + terminate + " " + loopCond + ";\n";
    res += "NOT " + terminate + ";\n";
    res += "\nGOTOIF " + terminate + " " + end + ";\n";
    
    res += visit(children[1]);
    
    res += "GOTOIF true " + begin + "\n";
    res += "LABEL " + end + ";\n";
    return res;
}


std::string Assembl_compiler::visitConst(Const *cnst)
{
    std::string res;
    std::string var = std::string("tmp") + std::to_string(++Ntmp);
    res += "int " + var + ";\n";
    res += "MOVC " + var + " " + std::to_string(cnst->value) + ";\n";
    return res;
}

std::string Assembl_compiler::visitVar(Var *var)
{
    std::string res;
    std::string var1 = std::string("tmp") + std::to_string(++Ntmp);
    res += var->type + " " + var1 + ";\n";
    res += "MOV " + var1 + " " + var->name + ";\n";
    return res;
}

std::string Assembl_compiler::visitNot(Not *_not)
{
    assert(_not->Nchildren == 1);
    assert(_not->children[0]->ofclass<Expression*>());
    std::string res;
    res += visit(_not->children[0]);
    // the result of visit is now contained in "tmp{Ntmp}"
    std::string var1 = std::string("tmp") + std::to_string(Ntmp);
    res += "NOT " + var1 + ";\n";
    return res;
}


std::string Assembl_compiler::visitBinExpr(BinaryExpr *binExpr)
{
    assert(binExpr->Nchildren == 2 &&
           binExpr->children[0]->ofclass<Expression*>() &&
           binExpr->children[1]->ofclass<Expression*>());
    std::string res;
    
    // each call of visit for Expressions creates new tmp variables,
    // increasion Ntmp counter, and the result is contained in "tmp{Ntmp}"
    // 1. result of the first patr
    res += visit(binExpr->children[0]);
    std::string var1 = std::string("tmp") + std::to_string(Ntmp);
    // 2. result of the second part
    res += visit(binExpr->children[1]);
    std::string var2 = std::string("tmp") + std::to_string(Ntmp);
    // 3. total result
    std::string var3 = std::string("tmp") + std::to_string(++Ntmp);
    std::string type;
    switch(binExpr->getType())
    {
        case(Object::Sum):
        case(Object::Prod):
            type = "int ";
            break;
        case(Object::And):
        case(Object::Or):
        case(Object::Less):
            type = "bool ";
            break;
        default:
            assert(false && "wrong type of BinaryExpr");
    }
    res += type + " " + var3 + ";\n";
    char operation = binExpr->getOperation()[0];
    std::string OPER;
    switch (operation)
    {
        case '+':
            OPER = "ADD";
            break;
        case '*':
            OPER = "MUL";
            break;
        case('<'):
            OPER = "LESS";
            break;
        case('&'):
            OPER = "AND";
            break;
        case('|'):
            OPER = "OR";
            break;
        default:
            assert(false && "wrong type of BinaryExpr");
    }
    res += OPER + " " + var1 + " " + var2 + ";\n";
    res += "MOV " + var3 + " " + var1 + ";\n";
    return res;
}

std::string Assembl_compiler::visitSum(Sum *sum)
{
    return visitBinExpr(sum);
}

std::string Assembl_compiler::visitProd(Prod *prod)
{
    return visitBinExpr(prod);
}

std::string Assembl_compiler::visitLess(Less *less)
{
    return visitBinExpr(less);
}

std::string Assembl_compiler::visitAnd(And *_and)
{
    return visitBinExpr(_and);
}

std::string Assembl_compiler::visitOr(Or *_or)
{
    return visitBinExpr(_or);
}


//~~~~~Assembl_compiler~~~~~//

