//
//  TextBoxes.cpp
//  TextEditor
//
//  Created by Foma Mironenko on 12.11.2020.
//

#include <algorithm>
#include "TextBoxes.hpp"

ArrayHandler::ArrayHandler(unsigned h,
                           unsigned w):
    offsetH(0),
    offsetW(0),
    h(h),
    w(w),
master(true)
{
  source = new char*[h];
  for(int i = 0; i < h; i++)
  {
      source[i] = new char[w];
      for(int j = 0; j < w; j++)
      {
          source[i][j] = ' ';
      }
  }
}

ArrayHandler::ArrayHandler(ArrayHandler & other,
                           unsigned offsetH,
                           unsigned offsetW,
                           unsigned h,
                           unsigned w):
    source(other.source),
    offsetH(other.offsetH + offsetH),
    offsetW(other.offsetW + offsetW),
    h(h),
    w(w),
    master(false)
{
  assert(other.h >= offsetH + h &&
         other.w >= offsetW + w);
}

ArrayHandler::~ArrayHandler()
{
    if(master){
        for(int i = 0; i < h; i++)
        {
            delete source[i];
        }
        delete source;
    }
}

char *ArrayHandler::operator[] (unsigned i)
{
    // assume that the second index will be correct
    assert(i < h);
    return (source[offsetH + i] + offsetW);
}

void ArrayHandler::fill(char c)
{
    for(int i = 0; i < h; i++)
    {
        for(int j = 0; j < w; j++)
        {
            (*this)[i][j] = c;
        }
    }
}

void ArrayHandler::show()
{
    for(int i = 0; i < h; i++)
    {
        for(int j = 0; j < w; j++)
        {
            std::cout << (*this)[i][j] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}



bool SubBox::intersects(SubBox * other)
{
    assert(other);
    bool iH =   std::max(this->offsetH(), other->offsetH()) <=
                std::min(this->offsetH() + this->height(),
                         other->offsetH() + other->height()) - 1;
    bool iW =   std::max(this->offsetW(), other->offsetW()) <=
                std::min(this->offsetW() + this->width(),
                         other->offsetW() + other->width()) - 1;
    return iH && iW;
}

void SubBox::print(ArrayHandler & arr)
{
    ArrayHandler childPrinter(arr,
                              offsetH(),
                              offsetW(),
                              height(),
                              width());
    box->print(childPrinter);
}



void CharBox::print(ArrayHandler & arr)
{
    assert(arr.w == width() && arr.h == height());
    arr[0][0] = c;
}



TableBox::~TableBox()
{
    while(!elems.empty())
    {
        auto last = elems.back();
        delete last;
        elems.pop_back();
    }
}

bool TableBox::canInsert(SubBox *SB, bool force)
{
    if(!force){
        for (auto & it : elems)
        {
            if(it != SB && SB->intersects(it)) {
                return false;
            }
        }
    }
    if(SB->offsetH() + SB->height() > this->height() ||
       SB->offsetW() + SB->width()  > this->width())
    {
        return false;
    }
    return true;
}

bool TableBox::addBox(Box *newB,
            unsigned offsetH,
            unsigned offsetW,
            bool force)
{
    SubBox *newSB = new SubBox(newB, this, offsetH, offsetW);
    if (canInsert(newSB, force)) {
        elems.push_back(newSB);
        return true;
    }
    delete newSB;
    return false;
}

bool TableBox::moveTo(SubBox *child, unsigned newH, unsigned newW)
{
    if(child->getParent() != this) {
        return false;
    }
    unsigned prevH = child->offsetH(),
             prevW = child->offsetW();
    child->setOffsetH(newH);
    child->setOffsetW(newW);
    if(!canInsert(child, false)) {
        child->setOffsetH(prevH);
        child->setOffsetW(prevW);
        return false;
    }
    return true;
}

SubBox *TableBox::operator[] (unsigned i)
{
    assert(i < elems.size());
    SBiter res = elems.begin();
    for(int j = 0; j < i; j++)
    {
        res++;
    }
    return *res;
}

void TableBox::print(ArrayHandler & arr)
{
    assert(arr.w == width() && arr.h == height());
    for(auto & child : elems)
    {
        child->print(arr);
    }
}



void FrameBox::print(ArrayHandler & arr)
{
    arr[0][0] =
    arr[0][arr.w - 1] =
    arr[arr.h - 1][0] =
    arr[arr.h - 1][arr.w - 1] =
    '+';
    for (int i = 1; i < arr.h - 1; i++)
    {
        arr[i][0] = arr[i][arr.w - 1] = '|';
    }
    for (int j = 1; j < arr.w - 1; j++)
    {
        arr[0][j] = arr[arr.h - 1][j] = '-';
    }
    data->print(arr);
}

TextEditor *FrameBox::getEditor()
{
   return dynamic_cast<TextEditor*>(data->getBox());
}



ScriptBox::ScriptBox(const std::string s):
    TableBox(s.size(), 1)
{
    for(int i = 0; i < s.size(); i++)
    {
        assert(this->addBox(CharBox::create(s[i]), i, 0, true));
    }
}

LineBox::LineBox(const std::string s):
    TableBox(1, s.size())
{
    for(int i = 0; i < s.size(); i++)
    {
        this->addBox(CharBox::create(s[i]), 0, i, true);
    }
}


void TextEditor::print(ArrayHandler & arr)
{
    text.helper.render();
    unsigned fstL = scroll->firstLine;
    assert(txtLns->width == arr.w  - 2);
    assert(scroll->height == arr.h);
    for(int i = 0; i < arr.h; i++ )
    {
        if(fstL + i < scroll->Npos)
        {
            unsigned fstInd = txtLns->lines[fstL + i][0];
            for(int j = txtLns->lines[i][0]; j < txtLns->lines[i][1]; j++)
            {
                if(text.str[j] != '\n') {
                    arr[i][j - fstInd] = text.str[j];
                }
            }
        }
        if(i == scroll->position) {
            arr[i - fstL][arr.w - 1] = '*';
        } else {
            arr[i - fstL][arr.w - 1] = '^';
        }
        arr[i - fstL][arr.w - 2] = '|';
    }
}

void TextEditor::setText(std::string str)
{
    text.setText(str);
}

bool TextEditor::click(unsigned i, unsigned j)
{
    if(i >= scroll->height ||
       j >= txtLns->width + 2)
    {
        return false;
    }
    if(j < txtLns->width) {
        text.click(i, j);
    } else {
    // if user clicks a scroll bar, we process it
    // as a click onto the last symbol of the line
        text.click(i, txtLns->width - 1);
    }
    cursor.scrlInd = scroll->position;
    cursor.textInd = text.position;
    cursor.curH = scroll->position - scroll->firstLine;
    cursor.curW = std::min(txtLns->lines[cursor.curH][1] -
                       txtLns->lines[cursor.curH][0],
                       j);
    return true;
}

bool TextEditor::mvUp()
{
    if(scroll->position == scroll->firstLine) {
        if(scroll->position != 0) {
            scroll->firstLine--;
            assert(cursor.curH == 0);
            click(cursor.curH, cursor.curW);
        } else {
            return false;
        }
    } else {
        click(cursor.curH - 1, cursor.curW);
    }
    return true;
}

bool TextEditor::mvDown()
{
    if(scroll->position ==
       scroll->firstLine + scroll->height - 1)
    {
        if(scroll->position != scroll->Npos - 1) {
            scroll->firstLine++;
            click(cursor.curH, cursor.curW);
        } else {
            return false;
        }
    } else {
        click(cursor.curH + 1, cursor.curW);
    }
    return true;
}

bool TextEditor::mvLeft()
{
    return false;
}

bool TextEditor::mvRight()
{
    return false;
}

void TextEditor::write(char c)
{
    text.insert(c);
}



