//
//  Handles.cpp
//  TextEditor
//
//  Created by Foma Mironenko on 19.11.2020.
//

#include "Handles.hpp"

bool Scroll::setPos(unsigned pos)
{
    if(pos >= Npos) {
        return false;
    }
    position = pos;
    return true;
}

void Scroll::setH(unsigned h)
{
    height = h;
}

void Scroll::click(unsigned i)
{
    assert(i < height);
    position = firstLine + i;
}



bool Text::setPos(unsigned pos)
{
    if(pos >= Npos + 1) {
        return false;
    }
    position = pos;
    return true;
}

void Text::insert(char c)
{
    str.insert(position, 1, c);
    Npos++;
    position++;
}

void Text::clear()
{
    str.erase(position, 1);
    Npos--;
    position--;
}

void Text::setText(std::string txt)
{
    str = txt;
    Npos = txt.size();
    position = 0;
}

void Text::click(unsigned i, unsigned j)
{
    helper.render();
    position = helper.click(i, j);
}


void Lines::render()
{
    assert(consumer->position <= consumer->str.size());
    assert(consumer->Npos == consumer->str.size());
    lines.clear();
    unsigned cur = 0;
    unsigned begin = 0;
    
    while(true)
    {
        if(cur == consumer->position) {
            scrl.position = lines.size();
            if(scrl.firstLine > scrl.position) {
                scrl.firstLine = scrl.position;
            }
            if(scrl.firstLine + scrl.height - 1 < scrl.position) {
                scrl.firstLine = scrl.position - scrl.height + 1;
            }
        }
        
        if(consumer->str[cur] == '\n') {
            lines.push_back( {begin, cur - 1} );
            begin = cur + 1;
        } else if (cur - begin == width ||
                   cur == consumer->Npos) {
            lines.push_back( {begin, cur - 1} );
            begin = cur;
        }
        if(cur == consumer->Npos) {
            break;
        }
        cur++;
    }
    scrl.Npos = lines.size();
}

void Lines::setW(unsigned w)
{
    // two lines for scroll bar
    width = w - 2;
}

unsigned Lines::click(unsigned i, unsigned j)
{
    assert(i < scrl.height && j < width);
    const std::array<unsigned, 2> & line =
        (i + scrl.firstLine < lines.size() ?
         lines[i + scrl.firstLine] :
         lines.back());
    scrl.position = (i + scrl.firstLine < lines.size() ?
                     i + scrl.firstLine :
                     lines.size() - scrl.firstLine);
    if(j + line[0] < line[1]) {
        return line[0] + j;
    }
    return line[1];
}
