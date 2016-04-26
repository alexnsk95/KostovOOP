#ifndef BASE_H
#define BASE_H
#include <iostream>

class Printable {
public:
    Printable() {}
    virtual ~Printable() {}
    virtual std::string to_string() const = 0;
    friend std::ostream& operator<<(std::ostream& stream,Printable const& printable) {
        return stream << printable.to_string();
    }
};

class Named:public Printable {
public:
    Named() = delete;
    Named(std::string name):m_name(name) {}
    std::string to_string() const override {
        return m_name;
    }
private:
    std::string m_name;
};

#endif // BASE_H
