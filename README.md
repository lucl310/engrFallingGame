# engrGameSystem
Game system with the adafruit CPX board for ENGR 103

The game will be a simple rhythm-based game, similar to Dance Dance Revolution or Bop-It! in which the player must hit either the left or right button in order to hit the corresponding light on time. The light should trail down from the top and when it hits the bottom, if it is hit on time, it will disappear, otherwise it will be a game-over state. Every 15 seconds that the player survives, the game will become more difficult, leveling up, causing the lights to appear more frequently and fall at a faster rate. The player’s score will be shown at the end through certain colored lights denoting what level they ended on. The player will signal the start of the game by pressing both buttons and once they die, they can begin a new game by pressing these buttons again. This will reset all scores and variables and start a new game session

The inputs will be through the two buttons provided on the CPX board and the output will be shown through lights and sounds
