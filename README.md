# -Insert game title here-
EC327 Summer 2023 Project

This repository is [insert description here].

GIF demo of the program:
!['file name']('path to file')

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
git clone https://github.com/RX-00/EC327_Proj
```
Next, we need to go inside the main directory and build the project.
```sh
cd EC327_Proj
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
./EC327_Proj
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