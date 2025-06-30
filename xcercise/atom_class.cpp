#include <iostream>
#include <string>
#include <stdexcept>

class Atom{
    private:
        std::string symbol;
        int atomicNumber;
        float mass;
    public:
        std::string getSymbol() const { return symbol; }
        int getAtomicNumber() const { return atomicNumber; }
        float getMass() const { return mass; }

        Atom(){
            symbol=" ";
            atomicNumber=0;
            mass=0.0f;
        }
        Atom(std::string sym, int aNum, float m): symbol(sym), atomicNumber(aNum), mass(m){
            if(mass<0.0f){
                throw "Mass cannot be negative!";
            }
        }
        Atom (Atom& temp) {
            symbol=temp.symbol;
            atomicNumber=temp.atomicNumber;
            mass=temp.mass;
        }
        Atom (Atom&& temp) noexcept {
            symbol=std::move(temp.symbol);
            atomicNumber=temp.atomicNumber;
            mass=temp.mass;
            
            // Nulling out
            temp.symbol=" ";
            temp.atomicNumber=0;
            temp.mass=0.0f;
        }
        void display(){
            std::cout << symbol << " | Atomic number: " << atomicNumber << " | Mass: " << mass << std::endl;
        }
        
        //Operators
        Atom operator+(Atom const& rhs){
            Atom res;
            res.symbol = symbol + rhs.symbol;

            res.atomicNumber = atomicNumber + rhs.atomicNumber;
            res.mass = mass + rhs.mass;
            return res;
        }
        Atom operator-(Atom const& rhs){
            Atom res;
            res.symbol = symbol;
            int erase_idx = res.symbol.rfind(rhs.symbol);
            if(erase_idx!=-1){
                res.symbol.erase(erase_idx, 1);
                if (res.symbol.empty()) {
                    res.symbol = " ";
                }
            }

            res.atomicNumber = atomicNumber - rhs.atomicNumber;
            res.mass = mass - rhs.mass;
            if(res.mass<0.0f){
                res.mass=0.0f;
            }
            return res;
        }
        
        Atom& operator=(const Atom& rhs) {
            if (this != &rhs) {
                symbol=rhs.symbol;
                atomicNumber=rhs.atomicNumber;
                mass=rhs.mass;
            }
            return *this;
        }
        Atom& operator+=(const Atom& rhs) {
            symbol+=rhs.symbol;
            atomicNumber+=rhs.atomicNumber;
            mass+=rhs.mass;
            return *this;
        }
        bool operator==(const Atom& rhs){
            return symbol==rhs.symbol&&atomicNumber==rhs.atomicNumber&&mass==rhs.mass;
        }
};

class Compound {
    private:
        Atom* array;
        std::size_t size;
        std::size_t amount_contained;
    
    public:
        Compound(std::size_t s): size(s){
            amount_contained=0;
            array = new Atom[size];
            
        }
        ~Compound(){
            delete[] array;
        }
        
        void addAtom(const Atom& atom){
            if(amount_contained>=size){
                throw std::overflow_error("Compound has exceeded the maximum assigned capacity!");
            }
            array[amount_contained]=atom;
            amount_contained++;
        }
        Compound& operator+=(const Atom& rhs) {
            addAtom(rhs);
            return *this;
        }
};

int main() {
    // Tests
    
    Atom hydrogen("H", 1, 1.008);
    Atom oxygen("O", 8, 15.999);

    hydrogen.display();
    oxygen.display();

    Atom water = hydrogen + hydrogen + oxygen;
    water.display();

    if (hydrogen == oxygen) {
        std::cout << "Hydrogen and Oxygen are equal." << std::endl;
    }
    
    Compound c1(3);
    c1+=hydrogen;
    c1+=hydrogen;
    
    //No space for third element so it errors.
    c1+=oxygen;
    return 0;
}