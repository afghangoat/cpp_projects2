#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <memory>
#include <cstdlib>
#include <cassert>

const bool inDebug=true;

bool simulationRunning=true;

//TEMPS:
bool tempsavevar=true;

#define GAME_NAME "Noita Clone"
#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 1000
#define CHUNK_SIZE 256

#define FPS 50
#define UPDATE_FRAMES 2
#define FRAME_TIME (1000/FPS)
#define RAD 0.0174532925

#define VERT_SIZE 3

#define BRUSH_SIZE 1

#define explosion_radius 4
#define fire_spread_rate 3
#define plant_growth_rate 16
#define plant_petal_rate 3 //The less is the more are the petals

inline void debugAssert(bool statement, const char* comment){
	if(statement==true){
		return;
	}
	std::cout << "Assertion failed: " << comment << "\n";
	if(inDebug==true){
		exit(1);
	} else {
		assert(0);
	}
}
//TODO 
/*
friend class ClassB;

gameconfig class,
gamemanager class,
entity class

*/

template <typename T = double> T difference(T p1, T p2){
	return fabs(p1-p2);
}
//difference<double>(3.0, 7.0)

void outOfMemHandler(){
	std::cout << "Out of memory error!\n";
	exit(1);
}

class Renderer {
	private:
		sf::Texture sky;
		sf::Sprite sky_sprite;
	public:
		sf::RenderWindow& window;

		// Constructor to initialize with an existing window
		Renderer(sf::RenderWindow& win) : window(win) {
			
		}

		void init() {
			window.setFramerateLimit(FPS);
			if (!sky.loadFromFile("bg1.png")) {
				perror("Couldn't load texture \"bg1.png\"!");
				return;
			} else {
				sky_sprite.setTexture(sky);
				sky_sprite.setPosition(0,0);
			}
			
		}
		
		void draw_sky(){
			window.draw(sky_sprite);
		}
		void set_sky_scale(double x){
			sky_sprite.setScale(x,x);
		}
};

namespace alives{
	
	class Entity{
		protected:
			sf::Vector2f position;
			sf::Vector2f hitsize;
		public:
			Entity(sf::Vector2f new_pos, sf::Vector2f new_size): position(new_pos), hitsize(new_size) {
				//Stub
			}
			virtual bool colliding(){
				/*if(bottomblock==sandsim::AIR){
					
				}*/
				return false;
			}
			
			~Entity(){
				
			}
	};
	
	//Entity consists of tiles
	class AutomatonEntity: public Entity{ //public Entity or private Entity?
		protected:
			std::vector<int> body;
			sf::Vector2f heartPosition;
			
		public:
			
			AutomatonEntity(sf::Vector2f new_pos, sf::Vector2f new_size, sf::Vector2f heart_pos): Entity(new_pos, new_size), body(new_size.x*new_size.y,0), heartPosition(heart_pos) {
			}

			bool colliding() override {
				//Uhh???
				return true;
			}
	};
	
	//Entity consists of an animated sprite
	class SpriteEntity : public Entity {
		protected:
			int animation_frames;
		public:
			SpriteEntity(sf::Vector2f new_pos, sf::Vector2f new_size): Entity(new_pos, new_size) {
				
			}

			bool colliding() override {
				return true;
			}
	};
	//Functor
	/*
	Lista<int>::iterator i1,i2
	for(i1=0;i1<i2;i1++){
		
	}
	
	public:
		class iterator; //Predefine
	
	template<class T, class F=lesser>
	struct lesser{
		bool operator(const T& a, const T& b){
			return a<b;
		}
	}
	
	*/
	
	//template <> ??
	
	/*template <class T, size_t s = 10, int i = 0, template <class P> , bool select( T, T )> //class P = Pont
	class EntityArray{
		private:
			T t[s];
			int x;
			void chk (size_t i) const {
				if(i>=s){
					throw "Index hiba!";
				}
			}
		public:
		
			EntityArray(){
				x=i;
			}
			
			T& operator[](size_t i){
				chk(i);
				return t[i];
			}
			const T& operator[](size_t i) const {
				chk(i);
				return t[i];
			}
	};*/

}

//TODO snow,ice, steel, nuclear waste (emits), firework (goes up and randomly explodes), explosion, salt, brine, meat, cooked meat, potato, cooked potato,
//wire, active_wire, generator, sawblade (destroy nearby things to smoke), antimatter, lightbulb, active_lightbulb, led_rgb, active_led_rgb, pressureplate (give energy when something on top), snake, BOMB (explodes on electricity),Door = Can be activa-not active (can be walked through, if a door is connected to an inactive one, it becomes inactive)
//creepy liquid
namespace sandsim{
	enum TileTypes {
		NONE=-1,
		AIR=0,
		SAND=1,
		WATER=2,
		ROCK=3,
		STEAM=4,
		LAVA=5,
		ACID=6,
		GRAVEL=7,
		COPPER=8,
		OIL=9,
		FIRE=10,
		SMOKE=11,
		WOOD=12,
		GLUE=13,
		DIRT=14,
		SEED=15,
		PLANT=16,
		PETAL=17,
		ALGAE=18,
		BAMBOO=19,
		SAPLING=20,
		LEAVES=21,
		COAL=22,
		ASH=23,
		THERMITE=24,
		
		TILE_COUNT
		
	};

	const int ShadingVariability=20;
	
	sf::Color ColorMap[TILE_COUNT] = {
		sf::Color(0,0,0), //AIR
		sf::Color(255,255,153), //SAND
		sf::Color(102,153,255), //WATER
		sf::Color(110,110,110), //ROCK
		sf::Color(230,230,230), //STEAM
		sf::Color(255,60,30), //LAVA
		sf::Color(204,51,153), //ACID
		sf::Color(170,170,170), //GRAVEL
		sf::Color(204,102,0), //COPPER
		sf::Color(82,82,38), //OIL
		sf::Color(255,110,21), //FIRE
		sf::Color(60,60,60), //SMOKE
		sf::Color(153,76,0), //WOOD
		sf::Color(255,255,153), //GLUE
		sf::Color(113,46,10), //DIRT
		sf::Color(255,153,153), //SEED
		sf::Color(102,153,0), //PLANT
		sf::Color(102,255,255), //PETAL
		sf::Color(100,215,50), //ALGAE
		sf::Color(153, 204, 0), //BAMBOO
		sf::Color(0,51,0), //SAPLING
		sf::Color(0,153,51), //LEAVES
		sf::Color(20,20,20), //COAL
		sf::Color(190,140,140), //ASH
		sf::Color(204,0,0) //THERMITE
	};
	//Kell egy tile ref, minden tárolja a koordokat

	//Todo function to fill this map up, probably put in gameConfig class

	class Tile{
		protected:
			int type;
			sf::Color color;
			int updateCycle;
			
		public:
			
			
			explicit Tile(int base_type, sf::Color base_color): type(base_type), color(base_color) {
				updateCycle=0;
			}
			~Tile() {}

			//virtual void update(Chunk& chunk, int y, int x) = 0;
			
			int getCycle() const {
				return updateCycle;
			}
			
			void setCycle(int newCycle){
				updateCycle=newCycle;
			}
			
			void processCycle(){
				updateCycle++;
			}
			
			void setType(int new_type){
				
				//debugAssert((new_type<TILE_COUNT&&new_type>NONE),"Invalid supplied type at Tile.setType()!");
				
				type=new_type;
				color=ColorMap[type];
				
				int rVariation = (rand() % (ShadingVariability * 2 + 1)) - ShadingVariability;

				color.r = std::clamp(color.r + rVariation, 0, 255);
				color.g = std::clamp(color.g + rVariation, 0, 255);
				color.b = std::clamp(color.b + rVariation, 0, 255);
			}
			
			int getType() const {
				return type;
			}
			
			sf::Color getColor() const {
				return color;
			}
			
			void setColor(sf::Color new_color){
				color=new_color;
			}
	};
	//Materials maybe??
	/*class Sand : public Tile {
		public:
			Sand() : Tile(SAND, ColorMap[SAND]) {
				
			}

			void update(Chunk& chunk, int y, int x) override {
				int belowIndex = getTileDataAt(y + 1 , x);
				int leftBelowIndex = getTileDataAt(y + 1 , x - 1);
				int rightBelowIndex = getTileDataAt(y + 1 , x + 1);

				if (belowIndex == AIR || belowIndex == WATER) {
					swapTiles(y, x, y + 1, x);

				}
							
				else if (leftBelowIndex == AIR || leftBelowIndex == WATER) {
					swapTiles(y, x, y + 1, x - 1);

				}
							
				else if ( rightBelowIndex == AIR || rightBelowIndex == WATER) {
					swapTiles(y, x, y + 1, x + 1);
				}
			}
	};*/

	//TODO limiting inheritance LoneChunk (no neighbors, easier to simulate)
	class EChunk{
		private:
			int offset_x;
			int offset_y;
			std::vector<std::shared_ptr<Tile>> chunk_data;
			
			int chunkUpdateCycle;
			
			EChunk* chunkAbove;
			EChunk* chunkRightAbove;
			EChunk* chunkLeftAbove;
			EChunk* chunkBelow;
			EChunk* chunkRightBelow;
			EChunk* chunkLeftBelow;
			EChunk* chunkRight;
			EChunk* chunkLeft;
			
		public:
			EChunk(int cX, int cY): chunk_data(CHUNK_SIZE * CHUNK_SIZE, Tile(0, sf::Color(0, 0, 0))){
				offset_x=cX*CHUNK_SIZE*VERT_SIZE;
				offset_y=cY*CHUNK_SIZE*VERT_SIZE;
				chunkUpdateCycle=0;
				//Tile& tile = matrix[row * CHUNK_SIZE + col];
				
				chunkAbove=nullptr;
				chunkRightAbove=nullptr;
				chunkLeftAbove=nullptr;
				chunkBelow=nullptr;
				chunkRightBelow=nullptr;
				chunkLeftBelow=nullptr;
				chunkRight=nullptr;
				chunkLeft=nullptr;
				
				//Adjustable action
				generateChunk();
			}
			
			EChunk(const EChunk& copient): offset_x(copient.offset_x), 
				offset_y(copient.offset_y),
				chunk_data(copient.chunk_data), 
				chunkUpdateCycle(copient.chunkUpdateCycle),
				chunkAbove(copient.chunkAbove ? new EChunk(*copient.chunkAbove) : nullptr),
				chunkRightAbove(copient.chunkRightAbove ? new EChunk(*copient.chunkRightAbove) : nullptr),
				chunkLeftAbove(copient.chunkLeftAbove ? new EChunk(*copient.chunkLeftAbove) : nullptr),
				chunkBelow(copient.chunkBelow ? new EChunk(*copient.chunkBelow) : nullptr),
				chunkRightBelow(copient.chunkRightBelow ? new EChunk(*copient.chunkRightBelow) : nullptr),
				chunkLeftBelow(copient.chunkLeftBelow ? new EChunk(*copient.chunkLeftBelow) : nullptr),
				chunkRight(copient.chunkRight ? new EChunk(*copient.chunkRight) : nullptr),
				chunkLeft(copient.chunkLeft ? new EChunk(*copient.chunkLeft) : nullptr) {
				// :(
			}
			
			~EChunk() {
				//delete[] chunk_data;

				chunkAbove = nullptr;

				chunkRightAbove = nullptr;
				
				chunkLeftAbove = nullptr;
				
				chunkBelow = nullptr;
				
				chunkRightBelow = nullptr;
				
				chunkLeftBelow = nullptr;
				
				chunkRight = nullptr;
				
				chunkLeft = nullptr;
			}
			
			int getOffsetX() const {
				return offset_x;
			}
			int getOffsetY() const {
				return offset_y;
			}
			
			void setOffset(int cX,int cY){
				offset_x=cX;
				offset_y=cY;
			}
			
			void setChunkRelative(EChunk* relChunk,int relx,int rely) {
				switch(rely){
					case -1:
						if(relx==1){
							chunkRightAbove = relChunk;
						} else if(relx==-1){
							chunkLeftAbove = relChunk;
						} else {
							chunkAbove = relChunk;
						}
						break;
					case 0:
						if(relx==1){
							chunkRight = relChunk;
						} else if(relx==-1){
							chunkLeft = relChunk;
						}
						break;
					case 1:
						if(relx==1){
							chunkRightBelow = relChunk;
						} else if(relx==-1){
							chunkLeftBelow = relChunk;
						} else {
							chunkBelow = relChunk;
						}
						break;
				}
			}

			Tile& getTile(int row, int col) {
				return *chunk_data[row * CHUNK_SIZE + col];
			}
			
			int getTileData(int row, int col) const {
				return chunk_data[row * CHUNK_SIZE + col]->getType();
			}
			
			int getTileDataAt(int row, int col) const {
				//Get tile data
				if(row<0){ //Left
					if(col<0){
						if(chunkLeftAbove==nullptr){
							return NONE;
						} else {
							return chunkLeftAbove->chunk_data[(CHUNK_SIZE-1) * CHUNK_SIZE + col+CHUNK_SIZE]->getType();
						}
						
						
					} else if(col>=CHUNK_SIZE){
						if(chunkLeftBelow==nullptr){
							return NONE;
						} else {
							return chunkLeftBelow->chunk_data[(CHUNK_SIZE-1) * CHUNK_SIZE + col-CHUNK_SIZE]->getType();
						}
						
						
					} else if(chunkLeft!=nullptr){
						return chunkLeft->chunk_data[(CHUNK_SIZE-1) * CHUNK_SIZE + col]->getType();
						
					} else {
						return NONE;
					}
				} else if(row>CHUNK_SIZE-2){ //Right
					if(col<0){
						if(chunkRightAbove==nullptr){
							return NONE;
						} else {
							return chunkRightAbove->chunk_data[(row-CHUNK_SIZE) * CHUNK_SIZE + col+CHUNK_SIZE]->getType();
						}
						
						
					} else if(col>=CHUNK_SIZE){
						if(chunkRightBelow==nullptr){
							return NONE;
						} else {
							return chunkRightBelow->chunk_data[(row-CHUNK_SIZE) * CHUNK_SIZE + col-CHUNK_SIZE]->getType();
						}
						
						
					} else if(chunkRight!=nullptr){
						return chunkRight->chunk_data[(row-CHUNK_SIZE) * CHUNK_SIZE + col]->getType();
						
						
					} else {
						return NONE;
					}
				} else {
					if(col<0){
						if(chunkAbove==nullptr){
							return NONE;
						} else {
							return chunkAbove->chunk_data[row * CHUNK_SIZE + col+CHUNK_SIZE]->getType();
						}
						
						
					} else if(col>=CHUNK_SIZE){
						if(chunkBelow==nullptr){
							return NONE;
						} else {
							return chunkBelow->chunk_data[row * CHUNK_SIZE + col-CHUNK_SIZE]->getType();
						}
						
					} else {
						return chunk_data[row * CHUNK_SIZE + col]->getType();

					}
				}
				return NONE;
			}
			
			int getTileDataAt(int a) const {
				
				int row = a / CHUNK_SIZE;
				int col = a % CHUNK_SIZE;
				
				//Get tile data
				if(row<0){ //Left
					if(col<0){
						if(chunkLeftAbove==nullptr){
							return NONE;
						} else {
							return chunkLeftAbove->chunk_data[(CHUNK_SIZE-1) * CHUNK_SIZE + col+CHUNK_SIZE]->getType();
						}
						
						
					} else if(col>=CHUNK_SIZE){
						if(chunkLeftBelow==nullptr){
							return NONE;
						} else {
							return chunkLeftBelow->chunk_data[(CHUNK_SIZE-1) * CHUNK_SIZE + col-CHUNK_SIZE]->getType();
						}
						
						
					} else if(chunkLeft!=nullptr){
						return chunkLeft->chunk_data[(CHUNK_SIZE-1) * CHUNK_SIZE + col]->getType();
						
					} else {
						return NONE;
					}
				} else if(row>CHUNK_SIZE-2){ //Right
					if(col<0){
						if(chunkRightAbove==nullptr){
							return NONE;
						} else {
							return chunkRightAbove->chunk_data[(row-CHUNK_SIZE) * CHUNK_SIZE + col+CHUNK_SIZE]->getType();
						}
						
						
					} else if(col>=CHUNK_SIZE){
						if(chunkRightBelow==nullptr){
							return NONE;
						} else {
							return chunkRightBelow->chunk_data[(row-CHUNK_SIZE) * CHUNK_SIZE + col-CHUNK_SIZE]->getType();
						}
						
						
					} else if(chunkRight!=nullptr){
						return chunkRight->chunk_data[(row-CHUNK_SIZE) * CHUNK_SIZE + col]->getType();
						
						
					} else {
						return NONE;
					}
				} else {
					if(col<0){
						if(chunkAbove==nullptr){
							return NONE;
						} else {
							return chunkAbove->chunk_data[row * CHUNK_SIZE + col+CHUNK_SIZE]->getType();
						}
						
						
					} else if(col>=CHUNK_SIZE){
						if(chunkBelow==nullptr){
							return NONE;
						} else {
							return chunkBelow->chunk_data[row * CHUNK_SIZE + col-CHUNK_SIZE]->getType();
						}
						
					} else {
						return chunk_data[row * CHUNK_SIZE + col]->getType();

					}
				}
				return NONE;
			}
			
			void setTile(int row, int col, std::shared_ptr<Tile> tile){
				
				chunk_data[row * CHUNK_SIZE + col]->setType(type);
			}
			
			void setCycleAt(int row, int col, int newcycle){
				if(row<0){ //Left
					if(col<0&&chunkLeftAbove!=nullptr){
						chunkLeftAbove->chunk_data[(CHUNK_SIZE-1) * CHUNK_SIZE + col+CHUNK_SIZE].setCycle(newcycle);
						
					} else if(col>=CHUNK_SIZE&&chunkLeftBelow!=nullptr){
						chunkLeftBelow->chunk_data[(CHUNK_SIZE-1) * CHUNK_SIZE + col-CHUNK_SIZE].setCycle(newcycle);
						
					} else if(chunkLeft!=nullptr){
						chunkLeft->chunk_data[(CHUNK_SIZE-1) * CHUNK_SIZE + col].setCycle(newcycle);
						
					}
				} else if(row>CHUNK_SIZE-2){ //Right
					if(col<0&&chunkRightAbove!=nullptr){
						chunkRightAbove->chunk_data[(row-CHUNK_SIZE) * CHUNK_SIZE + col+CHUNK_SIZE].setCycle(newcycle);
						
					} else if(col>=CHUNK_SIZE&&chunkRightBelow!=nullptr){
						chunkRightBelow->chunk_data[(row-CHUNK_SIZE) * CHUNK_SIZE + col-CHUNK_SIZE].setCycle(newcycle);
						
					} else if(chunkRight!=nullptr){
						chunkRight->chunk_data[(row-CHUNK_SIZE) * CHUNK_SIZE + col].setCycle(newcycle);
						
					}
				} else {
					if(col<0&&chunkAbove!=nullptr){
						chunkAbove->chunk_data[row * CHUNK_SIZE + col+CHUNK_SIZE].setCycle(newcycle);
						
					} else if(col>=CHUNK_SIZE&&chunkBelow!=nullptr){
						chunkBelow->chunk_data[row * CHUNK_SIZE + col-CHUNK_SIZE].setCycle(newcycle);
						
					} else {
						chunk_data[row * CHUNK_SIZE + col].setCycle(newcycle);
					}
					
				}
			}
			
			void setTileAt(int row, int col, int type){
				if(row<0){ //Left
					if(col<0&&chunkLeftAbove!=nullptr){
						chunkLeftAbove->setTile(row+CHUNK_SIZE,col+CHUNK_SIZE,type);
						
					} else if(col>=CHUNK_SIZE&&chunkLeftBelow!=nullptr){
						chunkLeftBelow->setTile(row+CHUNK_SIZE,col-CHUNK_SIZE,type);
						
					} else if(chunkLeft!=nullptr){
						chunkLeft->setTile(row+CHUNK_SIZE,col,type);
						
					}
				} else if(row>CHUNK_SIZE-2){ //Right
					if(col<0&&chunkRightAbove!=nullptr){
						chunkRightAbove->setTile(row-CHUNK_SIZE,col+CHUNK_SIZE,type);
						
					} else if(col>=CHUNK_SIZE&&chunkRightBelow!=nullptr){
						chunkRightBelow->setTile(row-CHUNK_SIZE,col-CHUNK_SIZE,type);
						
					} else if(chunkRight!=nullptr){
						chunkRight->setTile(row-CHUNK_SIZE,col,type);
						
					}
				} else {
					if(col<0&&chunkAbove!=nullptr){
						chunkAbove->setTile(row,col+CHUNK_SIZE,type);
						
					} else if(col>=CHUNK_SIZE&&chunkBelow!=nullptr){
						chunkBelow->setTile(row,col-CHUNK_SIZE,type);
						
					} else {
						setTile(row,col,type);
						
					}
				}
				//Get which chunk is needed
				//Todo: call setTile
				//chunk_data[row * CHUNK_SIZE + col].setType(type);
			}
			
			
			
			void generateChunk(){
				
				for(int x=0;x<CHUNK_SIZE;++x){
					
					for(int y=0;y<CHUNK_SIZE;++y){
						if(y==CHUNK_SIZE-1){
							setTile(y,x,ROCK);
						} else {
							if((rand() % 4) == SAND){
								setTile(y,x,SAND);
							}
							if((rand() % 5) == WATER){
								setTile(y,x,WATER);
							}
						}
					}
				}
			}
			
			void swapTiles(int y1, int x1, int y2, int x2){
				//Get which chunk is needed
				int tdata2=getTileDataAt(y2,x2);
				if(tdata2==-1||y1<0||x1<0||x1>CHUNK_SIZE||y1>CHUNK_SIZE){
					return;
				}
				
				int temp_type=chunk_data[y1*CHUNK_SIZE+x1].getType();
				int temp_updateCycle=chunk_data[y1*CHUNK_SIZE+x1].getCycle();
				
				
				setTileAt(y1,x1,getTileDataAt(y2,x2));
				setTileAt(y2,x2,temp_type);
				
				
				//chunk_data[y1*CHUNK_SIZE+x1].setType(chunk_data[y2*CHUNK_SIZE+x2].getType());
				//chunk_data[y2*CHUNK_SIZE+x2].setType(temp_type);
				
				
				setCycleAt(y2,x2,temp_updateCycle);
			}
			
			void updateTile(int y, int x){ //y,x
				chunk_data[y*CHUNK_SIZE+x].processCycle();
				//chunk_data[y*CHUNK_SIZE+x].update(*this, y, x);
				//TODO refactor
				switch(chunk_data[y*CHUNK_SIZE+x].getType()){
					case SAND:{
							
							int belowIndex = getTileDataAt(y + 1 , x);
							int leftBelowIndex = getTileDataAt(y + 1 , x - 1);
							int rightBelowIndex = getTileDataAt(y + 1 , x + 1);

							if (belowIndex == AIR || belowIndex == WATER) {
								swapTiles(y, x, y + 1, x);

							}
							
							else if (leftBelowIndex == AIR || leftBelowIndex == WATER) {
								swapTiles(y, x, y + 1, x - 1);

							}
							
							else if ( rightBelowIndex == AIR || rightBelowIndex == WATER) {
								swapTiles(y, x, y + 1, x + 1);
							}
						}
						break;
					case WATER:{
							
							int rightIndex = getTileDataAt(y , x+1);
							int leftIndex = getTileDataAt(y , x-1);
							int belowIndex = getTileDataAt(y + 1 , x);
							int leftBelowIndex = getTileDataAt(y + 1 , x - 1);
							int rightBelowIndex = getTileDataAt(y + 1 , x + 1);

							if (belowIndex == AIR) {
								swapTiles(y, x, y + 1, x);
							}
							
							else if (leftBelowIndex == AIR) {
								swapTiles(y, x, y + 1, x - 1);
							}
							
							else if (rightBelowIndex == AIR) {
								swapTiles(y, x, y + 1, x + 1);
							} else if(rightIndex == AIR){ //Right
								swapTiles(y, x, y, x + 1);
							} else if(leftIndex == AIR){ //Left
								swapTiles(y, x, y, x - 1);
							}
						}
						break;
					case STEAM:{ //TODO refactor this
							int chmod=0;
							int upIndex = (y - 1) * CHUNK_SIZE + x;
							if (y <=0) {
								chmod=1;
								upIndex+=CHUNK_SIZE;
							}
								
							int randdir=0;
							if(rand()%3==0){
								randdir=1;
							}
							else if(rand()%3==0){
								randdir=-1;
							}
							int d=getTileDataAt(upIndex+randdir);
							if (d == AIR || d == WATER) {
								swapTiles(y, x, y - 1+chmod, x+randdir);
							}
						}
						break;
					
					case LAVA:{
							
							int rightIndex = getTileDataAt(y , x+1);
							int leftIndex = getTileDataAt(y , x-1);
							int belowIndex = getTileDataAt(y + 1 , x);
							int leftBelowIndex = getTileDataAt(y + 1 , x - 1);
							int rightBelowIndex = getTileDataAt(y + 1 , x + 1);

							if (belowIndex == AIR) {
								swapTiles(y, x, y + 1, x);
								break;
							} else if(belowIndex == WATER){
								setTileAt(y,x,STEAM);
								setTileAt(y+1,x,ROCK);
							}
							else if (leftBelowIndex == AIR) {
								swapTiles(y, x, y + 1, x - 1);
								break;
							}
							else if (rightBelowIndex == AIR) {
								swapTiles(y, x, y + 1, x + 1);
								break;
								
							}
							//Right
							if(rightIndex == AIR){
								swapTiles(y, x, y, x + 1);
								break;
							} else if(rightIndex == WATER){
								setTileAt(y,x,STEAM);
								setTileAt(y,x+1,ROCK);
							}
							
							//Left
							if(leftIndex == AIR){
								swapTiles(y, x, y, x - 1);
								break;
							} else if(leftIndex == WATER){
								setTileAt(y,x,STEAM);
								setTileAt(y,x-1,ROCK);
							}
							
							
							if( getTileDataAt((y - 1), x)==WATER){
								setTileAt(y,x,STEAM);
								setTileAt(y-1,x,ROCK);
							}
						}
						break;
					case ACID:{
							
							int belowIndex = getTileDataAt(y + 1 , x);
							int leftBelowIndex = getTileDataAt(y + 1 , x - 1);
							int rightBelowIndex = getTileDataAt(y + 1 , x + 1);

							if (belowIndex == AIR) {
								swapTiles(y, x, y + 1, x);

							} else if(belowIndex != ACID&&rand()%8==0){
								setTileAt(y+1,x,AIR);
								if(rand()%9==0){
									setTileAt(y,x,STEAM);
								}
							}
							
							else if (leftBelowIndex == AIR) {
								swapTiles(y, x, y + 1, x - 1);

							}
							
							else if ( rightBelowIndex == AIR) {
								swapTiles(y, x, y + 1, x + 1);
							}
						}
						break;
					case GRAVEL:{
							
							int rightIndex = getTileDataAt(y , x+1);
							int leftIndex = getTileDataAt(y , x-1);
							int belowIndex = getTileDataAt(y + 1 , x);

							if (rightIndex == AIR||leftIndex==AIR||rightIndex == WATER||leftIndex==WATER) {
								if(belowIndex==AIR||belowIndex==WATER){
									swapTiles(y, x, y + 1, x);
								}
								
							}
						}
						break;
					case COPPER:{
							//TODO make it generate smoke on electricity
							int belowIndex = getTileDataAt(y + 1 , x);
							int leftBelowIndex = getTileDataAt(y + 1 , x - 1);
							int rightBelowIndex = getTileDataAt(y + 1 , x + 1);

							if (belowIndex == AIR || belowIndex == WATER || belowIndex == SAND) {
								swapTiles(y, x, y + 1, x);

							}
							
							else if (leftBelowIndex == AIR || leftBelowIndex == WATER || belowIndex == SAND) {
								swapTiles(y, x, y + 1, x - 1);

							}
							
							else if ( rightBelowIndex == AIR || rightBelowIndex == WATER || belowIndex == SAND) {
								swapTiles(y, x, y + 1, x + 1);
							}
						}
						break;
					case OIL:{
							//TODO make fire
							int upIndex = getTileDataAt(y-1 , x);
							//int upRightIndex = getTileDataAt(y-1 , x+1);
							//int upLeftIndex = getTileDataAt(y-1 , x-1);
							
							int rightIndex = getTileDataAt(y , x+1);
							int leftIndex = getTileDataAt(y , x-1);
							int belowIndex = getTileDataAt(y + 1 , x);
							int leftBelowIndex = getTileDataAt(y + 1 , x - 1);
							int rightBelowIndex = getTileDataAt(y + 1 , x + 1);
							
							if (belowIndex == AIR) {
								swapTiles(y, x, y + 1, x);
							}
							
							else if (leftBelowIndex == AIR) {
								swapTiles(y, x, y + 1, x - 1);
							}
							
							else if (rightBelowIndex == AIR) {
								swapTiles(y, x, y + 1, x + 1);
							} else if(rightIndex == AIR){ //Right
								swapTiles(y, x, y, x + 1);
							} else if(leftIndex == AIR){ //Left
								swapTiles(y, x, y, x - 1);
							} else if (upIndex == WATER) {
								swapTiles(y, x, y - 1, x);
							} else if (rightIndex == WATER) {
								swapTiles(y, x, y , x+1);
							} else if (leftIndex == WATER) {
								swapTiles(y, x, y , x-1);
							}
						}
						break;
					
					case FIRE:{
						
							//TODO make fire
							int upIndex = getTileDataAt(y-1 , x);
							
							int rightIndex = getTileDataAt(y , x+1);
							int leftIndex = getTileDataAt(y , x-1);
							
							int belowIndex = getTileDataAt(y + 1 , x);
							int leftBelowIndex = getTileDataAt(y + 1 , x - 1);
							int rightBelowIndex = getTileDataAt(y + 1 , x + 1);
							
							//BURN
							if(rand()%fire_spread_rate==0){
								if (belowIndex == OIL||belowIndex == WOOD||belowIndex == PLANT||belowIndex == LEAVES||belowIndex == COAL) {
									setTileAt(y+1,x,FIRE);
									setTileAt(y,x,SMOKE);
								}
								if (upIndex == OIL||upIndex == WOOD||upIndex == PLANT||upIndex == LEAVES||upIndex == COAL) {
									setTileAt(y-1,x,FIRE);
									setTileAt(y,x,SMOKE);
								}
								if (rightIndex == OIL||rightIndex == WOOD||rightIndex == PLANT||rightIndex == LEAVES||rightIndex == COAL) {
									setTileAt(y,x+1,FIRE);
									setTileAt(y,x,SMOKE);
								}
								if (leftIndex == OIL||leftIndex == WOOD||leftIndex == PLANT||leftIndex == LEAVES||leftIndex == COAL) {
									setTileAt(y,x-1,FIRE);
									setTileAt(y,x,SMOKE);
								}
								if(rand()%12==0){
									if(rand()%2==0){
										setTileAt(y,x,SMOKE);
									} else{
										setTileAt(y,x,ASH);
									}
									
								} else {
									setTileAt(y,x,FIRE); //Burn anim
								}
							}
							else if (belowIndex == AIR) {
								swapTiles(y, x, y + 1, x);

							}
							else if (leftBelowIndex == AIR) {
								swapTiles(y, x, y + 1, x - 1);

							}
							else if ( rightBelowIndex == AIR) {
								swapTiles(y, x, y + 1, x + 1);
								
							} else if (belowIndex == WATER) {
								setTileAt(y,x,STEAM);
							} else if (upIndex == WATER) {
								setTileAt(y,x,STEAM);
							} else if (rightIndex == WATER) {
								setTileAt(y,x,STEAM);
							} else if (leftIndex == WATER) {
								setTileAt(y,x,STEAM);
							}
							

						}
						break;
					case SMOKE:{ //TODO refactor this
							int chmod=0;
							int upIndex = (y - 1) * CHUNK_SIZE + x;
							if (y <=0) {
								chmod=1;
								upIndex+=CHUNK_SIZE;
							}
								
							int randdir=0;
							if(rand()%2==0){
								randdir=1;
							}
							else if(rand()%2==0){
								randdir=-1;
							}
							
							int d=getTileDataAt(upIndex+randdir);
							if (d == AIR ) {
								swapTiles(y, x, y - 1+chmod, x+randdir);
								if(rand()%100==0){
									setTileAt(y,x,SMOKE);
								}
							}
						}
						break;
					case GLUE:{
							//int upIndex = getTileDataAt(y - 1 , x);
							int belowIndex = getTileDataAt(y + 1 , x);
							int leftIndex = getTileDataAt(y , x - 1);
							int rightIndex = getTileDataAt(y , x + 1);

							if (belowIndex == AIR && (rightIndex == AIR || rightIndex == GLUE) && (leftIndex == AIR || leftIndex == GLUE)) {
								swapTiles(y, x, y + 1, x);
							}
						}
						break;
					case DIRT:{
							
							int belowIndex = getTileDataAt(y + 1 , x);
							int leftBelowIndex = getTileDataAt(y + 1 , x - 1);
							int rightBelowIndex = getTileDataAt(y + 1 , x + 1);

							if (belowIndex == AIR || belowIndex == WATER) {
								swapTiles(y, x, y + 1, x);

							}
							
							else if (rand()%4==0 && (leftBelowIndex == AIR || leftBelowIndex == WATER)) {
								swapTiles(y, x, y + 1, x - 1);

							}
							
							else if (rand()%4==0 && (rightBelowIndex == AIR || rightBelowIndex == WATER)) {
								swapTiles(y, x, y + 1, x + 1);
							}
						}
						break;
					case SEED:{
							
							int belowIndex = getTileDataAt(y + 1 , x);
							int upIndex = getTileDataAt(y - 1 , x);

							if (belowIndex == AIR || belowIndex == WATER) {
								swapTiles(y, x, y + 1, x);

							}
							
							else if (belowIndex == DIRT && upIndex == WATER) {
								setTileAt(y-1,x,AIR);
								setTileAt(y,x,PLANT);

							}
						}
						break;
					case PLANT:{
							
							int belowIndex = getTileDataAt(y + 1 , x);
							int upIndex = getTileDataAt(y - 1 , x);
							int upRightIndex = getTileDataAt(y - 1 , x+1);
							int upLeftIndex = getTileDataAt(y - 1 , x-1);

							if (belowIndex == WATER||upIndex==GRAVEL||upIndex==DIRT) {
								swapTiles(y, x, y + 1, x);

							}
							
							//GROW
							else if (rand()%plant_growth_rate==0) { //belowIndex == DIRT && upIndex == WATER
								if(rand()%4!=0&&upIndex==AIR){
									setTileAt(y-1,x,PLANT);
								} else if(rand()%2==0&&upRightIndex==AIR){
									setTileAt(y-1,x+1,PLANT);
								} else if(upLeftIndex==AIR){
									setTileAt(y-1,x-1,PLANT);
								}
							
							//PETAL BLOOM
							} else if(rand()%(plant_growth_rate*plant_petal_rate)==0){
								setTileAt(y-1,x-1,PETAL);
							}
						}
						break;
					case PETAL:{
							
							int belowIndex = getTileDataAt(y + 1 , x);

							if (belowIndex == AIR) {
								swapTiles(y, x, y + 1, x);

							} else if(belowIndex==WATER){
								setTileAt(y,x,AIR);
							} else if(belowIndex==LAVA||belowIndex==FIRE){
								setTileAt(y,x,SMOKE);
							}
						}
						break;
					case ALGAE:{
							
							int upIndex = getTileDataAt(y-1 , x);
							int belowIndex = getTileDataAt(y + 1 , x);

							if (belowIndex == AIR||belowIndex == WATER) {
								swapTiles(y, x, y + 1, x);
							}
							
							else if (upIndex == WATER&&rand()%plant_growth_rate==0) {
								setTileAt(y-1,x,ALGAE);
							}
						}
						break;
					case BAMBOO:{
							// bamboo (grows upwards on sand, only on sand and dirt), 
							int upIndex = getTileDataAt(y-1 , x);
							int belowIndex = getTileDataAt(y + 1 , x);

							if (belowIndex == AIR) {
								swapTiles(y, x, y + 1, x);
							}
							
							else if ((belowIndex==SAND||belowIndex==BAMBOO||belowIndex==DIRT)&&upIndex == AIR&&rand()%plant_growth_rate==0) {
								setTileAt(y-1,x,BAMBOO);
							}
						}
						break;
					case SAPLING:{

							int upIndex = getTileDataAt(y-1 , x);
							int belowIndex = getTileDataAt(y + 1 , x);
							int leftIndex = getTileDataAt(y , x-1);
							int rightIndex = getTileDataAt(y , x+1);

							if (belowIndex == AIR&&rightIndex!=WOOD&&leftIndex!=WOOD) {
								swapTiles(y, x, y + 1, x);
							}
							
							else if ((belowIndex==WOOD||belowIndex==DIRT||rightIndex==WOOD||leftIndex==WOOD)&&upIndex == AIR&&rand()%(plant_growth_rate/2)==0) {
								
								setTileAt(y,x,WOOD);
								if(rand()%(plant_growth_rate/3)==0){
									if(rand()%2==0&&leftIndex==AIR){
										setTileAt(y,x-1,SAPLING);
									} else if(rightIndex==AIR){
										setTileAt(y,x+1,SAPLING);
									}
								}
								if(rand()%(plant_growth_rate*plant_petal_rate)==0){
									setTileAt(y-1,x,LEAVES);
								} else {
									setTileAt(y-1,x,SAPLING);
								}
							}
						}
						break;
					case COAL:{
							
							int belowIndex = getTileDataAt(y + 1 , x);
							int leftBelowIndex = getTileDataAt(y + 1 , x - 1);
							int rightBelowIndex = getTileDataAt(y + 1 , x + 1);

							if (belowIndex == AIR || belowIndex == WATER) {
								swapTiles(y, x, y + 1, x);

							}
							
							else if (leftBelowIndex == AIR || leftBelowIndex == WATER) {
								swapTiles(y, x, y + 1, x - 1);

							}
							
							else if ( rightBelowIndex == AIR || rightBelowIndex == WATER) {
								swapTiles(y, x, y + 1, x + 1);
							}
						}
						break;
					case ASH:{
							
							int belowIndex = getTileDataAt(y + 1 , x);
							int leftBelowIndex = getTileDataAt(y + 1 , x - 1);
							int rightBelowIndex = getTileDataAt(y + 1 , x + 1);

							if (belowIndex == AIR || belowIndex == WATER) {
								swapTiles(y, x, y + 1, x);

							}
							
							else if (leftBelowIndex == AIR || leftBelowIndex == WATER) {
								swapTiles(y, x, y + 1, x - 1);

							}
							
							else if ( rightBelowIndex == AIR || rightBelowIndex == WATER) {
								swapTiles(y, x, y + 1, x + 1);
							}
						}
						break;
					case THERMITE:{
							
							int upIndex = getTileDataAt(y-1 , x);
							int belowIndex = getTileDataAt(y + 1 , x);
							int leftBelowIndex = getTileDataAt(y + 1 , x - 1);
							int rightBelowIndex = getTileDataAt(y + 1 , x + 1);
							int leftIndex = getTileDataAt(y , x - 1);
							int rightIndex = getTileDataAt(y , x + 1);
							
							if(belowIndex==FIRE||upIndex==FIRE||leftIndex==FIRE||rightIndex==FIRE){
								
								for(int ax=x-explosion_radius;ax<x+explosion_radius;ax++){
									for(int ay=y-explosion_radius;ay<y+explosion_radius;ay++){
										if(abs(ax-ay)<explosion_radius){
											if(rand()%2==0){
												setTileAt(ay,ax,AIR);
											} else {
												setTileAt(ay,ax,SMOKE);
											}
										}
									}
								}
							}
							if (belowIndex == AIR || belowIndex == WATER) {
								swapTiles(y, x, y + 1, x);

							}
							
							else if (leftBelowIndex == AIR || leftBelowIndex == WATER) {
								swapTiles(y, x, y + 1, x - 1);

							}
							
							else if ( rightBelowIndex == AIR || rightBelowIndex == WATER) {
								swapTiles(y, x, y + 1, x + 1);
							}
						}
						break;
				}
			}
			
			void updateChunk(){
				for(int x=0;x<CHUNK_SIZE;++x){
					
					for(int y=0;y<CHUNK_SIZE;++y){
						if(chunk_data[y*CHUNK_SIZE+x].getCycle()==chunkUpdateCycle){
							updateTile(y,x);
						}
					}
				}
				chunkUpdateCycle++;
			}
			
			void drawChunk(Renderer& renderer){ //, const sf::View& view
				sf::VertexArray walls(sf::Quads);
				walls.resize(CHUNK_SIZE * CHUNK_SIZE * 4);
				
				/*sf::FloatRect viewBounds(
					view.getCenter() - view.getSize() / 2.f,
					view.getSize()
				);*/
		
				for(int x=0;x<CHUNK_SIZE;++x){
					/*loat xPos = x * VERT_SIZE;

					// Skip columns outside the view bounds
					if (xPos + VERT_SIZE < viewBounds.left || xPos > viewBounds.left + viewBounds.width) {
						continue;
					}*/

					for(int y=0;y<CHUNK_SIZE;++y){
						/*float yPos = y * VERT_SIZE;

						// Skip rows outside the view bounds
						if (yPos + VERT_SIZE < viewBounds.top || yPos > viewBounds.top + viewBounds.height) {
							continue;
						}*/
						if(chunk_data[y*CHUNK_SIZE+x].getType()!=AIR){
							
							sf::Color tclr=chunk_data[y*CHUNK_SIZE+x].getColor();
							sf::Vector2f topLeft(offset_x+x * VERT_SIZE, offset_y+ y*VERT_SIZE);
							sf::Vector2f topRight(offset_x+x * VERT_SIZE + VERT_SIZE,offset_y+ y*VERT_SIZE);
							sf::Vector2f bottomLeft(offset_x+x * VERT_SIZE,offset_y+ y*VERT_SIZE+VERT_SIZE);
							sf::Vector2f bottomRight(offset_x+x * VERT_SIZE + VERT_SIZE,offset_y+ y*VERT_SIZE+VERT_SIZE);

							walls.append(sf::Vertex(topLeft, tclr));
							walls.append(sf::Vertex(topRight, tclr));
							walls.append(sf::Vertex(bottomRight, tclr));
							walls.append(sf::Vertex(bottomLeft, tclr));
							
						}
						
					}
				}
				
				renderer.window.draw(walls);
			}
	};
	
	//load in chunks from txt file,
	//loader class,
	class ChunkLoader{
		private:
			
		public:
			ChunkLoader(){
				
			}
			
			void saveChunk(EChunk* chunk,std::string filename){
				std::ofstream chunksave(filename+".dat");
				
				std::string offset_config=std::to_string(chunk->getOffsetX())+" "+std::to_string(chunk->getOffsetY())+'\n';
				chunksave << offset_config;
				for(int y=0;y<CHUNK_SIZE;y++){
					std::string t_line="";
					for(int x=0;x<CHUNK_SIZE;x++){
						t_line+=std::to_string(chunk->getTileData(y,x))+" ";
					}
					t_line+="\n";
					chunksave << t_line;
				}
				
				chunksave.close();
			}
			
			void loadChunk(EChunk* temp_chunk,std::string filename){
				std::string temp_line;

				std::ifstream chunkload(filename+".dat");
				
				int cur_line=0;

				while (getline (chunkload, temp_line)&&cur_line!=CHUNK_SIZE+1) {
					if(cur_line==0){
						int offx;
						int offy;

						std::istringstream parser_offsets(temp_line);
						parser_offsets >> offx >> offy;

						if (parser_offsets) {
							temp_chunk->setOffset(offx,offy);
						} else {
							std::cerr << "Hibás régiófájl a beolvasáskor." << std::endl;
						}
					} else {
						std::istringstream parser_values(temp_line);
						std::vector<int> numbers;
						int value;

						int r_col=0;
						while (parser_values >> value) {
							
							temp_chunk->setTile(cur_line-1,r_col,value); //cur_line-1,r_col
							r_col++;
						}
					}
					
					cur_line++;
				}

				chunkload.close();
			}
			
			~ChunkLoader(){
				
			}
	};
	
}

class Player {
	public:
		double posx;
		double posy;
		double speed;
		
		int palette_type;
		sf::CircleShape shape;
		
		int a_down;
		int s_down;
		int d_down;
		int w_down;
		
		int size;

		// Constructor
		Player(double x, double y) : posx(x), posy(y), shape(10) { // Use initializer list for CircleShape
			w_down=0;
			a_down=0;
			s_down=0;
			d_down=0;
			size=30;
			shape.setFillColor(sf::Color(100, 250, 50)); // Set initial color
			speed=5.0;
			std::cout << "Player Init!" << "\n";
			
			palette_type=sandsim::ROCK;
		}

		void draw(Renderer& renderer) {
			shape.setPosition(static_cast<int>(posx), static_cast<int>(posy));
			renderer.window.draw(shape);
			
		}

		void update_input() {
			
			/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
				a_down=1;
			} else {
				a_down=0;
			}
			
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
				d_down=1;
			} else {
				d_down=0;
			}
			
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
				
				w_down=1;
			} else {
				w_down=0;
			}
			
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
				s_down=1;
			} else {
				s_down=0;
			}
			
			if(a_down==1){
				posx-=speed;
			}
			
			if(d_down==1){
				posx+=speed;
			}*/
			
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
				simulationRunning=true;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
				simulationRunning=false;
			}
			
			if(w_down==1){
				posy-=speed;
			}
			
			if(s_down==1){
				posy+=speed;
			}
			
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
				palette_type=sandsim::AIR;
			} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
				palette_type=sandsim::DIRT;
			} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) {
				palette_type=sandsim::WATER;
			} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) {
				palette_type=sandsim::ROCK;
			} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5)) {
				palette_type=sandsim::PLANT;
			} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6)) {
				palette_type=sandsim::FIRE;
			} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num7)) {
				palette_type=sandsim::SEED;
			} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num8)) {
				palette_type=sandsim::GRAVEL;
			} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num9)) {
				palette_type=sandsim::ALGAE;
			} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num0)) {
				palette_type=sandsim::SAPLING;
			}
			
		}
		
};

double dist(double ax,double ay,double bx, double by){
	return (sqrt((bx-ax)*(bx-ax)+(by-ay)*(by-ay)));
}

//void drawMap2D(Renderer renderer){
				// change the size to 100x100
				//rectangle.setPosition(x*CELL_SIZE,y*CELL_SIZE);
				////renderer.window.draw(rectangle);

int main() {
	std::set_new_handler(outOfMemHandler);
	std::srand(static_cast<unsigned>(std::time(0)));
	
    // Create the SFML window
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), GAME_NAME);

    // Initialize Renderer
    Renderer renderer(window);
    renderer.init();
	renderer.set_sky_scale(2.0);

    // Create a Player instance
    Player player(100, 100);
	
	sandsim::EChunk chunk1(0,0); //Temp
	sandsim::EChunk chunk2(1,0);
	
	sandsim::ChunkLoader temp_chunk_loader;
	//
	
	temp_chunk_loader.loadChunk(&chunk1,"chunk1");
	
	chunk1.setChunkRelative(&chunk2,0,1);
	
	
	chunk2.setChunkRelative(&chunk1,0,-1);

	int game_timer=0;
    // Main loop
    while (window.isOpen()==true) {
        sf::Event event;
        while (window.pollEvent(event)==true) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Update player input
        player.update_input();

        // Clear the window
        window.clear(sf::Color::Black);
		//renderer.draw_sky();
		
		//renderer.draw_sky();
		
		if( sf::Mouse::isButtonPressed( sf::Mouse::Button::Left ) ){
			//TODO get_chunk boundary
			if(sf::Mouse::getPosition(window).x<CHUNK_SIZE*VERT_SIZE&&sf::Mouse::getPosition(window).y<CHUNK_SIZE*VERT_SIZE){
				for(int i=static_cast<int>(sf::Mouse::getPosition(window).y/VERT_SIZE)-BRUSH_SIZE/2;i<=static_cast<int>(sf::Mouse::getPosition(window).y/VERT_SIZE)+BRUSH_SIZE/2;i++){
					for(int j=static_cast<int>(sf::Mouse::getPosition(window).x/VERT_SIZE)-BRUSH_SIZE/2;j<=static_cast<int>(sf::Mouse::getPosition(window).x/VERT_SIZE)+BRUSH_SIZE/2;j++){
						if(j>0&&i>0&&j<CHUNK_SIZE*VERT_SIZE&&i<CHUNK_SIZE*VERT_SIZE){
							chunk1.setTile(i,j,player.palette_type);
						}
					}
				}
				
			}
			//
		}
		
		if(game_timer%UPDATE_FRAMES==0&&simulationRunning==true){
			chunk1.updateChunk();
			chunk2.updateChunk();
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)&&tempsavevar==true) {
			std::cout << "Saved chunk1.dat" << std::endl;
			temp_chunk_loader.saveChunk(&chunk1,"chunk1");
			tempsavevar=false;
		}
		
		chunk1.drawChunk(renderer);
		chunk2.drawChunk(renderer);

        // Draw the player
		player.draw(renderer);
		
        // Display the window contents
        renderer.window.display();
		
		game_timer++;
    }

    return 0;
}
