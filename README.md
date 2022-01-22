# grapher
A pure C++ program(can be used as library), that "converts" images to math graphs

## How it works
**grapher** makes grayscale mode image from input, makes monochrome image. After program will work only with monochrome image. **grapher** searches for diagonal, horizontal, vertical "sections", individual pixels(pixels not included in sections) on image. It converts sections and invidual pixels coordinates to math expressions. Next, it exports expressions for [Desmos](https://desmos.com/calculator)

## Build and requirements
Just build with any C++ compiler, for sure, it maybe should be not older than 5 years old :)

## Notes
1. Remove _CRT_SECURE_NO_WARNINGS_ if you are not on Windows based machine
2. Works only with BMP format, but all bit widthes.
3. Searching for diagonals takes a lot of time

## Documentation(if using as library)
1. Check Utils.h, BMP.h, all functions here
Including and using:

## Using as library
1. Include Utils.h header
2. Define pointsize and offset variables(like in main.cpp), they are external.
3. Create BMP object, set it's filename variable.
4. Use bmp.open, check Docs section and main.cpp

## Using as program
1. Compile
2. Open using Prog FILEIN FILEOUT POINTSIZE OFFSET | 
FILEIN - filename of image, FILEOUT - filename of file to return math expressions, POINTSIZE - point size of individual pixels(recomendated 0.3), OFFSET - offset by 0 0.

## Examples
https://www.desmos.com/calculator/wzx2pkdtfy

Good luck

