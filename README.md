# Mandelbrot Explorer
> Explore the fascinating world of fractal images.

With the mandelbrot explorer you can move around and zoom deeply into the [mandelbrot set](https://en.wikipedia.org/wiki/Mandelbrot_set).
It is possible to switch between randomly created color palettes, so the number of beautiful images is literally infinite.
You can safe images you like and since they have the resolution of your desktop they are well suited as wallpaper.

## Installation

### Linux

On Linux you have to install [SDL2](https://wiki.libsdl.org/Installation).

```sh
sudo apt-get install libsdl2-2.0
```
Save the [executable](https://github.com/the5avage/mandex/releases/download/v1.0.0/mandex) on your PC.
Open a terminal in the same directory and start the program with:
```sh
./mandex
```

### Windows

Extract the [zip](https://github.com/the5avage/mandex/releases/download/v1.0.0/mandex_windows.zip) 
file and double click on mandex.exe

## Usage

Before you start you should remeber (or write down) the basic control keys.

| Key | Action |
|---|---|
| Esc | Close the application |
| &#8592; &#8594; &#8593; &#8595; | move around with arrow keys |
| i | zoom **i**n |
| o | zoom **o**ut |
| c | change **c**olor palette |
| p | saves the currently displayed image (aka. **p**rint) |

Images are saved in the directory which contains the executable as .bmp files.

The calculation of the mandelbrot set is computationally intensive. Dependent on your cpu and how deep you zoom in
it might take a while until the image is fully rendered.

## Development setup

To build from source you have to install [SDL2](https://wiki.libsdl.org/Installation) development library. 

### Linux

```sh
sudo apt-get install libsdl2-dev
```

### Windows

The windows release was build with msys2 and mingw64.
If you want to build the debug version on windows you have to edit the makefile in ``bin/debug`` because AddressSanitizer 
is not available on windows.

## Contributing

There is still a lot of room for improvment and i would be glad if you want to contribute.
Since the mandelbrot set is a mathematically easy, but computationally expensive, algorithm it is well suited to learn about high performance computing.
Here is a short list of improvments, which i think are intresting. If you have an idea you can just create an issue and explain it.

* Optimize the calculation of the mandelbrot set for speed by
  * mathematically improvment of the algorithm
  * vectorization
  * use the GPU
  
* Right now it is not possible to zoom in infinitely because at some point the values are too small for doubles. Maybe there is a mathematically way to get around this.

* Add other fractals like the [julia set](https://en.wikipedia.org/wiki/Julia_set) or the [buddabrot](https://en.wikipedia.org/wiki/Buddhabrot).
  * Contribute pseudo code. I will implement it if i have time.
  * If you want to contribute a fractal as C-Code look at the mandelbrot.h/.c files. The interface you provide should be the same.

## Licence

Distributed under the MIT licence. See [licence](LICENCE.txt) file for more information.
