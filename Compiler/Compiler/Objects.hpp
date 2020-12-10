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
#include <type_traits>


class Object
{
public:
    
    enum Type {
    // statements
        Decl,
        Assi,
        Blck,
        Cond,
        Loop,
    // Expressions
        Const,
        Var,
        Not,
    // Binary Expressions
        Sum,
        Prod,
        Less,
        And,
        Or
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
    
    template<class subClass>
    bool ofclass()
    {
        return (dynamic_cast<subClass>(this) != NULL);
    }
    
    const unsigned Nchildren;
    Object **children;
};


class Statement : public Object
{
public:
    virtual Type getType() = 0;
};

class Declaration : public Statement
{
public:
    Type getType() {
        return Object::Decl;
    }
};

class Assignment : public Statement
{
public:
    Type getType() {
        return Object::Assi;
    }
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
    virtual Type getType() = 0;
};


class Const : public Expression
{
public:
    Type getType() {
        return Object::Const;
    }
    int value;
};

class Var : public Expression
{
public:
    Type getType() {
        return Object::Var;
    }
    std::string type;
    std::string name;
};

class Not : public Expression
{
public:
    Type getType() {
        return Object::Not;
    }
};

class BinaryExpr : public Expression
{
public:
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


#endif /* Objects_hpp */
