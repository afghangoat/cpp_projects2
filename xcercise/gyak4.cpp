
#include <iostream>
#include <cstring>

class String_2 {
    char *str;  // Pointer to dynamically allocated memory
    size_t len; // The actual length of the string (excluding the null terminator)

public:
    String_2(const char *s = ""); // Constructor from C-string
    String_2(char ch); // Constructor from a single character
    String_2(const String_2&); // Copy constructor
    String_2& operator=(const String_2&); // Copy assignment operator
    String_2& operator+=(char); // Appends a character to the string
    char& operator[](size_t); // Non-const index operator
    const char& operator[](size_t) const; // Const index operator
    String_2 operator+(const String_2&) const; // Concatenates two strings
    String_2 operator+(char) const; // Concatenates string and char
    const char* c_str() const; // Converts to C-string
    size_t length() const; // Returns the length of the string
    size_t find(const char) const; // Finds the first occurrence of a character
    void erase(); // Clears the string
    ~String_2(); // Destructor
};

// Default constructor
String_2::String_2(const char* s) {
    if (s) {
        len = std::strlen(s);
        str = new char[len + 1];  // Allocate space for the string
        std::strcpy(str, s);      // Copy the C-string into str
    } else {
        len = 0;
        str = new char[1]{ '\0' }; // Initialize an empty string
    }
}

// Constructor from a single character
String_2::String_2(char ch) {
    len = 1;
    str = new char[2]; // Allocate space for 1 character + null terminator
    str[0] = ch;
    str[1] = '\0'; // Null terminate the string
}

// Copy constructor
String_2::String_2(const String_2& other) {
    len = other.len;
    str = new char[len + 1];  // Allocate memory
    std::strcpy(str, other.str);  // Copy the string
}

// Copy assignment operator
String_2& String_2::operator=(const String_2& rhs) {
    if (this != &rhs) {  // Prevent self-assignment
        delete[] str;  // Release existing memory
        len = rhs.len;
        str = new char[len + 1];  // Allocate new memory
        std::strcpy(str, rhs.str);  // Copy the string
    }
    return *this;
}

// Appends a character to the string
String_2& String_2::operator+=(char ch) {
    char* new_str = new char[len + 2];  // Allocate space for new character and null terminator
    std::strcpy(new_str, str);           // Copy the existing string
    new_str[len] = ch;                   // Append the new character
    new_str[len + 1] = '\0';             // Null terminate the new string
    delete[] str;                        // Release old memory
    str = new_str;                       // Point to the new string
    len++;                               // Update length
    return *this;
}

// Non-const index operator
char& String_2::operator[](size_t i) {
    return str[i];
}

// Const index operator
const char& String_2::operator[](size_t i) const {
    return str[i];
}

// Concatenates two strings
String_2 String_2::operator+(const String_2& rhs) const {
    String_2 new_str;
    new_str.len = len + rhs.len;
    new_str.str = new char[new_str.len + 1];  // Allocate memory for the result
    std::strcpy(new_str.str, str);             // Copy the first string
    std::strcat(new_str.str, rhs.str);         // Append the second string
    return new_str;
}

// Concatenates a string and a character
String_2 String_2::operator+(char ch) const {
    String_2 new_str(*this);  // Copy the current string
    new_str += ch;            // Use += to append the character
    return new_str;
}

// Converts to C-string
const char* String_2::c_str() const {
    return str;
}

// Returns the length of the string
size_t String_2::length() const {
    return len;
}

// Finds the first occurrence of a character
size_t String_2::find(const char ch) const {
    for (size_t i = 0; i < len; ++i) {
        if (str[i] == ch) {
            return i;
        }
    }
    return std::string::npos;  // Return a value indicating "not found"
}

// Clears the string
void String_2::erase() {
    delete[] str;  // Release the existing memory
    str = new char[1]{ '\0' };  // Create an empty string
    len = 0;  // Reset the length
}

// Destructor
String_2::~String_2() {
    delete[] str;  // Release the allocated memory
}

int main() {
    // Write C++ code here

    return 0;
}

// Online C++ compiler to run C++ program online
/*template <typename T>
using Vec = std::vector<T>;

Vec<int> v = {1, 2, 3};  // Equivalent to std::vector<int>*/

/*// Create a vector called cars that will store strings
vector<string> cars = {"Volvo", "BMW", "Ford", "Mazda"};

// Create a vector iterator called it
vector<string>::iterator it;

// Loop through the vector with the iterator
for (it = cars.begin(); it != cars.end(); ++it) {
  cout << *it << "\n";
}*/

/*
#include <iostream>

class Auto{
    static int count;
    public:
    Auto(){
        std::cout << "Auto created with id" << inc_count();
    }
    int inc_count(){
        count++;
        return count;
    }
};
int Auto::count = 0;

int get_uuid(){
    static int a=0;
    a++;
    return a;
}
int main() {
    // Write C++ code here
    Auto a1;
    Auto a2;
    std::cout << "Try programiz.pro" << get_uuid() << get_uuid();

    return 0;
}*/

/*#include <iostream>
#include <cstring>

class MyString{
    private:
        char* str; //including null value
        int len; //No \0 in length
    public:
        MyString(const char* s=""){
            if(s){
                len=std::strlen(s);
                str= new char[len+1];
                std::strcpy(str,s);
            } else {
                len=0;
                str= new char[1];
                str[0]='\0';
            }
            printself();
        }
        void printself(){
            std::cout << str << "\n";
        }
        MyString(char ch){
            len=1;
            str= new char[2];
            str[0]=ch;
            str[1]='\0';
        }
        MyString(const MyString& other){
            len=other.len;
            delete[] str;
            str=new char[len+1];
            std::strcpy(str,other.str);
            
            printself();
        } // Copy constructor
        
        MyString& operator=(const MyString& rhs) {
            if(this != &rhs){
                delete[] str;
                len=rhs.len;
                str = new char[len+1];
                std::strcpy(str,rhs.str);
            }
            return *this;
        }
        // Appends a character to the string
        MyString& operator+=(char ch) {
            char* new_str = new char[len + 2];
            std::strcpy(new_str, str);
            new_str[len] = ch;
            new_str[len + 1] = '\0';
            delete[] str;
            str = new_str;
            len++;
            
            printself();
            return *this;
            
        }
        const char& operator[](size_t i) {
            return str[i];
        }
        const char& operator[](size_t i) const {
            return str[i];
        }
        MyString operator+(const MyString& rhs) const {
            MyString new_str;
            new_str.len=len+rhs.len;
            new_str.str=new char[new_str.len+1];
            std::strcpy(new_str.str,str);
            std::strcat(new_str.str,rhs.str);
            
            return new_str;
        }
        // Concatenates a string and a character EZ ROHADT GOATED
        MyString operator+(char ch) const {
            MyString new_str(*this);  // Copy the current string
            new_str += ch;            // Use += to append the character
            return new_str;
        }
        const char* c_str() const{
            return str;
        }
        const size_t get_length() const{
            return len;
        }
        void erase() {
            delete[] str;
            str = new char[1]{ '\0' };
            len = 0;
        }
        
        ~MyString(){
            delete[] str;
        }
    
};
int main() {
    // Write C++ code here
    MyString str1("AAA");
    str1+='a';

    return 0;
}*/
