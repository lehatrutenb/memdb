#include <memory>

struct A {
    virtual void puk() = 0;
};

struct B : public A {
    void puk() {

    }
    int a;
};

int main() {
    std::shared_ptr<A> sp{new B{}};
}