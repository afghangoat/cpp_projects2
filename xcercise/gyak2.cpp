// Online C++ compiler to run C++ program online
#include <iostream>

int multiply1(int a,int b){
    return a*b;
}

void multiply2(int a,int b,int* c){
    *c=a*b;
}

void multiply3(int a,int b,int& c){
    c=a*b;
}

int main() {
    
    int a=3;
    int b=4;
    int c=multiply1(a,b);
    int& d = a;
    a=2;
    
    multiply3(4,3,c);
    
    std::cout << c << "\n";
    std::cout << "End program " << std::endl;
    
    return 0;
}