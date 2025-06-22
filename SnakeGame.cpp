#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <sstream>

const int TILE_SIZE = 20;
const int WIDTH = 800;
const int HEIGHT = 600;
const int COLS = WIDTH / TILE_SIZE;
const int ROWS = HEIGHT / TILE_SIZE;

enum Direction { UP, DOWN, LEFT, RIGHT };

struct SnakeSegment {
    int x, y;
    SnakeSegment(int x, int y) : x(x), y(y) {}
};

class SnakeGame {
private:
    sf::RenderWindow window;
    std::vector<SnakeSegment> snake;
    Direction dir;
    sf::Vector2i apple;
    bool gameOver;
    int score;
    float speed;
    sf::Clock clock;
    sf::Font font;
    sf::Text scoreText;
    sf::SoundBuffer eatBuffer, gameOverBuffer;
    sf::Sound eatSound, gameOverSound;

public:
    SnakeGame()
        : window(sf::VideoMode(WIDTH, HEIGHT), "🐍 Snake Game"),
          dir(RIGHT), gameOver(false), score(0), speed(0.15f)
    {
        window.setFramerateLimit(60);
        snake.emplace_back(COLS / 2, ROWS / 2);
        srand(static_cast<unsigned>(time(0)));
        spawnApple();

        if (!font.loadFromFile("assets/arial.ttf"))
            throw std::runtime_error("Font not found!");

        scoreText.setFont(font);
        scoreText.setCharacterSize(24);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(10, 10);

        if (!eatBuffer.loadFromFile("assets/apple.wav") ||
            !gameOverBuffer.loadFromFile("assets/gameover.wav"))
            throw std::runtime_error("Sound files not found!");

        eatSound.setBuffer(eatBuffer);
        gameOverSound.setBuffer(gameOverBuffer);
    }

    void spawnApple() {
        apple.x = rand() % COLS;
        apple.y = rand() % ROWS;
    }

    void run() {
        float timer = 0.0f;

        while (window.isOpen()) {
            float delta = clock.restart().asSeconds();
            timer += delta;

            handleEvents();

            if (timer >= speed && !gameOver) {
                update();
                timer = 0.0f;
            }

            render();
        }
    }

    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up && dir != DOWN) dir = UP;
                else if (event.key.code == sf::Keyboard::Down && dir != UP) dir = DOWN;
                else if (event.key.code == sf::Keyboard::Left && dir != RIGHT) dir = LEFT;
                else if (event.key.code == sf::Keyboard::Right && dir != LEFT) dir = RIGHT;
                else if (event.key.code == sf::Keyboard::R && gameOver) reset();
            }
        }
    }

    void update() {
        for (int i = snake.size() - 1; i > 0; --i)
            snake[i] = snake[i - 1];

        switch (dir) {
            case UP:    snake[0].y--; break;
            case DOWN:  snake[0].y++; break;
            case LEFT:  snake[0].x--; break;
            case RIGHT: snake[0].x++; break;
        }

        // Collision with self or wall
        if (snake[0].x < 0 || snake[0].x >= COLS ||
            snake[0].y < 0 || snake[0].y >= ROWS)
        {
            gameOverSound.play();
            gameOver = true;
        }

        for (size_t i = 1; i < snake.size(); ++i)
            if (snake[0].x == snake[i].x && snake[0].y == snake[i].y)
                gameOver = true;

        // Eat apple
        if (snake[0].x == apple.x && snake[0].y == apple.y) {
            snake.push_back(SnakeSegment(-1, -1));  // Add new tail segment
            score += 10;
            eatSound.play();
            speed = std::max(speed - 0.01f, 0.05f); // Increase speed
            spawnApple();
        }
    }

    void render() {
        window.clear(sf::Color::Black);

        sf::RectangleShape tile(sf::Vector2f(TILE_SIZE - 1, TILE_SIZE - 1));

        // Draw apple
        tile.setFillColor(sf::Color::Red);
        tile.setPosition(apple.x * TILE_SIZE, apple.y * TILE_SIZE);
        window.draw(tile);

        // Draw snake
        tile.setFillColor(sf::Color::Green);
        for (auto& segment : snake) {
            tile.setPosition(segment.x * TILE_SIZE, segment.y * TILE_SIZE);
            window.draw(tile);
        }

        // Draw score
        std::stringstream ss;
        ss << "Score: " << score;
        scoreText.setString(ss.str());
        window.draw(scoreText);

        if (gameOver) {
            sf::Text overText("Game Over! Press R to restart.", font, 30);
            overText.setFillColor(sf::Color::White);
            overText.setPosition(WIDTH / 2 - 180, HEIGHT / 2 - 20);
            window.draw(overText);
        }

        window.display();
    }

    void reset() {
        snake.clear();
        snake.emplace_back(COLS / 2, ROWS / 2);
        dir = RIGHT;
        gameOver = false;
        score = 0;
        speed = 0.15f;
        spawnApple();
    }
};

int main() {
    try {
        SnakeGame game;
        game.run();
    } catch (const std::exception& e) {
        std::cerr << "❌ Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
