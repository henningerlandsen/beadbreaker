Bead Breaker
============ 

Notes
-----
I hope you will have fun testing the game. I think I have covered most cases described in the specs. The core functionality of the game is unit-tested. Tests are provided in the XCode project. The view and animation part of the game is not tested due to the limited time-frame. Since it is built to very specific specs, flexibility has not been a big concern. That being said, I believe the code to be maintainable should be easy to generalize to a higher degree. E.g. flexible board sizes, additional piece types and so on. Personally, I would have added a score counter, multiple levels and fancy rewarding particle explosions as first improvements.

The bundle consists of the following:
- /Build:		Contains a build for Mac OS X 10.9 64bit zipped
- /Resources:	Image sprites for the game
- /Source:		The source code for the project
- /XCode:		Folder containing an XCode project and unit tests


Main Structure
--------------
The main design of the game is around an adaption of the Model-View-Controller pattern, where the BoardModel serves at the interface for manipulating the pieces on the board. The Game class serves as the Controller, handling input, manipulating the model and notifying the view.

Board Model:
   Employs most of the game rules and dynamics. Having this representation of the board at an early stage made it possible to create unit tests for the entire game dynamics before proceding with drawing graphics.

Game:
   Game is built around a very simplistic state based game loop. It runs the loop until it reaches the Quit state. It handles input events, manipulates the model and view according to the game state.

Board View:
	Holds all sprites and its own representation of the board. Although not a very generic view, it manages to separate the state of the sprites from the game logic. It defines how the animations should be performed and how the game should be drawn to screen.


Utility classes
---------------
Board:
	Main datastructure of the game. It simply wraps a two-dimensional array and provides a single point of entry for manipulation.

Index:
	Represents a position on the board. By implementing comparison operators indices are sortable, column first, easing algorithms for falling piece calculation.

Sprite:
	Texture, size per sprite and number of sprites in the sprite sheet.

Painter:
	Uses SDL_Renderer to render sprites at a given position and scale.