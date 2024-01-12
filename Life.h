#ifndef __LIFE_H__
#define __LIFE_H__

#include <string>
class GameBoard;
using namespace std;

class Life {
public:
  // Default constructor
  Life() = default;

  // Construct from a file stream. The file stream must point to an open figure
  // file. That format has the rowCount and colCount on the first line, and the
  // initial state of cells on the remaining lines (by convention, 'O'
  // represents living cells, and dots represent dead cells.
  Life(ifstream &ifs);

  // Copy constructor. Construct using the state of the passed game of life
  // its rowCount, colCount and a deep copy if its cells.
  Life(const Life &rhs);

  // Construct with row and column counts and all dead (==false) cells
  Life(int rowCount, int colCount);

  // Copy assignment
  Life &operator=(const Life &rhs);

  // HINT: Notice that the constructors and assignment operator all need
  // the same thing: they all need to set the row and col count on a game
  // they all need to allocate new cells, and some of them need to copy
  // the values of those cells from an existing array.
  //
  // Remember about don't repeat yourself?  Well, don't repeat yourself.
  // For real, don't repeat yourself :-)

  // Destructor, which deallocates cells
  ~Life();

  // Return the value of the cell at the specified position
  bool cellValueAt(int row, int col) const;

  // HINT: You might find it beneficial to create two versions of
  // cellValueAt, where one checks the row, col for being in bounds
  // and the other assumes they are.

  // Set the value of the cell at the specified position
  void setCellValueAt(int row, int col, bool value);

  // Draw the current state on the given GameBoard, and return
  // the total number of living cells
  int draw(GameBoard *board) const;

  // Return a new Life object representing the receiver's state updated
  // according to the rules of Conway's Game of Life. In a nutshell:
  //
  // Non-living cells with exactly 3 live neighbors come to life in the next
  // generation Living cells with 2 or 3 live neighbors remain living in the
  // next generation Living cells with fewer than 2 live neighbors die of
  // loneliness in the next generation Living cells with 4 or more live
  // neighbors die of overcrowding in the next generation
  Life nextLife() const;

  // Dimension getters
  int rowCount() const { return _rowCount; }
  int colCount() const { return _colCount; }

  // Computed getter returning the total cell count
  int cellCount() const { return _rowCount * _colCount; }

  // Honors section - Decide how specifically to encode the complete state of
  // the receiver and return that encoding as a string
  //
  // Non-honors - try it for fun, or just return "";
  string toString() const;

private:
  int _rowCount = 0;      // Number of rows in the grid
  int _colCount = 0;      // Number of columns in the grid
  bool *_cells = nullptr; // Dynamic array of cell values

  // Return the number of living neighbors for the cell at the specified
  // position
  int livingNeighbors(int row, int col) const;
};

#endif
