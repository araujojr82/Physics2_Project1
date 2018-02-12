Physics 2 Project 1 

Please compile it in 64 bits

Author: Euclides Araujo

User input keys:
W,A,S,D,Q,E: Controls the camera position; NOT THE BALL
UP, DOWN, LEFT and RIGHT arrows: Control the selected ball 
TAB: Change the selected ball
ENTER: Enter the debug mode


Configuration of the scene is done via the objects.txt file.
Each line can create 1 object in the scene, the line structure is:
meshname posX posY posZ scale mass

Meshname is String pointing to the corresponding mesh in the meshlist.txt
The other values are float

meshname	  X    Y    Z  SCALE MASS
example:
ball		 0.0  0.0  7.0  1.5  7.5
ball		-3.0 -3.0  7.0  1.2  6.0
ball		 3.0  3.0  7.0  1.0  5.0
floor_plane	 0.0  0.0  0.0  4.0  0.0
north_plane	 0.0  4.0  0.0  4.0  0.0
south_plane	 0.0 -4.0  0.0  4.0  0.0
left_plane	-4.0  0.0  0.0  4.0  0.0
right_plane	 4.0  0.0  0.0  4.0  0.0