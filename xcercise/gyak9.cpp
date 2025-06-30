#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // Lambda function that returns true for even numbers
    auto is_even = [](int n) -> bool {
        return n % 2 == 0;
    };

    std::cout << "Even numbers: ";
    std::for_each(numbers.begin(), numbers.end(), [&](int n) {
        if (is_even(n)) {  // Use the boolean lambda function as a predicate
            std::cout << n << " ";
        }
    });

    return 0;
}


#include <iostream>

#include <iostream>

// Modern interface
class IPrinter {
public:
    virtual void print() const = 0;
    virtual ~IPrinter() = default;
};
#include <iostream>

// Legacy class with a different interface
class LegacyPrinter {
public:
    void printText() const {
        std::cout << "Printing from Legacy Printer!\n";
    }
};

// Adapter that adapts LegacyPrinter to IPrinter
class PrinterAdapter : public IPrinter {
private:
    LegacyPrinter* legacyPrinter;  // Reference to the legacy printer

public:
    PrinterAdapter(LegacyPrinter* legacyPrinter) : legacyPrinter(legacyPrinter) {}

    // Implementing the modern interface by calling legacy method
    void print() const override {
        legacyPrinter->printText();  // Adapting the call to the legacy method
    }
};

// Online C++ compiler to run C++ program online
#include <iostream>
#include <vector>
#include <algorithm>

void printfn(int x) {
    std::cout << x << ",";
}

template <typename T>
struct myless {
    bool operator()(const T& left, const T& right) const {
        return left < right;
    }
};


int main() {
    std::vector<int> input_data;
    int x;
    while(std::cin >> x){
        input_data.push_back(x);
    }
    
    std::cout << "Inputs are:\n-----------------\n|";
    for(std::vector<int>::iterator it=input_data.begin();it!=input_data.end();it++){
        std::cout << *it << "|";
    }
    std::cout << "\nEven numbers:\n";
    
    std::for_each(input_data.begin(), input_data.end(), [](int x)
    { 
        if (x%2==0){
            std::cout << x << ","; 
        }
    
    }
    );
    
    std::cout << "\nDoubling it..:\n";
    
    std::for_each(input_data.begin(), input_data.end(), [](int &x)
    { 
        x *= 2;
    }
    );
    
    std::for_each(input_data.begin(), input_data.end(), printfn);

    int sum = 0;
    std::for_each(input_data.begin(), input_data.end(), [&sum](int x) { sum += x; });
    std::cout << "\nSum: " << sum << "\n";
    
    std::sort(input_data.begin(), input_data.end(), myless<int>());

    std::cout << "Sorted numbers: ";
    for (int num : input_data) {
        std::cout << num << " ";
    }
    std::cout << "\n";

    return 0;
}