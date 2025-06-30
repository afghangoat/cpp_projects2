
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <memory>
#include <cstdlib>
#include <cassert>
#include <map>
#include <list>

/*template <class T, class Allocator= allocator<T> >
class vector{
	public:
		typedef T value_type;
		typedef Allocator allocator_type;
		class iterator; //Predefine
		class const_iterator;
		typedef typename Allocator::size_type size_type; //typename -so compiler knows it's a type
		typedef typename Allocator::difference_type difference_type;
		typedef typename Allocator::reference reference;
		typedef typename Allocator::pointer pointer;
		typedef typename std::reverse_iterator<iterator> reverse_iterator;
		
		//init,copy,de-init
		explicit vector(const Allocator& = Allocator());
		explicit vector(size_type, const Allocator& = Allocator());
		
		vector(size_type, const T&, const Allocator& = Allocator());
		vector(const vector<T,Allocator>&);
		vector<T,Allocator>& operator=(const vector<T,Allocator>&);
		
		template <class InputIterator>
			void assign(InputIterator start, InputIterator finish);
		
		void assign(size_type, const);
		allocator_type get_allocator() const;
		
		//Iters
		iterator begin();
		const_iterator begin() const;
		iterator end();
		const_iterator end() const;
		
		reverse_iterator rbegin();
		const_reverse_i rbegin();
		
};*/

//throw std::logic_error("Kezdő posba jutott");

//static const int MaxSize=8;

//Context
class Context{
	std::map<std::string, int> values;
	public:
		void setVar(std::string s, int i){
			values[s]=i;
		}
		int getVar(std::string s){
			return values[s];
		}
};
//Expression
class Expression{
	protected:
		Context* ctx;
	public:
		virtual void setContext(Context* c){
			ctx=c;
		}
		virtual int evaluate() const = 0; //abstract
		virtual ~Expression() {}
};

//Value
class Value : public Expression {
	int value;
	public:
		Value(int v=0) : value(v){}
		int evaluate() const;
};
//in .c
int Value::evaluate() const {
	return value;
}

//Operator
class Operator: public Expression{
	protected:
		Expression* op[2];
	public:
		void setContext(Context* c){
			Expression::setContext(c); //Call virtual
			op[0]->setContext(c);
			op[1]->setContext(c);
		}
		Operator(Expression* e1,Expression* e2){
			op[0]=e1;
			op[1]=e2;
		}
		void setOperand(Expression* e, int n){
			op[n]=e;
		}
};

//Addition
class Addition: public Operator{
	public:
		Addition(Expression* e1,Expression* e2):Operator(e1,e2){
			
		}
		int evaluate() const;
		
};
int Addition::evaluate() const {
	return op[0]->evaluate() + op[1]->evaluate();
}

class Factory{
	private:
		std::list<Expression*> created;
		std::map<int, Value*> values;
	public:
		Value* val(int i);
		Addition* add(Expression* e1, Expression* e2);
		
		~Factory(){
			while(!created.empty()){
				delete created.front();
				created.pop_front();
			}
		}
};
Value* Factory::val(int i){
	if(NULL == values[i]){
		values[i]=new Value(i);
		created.push_back(values[i]);
	}
	return values[i];
}
Addition* Factory::add(Expression* e1, Expression* e2){
	Addition* a = new Addition(e1,e2);
	created.push_back(a);
	return a;
}

class Variable: public Expression{
	std::string name;
	public:
		Variable(std::string n="x"): name(n){
			
		}
		int evaluate() const {
			return ctx->getVar(name);
		}
};

/*bool isDigit(char ch){
	return isdigit(ch)!=0;
}*/


template <typename T>
class Example {
public:
    void print() {
        std::cout << "General template\n";
    }
};

// Full specialization for `int`
template <>
class Example<int> {
public:
    void print() {
        std::cout << "Specialized template for int\n";
    }
};
// Full specialization for `double`
template <>
class Example<double> {
public:
    void print() {
        std::cout << "Specialized template for double\n";
    }
};
//Constexprs
constexpr int square(int x) {
    return x * x;
}

constexpr int result = square(5); // Compile-time evaluation
int runtimeResult = square(10);  // Runtime evaluation

struct Point {
    int x, y;
    constexpr Point(int xVal, int yVal) : x(xVal), y(yVal) {}
    constexpr int getX() const { return x; }
};

constexpr Point p(3, 4); // Compile-time construction
constexpr int x = p.getX(); // Compile-time evaluation

constexpr int arraySize = 10;
int arr[arraySize]; // Valid because arraySize is known at compile time
//consteval = stronger constexpr

//class A2 : virtual public B {};
//virtual used to solve the diamond problem if C->a1 and a2 and b<-a1,a2 then the same variables will be inherited only once.

int main() {
	/*std::map<string,int> szamlalo;
	string szo;
	while(std::cin >>szo){
		string::iterator vege=
		remove_if(szo.begin(),szo.end(),isDigit);
		szo.erase(vege,szo.end());
		if (!szo.empty()){
			szamlalo[szo]+=1;
		}
	}*/
	
	{ Factory f;
	
		Value* v3=f.val(3);
		Value* v4=f.val(4);
		
		Expression* e = f.add(v3,v4);
		//Addition a(&v3,&v4);
    
		std::cout << e->evaluate() << std::endl;
	}
    return 0;
}
