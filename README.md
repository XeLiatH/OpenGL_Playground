# OpenGL Playground
 
 > More of a scene with some moving parts and interactive camera

## Controls
 
* `W` - Move forward
* `A` - Move left
* `S` - Move back
* `D` - Move right
* `Esc` or `Q` - Quit the game
* `F` - Toggle fullscreen and windowed fullscreen

## Scene description

* 36x36 floor
* One wall tile
* When player collised with the wall, the wall disappears
* In the background thread there is a motion tracker which moves with the wall a little in x direction
* Levitating backpack in the middle
* 4 point light
* Sun pointing to the center of the scene (redish color)
* player (aka Camera class) is equipped with a flashlight and is locked in Y axis to achieve the 'FPS camera style'.
* mouse can be pointed in general direction with bounded pitch
* player can zoom in/out with mouse wheel
* every model was made in blender and is loaded as an `.obj` file
* Skybox is some sort of a red planet

## Installation

* Extract .zip file from v1.0 release
* Run the .exe file

## Credit

Thank you to [learnopengl.com](https://learnopengl.com/)
