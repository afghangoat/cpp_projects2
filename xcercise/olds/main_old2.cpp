#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <cstdlib>

#define GAME_NAME "Noita Clone"
#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 1000
#define CHUNK_SIZE 256

#define FPS 40
#define UPDATE_FRAMES 2
#define FRAME_TIME (1000/FPS)
#define RAD 0.0174532925

#define VERT_SIZE 3

#define BRUSH_SIZE 5

double difference(double p1, double p2){
	return fabs(p1-p2);
}
int map[]={
	1,1,1,1,1,1,1,1,
	1,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,1,
	1,0,0,2,0,0,0,1,
	1,0,0,3,0,0,0,1,
	1,0,1,1,1,4,3,1,
	1,0,0,0,0,0,0,1,
	1,1,1,1,1,1,1,1
};

int mapFloors[]={
	2,3,2,1,2,2,2,2,
	2,3,2,1,2,2,2,2,
	2,3,2,1,2,2,2,2,
	2,3,2,1,2,2,2,2,
	2,3,2,1,2,2,2,2,
	2,3,2,3,2,2,2,2,
	2,3,2,3,2,2,2,2,
	2,3,2,3,2,2,2,2
};

int mapCeilings[]={
	2,2,2,2,2,2,2,2,
	2,0,0,2,2,2,2,2,
	2,0,0,2,2,2,2,2,
	2,2,2,2,2,2,2,2,
	2,2,2,2,2,2,2,2,
	2,2,2,2,2,2,2,2,
	2,2,2,2,2,2,2,2,
	2,2,2,2,2,2,2,2
};

class Renderer {
	private:
		//sf::Texture sky;
		//sf::Sprite sky_sprite;
	public:
		sf::RenderWindow& window;

		// Constructor to initialize with an existing window
		Renderer(sf::RenderWindow& win) : window(win) {
			
		}

		void init() {
			window.setFramerateLimit(FPS);
			/*if (!sky.loadFromFile("bg1.png")) {
				perror("Couldn't load texture \"bg1.png\".");
				return;
			} else {
				sky_sprite.setTexture(sky);
				sky_sprite.setPosition(0,0);
				sky_sprite.setScale(2.0,2.0);
			}
		}
		void draw_sky(){
			window.draw(sky_sprite);
		}*/
		}
};

enum TileTypes {
	AIR=0,
	SAND=1,
	WATER=2,
	ROCK=3,
	SMOKE=4,
	LAVA=5
	
};

const int ShadingVariability=20; //The +- rgb addings TODO - apply grain
std::unordered_map<int, sf::Color> ColorMap = {
    {AIR, sf::Color(0, 0, 0)},
    {SAND, sf::Color(255, 255, 153)},
	{WATER, sf::Color(102, 153, 255)},
	{ROCK, sf::Color(110,110,110)},
	{SMOKE, sf::Color(230,230,230)},
	{LAVA, sf::Color(255,110,21)}
};

//Todo function to fill this map up, probably put in gameConfig class

class Tile{
	private:
		int type;
		sf::Color color;
		int updateCycle;
		
	public:
		
		
		Tile(int base_type, sf::Color base_color): type(base_type), color(base_color) {
			updateCycle=0;
		}
		
		int getCycle(){
			return updateCycle;
		}
		
		void setCycle(int newCycle){
			updateCycle=newCycle;
		}
		
		void processCycle(){
			updateCycle++;
		}
		
		void setType(int new_type){
			type=new_type;
			color=ColorMap[type];
			
			int rVariation = (rand() % (ShadingVariability * 2 + 1)) - ShadingVariability;
			int gVariation = (rand() % (ShadingVariability * 2 + 1)) - ShadingVariability;
			int bVariation = (rand() % (ShadingVariability * 2 + 1)) - ShadingVariability;

			color.r = std::clamp(color.r + rVariation, 0, 255);
			color.g = std::clamp(color.g + gVariation, 0, 255);
			color.b = std::clamp(color.b + bVariation, 0, 255);
		}
		
		int getType(){
			return type;
		}
		
		sf::Color getColor(){
			return color;
		}
		
		void setColor(sf::Color new_color){
			color=new_color;
		}
		
		~Tile(){
			type=0;
		}
};

class Chunk{
	private:
		int offset_x;
		int offset_y;
		std::vector<Tile> chunk_data;
		
		int chunkUpdateCycle;
	public:
		Chunk(int cX, int cY): chunk_data(CHUNK_SIZE * CHUNK_SIZE, Tile(0, sf::Color(0, 0, 0))){
			offset_x=cX*CHUNK_SIZE*VERT_SIZE;
			offset_y=cY*CHUNK_SIZE*VERT_SIZE;
			chunkUpdateCycle=0;
			//Tile& tile = matrix[row * CHUNK_SIZE + col];
			
			//Adjustable action
			generateChunk();
		}
		
		~Chunk() = default;

		Tile& getTile(int row, int col) {
			return chunk_data[row * CHUNK_SIZE + col];
		}
		
		void setTile(int row, int col, int type){
			chunk_data[row * CHUNK_SIZE + col].setType(type);
		}
		
		void generateChunk(){
			
			for(int x=0;x<CHUNK_SIZE;++x){
				
				for(int y=0;y<CHUNK_SIZE;++y){
					if((rand() % 4) == SAND){
						setTile(y,x,SAND);
					}
					if((rand() % 5) == WATER){
						setTile(y,x,WATER);
					}
				}
			}
		}
		
		void swapTiles(int y1, int x1, int y2, int x2){
			int temp_type=chunk_data[y1*CHUNK_SIZE+x1].getType();
			int temp_updateCycle=chunk_data[y1*CHUNK_SIZE+x1].getCycle();
			
			chunk_data[y1*CHUNK_SIZE+x1].setType(chunk_data[y2*CHUNK_SIZE+x2].getType());
			chunk_data[y2*CHUNK_SIZE+x2].setType(temp_type);
			
			
			chunk_data[y2*CHUNK_SIZE+x2].setCycle(temp_updateCycle);
		}
		
		void updateTile(int y, int x){ //y,x
			chunk_data[y*CHUNK_SIZE+x].processCycle();
			switch(chunk_data[y*CHUNK_SIZE+x].getType()){
				case SAND:
					if (y != CHUNK_SIZE - 1) {
						
						int belowIndex = (y + 1) * CHUNK_SIZE + x;
						int leftBelowIndex = belowIndex - 1;
						int rightBelowIndex = belowIndex + 1;

						if (chunk_data[belowIndex].getType() == AIR || chunk_data[belowIndex].getType() == WATER) {
							swapTiles(y, x, y + 1, x);
						}
						
						else if (x > 0 && (chunk_data[leftBelowIndex].getType() == AIR || chunk_data[leftBelowIndex].getType() == WATER)) {
							swapTiles(y, x, y + 1, x - 1);
						}
						
						else if (x < CHUNK_SIZE - 1 && (chunk_data[rightBelowIndex].getType() == AIR || chunk_data[rightBelowIndex].getType() == WATER)) {
							swapTiles(y, x, y + 1, x + 1);
						}
					}
					break;
				case WATER:
					if (y != CHUNK_SIZE - 1) {
						
						int belowIndex = (y + 1) * CHUNK_SIZE + x;
						int rightIndex = y * CHUNK_SIZE + x+1;
						int leftIndex = y * CHUNK_SIZE + x-1;
						int leftBelowIndex = belowIndex - 1;
						int rightBelowIndex = belowIndex + 1;

						if (chunk_data[belowIndex].getType() == AIR) {
							swapTiles(y, x, y + 1, x);
						}
						
						else if (x > 0 && (chunk_data[leftBelowIndex].getType() == AIR)) {
							swapTiles(y, x, y + 1, x - 1);
						}
						
						else if (x < CHUNK_SIZE - 1 && (chunk_data[rightBelowIndex].getType() == AIR)) {
							swapTiles(y, x, y + 1, x + 1);
						} else if(x < CHUNK_SIZE - 1 && (chunk_data[rightIndex].getType() == AIR)){ //Right
							swapTiles(y, x, y, x + 1);
						} else if(x >0 && (chunk_data[leftIndex].getType() == AIR)){ //Left
							swapTiles(y, x, y, x - 1);
						}
					}
					break;
				case SMOKE:
					if(1){
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
						if (x+randdir>0 && x+randdir<CHUNK_SIZE-1 && (chunk_data[upIndex+randdir].getType() == AIR || chunk_data[upIndex+randdir].getType() == WATER)) {
							swapTiles(y, x, y - 1+chmod, x+randdir);
						}
					}
					break;
				
				case LAVA:
					if (y != CHUNK_SIZE - 1) {
						
						int belowIndex = (y + 1) * CHUNK_SIZE + x;
						int rightIndex = y * CHUNK_SIZE + x+1;
						int leftIndex = y * CHUNK_SIZE + x-1;
						int leftBelowIndex = belowIndex - 1;
						int rightBelowIndex = belowIndex + 1;

						if (chunk_data[belowIndex].getType() == AIR) {
							swapTiles(y, x, y + 1, x);
							break;
						} else if(chunk_data[belowIndex].getType() == WATER){
							setTile(y,x,SMOKE);
							setTile(y+1,x,ROCK);
						}
						else if (x > 0 && (chunk_data[leftBelowIndex].getType() == AIR)) {
							swapTiles(y, x, y + 1, x - 1);
							break;
						}
						else if (x < CHUNK_SIZE - 1 && (chunk_data[rightBelowIndex].getType() == AIR)) {
							swapTiles(y, x, y + 1, x + 1);
							break;
							
						}
						if(x < CHUNK_SIZE - 1){ //Right
							if(chunk_data[rightIndex].getType() == AIR){
								swapTiles(y, x, y, x + 1);
								break;
							} else if(chunk_data[rightIndex].getType() == WATER){
								setTile(y,x,SMOKE);
								setTile(y,x+1,ROCK);
							}
							
						}
						if(x >0){ //Left
							if(chunk_data[leftIndex].getType() == AIR){
								swapTiles(y, x, y, x - 1);
								break;
							} else if(chunk_data[leftIndex].getType() == WATER){
								setTile(y,x,SMOKE);
								setTile(y,x-1,ROCK);
							}
						}
						if(y>0 && chunk_data[(y - 1) * CHUNK_SIZE + x].getType()==WATER){
							setTile(y,x,SMOKE);
							setTile(y-1,x,ROCK);
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
			
			palette_type=ROCK;
		}

		void draw(Renderer& renderer) {
			shape.setPosition(static_cast<int>(posx), static_cast<int>(posy));
			renderer.window.draw(shape);
		}

		void update_input() {
			
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
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
			}
			
			/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
				FLOOR_COEF++;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
				FLOOR_COEF--;
			}*/
			
			
			if(w_down==1){
				posy-=speed;
			}
			
			if(s_down==1){
				posy+=speed;
			}
			
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
				palette_type=AIR;
			} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
				palette_type=SAND;
			} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) {
				palette_type=WATER;
			} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) {
				palette_type=ROCK;
			} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5)) {
				palette_type=SMOKE;
			} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6)) {
				palette_type=LAVA;
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
	std::srand(static_cast<unsigned>(std::time(0)));
	
    // Create the SFML window
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), GAME_NAME);

    // Initialize Renderer
    Renderer renderer(window);
    renderer.init();

    // Create a Player instance
    Player player(100, 100);
	
	Chunk chunk1(0,0); //Temp

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
		
		if( sf::Mouse::isButtonPressed( sf::Mouse::Button::Left ) ){
			//TODO get_chunk boundary
			if(sf::Mouse::getPosition(window).x<CHUNK_SIZE*VERT_SIZE&&sf::Mouse::getPosition(window).y<CHUNK_SIZE*VERT_SIZE){
				for(int i=static_cast<int>(sf::Mouse::getPosition(window).y/VERT_SIZE)-BRUSH_SIZE/2;i<static_cast<int>(sf::Mouse::getPosition(window).y/VERT_SIZE)+BRUSH_SIZE/2;i++){
					for(int j=static_cast<int>(sf::Mouse::getPosition(window).x/VERT_SIZE)-BRUSH_SIZE/2;j<static_cast<int>(sf::Mouse::getPosition(window).x/VERT_SIZE)+BRUSH_SIZE/2;j++){
						if(j>0&&i>0&&j<CHUNK_SIZE*VERT_SIZE&&i<CHUNK_SIZE*VERT_SIZE){
							chunk1.setTile(i,j,player.palette_type);
						}
					}
				}
				
			}
			//
		}
		
		if(game_timer%UPDATE_FRAMES==0){
			chunk1.updateChunk();
		}
		
		chunk1.drawChunk(renderer);

        // Draw the player
		player.draw(renderer);
		
        // Display the window contents
        renderer.window.display();
		
		game_timer++;
    }

    return 0;
}
