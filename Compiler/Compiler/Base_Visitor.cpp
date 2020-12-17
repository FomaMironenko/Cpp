
#include "Visitor.hpp"

std::string Visitor::visit(Object *obj)
{
    if(!obj->cache.empty() &&
       obj->cacheLevel == Object::globCacheLevel)
    {
        return obj->cache;
    }
    
    std::string res;
    switch (obj->getType())
    {
        case(Object::Decl):
            res = visitDecl((Declaration*)obj);
            break;
            
        case(Object::Assi):
            res = visitAssi((Assignment*)obj);
            break;
            
        case(Object::Blck):
            res = visitBlock((Block*)obj);
            break;
            
        case(Object::Cond):
            res = visitCond((Conditional*)obj);
            break;
            
        case(Object::Loop):
            res = visitLoop((Loop*)obj);
            break;
            
        case(Object::Const):
            res = visitConst((Const*)obj);
            break;
            
        case(Object::Var):
            res = visitVar((Var*)obj);
            break;
            
        case(Object::Not):
            res = visitNot((Not*)obj);
            break;
            
        case(Object::Sum):
            res = visitSum((Sum*)obj);
            break;
            
        case(Object::Prod):
            res = visitProd((Prod*)obj);
            break;
            
        case(Object::Less):
            res = visitLess((Less*)obj);
            break;
            
        case(Object::And):
            res = visitAnd((And*)obj);
            break;
            
        case(Object::Or):
            res = visitOr((Or*)obj);
            break;
            
        default:
            assert(false && "unknown object type");
    }
    obj->cacheLevel = Object::globCacheLevel;
    return (obj->cache = res);
}

