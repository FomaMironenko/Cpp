//
//  main.cpp
//  Compiler
//
//  Created by Foma Mironenko on 26.11.2020.
//

#include <iostream>

#include "Visitor.hpp"
#include "Objects.hpp"

void visitorTest1(C_compiler & ccomp, Assembl_compiler & ascomp)
{
    Const *True = new Const(1);
    Var *x = new Var("x", "bool");
    Var *y = new Var("y", "bool");
    
    Declaration *decl1 = new Declaration(x, True);
    Declaration *decl2 = new Declaration(y);
    
    Not *notX = new Not(x);
    Assignment *assi = new Assignment(y, notX);
    
    Block programm({decl1, decl2, assi});
    
    std::cout << "\n\nC code:\n\n";
    std::cout << ccomp.visit(&programm);
    std::cout << "\n\n\nAssembler code:\n\n";
    std::cout << ascomp.visit(&programm);
    std::cout << "\n\n";
}

void visitorTest2(C_compiler & ccomp, Assembl_compiler & ascomp)
{
    Const *C42 = new Const(42);
    Const *C4  = new Const(4);
    Const *C10 = new Const(10);
    Var *x = new Var("x", "int");
    Var *y = new Var("y", "int");
    
    Declaration *decl1 = new Declaration(x, C42);
    Declaration *decl2 = new Declaration(y, C4);
    
    Sum *sumX = new Sum(x, C10);
    Assignment *assi = new Assignment(y, sumX);
    Less *cond = new Less(y, sumX);
    
    Conditional *it = new Conditional(cond, assi);
    
    Block programm({decl1, decl2, it});
    
    std::cout << "\n\nC code:\n\n";
    std::cout << ccomp.visit(&programm);
    std::cout << "\n\n\nAssembler code:\n\n";
    std::cout << ascomp.visit(&programm);
    std::cout << "\n\n";
}

void compileTest()
{
    Program prog;
    
    Const *C5  = new Const(5);
    Const *C10 = new Const(10);
    Const *C15 = new Const(15);
    Const *C1  = new Const(1);
    Var *x = new Var("x", "int");
    Var *y = new Var("y", "int");
    
    prog.addInstr(new Declaration(x, C5));
    prog.addInstr(new Declaration(y, C10));
    
    Less *cond1 = new Less(x, C15);
    Less *cond2 = new Less(y, C15);
    And  *cond  = new  And(cond1, cond2);
    
    Sum *addX = new Sum(x, C1);
    Sum *addY = new Sum(y, C1);
    Assignment *assiX = new Assignment(x, addX);
    Assignment *assiY = new Assignment(y, addY);
    Block *blck = new Block({assiX, assiY});
    Loop *loop = new Loop(cond, blck);
    
    prog.addInstr(loop);
    prog.addInstr(assiY);
    prog.addInstr(assiY);
    
    std::cout << "\n\nC code:\n\n";
    std::cout << C_compiler::compile(&prog);
    std::cout << "\n\n\nAssembler code:\n\n";
    std::cout << Assembl_compiler::compile(&prog);
    std::cout << "\n\n";
}

void cacheTest()
{
    Program prog;
    
    Const *C5  = new Const(5);
    Const *C10 = new Const(10);
    Const *C   = new Const(15);
    Const *C1  = new Const(1);
    Const *C0  = new Const(0);
    Var *x = new Var("x", "int");
    Var *y = new Var("y", "int");
    
    prog.addInstr(new Declaration(x, C5));
    prog.addInstr(new Declaration(y, C10));
    
    Less *cond = new Less(y, C);
    
    Sum *addX = new Sum(x, C1);
    Sum *addY = new Sum(y, C1);
    Assignment *assiX = new Assignment(x, addX);
    Assignment *assiY = new Assignment(y, addY);
    Block *blck = new Block({assiX, assiY});
    Loop *loop = new Loop(cond, blck);
    
    Assignment *reset = new Assignment(y, C0);
    
    prog.addInstr(loop);
    prog.addInstr(reset);
    prog.addInstr(loop);
    prog.addInstr(reset);
    
    
    std::cout << "\n\n";
    std::cout << C_compiler::compile(&prog) << std::endl;
    
    y->name = "z";
    C->value = 11;
    std::cout << "\n\n";
    std::cout << C_compiler::compile(&prog);
    std::cout << "\n\n\n";
}


int main()
{
    // to check memory usage uncomment the following line
    // Object::quiet = false;
    C_compiler ccomp;
    Assembl_compiler ascomp;
    
    // visitorTest1(ccomp, ascomp);
    // visitorTest2(ccomp, ascomp);
    // compileTest();
    cacheTest();
    std::cout << "allocated   Objects: " << Object::allocated << "\n";
    std::cout << "deallocated Objects: " << Object::deallocated << "\n\n";
    std::cout << "FINISHED TESTS\n\n";
}
