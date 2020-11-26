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
protected:
    
    virtual std::string visit(Object *) = 0;
    
    std::string programm;
};

class C_compiler : public Visitor
{
public:
    std::string visit(Object *);
    
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


class Assembl_Compiler : public Visitor
{
public:
    std::string visit(Object *);
    
private:
    std::string visitDECL(DECL *);
    std::string visitMOVC(MOVC *);
    std::string visitMOV(MOV *);
    std::string visitADD(ADD *);
    std::string visitMUL(MUL *);
    
    std::string visitLESS(LESS *);
    std::string visitAND(AND *);
    std::string visitOR(OR *);
    std::string visitNOT(NOT *);
    std::string visitLABEL(LABEL *);
    std::string visitGOTOIF(GOTOIF *);
};




#endif /* Visitor_hpp */
