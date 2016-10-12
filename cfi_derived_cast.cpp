#include <iostream>

struct Base {
    Base(const std::string &s) : name(s) {}
    virtual ~Base() {}

    std::string name;
};

struct Derived : Base {
    Derived(const std::string &s) : Base(s) {}
    virtual ~Derived() {}

    const unsigned long variable  = 0x12345678;

    void printName() {
        std::cout << "I am: " << name << ", my member variable is: " << std::hex << variable << std::endl;
    }
};

int main(int argc, const char* argv[]) {

    Base B("base class");
    Derived D("derived class");

    (void)(argc);
    (void)(argv);

    D.printName();

    // this is illegal
    Derived &dptr = static_cast<Derived&>(B);

    dptr.printName();

    return 0;
}
