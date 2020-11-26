//
//  Handles.hpp
//  TextEditor
//
//  Created by Foma Mironenko on 19.11.2020.
//

#ifndef Handles_hpp
#define Handles_hpp

#include <stdio.h>
#include <string>
#include <algorithm>
#include <vector>
#include <array>


class TextEditor;

class Viewer;
class Scroll;
class Text;

class Lines;

class Viewer
{
protected:
    Viewer(unsigned Npos = 1): position(0), Npos(Npos)
    {
        assert(Npos != 0);
    }
    unsigned position;
    // number of avaliable positions
    // invariant: position >= 0 && position < Npos
    unsigned Npos;
    
    virtual bool setPos(unsigned pos) = 0;
};


class Scroll : public Viewer
{
    friend class TextEditor;
    friend class Lines;
public:
    Scroll(unsigned h = 1): Viewer(1), height(h), firstLine(0)
    {   }
    bool setPos(unsigned pos);
    void setH(unsigned h);
    void click(unsigned i);
    
    unsigned firstLine;
    unsigned height;
};


class Lines
{
    friend class TextEditor;
public:
    Lines(const Text *con): consumer(con), scrl()
    {   }
    
    void render();
    void setW(unsigned w);
    unsigned click(unsigned i, unsigned j);
    
    const Text *consumer;
    Scroll scrl;
    unsigned width;
    
    std::vector<std::array<unsigned, 2> > lines;
};


class Text : public Viewer
{
    friend class TextEditor;
    friend class Lines;
public:
    Text(): Viewer(), helper(this)
    {   }
    Text(std::string txt): Viewer(txt.size()), helper(this), str(txt)
    {   }
    void insert(char);
    void clear();
    bool setPos(unsigned pos);
    void setText(std::string txt);
    void click(unsigned i, unsigned j);
private:
    Lines helper;
    std::string str;
};

struct Coursor
{
    friend class TextEditor;
public:
    Coursor() : textInd(0), scrlInd(0), curH(0), curW(0)
    {   }
    
    unsigned textInd;
    unsigned scrlInd;
    unsigned curH;
    unsigned curW;
    
};


#endif /* Handles_hpp */
