#ifndef CONTAINER_H
#define CONTAINER_H
#include <memory>
#include <iostream>
#include <functional>

const std::string EMPTY_CONTAINER="Container is empty. Nothing to delete.";

template<class Element>
class Container {
    struct Stub {
        //TODO:see comments
        //How can an object without copy constructor can be put in the container?
        //How can all push / and Stub(Element const& x,...) methods
        //be generated only for classes which have copy constructor defined?
        Stub(Element const& x,Stub* prev,Stub* next):data(x),next(next),prev(prev) {}
        Stub(Element&& x,Stub* prev,Stub* next):data(std::move(x)),next(next),prev(prev) {}
        Element data;
        Stub * next;
        Stub * prev;
    };
public:
    Container():mFirst(nullptr),mLast(nullptr),size(0) {}
    ~Container() { clear(); }
    Container(Container const&in):mFirst(nullptr),mLast(nullptr),size(0) { append(in); }
    Container(Container&& in):mFirst(std::move(in.mFirst)), mLast(std::move(in.mLast)), size(std::move(in.size))
    {
        in.mFirst=in.mLast=nullptr;
        in.size=0;
    }
    Container& operator=(Container const& in) {
        if(&in != this) {
            clear();
            append(in);
        }
        return *this;
    }
    Container& operator=(Container&& in) {
        std::swap(size, in.size);
        std::swap(mFirst, in.mFirst);
        std::swap(mLast, in.mLast);
        return *this;
    }

    void push_back(Element const& e) {
        Stub * s = new Stub(e, mLast, nullptr);
        if(mLast)
            mLast->next=s;
        else
            mFirst=s;
        mLast=s;
        ++size;
    }

    void push_front(Element const& e) {
        Stub * s = new Stub(e, nullptr, mFirst);
        if(mFirst)
            mFirst->prev=s;
        else
            mLast=s;
        mFirst=s;
        ++size;
    }

    void push_back(Element&& e) {
        Stub * s = new Stub(std::move(e), mLast, nullptr);
        if(mLast)
            mLast->next=s;
        else
            mFirst=s;
        mLast=s;
        ++size;
    }

    void push_front(Element&& e) {
        Stub * s = new Stub(std::move(e), nullptr, mFirst);
        if(mFirst)
            mFirst->prev=s;
        else
            mLast=s;
        mFirst=s;
        ++size;
    }

    void pop_back() {
        if(empty()) throw std::out_of_range(EMPTY_CONTAINER);
        Stub * cur = mLast;
        mLast = cur->prev;
        if(mLast == nullptr) {
            mFirst = nullptr;
        } else {
            mLast->next=nullptr;
        }
        cur->prev=cur->next=nullptr;
        delete cur;
        --size;
    }

    void pop_front() {
        if(empty()) throw std::out_of_range(EMPTY_CONTAINER);
        Stub * cur = mFirst;
        mFirst = cur->next;
        if(mFirst == nullptr) {
            mLast = nullptr;
        } else {
            mFirst->prev=nullptr;
        }
        cur->prev=cur->next=nullptr;
        delete cur;
        --size;
    }

    Element& get_back() { return mLast->data; }
    Element& get_front() { return mFirst->data; }
    Element const& get_back() const { return mLast->data; }
    Element const& get_front() const { return mFirst->data; }

    //typedef bool (*callable)(Element &e);
    //typedef bool (*c_callable)(Element const&e);
    typedef std::function<bool(Element&)> callable;
    typedef std::function<bool(Element const&)> c_callable;
    bool iterate(c_callable const f) const {
        Stub * current = mFirst;
        bool ret = true;
        while(current && ret) {
            ret = f(current->data);
            current = current->next;
        }
        return ret;
    }
    bool iterate(callable const f) {
        Stub * current = mFirst;
        bool ret = true;
        while(current && ret) {
            ret = f(current->data);
            current = current->next;
        }
        return ret;
    }

    void append(Container const& in) {
        in.iterate([this] (Element const& e) {
            this->push_back(e);
        });
    }

    void clear() { while(!empty()) pop_back(); }
    size_t get_count() const { return size; }
    bool empty() const { return mFirst==nullptr; }
private:
    Stub * mFirst;
    Stub * mLast;
    size_t size;
};

#endif // CONTAINER_H
