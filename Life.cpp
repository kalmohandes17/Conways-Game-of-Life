#include "Life.h"
#include "GameBoard.h"
#include <fstream>
#include <iostream>
#include <sstream>

Life::Life(ifstream &ifs) {

  // ====> DJH: This checks to see if line is two bytes long
  // I don't think any line will qualify.  The input file has two
  // numbers on the first line. They should just be read in
  // Also, after reading, clear remaining whitespace with >> ws;

  ifs >> _rowCount >> _colCount >> ws;

  // ====> DJH: the very important missing thing here is the allocation of cells
  // We want an array of bools that big enough to hold a bool for every value
  // in the file (after the first line).
  // The way to allocate an array of SomeType is
  // SomeType *arrayOfSomeType = new SomeType[count]();
  //
  // The parens at the end call the SomeType default constructor, which usually
  // zeros out the values. The way to free that array is: delete []
  // arrayOfSomeType;  // notice the [] brackets
  _cells = new bool[_rowCount * _colCount]();
  unsigned index = 0;
  string line;

  while (getline(ifs, line)) {
    for (int col = 0; col < line.size(); ++col) {
      // ====> DJH:  here, we've got a line and we're looping it.
      // no need for a stream. Look at each char with line[i]

      // ====> DJH: new bool is kind of nonsensical here.
      // It looks like an allocation, but it's just writing a zero at an offset
      // off of an uninitialized pointer.
      _cells[index] = line[col] == 'O';
      ++index;
    }
  }
}

Life::Life(const Life &rhs) {
  _rowCount = rhs._rowCount;
  _colCount = rhs._colCount;
  // ====> DJH: here again, missing an imporant allocation step
  int count = _rowCount * _colCount;
  _cells = new bool[count]();
  for (int i = 0; i < count; ++i) {
    _cells[i] = rhs._cells[i];
  }
}

Life::Life(int rowCount, int colCount) {
  _rowCount = rowCount;
  _colCount = colCount;
  // ====> DJH: same - allocate the cells
  // they can be set to false with (); at the end of the allocation

  _cells = new bool[_rowCount * _colCount]();
}

Life &Life::operator=(const Life &rhs) {
  if (this != &rhs) {
    _rowCount = rhs._rowCount;
    _colCount = rhs._colCount;

    // ====> DJH:  this one is missing the allocation too
    // but before allocating here, the old array must be freed
    // one delete for the whole array
    delete[] _cells;
    int count = _rowCount * _colCount;
    _cells = new bool[count]();

    for (int i = 0; i < count; ++i) {
      _cells[i] = rhs._cells[i];
    }
  }
  return *this;
}

// ====> DJH: see comments about freeing at the top
Life::~Life() { delete[] _cells; }

// ====> DJH: Look at the array labs or my class presentation from Tuesday
// the index of a row and col is not row*col
bool Life::cellValueAt(int row, int col) const {
  if ((row < _rowCount) && (0 <= row) && (col < _colCount) && (0 <= col)) {
    return _cells[(row * _colCount) + col];
  }
  return false;
}

// ====> DJH: Look at the array labs or my class presentation from Tuesday
// the index of a row and col is not row*col
void Life::setCellValueAt(int row, int col, bool value) {
  _cells[(row * _colCount) + col] = value;
}

int Life::draw(GameBoard *board) const {
  int livingCells = 0;
  for (int row = 0; row < rowCount(); ++row) {
    for (int col = 0; col < colCount(); ++col) {
      char glyph = cellValueAt(row, col) ? 'O' : '.';
      if (glyph == 'O') {
        ++livingCells;
      }
      board->setTileAt(row, col, glyph);
    }
  }
  return livingCells;
}

// Life Life::nextLife() const {

//   int elements = rowCount() * colCount();
//   for (int i = 0; i < elements; ++i) {
//     if (!(_cells[i]) && livingNeighbors() == 3) {
//       cellState = true;
//   }
//   else if ((cellState) && ((livingNeighbors() <= 3) && (livingNeighbors() >=
//   2)) {
//       continue;
//   }
//   else {
//       cellState = false;
//   }
//   }
// }

Life Life::nextLife() const {
  Life nextLife(_rowCount, _colCount);
  for (int row = 0; row < _rowCount; ++row) {
    for (int col = 0; col < _colCount; ++col) {
      // we need the cell state at row and col
      // we need the living neigbors at row and col
      // we need to determine when the nextLife at that row and col should be
      // true according to the rules...
      bool currLifeValue = cellValueAt(row, col);
      int numLivingNeighbors = livingNeighbors(row, col);
      
      bool nextLifeValue = (numLivingNeighbors == 2 && currLifeValue) || numLivingNeighbors == 3;
      nextLife.setCellValueAt(row,col,nextLifeValue);
      
    }
  }
  return nextLife;
}

// ====> DJH: Big! We want small.
// ideas: only the true values
// pack 2 of them into 3 bytes (why 24 bits?)
// "run length encode" them. google that if you're interested
string Life::toString() const {
  string instanceOfLife = "";

  for (int row = 0; row < _rowCount; ++row) {
    for (int col = 0; col < _colCount; ++col) {
      instanceOfLife.push_back(cellValueAt(row, col) ? 'O' : '.');
    }
    instanceOfLife += "\n";
  }
  return instanceOfLife;
}

int Life::livingNeighbors(int row, int col) const {
  int result = 0;

  for (int r = row - 1; r <= row + 1; ++r) {
    for (int c = col - 1; c <= col + 1; ++c) {
      if (cellValueAt(r, c)) {
        ++result;
      }
      // ====> DJH: same confusion here as above.  _cells needs a flat index
      // the index of row, col means you want to skip row*colCount indexes
      // to get to the start of a column, then you get to the col from there
      // by adding col
    }
  }
  if (cellValueAt(row, col)) {
    --result;
  }
  return result;
}
