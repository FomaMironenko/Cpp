//
//  Objects.hpp
//  Compiler
//
//  Created by Foma Mironenko on 26.11.2020.
//

#ifndef Objects_hpp
#define Objects_hpp

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <type_traits>

class Programm;

class Object;

class Statement;
class Declaration;
class Assignment;
class Block;
class Conditional;
class Loop;

class Expression;
class Const;
class Var;
class Not;

class BinaryExpr;
class Sum;
class Prod;
class Less;
class And;
class Or;



class Object
{
    friend class Program;
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
    
    Object(unsigned Nch);
    virtual ~Object();
    
    virtual Type getType() = 0;
    
    template<class subClass>
    bool ofclass()
    {
        return (dynamic_cast<subClass>(this) != NULL);
    }
    
    
    const unsigned Nchildren;
    Object **children;
    static bool quiet;
protected:
    static std::map<Object *, unsigned> refs;
    void assignChild(unsigned i, Object *child);
};


class Statement : public Object
{
public:
    Statement(unsigned Nch): Object(Nch)
    {   }
    virtual Type getType() = 0;
};

class Declaration : public Statement
{
public:
    Declaration(::Var *var, Expression * expr = nullptr);
    Type getType() {
        return Object::Decl;
    }
};

class Assignment : public Statement
{
public:
    Assignment(::Var *var, Expression *expr);
    Type getType() {
        return Object::Assi;
    }
};

class Block : public Statement
{
public:
    Block(std::vector<Statement*>);
    Type getType() {
        return Object::Blck;
    }
};

class Conditional : public Statement
{
public:
    Conditional(Expression *, Statement * _if, Statement * _else = nullptr);
    Type getType() {
        return Object::Cond;
    }
};

class Loop : public Statement
{
public:
    Loop(Expression *, Statement *);
    Type getType() {
        return Object::Loop;
    }
};



class Expression : public Object
{
public:
    Expression(unsigned Nch): Object(Nch)
    {   }
    virtual Type getType() = 0;
};


class Const : public Expression
{
public:
    Const(int val);
    Type getType() {
        return Object::Const;
    }
    int value;
};

class Var : public Expression
{
public:
    Var(std::string _name, std::string _type);
    Type getType() {
        return Object::Var;
    }
    std::string type;
    std::string name;
};

class Not : public Expression
{
public:
    Not(Expression *);
    Type getType() {
        return Object::Not;
    }
};

class BinaryExpr : public Expression
{
public:
    BinaryExpr(Expression *, Expression *);
    virtual std::string getOperation() = 0;
    virtual Type getType() = 0;
};

class Sum : public BinaryExpr
{
public:
    Sum(Expression *, Expression *);
    Type getType() {
        return Object::Sum;
    }
    std::string getOperation() { return "+"; }
};

class Prod : public BinaryExpr
{
public:
    Prod(Expression *, Expression *);
    Type getType() {
        return Object::Prod;
    }
    std::string getOperation() { return "*"; }
};

class Less : public BinaryExpr
{
public:
    Less(Expression *, Expression *);
    Type getType() {
        return Object::Less;
    }
    std::string getOperation() { return "<"; }
};

class And : public BinaryExpr
{
public:
    And(Expression *, Expression *);
    Type getType() {
        return Object::And;
    }
    std::string getOperation() { return "&&"; }
};

class Or : public BinaryExpr
{
public:
    Or(Expression *, Expression *);
    Type getType() {
        return Object::Or;
    }
    std::string getOperation() { return "||"; }
};


class Program
{
public:
    Program() = default;
    ~Program();
    void addInstr(Statement *);
    Block *toBlock();
private:
    std::list<Statement*> instructions;
};


#endif /* Objects_hpp */
