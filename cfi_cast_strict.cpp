#include <iostream>

struct Base {
    Base() {}
    virtual ~Base() {}
    virtual void func() {
        std::cout << "Base: " << __FUNCTION__ << "\n";
    } 
};

struct Derived : Base {
};

int main(int argc, const char *argv[]) {

    Base b;

    (void)(argc);
    (void)(argv);

    // this is undefined behavior, but will generally work
    Derived &d = static_cast<Derived&>(b);
    
    d.func();

    return 0;
}
