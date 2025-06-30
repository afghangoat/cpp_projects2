#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>  // For std::less

bool is_even(int n){
    return n%2==0;
}
int main() {
    std::vector<int> numbers = {10, 2, 8, 5, 3};

    // Sorting using std::less (default for ascending order)
    std::sort(numbers.begin(), numbers.end(), std::less<int>());
    
    //std::sort(numbers.begin(), numbers.end(), is_even);
    //Lambda [](int n) { return n > 5; }

    std::cout << "Sorted numbers: ";
    for (int n : numbers) {
        std::cout << n << " ";
    }
    std::cout << std::endl;
    
    auto it = std::find_if(numbers.begin(), numbers.end(), is_even);

    if (it != numbers.end()) {
        std::cout << "First even number: " << *it << std::endl;
    } else {
        std::cout << "No even numbers found." << std::endl;
    }
    
    //Lambda, waow
    auto it2 = std::find_if(numbers.begin(), numbers.end(), [](int n) { return n > 5; });

    if (it2 != numbers.end()) {
        std::cout << "First number greater than 5: " << *it2 << std::endl;
    } else {
        std::cout << "No number greater than 5 found." << std::endl;
    }

    return 0;
}
