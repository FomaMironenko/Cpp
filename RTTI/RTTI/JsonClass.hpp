//
//  JsonClass.hpp
//  RTTI
//
//  Created by Foma Mironenko on 15.10.2020.
//

#ifndef JsonClass_hpp
#define JsonClass_hpp

#include <stdio.h>
#include <string>
#include <map>
#include <vector>

class Data;
class Json;
class Number;
class String;
class List;
class Map;


class Data
{
public:
    enum Type
    {
        Nu,
        St,
        Li,
        Ma,
        Fl
    };
    
    virtual bool less(const Data &) const = 0;
    virtual Type getType() const = 0;
};

class Json
{
public:
    Json () = default;
    Json (Data * data): data(data)
    {    }
    bool operator<(const Json) const;
    
private:
    Data::Type rootType() const;
    
    Data *data;
};


class Number: public Data
{
public:
    Number(int n): Data(), num(n)
    {    }
private:
    bool less(const Data & other) const;
    Data::Type getType() const;
    
    int num;
};

class String: public Data
{
public:
    String(std::string s): Data(), str(s)
    {    }
private:
    bool less(const Data & other) const;
    Data::Type getType() const;
    
    std::string str;
};

class List: public Data
{
public:
    List(std::vector<Data*> & dat): Data()
    {
        for (auto & it : dat)
        {
            list.push_back(Json(it));
        }
    }
private:
    bool less(const Data & other) const;
    Data::Type getType() const;
    
    std::vector<Json> list;
};

class Map: public Data
{
public:
    Map(std::map<std::string, Data*> & mp)
    {
        for (auto & it : mp)
        {
            map[it.first] = Json(it.second);
        }
    }
private:
    bool less(const Data & other) const;
    Data::Type getType() const;
    
    std::map<std::string, Json> map;
};

class Float: public Data
{
public:
    Float(float fl): Data(), fl(fl)
    {   }
private:
    bool less(const Data & other) const;
    Data::Type getType() const;
    
    float fl;
};


#endif /* JsonClass_hpp */
