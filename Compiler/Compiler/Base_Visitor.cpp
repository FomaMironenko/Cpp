
#include "Visitor.hpp"

std::string Visitor::visit(Object *obj)
{
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
            
        case(Object::Not):
            return visitNot((Not*)obj);
            
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
            
        default:
            assert(false && "unknown object type");
    }
    return "";
}

