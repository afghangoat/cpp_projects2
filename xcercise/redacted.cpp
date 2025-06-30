// Online C++ compiler to run C++ program online
#include <iostream>
#include <vector>

/*double add(double a, double b){
    return a+b;
}
int add(int a, int b){
    return a+b;
}*/

template<typename T, typename U, typename V>
T add(U a, V b){
    return a+b;
}

template<>
int add(int a, int b){
    std::cout << "Lofasz";
    return a+b;
}


bool even(int num){
    if(num%2==0){
        return true;
    } else {
        return false;
    }
}

template<int U>
bool lesser_than(int num){
    if(num<U){
        return true;
    } else {
        return false;
    }
}

template<typename T>
void kiir(const std::vector<int>& vec, T pred){
    for(int i=0;i<vec.size();i++){
        if(pred(vec[i])==true){
             std::cout << vec[i] << ",";
        }
    }
}

template<int a1, int a2>
int add_with_template(){
    return a1+a2;
}
int add_with_template(){
    return 4+5;
}


int main() {
    std::vector<int> v={1,2,3,4,5,6,7};
    
    kiir(v,lesser_than<5>);
    kiir(v,lesser_than<7>);
    
    
    add<int>(5,4);
    
    add<double>(5,4);
    
    std::cout << "||" << (add_with_template<4,5>()) << std::endl;

    return 0;
}