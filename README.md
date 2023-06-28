# -Raycasting Engine Demo-
EC327 Summer 2023 Project

This repository is a demo of a raycast game engine. Similar to the game engine used in the original Wolfenstein 3D game by id Software back in 1992. Raycasting is a method to produce a pseudo three dimensional game world. This is achieved through rendering a 2D game world map into a first persion 3D view by casting rays from the player into the 2D map and drawing a 3D-esque first person world view based on the distance of the rays. Our implementation fully utilizes OOP practices as seen by the formulation of player, map, and renderer engine classes. We also utilize CMake to allow this repo to be easily compiled and debugged across platforms. This implementation of the raycasting algorithm also allows for a map editor in which the map is a gridworld. However, with this current implementation collision detection is not yet possible.

GIF demo of the program:
!['raycast_demo.gif'](content/raycast_demo.gif)

Authors:

- Roy Xing
- Junny Myung
- Alexa Wiencek

## How to Use
### Libraries
[SFML](https://www.sfml-dev.org/) is required. For Unix systems you can install it with your preferred package managers. For example, for Ubuntu:

```sh
sudo apt install libsfml-dev
```
For Windows systems you need to install SFML through their website: https://www.sfml-dev.org/download.php. As this project utilizes CMake, which is a cross-platform build system generator, it should be possible to build this project regardless of OS (platform agnostic).

### Building
First, you need to clone this repository to your local machine.
```sh
git clone https://github.com/RX-00/raycast
```
Next, we need to go inside the main directory and build the project.
```sh
cd raycast
mkdir build && cd build
cmake ..
cmake --build .
```
If you want to remove the build and rebuild a new version of the program use the following command and then go through the building process again.
```sh
cmake --build . --target clean
```

### Running
Within the build directory there should be an executable file that you can now run with the following command:
```sh
./raycast
```

### Bash Script
Alternatively, you could use the bash script in the main (root) directory to clean, build, and run the program:
```sh
./bash_build.sh
```

## References
The companion repository for the [CMake: Building SFML and Game Projects on Linux](https://dane-bulat.medium.com/cmake-building-sfml-and-game-projects-on-linux-3947b3ba6e8) article.

[F. Permandi's 1996 Tutorial on Ray-Casting](https://permadi.com/1996/05/ray-casting-tutorial-table-of-contents/)

[Lode Vandevenne's 2004 Tutorial Write-Up on Ray-Casting](https://lodev.org/cgtutor/raycasting.html)
