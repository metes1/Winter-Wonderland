Team Members:
Seda Mete 
Christina Mudarth

Description
---------------------------
Winter Wonderland: Our sad snowman is walking around, when it starts snowing the snowman gets excited and smiles and moves it’s arms faster. 

How to Run Program
--------------------
The program is run using the makefile located in the directory. While in the Project directory, type "make" to run the program. It will take a few seconds for the environment to load.

Controls
------------
- Left-Arrow   => Rotate scene left on y-axis
- Right-Arrow  => Rotate scene right on y-axis
- Up-Arrow Key => Zoom In
- Down-Arrow   => Zoom Out
- T            => Move Snowman forward
- G            => Move Snowman backward
- F            => Move Snowman left
- H            => Move Snowman right
- N            => Toggle night time mode
- S            => Toggle snowfall
- W            => Toggle wind
- C            => Toggle textures
- P            => Show 2D Overview pixel map
- q/ESC        => Quit program


Additional Features
---------------------------------
1) Lighting

2) Textures: Used for skybox and terrain, press "C" to change terrain textures.

3) Particle System: Used to create snowfall in the environment, press "S"

3) Terrain Generation: Used diamond-square algorithm

4) Non-Geometric Primitives : 2D overview pixel map of the terrain, shows height values of terrain. Press "P"

5) Animated character: Snowman moves his arms on his own. He can be moved by the user using keys T, G, F, H


References
----------------

1) Diamond-Square algorithm used in Terrain.cpp heavily references this code: https://medium.com/@nickobrien/diamond-square-algorithm-explanation-and-c-implementation-5efa891e486f

2) Obj Files:
Bush: https://github.com/harwooja/LowPoly/blob/master/images/tree.obj
Pine tree: https://free3d.com/3d-models/obj

3) Obj Downloader:
https://github.com/harwooja/LowPoly?fbclid=IwAR0SZmbdqxINsVhvr32mKtmCh16VKb-qVlDR_SiwAxF8hwYpUnh8owZF0iw
- We used this GitHub object loader library because all our models were low poly. So it wasn’t necessary to use a powerful object loader like assimp
- This GitHub included the object loader .cpp files including Model.cpp and Recourceloader.cpp, only used the .obj loader

4) PPM Files:
- taken from class tutorials

5) SkyBox Images:
https://www.dropbox.com/sh/phslacd8v9i17wb/AABui_-C-yhKvZ1H2wb3NykIa?dl=0
http://www.custommapmakers.org/skyboxes.php

6) Used to convert SkyBox images from .png to .ppm P3 files:
imagemagick.org