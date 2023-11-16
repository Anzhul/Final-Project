Test date: 2023.11.16 
*Will also be posted in the google dc*

Code modified based on version e38d136 (Xiaoyue’s latest version posted on 11.15 night)

Bugs already fixed:
1. Skipping level after dying (changed to the same level)
2. Player’s initial position not in the middle
3. Still printing lives and level even lives = 0

4. Change the velocity slower to give people time to react (and easy to debug)

5. ***For the reach part, I’ve already added an ultrasonic sensor to the board and got the right feedback, thinking of using its value instead of potentiometer_value as the input controlling the player (test code names “ultrasonicsensorTest” also posted)***

Details changed:
1. Change some of the magic numbers like 14

Bugs still needed to be fixed:
1. game_start() cannot be printed
2. will automatically reset the level when there’s still one Invader left
3. player could still go over the left-bound

4. ***After losing a life in level 1, only display print_lives() and print_level() afterward without showing the game, while lives continuously descends to 0, then game_over().***

Potential places that I feel may be wrong:
touch_bottom
touch_player