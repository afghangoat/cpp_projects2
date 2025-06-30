
class Animal{
	protected:
		int age;
		string name;
		bool alive;
		
	public:
	
		Animal(int new_age,string new_name){
			age=new_age;
			name=new_name;
			alive=true;
		}
		
		int get_age(){
			return age;
		}
		
		void die(){
			alive=false;
		}
};

class Bird: public Animal{
	private:
		double wing_length;
		sf::Color color;
	public:
	
		Bird(int new_age,string new_name, double new_wing_length){
			
			age=new_age;
			name=new_name;
			
			wing_length=new_wing_length;
		}
	
};

std::cout << animal.age;

Animal animal1(23,"Józsi");

Bird pityu(4,"Pityu",2.0);
pityu.die();

void kill_animal(Animal* animal){
	animal->alive=false;
}