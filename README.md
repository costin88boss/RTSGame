# RTSGame
An RTS multiplayer game/demo I guess...

## Building from Source
Make sure CMake is installed and run the following commmands:
```
cmake -S ./ -B out/ -G "Ninja"
cmake --build out/ --config Release --target RTSGame -j 6
```
**Note:** On GNU-Linux you may need to install additional dependencies, on Debian GNU-Linux and derivatives, the command is:
```
sudo apt-get update && sudo apt-get install libxrandr-dev libxcursor-dev libxi-dev libudev-dev libflac-dev libvorbis-dev libgl1-mesa-dev libegl1-mesa-dev libfreetype-dev
```
