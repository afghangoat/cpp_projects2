// Online C++ compiler to run C++ program online
#include <iostream>
#include <vector>


/*
diff between & and *

*/

template<typename T=int>
class Imaginary{
    private:
        T re;
        T im;
    public:
        Imaginary(T r, T i): re(r), im(i){
            
        }
        Imaginary(const Imaginary& new_ig){ //The i=something
            re=new_ig.re;
            im=new_ig.im;
        }
        
        //Operators
        Imaginary operator+(const Imaginary& new_ig) const {
            return Imaginary(re+new_ig.re,im+new_ig.im);
        }
        
        Imaginary operator++(int) { // ✅ Correct postfix
            Imaginary temp = *this; 
            ++(*this); // Use prefix to modify
            return temp;
        }
        
        Imaginary operator--(int) { // ✅ Correct postfix
            Imaginary temp = *this;
            --(*this); // Use prefix to modify
            return temp;
        }
		operator float() const {
			return re+im;
		}

        Imaginary operator++() { //++i
            return Imaginary(re+1,im);
        }
        Imaginary operator--() { //--i
            return Imaginary(re-1,im);
        }
        
        Imaginary operator-(const Imaginary& new_ig) const {
            return Imaginary(re-new_ig.re,im-new_ig.im);
        }
        
        void printself() const{
             std::cout << re<<"+"<<im<<"i\n";
        }
		
		// Overload << for easy printing
		friend std::ostream& operator<<(std::ostream& os, const Imaginary& num) {
			os << num.re << (num.im >= 0 ? "+" : "") << num.im << "i";
			return os;
		}
    
};

template<typename L=int>
class ImaginaryContainer {
private:
    std::vector<Imaginary<L>> data; // Internal storage

public:
    // Add an imaginary number
    void add(const Imaginary<L>& num) {
        data.push_back(num);
    }

    // Overload [] for element access
    Imaginary<L>& operator[](size_t index) {
        return data[index]; // No bounds checking (for simplicity)
        //Or return data.at(index); // ✅ Throws std::out_of_range if invalid index
    }

    const Imaginary<L>& operator[](size_t index) const {
        return data[index]; // Const version for read-only access
    }

    // Iterator functions
    auto begin() { return data.begin(); }
    auto end() { return data.end(); }
    auto begin() const { return data.begin(); }
    auto end() const { return data.end(); }

    // Size function
    size_t size() const { return data.size(); }
};


int main() {
    // Write C++ code here
    Imaginary im1(1,2);
    im1++;
    Imaginary im2=im1;
    Imaginary im3=im1+im2;
    
    ImaginaryContainer container;
    
    // Add some imaginary numbers
    container.add(Imaginary(1, 2)); //This is why const is needed
    container.add(Imaginary(3, 4));
    container.add(Imaginary(5, 6));
    container.add(im1);

    // Access elements using []
    container[0].printself();
    container[1].printself();
    container[3].printself();
    
    im3.printself();
    
    std::cout << "Iterating with range-based for loop:\n";
    
    // Iterate using range-based for loop
    for (const auto& num : container) {
        num.printself();
    }

    return 0;
}

/*
#include <sstream>
//Create an ostringstream object: You can instantiate an ostringstream object just like any other stream object.


std::ostringstream oss;
//Write to the stream: You can write to the ostringstream using the << operator, just like you would with std::cout.

int number = 42;
oss << "The answer is " << number << "!";
//Retrieve the string: Once you've added content to the stream, you can retrieve the accumulated string using the str() method.

std::string result = oss.str();
std::cout << result << std::endl;  // Output: The answer is 42!

//ostream:
std::ostream& os = std::cout;  // Reference to the standard output stream
os << "Hello, world!" << std::endl;  // Writing to the console

Frite file:
#include <fstream>

int main() {
    std::ofstream ofs("output.txt");  // Open file for writing

    if (ofs.is_open()) {
        ofs << "Hello, file!" << std::endl;
        ofs << "Writing more data..." << std::endl;
    }

    ofs.close();  // Close the file when done
    return 0;
}
*/