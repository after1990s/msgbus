#include <iostream>
#include "function_traits.h"
#include "messagebus.h"

using namespace std;

class C{
public:
    C(){ p = new char[1024];}
    ~C() {delete p; p = nullptr;}
    void callback(){
        *p = 3;
        std::cout << "C::callback" << std::endl;
    }
private:
    char *p = nullptr;
};
int main()
{
    auto k = to_function([](){std::cout << "123";});
    k();
    cout << "Hello World!" << endl;

    C * c = new C();
    std::function<void()> f0 = std::bind(&C::callback, c);
    test_function<decltype(f0)> f1;
    MessageBus bus;
    delete c;
    bus.attach(0, [](){std::cout << "attach 0";});
    bus.attach(0, std::forward<decltype(f0)>(f0));
    bus.attach(1, [](int i) {std::cout << "attach 1, param: " << i;});
    bus.sendMsg<void>(0);
    bus.sendMsg<void, int>(1, 3);
    return 0;
}
