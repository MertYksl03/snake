#include "header.h"

#define WINDOW_X SDL_WINDOWPOS_CENTERED
#define WINDOW_Y SDL_WINDOWPOS_CENTERED

#if 1
    #define WINDOW_WIDTH 800
    #define WINDOW_HEIGHT 1000
#else
    #define WINDOW_WIDTH 1920
    #define WINDOW_HEIGHT 1080
#endif

#define FPS 60
#define FRAME_TARGET_TIME 1000 / FPS

#define GRID_DIM 1000
#define GRID_SIZE 20
#define CELL_SIZE (GRID_DIM / GRID_SIZE)
#define CELL_NUMBER_VERTC (WINDOW_WIDTH / CELL_SIZE)
#define CELL_NUMBER_HORZ (WINDOW_HEIGHT / CELL_SIZE)

#define INIT_DELAY_TIME 150
#define DELAY_TIME_MIN 150

SDL_Window *window;
SDL_Renderer *renderer;

u_int8_t is_game_running = FALSE;
uint8_t is_game_paused = FALSE;
uint8_t is_grid_enabled = FALSE;
uint8_t is_wall_enabled = TRUE; //adds walls to edges of the window
u_int8_t is_play_random = FALSE;


int delay_time = INIT_DELAY_TIME; //it changes the whole delay time and it effects the snakes movement speed
int score = 0;

int last_frame_time = 0;

int grid_x = (WINDOW_WIDTH / 2) - (GRID_DIM / 2);
int grid_y = (WINDOW_HEIGHT / 2) - (GRID_DIM / 2);
int number_of_cells_outside_v = GRID_SIZE - CELL_NUMBER_VERTC;
int number_of_cells_outside_h = GRID_SIZE - CELL_NUMBER_HORZ;

typedef struct {
    int posX;
    int posY;
} apple;
apple Apple;

struct snake
{
    int posX;
    int posY;
    int dir;

    struct snake *next;
};

typedef struct snake Snake;

Snake *head;
Snake *tail;

enum {
    SNAKE_UP,
    SNAKE_DOWN,
    SNAKE_RIGHT,
    SNAKE_LEFT
};

//----------------TEMPORARY FUNC PROTOTYPES----------------
void reset_snake();
void generate_apple();
void increase_snake();

int initiliaze_window() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
        return FALSE;
    }

    window = SDL_CreateWindow(
        "snake",
        WINDOW_X,
        WINDOW_Y,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_BORDERLESS
    );

    if(!window) {
        fprintf(stderr, "Error creating SDL window: %s\n", SDL_GetError());
        return FALSE;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer) {
        fprintf(stderr,"Error creating SDL Renderer");
        return FALSE;
    }

    return TRUE;
}

void process_input(SDL_Event event) {

    while(SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                is_game_running = FALSE;
                break;
            case SDL_KEYUP:
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.scancode) {
                    case SDL_SCANCODE_ESCAPE:
                        is_game_running = FALSE;
                        break;
                    //wasd or arrow keys to navigate snake
                    case SDL_SCANCODE_UP:
                        if (head->dir != SNAKE_DOWN)
                            head->dir = SNAKE_UP;
                        break;
                    case SDL_SCANCODE_DOWN:
                        if (head->dir != SNAKE_UP)
                            head->dir = SNAKE_DOWN;
                        break;
                    case SDL_SCANCODE_RIGHT:
                        if (head->dir != SNAKE_LEFT)
                            head->dir = SNAKE_RIGHT;
                        break;
                    case SDL_SCANCODE_LEFT:
                        if (head->dir != SNAKE_RIGHT)
                            head->dir = SNAKE_LEFT;
                        break;
                    case SDL_SCANCODE_W:
                        if (head->dir != SNAKE_DOWN)
                            head->dir = SNAKE_UP;
                        break;
                    case SDL_SCANCODE_S:
                        if (head->dir != SNAKE_UP)
                            head->dir = SNAKE_DOWN;
                        break;
                    case SDL_SCANCODE_D:
                        if (head->dir != SNAKE_LEFT)
                            head->dir = SNAKE_RIGHT;
                        break;
                    case SDL_SCANCODE_A:
                        if (head->dir != SNAKE_RIGHT)
                            head->dir = SNAKE_LEFT;
                        break;
                    case SDL_SCANCODE_R:
                        reset_snake();
                        generate_apple();
                        break;
                    case SDL_SCANCODE_P:
                        is_game_paused = !is_game_paused;
                        break;
                    case SDL_SCANCODE_SPACE:
                        is_wall_enabled = !is_wall_enabled;
                        printf("is walls enabled: %i\n", is_wall_enabled);
                        break;
                    case SDL_SCANCODE_G:
                        is_grid_enabled = !is_grid_enabled;
                        printf("The grid is : %i\n", is_grid_enabled);
                        break;
                    case SDL_SCANCODE_X:
                        is_play_random = !is_play_random;
                        break;
                    default:
                        break;
                }
                break;
        }
    }

    return;
}

void reset_snake() {

    //free the snake body
    Snake *temp = head;
    while(head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }

    //create a new snake
    Snake *new = malloc(sizeof(Snake));
    if (new == NULL) {
        is_game_running = FALSE;
        return;
    }
    new->posX = rand() % (GRID_SIZE / 2) + (GRID_SIZE / 4);
    new->posY = rand() % (GRID_SIZE / 2) + (GRID_SIZE / 4);
    new->dir = rand() % 4;
    new->next = NULL;

    head = new;
    tail = new;

    //reset the delay time
    delay_time = INIT_DELAY_TIME;

    increase_snake();

    return;
}

void move_snake() {

    Snake *track = head;
    int prevX = head->posX;
    int prevY = head->posY;
    int prevDir = head->dir;

    //move the head
    switch (head->dir) {
        case SNAKE_UP:
            head->posY--;
            break;
        case SNAKE_DOWN:
            head->posY++;
            break;
        case SNAKE_RIGHT:
            head->posX++;
            break;
        case SNAKE_LEFT:
            head->posX--;
            break;
    }

    if (head->next != NULL) {
        track = head->next;
        while (track != NULL) {
            int saveX = track->posX;
            int saveY = track->posY;
            int saveDir = track->dir;

            track->posX = prevX;
            track->posY = prevY;
            track->dir = prevDir;

            track = track->next;

            prevX = saveX;
            prevY = saveY;
            prevDir = saveDir;
        }
    }

    return;
}

void detect_crash() {
    int head_screen_posX = head->posX - (number_of_cells_outside_v / 2);
    int head_scrren_posY = head->posY - (number_of_cells_outside_h / 2);
    if (head_screen_posX >= CELL_NUMBER_VERTC || head_screen_posX < 0 || head_scrren_posY >= CELL_NUMBER_HORZ || head_scrren_posY < 0) {

        if(is_wall_enabled) {
            // printf("Crash detected\n");
            reset_snake();
        } else {
            switch (head->dir) {
                case SNAKE_UP:
                    head->posY += CELL_NUMBER_HORZ;
                    break;
                case SNAKE_DOWN:
                    head->posY -= CELL_NUMBER_HORZ;
                    break;
                case SNAKE_RIGHT:
                    head->posX -= CELL_NUMBER_VERTC;
                    break;
                case SNAKE_LEFT:
                    head->posX += CELL_NUMBER_VERTC;
                    break;
            }
        }
    }

    //TODO: add collusion detect for body
    int head_posX = head->posX;
    int head_posY = head->posY;

    Snake *track = NULL;
    if (head->next != NULL) {
        track = head->next;
        while (track->next != NULL) {
            if (head_posX == track->posX && head_posY == track->posY) {
                // printf("Crash detected\n");
                reset_snake();
            }
            track = track->next;
        }
    }
    return;
}

void detect_apple() {
    if (head->posX == Apple.posX && head->posY == Apple.posY) {
        increase_snake();
        generate_apple();

        //change the score
        score += 10;

        //reduce the delay time
        if (delay_time > DELAY_TIME_MIN) {
            delay_time -= 10;
        } else {
            delay_time = DELAY_TIME_MIN; //idk if this is a unnecessary operation
        }

    }
    return;
}

void play_random() {
    int random_time = rand() % 3;
    int random_number = rand() % 4;

    if (random_time == 1) {
        generate_apple();
        increase_snake();
        switch (random_number) {
            case 1:
                if (head->dir != SNAKE_DOWN)
                    head->dir = SNAKE_UP;
                break;
            case 2:
                if (head->dir != SNAKE_UP)
                    head->dir= SNAKE_DOWN;
                break;
            case 3:
                if (head->dir != SNAKE_LEFT)
                    head->dir= SNAKE_RIGHT;
                break;
            case 4:
                if (head->dir != SNAKE_RIGHT)
                    head->dir= SNAKE_LEFT;
                break;
            default:
                if (head->dir != SNAKE_DOWN)
                    head->dir= SNAKE_UP;
                break;

        }
    }
}

void update() {

    if (is_play_random) {
        play_random();
    }
    move_snake();
    detect_crash();
    detect_apple();

    //sleep until reach the frame target
    // while (!SDL_TICKS_PASSED(SDL_GetTicks(), last_frame_time + FRAME_TARGET_TIME));

    // last_frame_time = SDL_GetTicks();

    SDL_Delay(delay_time);
    return;
}

void render_grid() {

    SDL_SetRenderDrawColor(renderer, 120, 120, 120 ,255);

    if (is_grid_enabled == TRUE) {

        SDL_Rect cell;
        cell.h = CELL_SIZE;
        cell.w = CELL_SIZE;

        for (int i = 0; i < GRID_SIZE; i++) {
            for (int j = 0; j <GRID_SIZE; j++) {
                cell.x = grid_x + (i * CELL_SIZE);
                cell.y = grid_y + (j * CELL_SIZE);
                SDL_RenderDrawRect(renderer, &cell);
            }
        }
    } else {
        SDL_Rect outline;

        outline.x = grid_x + 2 * CELL_SIZE; //change this
        outline.y = grid_y;
        outline.w = WINDOW_WIDTH;
        outline.h = WINDOW_HEIGHT;

        SDL_RenderDrawRect(renderer, &outline);
    }

    return;
}

void render_apple() {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    SDL_Rect appleRect;
    appleRect.h = CELL_SIZE;
    appleRect.w = CELL_SIZE;

    appleRect.x = grid_x + (Apple.posX * CELL_SIZE);
    appleRect.y = grid_y + (Apple.posY * CELL_SIZE);

    SDL_RenderFillRect(renderer, &appleRect);
    return;
}

void render_snake() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // change this later - head color

    SDL_Rect snake_seg;
    snake_seg.h = CELL_SIZE;
    snake_seg.w = CELL_SIZE;

    //render the head
    snake_seg.x = grid_x + (head->posX * CELL_SIZE);
    snake_seg.y = grid_y + (head->posY * CELL_SIZE);

    SDL_RenderFillRect(renderer, &snake_seg);


    //render the body
    if (head->next != NULL) {
        Snake *track = head->next;

        SDL_SetRenderDrawColor(renderer, 52, 235, 58, 255); // body color

        while (track != NULL) {

            snake_seg.x = grid_x + (track->posX * CELL_SIZE);
            snake_seg.y = grid_y + (track->posY * CELL_SIZE);

            SDL_RenderFillRect(renderer, &snake_seg);

            track = track->next;
        }
    }

    return;
}

void increase_snake() {
    Snake *newSegment = malloc(sizeof(Snake));
    if (newSegment == NULL) {
        printf("ERROR: malloc\n");
        is_game_running = FALSE;
        return;
    }
    switch (tail->dir) {
        case SNAKE_UP:
            newSegment->posX = tail->posX;
            newSegment->posY = tail->posY + 1;
            break;
        case SNAKE_DOWN:
            newSegment->posX = tail->posX;
            newSegment->posY = tail->posY - 1;
            break;
        case SNAKE_RIGHT:
            newSegment->posX = tail->posX - 1;
            newSegment->posY = tail->posY;
            break;
        case SNAKE_LEFT:
            newSegment->posX = tail->posX + 1;
            newSegment->posY = tail->posY;
            break;
    }

    newSegment->dir = tail->dir;
    newSegment->next = NULL;


    if (head->next == NULL) {
        head->next = newSegment;
    } else {
        tail->next = newSegment;
    }
    tail = newSegment;

    return;

}

void generate_apple() {
    int x = rand() % CELL_NUMBER_VERTC + (number_of_cells_outside_v / 2);
    int y = rand() % CELL_NUMBER_HORZ;

    //be sure the apple spawns in the grid
    if (x > CELL_NUMBER_VERTC || y > CELL_NUMBER_HORZ) {
        generate_apple();
    }

    //be sure the apple doesn't spawn on snake
    Snake *track = head;

    while(track != NULL) {
        if(track->posX == x && track->posY == y) {
            generate_apple();
            break;
        } else {
            Apple.posX = x;
            Apple.posY = y;
        }
        track = track->next;
    }

    return;
}


void setup() {
    //initialize snake
    reset_snake();

    generate_apple();

    return;
}

//destroy the renderer, windowf and quit
void quit() {
    //free the snake
    Snake *temp = head;
    while(head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }


    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(void) {
    srand(time(NULL));
    is_game_running = initiliaze_window();


    //event for input
    SDL_Event event;

    setup();

    while(is_game_running) {
        //game loop
        process_input(event);

        if(!is_game_paused) {


        // printf("x = %i\n", head->posX);
        // printf("y = %i\n", head->posY);
        // printf("apple.x = %i\n", Apple.posX);
        // printf("apple.y = %i\n", Apple.posY);

        update();

        //clear the screen
        SDL_SetRenderDrawColor(renderer , 0x11, 0x11, 0x11, 255); //background color
        SDL_RenderClear(renderer);
        //RENDER LOOP START

        render_grid();
        render_apple();
        render_snake();

        //RENDER LOOP END

        SDL_RenderPresent(renderer);

        }
    }

    // SDL_QuitEvent(event);
    quit();
    return 0;
}
