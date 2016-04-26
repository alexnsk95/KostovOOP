#ifndef SHAPESFACTORY_H
#define SHAPESFACTORY_H
#include "Shapes.h"
#include <map>
#include <vector>
#include <random>

class ShapeFactory {
private:
    ShapeFactory() {
      Register("Point",[] { return (Shape*)new Point(randDouble(), randDouble()); });
      Register("Circle",[] { return (Shape*)new Circle(randDouble(), randDouble(), fabs(randDouble())); });
      Register("Rect",[] {
          double x1=randDouble(),x2=randDouble(),y1=randDouble(),y2=randDouble();
          if(!(x1 <= x2)) std::swap(x1,x2);
          if(!(y1 <= y2)) std::swap(y1,y2);
          return (Shape*)new Rect(x1, y1, x2, y2);
      });
      Register("Square",[] { return (Shape*)new Square(randDouble(), randDouble(), fabs(randDouble())); });
      Register("PolyLine",[] {
          PolyLine * p = new PolyLine();
          for(int i=0;i < rand() % 10; ++i) p->AddPoint(Point(randDouble(), randDouble()));
          return (Shape*)p;
      });
      Register("Polygon",[] {
          Polygon * p = new Polygon();
          for(int i=0;i < rand() % 10; ++i) p->AddPoint(Point(randDouble(), randDouble()));
          return (Shape*)p;
      });
    }
    ShapeFactory(const ShapeFactory &) { }
    ShapeFactory &operator=(const ShapeFactory &) { return *this; }

    typedef Shape* (*Creator)();
    std::map<std::string, Creator> m_creators;
    std::vector< std::map<std::string, Creator>::const_iterator > m_cache_creators;
public:
    static double randDouble() {
        //TODO:add better generator if required
        return 0.0+rand()/100.0;
    }

    static ShapeFactory * GetInstance()
    {
        static ShapeFactory instance;
        return &instance;
    }

    void Register(const std::string &className, Creator creator) {
        auto it = m_creators.insert(std::make_pair(className, creator));
        m_cache_creators.push_back(it.first);
    }

    Shape * CreateRandomShape() const {
        //TODO:add better generator if required
        size_t index = rand() % m_cache_creators.size();
        return (m_cache_creators[index]->second)();
    }

    Shape * CreateRandomShape(const std::string &name) const {
        auto it = m_creators.find(name);
        return it != m_creators.end() ? (it->second)() : CreateRandomShape();
    }
};


#endif // SHAPESFACTORY_H
