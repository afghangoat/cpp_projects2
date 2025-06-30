#include <iostream>
#include <vector>
#include <memory>
#include <cstdlib>

// Forward declaration
class Grid;

// Base Particle class
class Particle {
public:
    virtual ~Particle() = default;

    virtual char getSymbol() const = 0; // Symbol for display
    virtual void update(Grid& grid, int x, int y) = 0; // Update behavior
};

// Derived Sand class
class Sand : public Particle {
public:
    char getSymbol() const override {
        return 'S';
    }

    void update(Grid& grid, int x, int y) override;
};

// Derived Water class
class Water : public Particle {
public:
    char getSymbol() const override {
        return 'W';
    }

    void update(Grid& grid, int x, int y) override;
};

// Grid class
class Grid {
private:
    int width, height;
    std::vector<std::vector<std::shared_ptr<Particle>>> cells;

public:
    Grid(int w, int h) : width(w), height(h), cells(h, std::vector<std::shared_ptr<Particle>>(w, nullptr)) {}

    std::shared_ptr<Particle> get(int x, int y) {
        if (x < 0 || x >= width || y < 0 || y >= height) return nullptr;
        return cells[y][x];
    }

    void set(int x, int y, std::shared_ptr<Particle> particle) {
        if (x >= 0 && x < width && y >= 0 && y < height)
            cells[y][x] = particle;
    }

    void update() {
        // Iterate from bottom to top to simulate gravity
        for (int y = height - 1; y >= 0; --y) {
            for (int x = 0; x < width; ++x) {
                if (cells[y][x])
                    cells[y][x]->update(*this, x, y);
            }
        }
    }

    void display() {
        for (const auto& row : cells) {
            for (const auto& cell : row) {
                if (cell)
                    std::cout << cell->getSymbol();
                else
                    std::cout << '.';
            }
            std::cout << '\n';
        }
    }
};

// Sand update behavior
void Sand::update(Grid& grid, int x, int y) {
    if (grid.get(x, y + 1) == nullptr) { // Move down if possible
        grid.set(x, y + 1, grid.get(x, y));
        grid.set(x, y, nullptr);
    } else if (grid.get(x - 1, y + 1) == nullptr) { // Move down-left
        grid.set(x - 1, y + 1, grid.get(x, y));
        grid.set(x, y, nullptr);
    } else if (grid.get(x + 1, y + 1) == nullptr) { // Move down-right
        grid.set(x + 1, y + 1, grid.get(x, y));
        grid.set(x, y, nullptr);
    }
}

// Water update behavior
void Water::update(Grid& grid, int x, int y) {
    if (grid.get(x, y + 1) == nullptr) { // Move down if possible
        grid.set(x, y + 1, grid.get(x, y));
        grid.set(x, y, nullptr);
    } else if (grid.get(x - 1, y) == nullptr) { // Move left
        grid.set(x - 1, y, grid.get(x, y));
        grid.set(x, y, nullptr);
    } else if (grid.get(x + 1, y) == nullptr) { // Move right
        grid.set(x + 1, y, grid.get(x, y));
        grid.set(x, y, nullptr);
    }
}

// Simulation class
class Simulation {
private:
    Grid grid;

public:
    Simulation(int width, int height) : grid(width, height) {}

    void addParticle(int x, int y, std::shared_ptr<Particle> particle) {
        grid.set(x, y, particle);
    }

    void run() {
        char command;
        while (true) {
            system("clear"); // Use "cls" for Windows
            grid.display();

            std::cout << "Enter command (q to quit): ";
            std::cin >> command;

            if (command == 'q') break;

            // Example: Simulate one frame
            grid.update();
        }
    }
};

// Main function
int main() {
    Simulation simulation(20, 10);

    // Add some sand and water particles
    simulation.addParticle(10, 0, std::make_shared<Sand>());
    simulation.addParticle(15, 0, std::make_shared<Water>());

    simulation.run();

    return 0;
}

/*std::shared_ptr<Tile> createMaterialFromId(int aid){
		switch(aid){
			case AIR:
				return std::make_shared<Air>();
			case SAND:
				return std::make_shared<Sand>();
			case WATER:
				return std::make_shared<Water>();
			case ROCK:
				return std::make_shared<Rock>();
			case STEAM:
				return std::make_shared<Steam>();
			case LAVA:
				return std::make_shared<Lava>();
			case ACID:
				return std::make_shared<Acid>();
			case GRAVEL:
				return std::make_shared<Gravel>();
			case COPPER:
				return std::make_shared<Copper>();
			case OIL:
				return std::make_shared<Oil>();
			case FIRE:
				return std::make_shared<Fire>();
			case SMOKE:
				return std::make_shared<Smoke>();
			case WOOD:
				return std::make_shared<Wood>();
			case GLUE:
				return std::make_shared<Glue>();
			case DIRT:
				return std::make_shared<Dirt>();
			case SEED:
				return std::make_shared<Seed>();
			case PLANT:
				return std::make_shared<Plant>();
			case PETAL:
				return std::make_shared<Petal>();
			case ALGAE:
				return std::make_shared<Algae>();
			case BAMBOO:
				return std::make_shared<Bamboo>();
			case SAPLING:
				return std::make_shared<Sapling>();
			case LEAVES:
				return std::make_shared<Leaves>();
			case COAL:
				return std::make_shared<Coal>();
			case ASH:
				return std::make_shared<Ash>();
			case THERMITE:
				return std::make_shared<Thremite>();
			default:
				return std::make_shared<Air>();
		}
	}*/