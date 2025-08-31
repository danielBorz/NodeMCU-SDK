#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 20
#define HEIGHT 10
#define MAX_SNAKE_LENGTH (WIDTH*HEIGHT)

typedef struct {
    int x;
    int y;
} Point;

static int is_snake(const Point snake[], int length, int x, int y) {
    for (int i = 0; i < length; ++i) {
        if (snake[i].x == x && snake[i].y == y) {
            return 1;
        }
    }
    return 0;
}

static void place_food(Point *food, const Point snake[], int length) {
    do {
        food->x = rand() % WIDTH;
        food->y = rand() % HEIGHT;
    } while (is_snake(snake, length, food->x, food->y));
}

static void draw_board(const Point snake[], int length, Point food, int score) {
    printf("\033[H\033[J");
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            if (is_snake(snake, length, x, y)) {
                putchar('O');
            } else if (food.x == x && food.y == y) {
                putchar('*');
            } else {
                putchar('.');
            }
        }
        putchar('\n');
    }
    printf("Score: %d\n", score);
}

int main(void) {
    srand((unsigned) time(NULL));
    Point snake[MAX_SNAKE_LENGTH];
    int length = 3;
    int dx = 1;
    int dy = 0;

    for (int i = 0; i < length; ++i) {
        snake[i].x = WIDTH / 2 - i;
        snake[i].y = HEIGHT / 2;
    }

    Point food;
    place_food(&food, snake, length);

    int score = 0;
    char dir;
    while (1) {
        draw_board(snake, length, food, score);
        printf("Move (WASD): ");
        if (scanf(" %c", &dir) != 1) {
            break;
        }
        if (dir == 'w' || dir == 'W') { dx = 0; dy = -1; }
        else if (dir == 's' || dir == 'S') { dx = 0; dy = 1; }
        else if (dir == 'a' || dir == 'A') { dx = -1; dy = 0; }
        else if (dir == 'd' || dir == 'D') { dx = 1; dy = 0; }

        Point new_head = { snake[0].x + dx, snake[0].y + dy };
        if (new_head.x < 0 || new_head.x >= WIDTH || new_head.y < 0 || new_head.y >= HEIGHT) {
            break;  // hit wall
        }
        if (is_snake(snake, length, new_head.x, new_head.y)) {
            break;  // hit self
        }

        if (new_head.x == food.x && new_head.y == food.y) {
            length++;
            score++;
            place_food(&food, snake, length);
        }

        for (int i = length - 1; i > 0; --i) {
            snake[i] = snake[i - 1];
        }
        snake[0] = new_head;

        if (length >= MAX_SNAKE_LENGTH) {
            break;  // filled board
        }
    }

    printf("Game over! Final score: %d\n", score);
    return 0;
}

