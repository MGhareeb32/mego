# Mego

Minecraft with MEGO's.

![ScreenShot](https://raw.github.com/MGhareeb32/mego/master/screenshot.png)

This is part of an academic course; everything is implemented from scratch for learning
purposes. Can be used as an intro to OpenGL 3.1+.

[![ScreenShot](https://raw.github.com/MGhareeb32/mego/master/youtube.png)](http://youtu.be/Q1xIcnBskSo)

### FEATURES

1. Basic game engine for OpenGL abstraction, game loop, cameras, light, transformation,
   collision detection, model/texture loading, materials, game entities, and hierarchies.
2. FPS player that can move, jump, pickup and place bricks.
3. Basic inventory and item switching system.
4. Small map generator.

## DEVELOPMENT

### ECLIPSE WITH MINGW

0. Clone the repo:
      git clone https://github.com/MGhareeb32/mego.git mego
1. Create a new Executable Hello World C++ Project in the mego folder.
2. Click next. Type 'src' into Source.
3. Click Finish.
4. Right click the project > Properties > C++ Genral >
   Paths and Symbols > Libraries.
5. Add each of the following: freeglut, glu32, opengl32, glew32, SOIL.
6. In Paths and Symbols > Library Paths:
    Click Add. Type in /mego/include/soil. Is a workspace path. Ok.
7. Hit Ok, Build, and Run. 

## TODO LIST
1. Optimise bricks rendering.
2. SQLite for storing the state.
3. Advanced terrain generator with extensible biomes.
4. Third person camera.
