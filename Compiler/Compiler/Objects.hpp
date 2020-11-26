//
//  Objects.hpp
//  Compiler
//
//  Created by Foma Mironenko on 26.11.2020.
//

#ifndef Objects_hpp
#define Objects_hpp

#include <stdio.h>
#include <string>
#include <vector>


class Object
{
public:
    enum Lang {
        CPasc,
        Assembler
    } langType;
    
    enum Type {
        Decl,
        Assi,
        Blck,
        Cond,
        Loop,
        Const,
        Var,
        Sum,
        Prod,
        Less,
        And,
        Or,
        Not
    };
    
    Object(unsigned Nch) : Nchildren(Nch), children(new Object*[Nch])
    {    }
    virtual ~Object()
    {
        for(int i = 0; i < Nchildren; i++)
        {
            delete children[i];
        }
        delete [] children;
    }
    
    virtual Type getType() = 0;
    
    const unsigned Nchildren;
    Object **children;
};




class Statement : public Object
{
public:
    static bool classof(Object * obj) {
        return dynamic_cast<Statement*>(obj) != 0;
    }
    virtual Type getType() = 0;
};

class Declaration : public Statement
{
public:
    Type getType() {
        return Object::Decl;
    }
    
    std::string name;
    std::string type;
};

class Assignment : public Statement
{
public:
    Type getType() {
        return Object::Assi;
    }
    std::string var;
};

class Block : public Statement
{
public:
    Type getType() {
        return Object::Blck;
    }
};

class Conditional : public Statement
{
public:
    Type getType() {
        return Object::Cond;
    }
};

class Loop : public Statement
{
public:
    Type getType() {
        return Object::Loop;
    }
};



class Expression : public Object
{
public:
    static bool classof(Object * obj) {
        return dynamic_cast<Expression*>(obj) != 0;
    }
    virtual Type getType() = 0;
};


class Const : public Expression
{
public:
    Type getType() {
        return Object::Const;
    }
    float value;
};

class Var : public Expression
{
public:
    Type getType() {
        return Object::Var;
    }
    std::string name;
};

class BinaryExpr : public Expression
{
public:
    static bool classof(Object * obj) {
        return dynamic_cast<BinaryExpr*>(obj) != 0;
    }
    virtual std::string getOperation() = 0;
    virtual Type getType() = 0;
};

class Sum : public BinaryExpr
{
public:
    Type getType() {
        return Object::Sum;
    }
    std::string getOperation() { return "+"; }
};

class Prod : public BinaryExpr
{
public:
    Type getType() {
        return Object::Prod;
    }
    std::string getOperation() { return "*"; }
};

class Less : public BinaryExpr
{
public:
    Type getType() {
        return Object::Less;
    }
    std::string getOperation() { return "<"; }
};

class And : public BinaryExpr
{
public:
    Type getType() {
        return Object::And;
    }
    std::string getOperation() { return "&&"; }
};

class Or : public BinaryExpr
{
public:
    Type getType() {
        return Object::Or;
    }
    std::string getOperation() { return "||"; }
};

class Not : public Expression
{
public:
    Type getType() {
        return Object::Not;
    }
};




class DECL : public Statement
{
    
};

class MOVC : public Statement
{
    
};

class MOV : public Statement
{
    
};

class ADD : public Expression
{
    
};

class MUL : public Expression
{
    
};

class LESS : public Expression
{
    
};

class AND : public Expression
{
    
};

class OR : public Expression
{
    
};

class NOT : public Expression
{
    
};

class LABEL : public Statement
{
    
};

class GOTOIF : public Statement
{
    
};






#endif /* Objects_hpp */
