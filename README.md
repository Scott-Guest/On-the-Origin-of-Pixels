
# On the Origin of Pixels
Inspired by John Conway's [Game of Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life), *On the Origin of Pixels* is an interactive demo which models evolution using cellular automata. It includes two other game modes exploring different aspects of evolution, as well as an implementation of the original Game of Life. See the usage section for more details.

## Installation
This application requires the C++ SFML library. To install, follow the download instructions from [here](https://www.sfml-dev.org/). If you have homebrew, you can also run
```
brew install sfml
```
Once you have SFML installed, simply clone this repository and run
```
make all
```
in the top directory.
## Usage
The application can be started by running
```
bin/pixels InputFile.txt [OutputFileName]
```
where `InputFile` is a settings file for the given game mode, and `OutputFileName` is the root name for data files. The output file is only used / necessary in the Average and Allele modes. See each game mode below for the formatting of the settings file.

### Controls
The controls common across all game modes are:
* <kbd>P</kbd> - Pause/Unpause the game.
* <kbd>R</kbd> - Restart the game, populating the board with new cells.
* <kbd>U</kbd> - Advance a single generation while paused.
* <kbd>&larr;</kbd> - Increase the speed of evolution.
* <kbd>&rarr;</kbd> - Decrease the speed of evolution.

### Default Mode
The Default mode is a standard implementation of Conway's Game of Life, with white cells alive and black cells dead. From Wikipedia, these rules are
```
1.  Any live cell with fewer than two live neighbors dies, as if by underpopulation.
2.  Any live cell with two or three live neighbors lives on to the next generation.
3.  Any live cell with more than three live neighbors dies, as if by overpopulation.
4.  Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction.
```
The settings file should be formatted as
```
Default
WINDOW WIDTH: [width of game window in pixels]
WINDOW HEIGHT: [height of game window in pixels]
GRID WIDTH: [width of board in cells]
GRID HEIGHT: [height of board in cells]
PROBABILITY ALIVE: [probability a cell starts alive]
```
where `PROBABILITY ALIVE` should be in the range `[0, 1]`. 

![Default](/images/Default.png?raw=true)

The title bar indicates the current generation.

### Average Mode
In Average Mode, every living cell starts with a random ROYGBV color. The reproduction rules are followed as in the Default mode, but with an added 10% chance that a new cell can be born at a dead location with exactly 2 living neighbors. 

Each cell is also assigned a fitness by treating its RGB color value as a point in 3D space, then taking the Euclidean distance between this point and some ideal color point. Fitness is scaled to be in the range `[0, 100]`,  and the ideal color is specified in the settings file. Additional controls for the ideal color are:
* <kbd>A</kbd> / <kbd>Z</kbd> - Increase / Decrease the ideal red channel.
* <kbd>S</kbd> / <kbd>X</kbd> - Increase / Decrease the ideal green channel.
* <kbd>D</kbd> / <kbd>C</kbd> - Increase / Decrease the ideal blue channel.
* <kbd>F</kbd> - Set the ideal color to red.
* <kbd>G</kbd> - Set the ideal color to white.
* <kbd>H</kbd> - Set the ideal color to blue.

When a new cell receives its color, it can do so through either asexual or sexual reproduction. For sexual reproduction, a cell's new color is taken as the average of its two most fit neighbors' colors. For asexual reproduction, a cell's new color is just a copy of its most fit neighbor's color. The mode of reproduction is also specified in the settings file. 

To introduce genetic variability, there is also a chance that a newly created cell's color will mutate. The chance of a mutation can be specified in the settings file. When a mutation does occur, its size is randomly selected such that the chance of a mutation of size `n` occurring is proportional to `1 / n^2`.

Recording can be toggle with the <kbd>N</kbd> key. While recording, each generation the ideal color, number of living cells, average mutation rate, average fitness, and standard deviation of fitness deviation is recorded. This data is written to `OutPutFileName_num.csv`, where `num` is incremented each time recording is toggled.

The settings file should be formatted as
```
Average
WINDOW WIDTH: [width of game window in pixels]
WINDOW HEIGHT: [height of game window in pixels]
GRID WIDTH: [width of board in cells]
GRID HEIGHT: [height of board in cells]
PROBABILITY ALIVE: [probability a cell starts alive]
IDEAL COLOR: [hex RGB value for ideal color (0x...)]
SEXUAL: [T for sexual or F for asexual]
MIN MUTATION RATE: [minimum mutation rate]
MAX MUTATION RATE: [maximum mutation rate]
```
where `PROBABILITY ALIVE`, `MIN MUTATION RATE`,  and `MAX MUTATION RATE` should be in the range `[0, 1]`. 

![Average](/images/Average.png?raw=true)

The title bar indicates the current generation, the ideal color, and whether data is being recorded.

### Allele Mode
In Allele mode, every cell is assigned an allele pair, where each allele is either a dominant blue color gene or a recessive yellow color gene. The initial frequency of alleles can be specified in the settings file, and the color of a cell indicates its phenotype. 

Reproduction rules are the same as in Average mode i,e, the standard Game of Life rules with an added 10% chance that a new cell can be born at a dead location with exactly 2 living neighbors. When a new cell is created, two of its living neighbors are randomly selected to be parents, and each passes one of their alleles to the new cell.

Recording can be toggled with <kbd>N</kbd>. While recording, each generation the frequency of dominant alleles, recessive alleles, dominant phenotypic cells, and recessive phenotypic cells is recorded. This data is written to `OutPutFileName_num.csv`, where `num` is incremented each time recording is toggled.

The settings file should be formatted as
```
Allele
WINDOW WIDTH: [width of game window in pixels]
WINDOW HEIGHT: [height of game window in pixels]
GRID WIDTH: [width of board in cells]
GRID HEIGHT: [height of board in cells]
PROBABILITY ALIVE: [probability a cell starts alive]
DOMINANT FREQ: [initial frequency of dominant allele]
```
where `PROBABILITY ALIVE` and `DOMINANT FREQ` should be in the range `[0, 1]`. 

![Allele](/images/Allele.png?raw=true)

The title bar indicates the current generation, whether data is being recorded, and the genotype and position of the cell currently under the mouse.

## License
This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## To-Do
* Refactor to make recording / specifying an output file optional.
* Fix bug where the first line of data is sometimes written to a separate file.
* Add mutations to Allele mode.
