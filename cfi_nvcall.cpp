#include <iostream>
#include <string>

struct Account {
    Account(const std::string &s) : name(s) {}
    virtual ~Account() {}
    void showName() {
        std::cout << "Account name is: " << name << std::endl;
    }
    void adminStuff() { std::cout << "Not Implemented" << std::endl; }
    std::string name;
};

struct UserAccount : Account {
    UserAccount(const std::string &s) : Account(s) {}
    virtual ~UserAccount() {}
    void adminStuff() { 
        std::cout << "Admin Work not permitted for a user account!" << std::endl;
    }
};

struct AdminAccount : Account {
    AdminAccount(const std::string &s) : Account(s) {}
    virtual ~AdminAccount() {}
    void adminStuff() { 
        std::cout << "Would do admin work in context of: " << this->name << std::endl;
    }
};

int main(int argc, const char *argv[]) {

    UserAccount* user = new UserAccount("user");
    AdminAccount* admin = new AdminAccount("admin");

    (void)(argc);
    (void)(argv);

    std::cout << "Admin check: " << std::endl;
    admin->showName();
    admin->adminStuff();

    std::cout << "User check: " << std::endl;
    user->showName();
    user->adminStuff();
    
    // imagine this change happens via memory corruption,
    // type confusion, deserialization vulnerability,
    // or a similar flaw
    Account *account = static_cast<Account*>(user);
    AdminAccount *admin_it = static_cast<AdminAccount*>(account);

    admin_it->showName();
    std::cout << "CFI Should prevent the actions below:" << std::endl;
    admin_it->adminStuff();

    return 0;
}
