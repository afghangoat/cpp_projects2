#include <iostream>
#include <sstream>
#include <string>
#include <vector>

enum TileTypes{
	NONE=0,
	WALL=1
};

class Tile{
	private:
		bool visited;
		int symbol;
		
	public:
		Tile(int sym) : symbol(sym) {
			visited=false;
		}
		Tile() : symbol(0) {
			visited=true;
		}
		
		bool get_visited() const {
			return visited;
		}
		
		void visit(){
			visited=true;
		}
		char get_symbol() const {
			switch (symbol){
				case NONE:{
					return ' ';
				}
				
				case WALL:{
					return '#';
				}
				default:{
					return '?';
				}
			}
			
		}
		void set_symbol(int sym) {
			symbol=sym;
		}
};
/*
#######
# #   #
###   #
#     #
#######

*/

class Labyrinth {
	private:
		int width;
		int height;
		int max_tries;
		std::vector<std::vector<Tile>> ldata;
		
	public:
		Labyrinth(int w, int h) : width(w), height(h), ldata(h, std::vector<Tile>(w, Tile(NONE))) {
			max_tries=w*h/2;
			generate_self();
		}

		void generate_base(){
			for(int y=0;y<height;y++){
				for(int x=0;x<width;x++){
					if(x%2==0||y%2==0||x==width-1||y==height-1){
						
						ldata[y][x].set_symbol(WALL);
					} else {
						
						ldata[y][x].set_symbol(NONE);
					}
					
				}
			}
		}
		bool is_valid(int x, int y){
			if(x<=0||y<=0||x>=width-1||y>=height-1){
				return false;
			}
			if(ldata[y][x].get_visited()==true){
				return false;
			}
			return true;
		}
		void walk_neighbors(int x, int y, int limit){
			
			if(limit<=0){
				return;
			}
			
			std::vector<std::pair<int, int>> directions;
			
			if (is_valid(x - 2, y)) { //left
				directions.push_back({x - 2, y});
			}
			if (is_valid(x + 2, y)) { //right
				directions.push_back({x + 2, y});
			}
			if (is_valid(x, y - 2)) { //up
				directions.push_back({x, y - 2});
			}
			if (is_valid(x, y + 2)) { //down
				directions.push_back({x, y + 2});
			}
			
			if (!directions.empty()) {
				int rand_idx = rand() % directions.size();
				int new_x = directions[rand_idx].first;
				int new_y = directions[rand_idx].second;

				int mid_x = (x + new_x) / 2;
				int mid_y = (y + new_y) / 2;

				ldata[mid_y][mid_x].set_symbol(NONE);

				walk_neighbors(new_x, new_y, limit - 1);
			}
			
		}
		
		void generate_self(){
			generate_base();
			int startX=1;
			int startY=1;
			
			walk_neighbors(startX,startY,max_tries);
			//rand()/%2
			
		}
		
		void print_labyrinth(){
			for(int y=0;y<height;y++){
				
				std::cout << "|";
				for(int x=0;x<width;x++){
					std::cout << ldata[y][x].get_symbol() << "|";
				}
				std::cout << std::endl;
			}
		}
		
		~Labyrinth(){
			//for(int i = 0; i < height; ++i){
			//	delete[] ldata[i];
			//}
			//delete[] ldata;
		}
};

int main() {
	srand(time(0));
	
	const int labyrinth_width=19;
	const int labyrinth_height=19;
	
	Labyrinth l1(labyrinth_width,labyrinth_height);
	l1.print_labyrinth();
	
    return 0;
}