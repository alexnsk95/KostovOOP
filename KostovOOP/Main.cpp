#include <stdio.h>
#include <iostream>
#include <memory>
#include <cstdlib>
#include <functional>
#include "../Container.h"
#include "../ShapesFactory.h"

int main()
{
    Container< std::unique_ptr <Shape> > a;
    ShapeFactory * factory = ShapeFactory::GetInstance();
//    std::string names[]= {"Point","Circle","Rect","Square","PolyLine","Polygon"};

    for(int i=0; i < 20; ++i) {
        a.push_back(std::unique_ptr<Shape>(factory->CreateRandomShape()));
    }
    std::cout << "count:" << Shape::GetCount() << std::endl;
    a.iterate([](std::unique_ptr<Shape>& shape) {
        std::cout << *shape << "\n";
        return true;
    });
    std::cout << "Removing Shapes..." << std::endl;
    a.clear();
    try {
        a.pop_back();
    } catch(std::out_of_range& e) {
        std::cout << "Exception:" << EMPTY_CONTAINER << e.what() << std::endl;
    }
    std::cout << "count:" << Shape::GetCount() << std::endl;

    getchar();
    return 0;
}

