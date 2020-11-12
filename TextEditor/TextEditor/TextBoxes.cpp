//
//  TextBoxes.cpp
//  TextEditor
//
//  Created by Foma Mironenko on 12.11.2020.
//

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

