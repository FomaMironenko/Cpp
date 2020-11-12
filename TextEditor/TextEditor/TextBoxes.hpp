

#ifndef TextBoxes_hpp
#define TextBoxes_hpp

#include <iostream>
#include <stdio.h>

#include <list>

#include <string>
#include <algorithm>



class Box;
class ArrayHandler;
class SubBox;
class CharBox;
class TableBox;
class FrameBox;
class ScriptBox;
class LineBox;


// represents a subarray
class ArrayHandler
{
public:
    ArrayHandler(unsigned, unsigned);
    ArrayHandler(ArrayHandler & other,
                 unsigned offsetH,
                 unsigned offsetW,
                 unsigned h,
                 unsigned w);
    ~ArrayHandler();
    
    char *operator[] (unsigned i);
    void fill(char c);
    void show();
    
    unsigned h;
    unsigned w;
    
private:
    
    bool master;
    unsigned offsetH;
    unsigned offsetW;
    char** source;
};


class Box
{
    friend class SubBox;
protected:
    
    Box() = delete;
    Box(unsigned h, unsigned w): h(h), w(w)
    {    }
    virtual ~Box() = default;
    
public:
    unsigned height() { return h; }
    unsigned width() { return w; }
    virtual void print(ArrayHandler & arr) = 0;
    
protected:
    const unsigned h, w;
};


class SubBox
{
public:
    SubBox() = delete;
    SubBox(Box *box, Box *parent, unsigned offsetH, unsigned offsetW):
        box(box),
        parent(parent),
        offH(offsetH),
        offW(offsetW)
    {    }
    ~SubBox()
    {
        delete box;
    }
    
    unsigned width() { return box->width(); }
    unsigned height() { return box->height(); }
    unsigned offsetH() { return offH; }
    unsigned offsetW() { return offW; }
    void setOffsetH(unsigned oh) { offH = oh; }
    void setOffsetW(unsigned ow) { offW = ow; }
    
    bool intersects(SubBox * other);
    void print(ArrayHandler & arr);
    Box *getBox() { return box; }
    Box *getParent() { return parent; }
    
private:
    Box *box;
    Box *parent;
    unsigned offH;
    unsigned offW;
};




class CharBox : public Box
{
    CharBox(char c = ' '): Box(1, 1), c(c)
    {   }
    
    char c;
public:
    ~CharBox() = default;
    static CharBox *create(char c) { return new CharBox(c); }
    
    void print(ArrayHandler & arr);
};


class TableBox : public Box
{
    typedef typename std::list<SubBox*>::iterator SBiter;
protected:
    TableBox() = delete;
    TableBox(unsigned h, unsigned w) : Box(h, w)
    {   }
    
public:
    // will call a destructor for all
    // SubBox es in elems, which will delete Boxes
    // contained in them
    ~TableBox();
    
    bool addBox(Box *newB,
                unsigned offsetH,
                unsigned offsetW,
                bool force = false);
    void print(ArrayHandler & arr);
    bool moveTo(SubBox *child, unsigned newH, unsigned newW);
    SubBox *operator[] (unsigned i);
    
    static TableBox *create(unsigned h, unsigned w) {
        return new TableBox(h, w);
    }
    
private:
    bool canInsert(SubBox *SB, bool force);
    
private:
    
    std::list<SubBox*> elems;
};

class FrameBox : public Box
{
    FrameBox() = delete;
    FrameBox(TableBox *son): Box(son->height() + 2, son->width() + 2)
    {
        data = new SubBox(son, this, 1, 1);
    }
    
public:
    ~FrameBox() { delete data; }
    
    void print(ArrayHandler & arr);
    
    static FrameBox *create(TableBox *son) {
        return new FrameBox(son);
    }
    
    TableBox *getTable() {
        return (TableBox*)data->getBox();
    }
    
private:
    SubBox *data;
};


class ScriptBox : public TableBox
{
    ScriptBox() = delete;
    ScriptBox(unsigned h): TableBox(h, 1)
    {   }
    ScriptBox(const std::string s);
    
    
public:
    ~ScriptBox() = default;
    static ScriptBox *create(const std::string s)
    {
        return new ScriptBox(s);
    }
};

class LineBox : public TableBox
{
    LineBox() = delete;
    LineBox(unsigned h): TableBox(1, h)
    {   }
    LineBox(const std::string s);
public:
    static LineBox *create(const std::string s)
    {
        return new LineBox(s);
    }
};


#endif /* TextBoxes_hpp */
