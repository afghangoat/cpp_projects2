#include <iostream>
class A{
    private:
        int a;
    public:
        A(int c): a(c){}
        int get_a() const{
            return a;
        }
};

std::ostream& operator<<(std::ostream& os, const A& a){
    os << a.get_a();
    return os;
}

int main(){
    A a1(4);
    std::cout << a1;
    return 0;
}