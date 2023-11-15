#include <gamma.h>
#include <RGBmatrixPanel.h>
#include <Adafruit_GFX.h>



// define the wiring of the LED screen
const uint8_t CLK  = 8;
const uint8_t LAT = A3;
const uint8_t OE = 9;
const uint8_t A = A0;
const uint8_t B = A1;
const uint8_t C = A2;

// Matrix Dimensions
const uint8_t MAT_WIDTH = 32;
const uint8_t MAT_HEIGHT = 16;

// Size of instances
const uint8_t INVADER_HEIGHT = 4;
const uint8_t INVADER_WIDTH = 4;

// Invader strength array
const uint8_t MAX_LEVEL = 4;
const uint8_t MAX_INVADERS = 16;
const int strengths[MAX_LEVEL][MAX_INVADERS] = {{1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0},
                                                {1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2},
                                                {1,2,3,4,5,1,2,3,4,5,1,2,3,4,5,1},
                                                {5,4,5,4,5,4,5,4,2,3,2,3,2,3,2,3}
                                               };


// define the wiring of the inputs
const int POTENTIOMETER_PIN_NUMBER = 5;
const int BUTTON_PIN_NUMBER = 10;

// global constant for the number of Invaders in the game
const int NUM_ENEMIES = 16;

// a global variable that represents the LED screen
RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);

// the following functions are for printing messages
void print_level(int level);
void print_lives(int lives);
void game_over();

class Color {
  public:
    int red;
    int green;
    int blue;
    Color() {
      red = 0;
      green = 0;
      blue = 0;
    }
    Color(int r, int g, int b) {
      red = r;
      green = g;
      blue = b;
    }
    uint16_t to_333() const {
      return matrix.Color333(red, green, blue);
    }
};

const Color BLACK(0, 0, 0);
const Color RED(4, 0, 0);
const Color ORANGE(6, 1, 0);
const Color YELLOW(4, 4, 0);
const Color GREEN(0, 4, 0);
const Color BLUE(0, 0, 4);
const Color PURPLE(1, 0, 2);
const Color WHITE(4, 4, 4);
const Color LIME(2, 4, 0);
const Color AQUA(0, 4, 4);

class Invader {
  public:
    // Constructors
    Invader() {
      x = 0;
      y = 0;
      strength = 0;
    }
    // sets values for private date members x and y
    Invader(int x_arg, int y_arg) {
      x = x_arg;
      y = y_arg;
    }
    // sets values for private data members
    Invader(int x_arg, int y_arg, int strength_arg) {
      x = x_arg;
      y = y_arg;
      strength = strength_arg;
    }
    // sets values for private data members
    void initialize(int x_arg, int y_arg, int strength_arg) {
      x = x_arg;
      y = y_arg;
      strength = strength_arg;
    }
    // Sets values for strength
    void set_strength(int strength_arg) {
      strength = strength_arg;
    }
    
    // getters
    int get_x() const {
      return x;
    }
    int get_y() const {
      return y;
    }
    int get_strength() const {
      return strength;
    }

    // Moves the Invader down the screen by one row
    // Modifies: y
    // Added erase() and draw() inside move() to improve efficiency
    void move() {
      // Each time it's called in the loop this moves to invader down one pixel
      // The max value for y is MAT_HEIGHT - INVADER_HEIGHT = 12. 
      if (y < (MAT_HEIGHT - INVADER_HEIGHT) && (strength > 0)) {
        erase();
        y++;
        draw();
      }
    }
    // draws the Invader if its strength is greater than 0
    // calls: draw_with_rgb
    // Modified to switch-case to improve efficiency
    void draw() {
      switch (strength) {
        case 1:
          draw_with_rgb(RED, BLUE);
          break;
        case 2:
          draw_with_rgb(ORANGE, BLUE);
          break;
        case 3:
          draw_with_rgb(YELLOW, BLUE);
          break;
        case 4:
          draw_with_rgb(GREEN, BLUE);
          break;
        case 5:
          draw_with_rgb(BLUE, BLUE);
          break;
        case 6:
          draw_with_rgb(PURPLE, BLUE);
          break;
        case 7:
          draw_with_rgb(WHITE, BLUE);
          break;
        default:
          break;
      }
    }
    
    // draws black where the Invader used to be
    // calls: draw_with_rgb
    void erase() {
      draw_with_rgb(BLACK, BLACK);
    }    
    
    // Invader is hit by a Cannonball.
    // Modifies: strength
    // calls: draw, erase
    void hit() {
      strength--;
      if (strength <= 0) {
        erase();
      }
      else {
        draw();
      }
    }

  private:
    int x;
    int y;
    int strength;
    
    // draws the Invader
    void draw_with_rgb(Color body_color, Color eye_color) {
      //Each of these specifies a specific point on the grid. 
      //The origin (x,y) is the upper left corner of the invader.
      matrix.drawPixel(x + 1, y, body_color.to_333());
      matrix.drawPixel(x + 2, y, body_color.to_333());
      matrix.drawPixel(x, y + 1, body_color.to_333());
      matrix.drawPixel(x + 1, y + 1, eye_color.to_333());
      matrix.drawPixel(x + 2, y + 1, eye_color.to_333());
      matrix.drawPixel(x + 3, y + 1, body_color.to_333());
      matrix.drawPixel(x, y + 1, body_color.to_333());
      matrix.drawPixel(x, y + 2, body_color.to_333());
      matrix.drawPixel(x + 1, y + 2, body_color.to_333());
      matrix.drawPixel(x + 2, y + 2, body_color.to_333());
      matrix.drawPixel(x + 3, y + 2, body_color.to_333());
      matrix.drawPixel(x, y + 3, body_color.to_333());
      matrix.drawPixel(x + 3, y + 3, body_color.to_333());
    }
};

class Cannonball {
  public:
    // Default constructor of Cannonball
    Cannonball() {
      // Default coordinates x and y are initialized
      x = 0;
      y = 0;
      // Default status is not fired
      fired = false;
    }
    
    // Resets private data members to initial values
    void reset() {
      x = 0;
      y = 0;
      fired = false;
    }
    
    // Getters
    int get_x() const {
      return x;
    }

    int get_y() const {
      return y;
    }

    bool has_been_fired() const {
      return fired;
    }
    
    // Setters
    // Default argument for y is MAT_HEIGHT - 2 = 14
    // i.e. the y-coordinate of the cannon
    void fire(int x_arg, int y_arg = MAT_HEIGHT - 2) {
      x = x_arg;
      y = y_arg;
      fired = true;
    }
    
    // Moves the Cannonball and detects if it goes off the screen
    // Modifies: y, fired
    // draw() is called in this function i.e. Cannonball is moved and drawn
    void move() {
     if (fired) {
      // Erase from its original position
      erase();
      // Move to its next position
      y--;
      // If goes off the screen, reset
      if (y < 0) {
        reset();
      }
      // If still in the screen, draw
      else {
        draw();
      }
     }
    }
    
    // Resets private data members to initial values
    void hit() {
      erase();
      reset();
    }
    
    // Draws the Cannonball, if it is fired
    void draw() {
      if (fired) {
      matrix.drawPixel(x, y, ORANGE.to_333());
      matrix.drawPixel(x, y - 1, ORANGE.to_333());
      }
      // Print alert message
      else {
        Serial.println("Cannot draw! No cannonball is fired.");
      }
    }
    
    // Draws black where the Cannonball used to be
    void erase() {
      matrix.drawPixel(x, y, BLACK.to_333());
      matrix.drawPixel(x, y - 1, BLACK.to_333());
    }

  private:
    int x;
    int y;
    bool fired;
};

class Player {
  public:
    Player() {
      // What are the default coordinates of player?
      x = (MAT_WIDTH / 2) - 1;
      y = MAT_HEIGHT - 1;
      lives = 3;
    }
    
    // getters
    int get_x() const {
      return x;
    }
    int get_y() const {
      return y;
    }
    int get_lives() const {
      return lives;
    }
    
    // setter
    void set_x(int x_arg) {
      x = x_arg;
    }
    
    // Modifies: lives
    void die() {
      if (lives > 0) {
        lives--;
      }
    }
    
    // draws the Player
    // calls: draw_with_rgb
    void draw() {
      // Player is always AQUA
      draw_with_rgb(AQUA);
    }
    
    // draws black where the Player used to be
    // calls: draw_with_rgb
    void erase() {
      draw_with_rgb(BLACK);
    }

  private:
    int x;
    int y;
    int lives;

    // sets private data members x and y to initial values
    void initialize(int x_arg, int y_arg) {
      x = x_arg;
      y = y_arg;
    }
    
    // draws the player
    void draw_with_rgb(Color color) {
      // Ensure x, y are within the bounds of the matrix
      if ((x > 0 && x < MAT_WIDTH) && (y == MAT_HEIGHT - 1)) {
        //draw the bottom 3
        matrix.drawPixel(x - 1, y, color.to_333());
        // (x, y) is the middle pixel of the bottom 3
        matrix.drawPixel(x, y, color.to_333());
        matrix.drawPixel(x + 1, y, color.to_333());
        //draw the top 1
        matrix.drawPixel(x, y - 1, color.to_333());
      }
    }
};

class Game {
  public:
    Game() {
      // Level should start from 1.
      level = 1;
      time = 0;
    }
    
    // sets up a new game of Space Invaders
    // Modifies: global variable matrix
    void setupGame() {
      // Initialize the game level
      level = 1;

      // Initialize the position of Player
      player.set_x((MAT_WIDTH / 2) - 1);

      // Initialize the position and strength of Invaders
      reset_level();
    }

    
    // advances the game simulation one step and renders the graphics
    // see spec for details of game
    // Modifies: global variable matrix
    void update(int potentiometer_value, bool button_pressed) {
      // FIXME: Game over
      if (player.get_lives() <= 0) {
        game_over();
        delay(2000);
        return;
      }

      // Update position of Player based on the value of the potentiometer
      player.erase();
      player.set_x(((MAT_WIDTH) * potentiometer_value) / 1024);
      player.draw();

      // Update position of Cannonball,
      // Detect if a new cannonball is being fired
      if (button_pressed && !ball.has_been_fired()) {
        ball.fire(player.get_x(), 14);
      }
      ball.move();

      // Touch detection
      for (int i = 0; i < NUM_ENEMIES; i++){
        /* x + 2, y + 2
           x + 3, y + 2
           x, y + 3
           x + 3, y + 3, body_color.to_333());
        */
      int x = enemies[i].get_x();
      int y = enemies[i].get_y();
 
        if (enemies[i].get_strength() > 0) {
          // Invader touches the bottom
          if (y + INVADER_HEIGHT == MAT_HEIGHT) {
            touch_bottom = true;
            // Invader should be erased and cleared after touching bottom
            enemies[i].erase();
            enemies[i].set_strength(0);
          }
          
          // Invader touches Player
          if (invader_touch_player(enemies[i], player)) {
            touch_player = true;
            // Invader should be erased and cleared after touching Player
            enemies[i].erase();
            enemies[i].set_strength(0);
          };

          // Invader hit by Cannonball
          if ((ball.get_x() == x + 1 && ball.get_y() == y + 2) ||
             (ball.get_x() == x + 2 && ball.get_y() == y + 2)  ||
             (ball.get_x() == x && ball.get_y() == y + 3) ||
             (ball.get_x() == x + 3 && ball.get_y() == y + 3)) {
              enemies[i].hit();
              ball.hit();
          }
        }
      }

      // Player loses one life if any Invader touches bottom or touches Player in one loop
      if (touch_bottom || touch_player) {
        player.die();
      }

      // Update Invaders
      for (int i = 0; i < NUM_ENEMIES; i++) {
        if (enemies[i].get_strength() <= 0) {
          enemies[i].erase();
        }
        else {
          enemies[i].draw();
          // *level == 1*: There is only 1 row of Invaders in level 1
          // *i >= (NUM_ENEMIES / 2)*: The second row should move at first
          // *second_row_cleared()*: The first row moves after second row is cleared.
          if ((level == 1) || (i >= (NUM_ENEMIES / 2)) || (second_row_cleared())) {
            // Move the Invaders at every 1/10 of the game time
            // There should be an initial delay before the first Invader moves
            if ((time % 10) == 0){
              enemies[i].move();
            } 
          }
        }
      }

      // Next level
      if (level_cleared()) {
        level++;
        reset_level();
      }

      
      time++;
    }

  private:
    int level;
    unsigned long time;
    Player player;
    Cannonball ball;
    Invader enemies[NUM_ENEMIES];
    // Check if any Invader touches bottom *in one loop*
    bool touch_bottom = false;
    // Check if any Invader touches Player *in one loop*
    bool touch_player = false;

    // Check if Invader touch the Player
    bool invader_touch_player(Invader invader, Player player) {
      // Coordinates of bottom pixels of Invader
      int invader_x[4] = {invader.get_x(), invader.get_x() + 1, invader.get_x() + 2, invader.get_x() + 3};
      int invader_y[4] = {invader.get_y() + 3, invader.get_y() + 2, invader.get_y() + 2, invader.get_y() + 3};
      // Coordinates of top pixels of Player
      int player_x[3] = {player.get_x() - 1, player.get_x(), player.get_x() + 1};
      int player_y[3] = {player.get_y(), player.get_y() - 1, player.get_y()};

      // Iterate through all the coordinates of possible touches
      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
          if ((invader_x[i] == player_x[j]) && (invader_y[i] == player_y[j])) {
            return true;
          }
        }
      }
      // No touch detected
      return false;
    }

    // Check if Player defeated all Invaders in the second row 
    bool second_row_cleared() {
      for (int i = (NUM_ENEMIES / 2); i < NUM_ENEMIES; i++) {
        if (enemies[i].get_strength() > 0) {
          return false;
        }
        return true;
      }
    }

    // Check if Player defeated all Invaders in current level
    bool level_cleared() {
      for (int i = 0; i < NUM_ENEMIES; i++) {
        if (enemies[i].get_strength() > 0) {
          return false;
        }
        return true;
      }
    }

    // set up a level
    void reset_level() {
      int invader_strength = 0;
      int invader_x = 0;
      int invader_y = 0;
      for (int i = 0; i < NUM_ENEMIES; i++) {          
        if (level >= 1 && level <= 4) {
          invader_strength = strengths[level - 1][i];
        }
        else {
          invader_strength = random(1, (NUM_ENEMIES / 2));
        }

        invader_x = (i % (NUM_ENEMIES / 2)) * 4;
        invader_y = 0;
        if (i >= (NUM_ENEMIES / 2)) { 
          // INVADER_HEIGHT = 4
          invader_y = INVADER_HEIGHT;
        }
        enemies[i].initialize(invader_x, invader_y, invader_strength);
      }

      // Print game level and lives of Player
      print_level(level);
      delay(2000);
      print_lives(player.get_lives());
      delay(2000);
      // Refresh the screen
      matrix.fillScreen(BLACK.to_333());
      return;
    }
};

// a global variable that represents the game Space Invaders
Game game;

// see https://www.arduino.cc/reference/en/language/structure/sketch/setup/
void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN_NUMBER, INPUT);
  matrix.begin();
  game.setupGame();
}

// see https://www.arduino.cc/reference/en/language/structure/sketch/loop/
void loop() {
  int potentiometer_value = analogRead(POTENTIOMETER_PIN_NUMBER);
  bool button_pressed = (digitalRead(BUTTON_PIN_NUMBER) == HIGH);
  game.update(potentiometer_value, button_pressed);
  delay(50);
}

// display Level
void print_level(int level) {
  // Refresh the screen
  matrix.fillScreen(BLACK.to_333());
  matrix.setTextSize(1);
  matrix.setTextColor(WHITE.to_333());
  // Set the starting position
  matrix.setCursor(0, 0);  
  // Adafruit already has library built in that outmatically outputs text
  // matrix.print() can only print single character
  //matrix.print()
  matrix.print('L');
  matrix.print('E');
  matrix.print('V');
  matrix.print('E');
  matrix.print('L');
  matrix.print(':');
  matrix.print(' ');
  matrix.print(level);
}

// display number of lives
void print_lives(int lives) {
  // Refresh the screen
  matrix.fillScreen(BLACK.to_333());
  matrix.setTextSize(1);
  matrix.setTextColor(WHITE.to_333());
  // Set the starting position
  matrix.setCursor(0, 0);  
  matrix.print('L');
  matrix.print('I');
  matrix.print('V');
  matrix.print('E');
  matrix.print('S');
  matrix.print(':');
  matrix.print(' ');
  matrix.print(lives);
}

// displays "game over"
void game_over() {
  // Refresh the screen
  matrix.fillScreen(BLACK.to_333());
  matrix.setTextSize(1);
  matrix.setTextColor(RED.to_333());
  matrix.setCursor(0, 0); 
  matrix.print('G');
  matrix.print('A');
  matrix.print('M');
  matrix.print('E');
  matrix.print(' ');
  matrix.print('O');
  matrix.print('V');
  matrix.print('E');
  matrix.print('R');
  matrix.print('!');
}

