// Online C++ compiler to run C++ program online
#include <iostream>
#include <vector>

#ifndef MONOTON_HPP
#define MONOTON_HPP

template<class Iter, class Fn>
bool monoton(Iter begin, Iter end, Fn pred) {
    if(begin == end)
        return true;

    for(Iter i = begin++; begin != end; i = begin++)
         if(!pred(begin,i))
            return false;
    return true;
}

#endif // MONOTON_HPP
bool non_decreasing(std::vector<int>::iterator a, std::vector<int>::iterator b) {
    return *b <= *a;  // Ensures the sequence does not decrease
}

template<typename T,int length>
class ClassArray{
    public:
        T array[length];
        void fill(T value){
            for(int i=0;i<length;i++){
                array[i]=value;
            }
        }
        T& at(int index){
            return array[index];
        }
        T& operator[](int i) {
            return array[i];
        }
        T operator[](int i) const{
            return array[i];
        }
};

int main() {
    ClassArray<int,10> arr1;
    arr1.fill(7);
    // Write C++ code here
    std::cout << arr1[3];
    
    std::vector<int> nums1 = {1, 2, 2, 3, 4, 5};
    std::vector<int> nums2 = {5, 4, 3, 2, 1}; // Decreasing
    std::vector<int> nums3 = {1, 1, 1, 1}; // Constant (valid non-decreasing)

    std::cout << std::boolalpha;  // Print `true/false` instead of `1/0`
    std::cout << "nums1 is non-decreasing: " << monoton(nums1.begin(), nums1.end(), non_decreasing) << '\n';
    std::cout << "nums2 is non-decreasing: " << monoton(nums2.begin(), nums2.end(), non_decreasing) << '\n';
    std::cout << "nums3 is non-decreasing: " << monoton(nums3.begin(), nums3.end(), non_decreasing) << '\n';


    return 0;
}