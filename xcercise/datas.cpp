// Online C++ compiler to run C++ program online
#include <iostream>
#include <array>
#include <vector>
#include <deque>
#include <algorithm>
#include <set>
#include <map>
#include <string>

int main() {
    // Write C++ code here
    int vardata=-1; //Def value
    //ARRAY
    int array[10]={1,2,3,4,5,6,7,8,9,10};
    vardata=array[2];
    
    //STD:ARRAY
    std::array<int, 10> array2;
    array2[16]=7; //In debug, it deapplies
    
    int arr2_length=array2.size();
    int* arr2_pointer=array2.data(); //Pointer to start of array
    for(int i=0;i<array2.size();i++){
        array2[i]=1;
    }
    //or auto i = b.begin();
    for(std::array<int,array2.size()>::iterator i= array2.begin(); i!=array2.end();++i){
        *i=2;
    }
    
    for (auto& i : array2){
        i=3;
    }
    //Algorithms
    std::fill(array2.begin(), array2.end(), 4);

    //Initializer lists
    array2={1,2,3,4,5,6,7,8,9,10};
    
    //Vector
    std::vector<int> vec;
    
    //Deque - láncolt listák
    std::deque<int> deck;
    
    //Set / Unordered set, only have 1 of any type of item, no front, no back, will sort object, unordered sets also exist
    std::set<int> dataset;
    dataset.insert(4);
    for (auto& str : dataset) {
        std::cout << str << ' ';
    }
    
    //Key value pairs
    std::map<std::string,int> imap;
    imap["one"]=1;
    for (auto& str : imap) {
        //Pair: key/value
        std::cout << str.first << str.second;
    }
    
    std::cout << "A";

    return 0;
}

//Unordered things hash their content