//
//  JsonClass.cpp
//  RTTI
//
//  Created by Foma Mironenko on 15.10.2020.
//

#include "JsonClass.hpp"

Data::Type Json::rootType() const
{
    return data->getType();
}

bool Json::operator<(Json other) const
{
    const std::type_info &t1 = typeid(*this->data);
    const std::type_info &t2 = typeid(*other.data);
    if(t1 != t2) {
        return t1.before(t2);
    }
    assert(this->rootType() == other.rootType());
    return this->data->less(*other.data);
}


Data::Type Number::getType() const
{
    return Data::Nu;
}
bool Number::less(const Data & other) const
{
    const Number & tmp = static_cast<const Number &>(other);
    return this->num < tmp.num;
}



Data::Type String::getType() const
{
    return Data::St;
}
bool String::less(const Data & other) const
{
    const String & tmp = static_cast<const String &>(other);
    return this->str < tmp.str;
}



Data::Type List::getType() const
{
    return Data::Li;
}
bool List::less(const Data & other) const
{
    const List & tmp = static_cast<const List &>(other);
    return this->list < tmp.list;
}



Data::Type Map::getType() const
{
    return Data::Ma;
}
bool Map::less(const Data & other) const
{
    const Map & tmp = static_cast<const Map &>(other);
    return this->map < tmp.map;
}


Data::Type Float::getType() const
{
    return Data::Fl;
}
bool Float::less(const Data & other) const
{
    const Float & tmp = static_cast<const Float &>(other);
    return this->fl < tmp.fl;
}
