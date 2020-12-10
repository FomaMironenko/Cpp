//
//  Visitor.hpp
//  Compiler
//
//  Created by Foma Mironenko on 26.11.2020.
//

#ifndef Visitor_hpp
#define Visitor_hpp

#include "Objects.hpp"

#include <stdio.h>
#include <string>


class Visitor
{
public:
    
    std::string visit(Object *);
    
protected:
    virtual std::string visitDecl(Declaration *) = 0;
    virtual std::string visitAssi(Assignment *) = 0;
    virtual std::string visitBlock(Block *) = 0;
    virtual std::string visitCond(Conditional *) = 0;
    virtual std::string visitLoop(Loop *) = 0;
    
    virtual std::string visitConst(Const *) = 0;
    virtual std::string visitVar(Var *) = 0;
    
    virtual std::string visitBinExpr(BinaryExpr *) = 0;
    virtual std::string visitSum(Sum *) = 0;
    virtual std::string visitProd(Prod *) = 0;
    virtual std::string visitLess(Less *) = 0;
    virtual std::string visitAnd(And *) = 0;
    virtual std::string visitOr(Or *) = 0;
    virtual std::string visitNot(Not *) = 0;
    
    std::string programm;
};



class C_compiler : public Visitor
{
private:
    std::string visitDecl(Declaration *);
    std::string visitAssi(Assignment *);
    std::string visitBlock(Block *);
    std::string visitCond(Conditional *);
    std::string visitLoop(Loop *);
    
    std::string visitConst(Const *);
    std::string visitVar(Var *);
    
    std::string visitBinExpr(BinaryExpr *);
    std::string visitSum(Sum *);
    std::string visitProd(Prod *);
    std::string visitLess(Less *);
    std::string visitAnd(And *);
    std::string visitOr(Or *);
    std::string visitNot(Not *);
    
    std::string tabs()
    {
        return std::string(indent, '\t');
    }
    
    unsigned indent = 0;
};


class Assembl_compiler : public Visitor
{
private:
    
    std::string visitDecl(Declaration *);
    std::string visitAssi(Assignment *);
    std::string visitBlock(Block *);
    std::string visitCond(Conditional *);
    std::string visitLoop(Loop *);
    
    std::string visitConst(Const *);
    std::string visitVar(Var *);
    
    //rewrites an expression with new tmp vars
    std::string visitExpr(Expression *);
    std::string visitBinExpr(BinaryExpr *);
    std::string visitSum(Sum *);
    std::string visitProd(Prod *);
    std::string visitLess(Less *);
    std::string visitAnd(And *);
    std::string visitOr(Or *);
    std::string visitNot(Not *);
    
    uint64_t Ntmp = 0;
    uint64_t Nlab = 0;
};




#endif /* Visitor_hpp */
