
#include <iostream>
#include "TextBoxes.hpp"

void testArray()
{
    unsigned h = 5, w = 7;
    ArrayHandler master(h, w);
    ArrayHandler sub1(master, 1, 2, 3, 4);
    ArrayHandler sub2(master, 0, 1, 2, 4);
    master.fill('0');
    sub1.fill('1');
    sub2.fill('2');
    master.show();
    sub1.show();
}

void testText()
{
    FrameBox *master = FrameBox::create(TableBox::create(15, 20));
    TableBox *tab = master->getTable();
    
    tab->addBox(FrameBox::create(LineBox::create("*")), 0, 0);
    tab->addBox(FrameBox::create(LineBox::create("Hello")), 0, 3);
    tab->addBox(FrameBox::create(ScriptBox::create("world")), 3, 0);
    tab->addBox(LineBox::create(std::string(6, '!')), 4, 4);
    tab->addBox(ScriptBox::create(std::string(5, '!')), 5, 4);
    
    ArrayHandler arr(master->height(), master->width());
    master->print(arr);
    arr.show();
    
    SubBox *hello = (*tab)[1];
    tab->moveTo(hello, 0, 6);
    SubBox *vert = (*tab)[4];
    tab->moveTo(vert, 6, 6);
    SubBox *fail = (*tab)[3];
    tab->moveTo(fail, 6, 6);
    SubBox *world = (*tab)[2];
    tab->moveTo(world, 8, 16);
    
    ArrayHandler arr1(master->height(), master->width());
    master->print(arr1);
    arr1.show();
    
    delete master;
}


int main()
{
    //testArray();
    testText();
}
