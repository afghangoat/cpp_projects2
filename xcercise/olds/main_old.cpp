#include <SDL2/SDL.h>
#include <iostream>
#include <vector>

#define GAME_NAME "Noita Clone"
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 600
#define GAME_WIDTH 100
#define GAME_HEIGHT 60

#define FPS 50
#define FRAME_TIME (1000/FPS)
#define RAD 0.0174532925

class Context{
	public:
		SDL_Window* window;
		SDL_Renderer* renderer;
};
class GameState{
	public:
		int state; //0 is quit, 1 is sandsim
		int delta_time;
		int last_frame_time;
		bool mousedown;
};

class PixelData{
	public:
		int pixel_id;
		int pixel_color[3]={0,0,0};
};
class Map {
	public:
		PixelData** map_pixels;
		std::vector<std::pair<int, int>> active_pixels;
 
		Map() {
			// Allocate memory for map_pixels
			map_pixels = new PixelData*[WINDOW_HEIGHT];
			for (int i = 0; i < WINDOW_HEIGHT; i++) {
				map_pixels[i] = new PixelData[WINDOW_WIDTH];
			}
		}

		~Map() {
			// Free allocated memory
			for (int i = 0; i < WINDOW_HEIGHT; i++) {
				delete[] map_pixels[i];
			}
			delete[] map_pixels;
		}
		
		void set_pixel(int x, int y, int id, int r, int g, int b) {
			if (x < 0 || x >= WINDOW_WIDTH || y < 0 || y >= WINDOW_HEIGHT) return;

			PixelData& pixel = map_pixels[y][x];
			if (pixel.pixel_id == 0 && id != 0) {
				// Pixel becomes active
				active_pixels.emplace_back(y, x);
			} else if (pixel.pixel_id != 0 && id == 0) {
				// Pixel becomes inactive
				auto it = std::find(active_pixels.begin(), active_pixels.end(), std::make_pair(y, x));
				if (it != active_pixels.end()) {
					active_pixels.erase(it);
				}
			}

			// Update pixel data
			pixel.pixel_id = id;
			pixel.pixel_color[0] = r;
			pixel.pixel_color[1] = g;
			pixel.pixel_color[2] = b;
		}
};
void draw_pixels(Map* map, SDL_Renderer* renderer) {
    for (const auto& [y, x] : map->active_pixels) {
        PixelData& pixel = map->map_pixels[y][x];

        SDL_SetRenderDrawColor(renderer, pixel.pixel_color[0], pixel.pixel_color[1], pixel.pixel_color[2], 255);
        SDL_RenderDrawPoint(renderer, x, y);
    }
}
void init_game_map(Map* map) {
    for (int y = 0; y < WINDOW_HEIGHT; y++) {
        for (int x = 0; x < WINDOW_WIDTH; x++) {
            PixelData cpdata;
            if (y < 2) {
                cpdata.pixel_id = 1;
                cpdata.pixel_color[0] = 200;
                cpdata.pixel_color[1] = 100;
                cpdata.pixel_color[2] = 50;
            } else {
                cpdata.pixel_id = 0;
            }
            map->map_pixels[y][x] = cpdata;
        }
    }
}
int init_renderer(Context* context){
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    context->window = SDL_CreateWindow(GAME_NAME,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,WINDOW_WIDTH,WINDOW_HEIGHT,SDL_WINDOW_SHOWN);
    if (context->window==NULL) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }
    context->renderer = SDL_CreateRenderer(context->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (context->renderer==NULL) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(context->window);
        SDL_Quit();
        return 1;
    }
	return 0;
}
void setup_gamestate(GameState* gamestate){
	gamestate->state=1;
	gamestate->delta_time=0;
	gamestate->last_frame_time=0;
	gamestate->mousedown=false;
}
void update_frame(GameState* gamestate) {
    int delay = FRAME_TIME - (SDL_GetTicks() - gamestate->last_frame_time);
    if (delay > 0 && delay <= FRAME_TIME) {
        SDL_Delay(delay);
    }
    gamestate->delta_time = (SDL_GetTicks() - gamestate->last_frame_time) / 1000.0;
    gamestate->last_frame_time = SDL_GetTicks();
}
void process_input(GameState* game_state) {
    SDL_Event event;
    SDL_PollEvent(&event);
   // player->moving = false;
    //player->shooting=false;
    if (event.type == SDL_QUIT) {
        game_state->state = 0;
        return;
    }
    //if(event.type==SDL_MOUSEMOTION){
    //    SDL_GetMouseState(&game_state->mouseposx,&game_state->mouseposy);
    //    return;
    //}
    if(event.type==SDL_MOUSEBUTTONDOWN){
        game_state->mousedown=true;
        return;
    }
    if(event.type==SDL_MOUSEBUTTONUP){
        game_state->mousedown=false;
    }
    const Uint8* keypresses = SDL_GetKeyboardState(NULL);
    if(keypresses[SDL_SCANCODE_ESCAPE]){
        game_state->state = 0;
    }
}
int main(int argc, char* argv[]) {

    Context context;
	init_renderer(&context);
	
	GameState game_state;
	setup_gamestate(&game_state);
	
	Map* game_map = new Map();
	init_game_map(game_map);
	
	while (game_state.state != 0) {
		process_input(&game_state);
		update_frame(&game_state);
		SDL_SetRenderDrawColor(context.renderer, 0, 0, 0, 255);//Draw bg
        SDL_RenderClear(context.renderer);
		switch(game_state.state){
			case 1:
				SDL_SetRenderDrawColor(context.renderer, 255, 0, 0, 255);
				SDL_Rect rect = { 300, 200, 200, 150 }; // x, y, width, height
				SDL_RenderFillRect(context.renderer, &rect);
				break;
		}
		SDL_RenderPresent(context.renderer);
	}


    SDL_DestroyRenderer(context.renderer);
    SDL_DestroyWindow(context.window);
    SDL_Quit();

    return 0;
}