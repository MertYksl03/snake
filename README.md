# Snake Game in C with SDL2

This repository contains the code for a simple Snake game written in C, utilizing the SDL2 library for graphics and handling user input. The game features basic mechanics, including snake movement, food collection, and collision detection.

## 🕹️ Game Features
- Snake moves in four directions: up, down, left, and right.
- Snake grows longer every time it eats food.
- The game ends if the snake collides with the wall or itself.
- Different game modes:
   - Random mod
   - Walls enabled
   - No walls

## 💻 Technologies Used
- **C** programming language
- **SDL2** for rendering graphics and handling input
- **Makefile** for building the project 

## ⚙️ Installation

1. Install SDL2 on your system:

   - **On Linux (Ubuntu/Debian):**
     ```bash
     sudo apt-get install libsdl2-dev
     ```
   - **On macOS (using Homebrew):**
     ```bash
     brew install sdl2
     ```

2. Clone this repository:

```bash
git clone https://github.com/MertYksl03/snake.git
cd snake
```
3. Compile the game:
  ```bash
    make build
  ```
4. Run the game:
  ```bash
  ./snake
  ```
5. Exit the game:
   You can press q or escape keys to close the game
## 🎮 How to Play
- Use the arrow keys(up, down, left, right) or W, A, S, D keys to control the snake.
- Eat the food (represented by a square) to make the snake grow.
- Avoid hitting the walls or the snake's own body.
- You can toggle the walls with pressing the space key. (enabling or disabling walls)
- You can pause the game by pressing p key.
- To play random press x key.

## 📸 Screenshots
![image](https://github.com/user-attachments/assets/dfff79f7-f895-463b-989d-a4fab0f8c8b7)
![image1](https://github.com/user-attachments/assets/b39822f5-b643-49cc-be61-480131e49fe4)
![image2](https://github.com/user-attachments/assets/8c28f62d-a7bc-4b88-9323-ef769e6ab32d)
![image3](https://github.com/user-attachments/assets/907242e7-3be0-46bc-962c-93cd654b6645)




