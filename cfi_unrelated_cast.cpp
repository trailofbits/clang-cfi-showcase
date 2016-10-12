#include <iostream>

struct Foo {
    Foo(const std::string &s): command(s)  {}
    virtual ~Foo() {}

    void fooStuff() {
        std::cout << "I am in " << __FUNCTION__ << "\n";
        std::cout << "And I would execute: " << command << "\n";
    }

    std::string command;
};

struct Bar {
    Bar(const std::string &s): name(s) {}
    virtual ~Bar() {}
    
    void barStuff() {
        std::cout << "I am in " << __FUNCTION__ << "\n";
        std::cout << "And I am called: " << name << "\n";
    }

    std::string name;
};


enum class WhichObject {
    FooObject,
    BarObject
};


static void *allocator(WhichObject w, const std::string& arg) {
    switch(w) {
        case WhichObject::FooObject:
            return new Foo(arg);
        case WhichObject::BarObject:
            return new Bar(arg);
    }
}

int main(int argc, const char *argv[]) {

    void *ptr = nullptr;
    (void)(argc);
    (void)(argv);

    // Assume an attacker can create Bar objects
    // with arbitrary arguments. 
    ptr = allocator(WhichObject::BarObject, "system(\"/bin/sh\")");

    // .. and there is a flaw to re-cast Bar objects
    // as Foo objects
    Foo *fooptr = static_cast<Foo*>(ptr);
    fooptr->fooStuff();

    return 0;
}
