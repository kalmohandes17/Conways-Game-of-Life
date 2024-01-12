#include "GameBoard.h"
#include "Snake.h"
#include "Candy.h"
#include "CandyMaker.h"

#include <iostream>

static const int kBoardRowCount = 20;
static const int kBoardColCount = 30;

/*****************************************************************************/

// return snake length concatenated with some status message 
string Message(string status, const Snake &snake) {
  return "Length: " + to_string(snake.length()) + ". " + status;
}

/*****************************************************************************/
int main() {
  bool gameOver = false;
  int turnCount = 0;
  bool twitchMode = false;
  int dRow = 0, dCol = 1;
  string status = "";

  GameBoard board(kBoardRowCount, kBoardColCount);
  board.setDisplayCoords(false);
  board.setDisplayEmptyTileDots(false);

  Snake snake;
  snake.setTiles(&board);

  CandyMaker myCandyMaker(kBoardRowCount, kBoardColCount);
  Candy myCandy = myCandyMaker.newCandy(snake);
  myCandy.draw(&board);
  
  /*** Code for initializing the candy mechanism goes here. ***

  The design discussion will likely suggest that something representing candy is
  created and initialized here.
  
  Once the design discussion finishes its work, additional code will be included
  in the instructions. You can copy/paste it from there.
  */
  
  while (!gameOver) {
    board.setMessage(Message(status, snake));
    status = "";

    // draw the tiles and status on the console
    board.updateConsole();

    unsigned int timeout = twitchMode ? 1 : 0;
    char cmd = board.nextCommandKey(timeout);

    int nextRow = snake.head()->row();
    int nextCol = snake.head()->col();

    switch (cmd) {
      case ' ':
        twitchMode = !twitchMode;
        break;
      case arrowUpKey:
        dRow = -1;
        dCol = 0;
        break;
      case arrowDownKey:
        dRow = 1;
        dCol = 0;
        break;
      case arrowRightKey:
        dCol = 1;
        dRow = 0;
        break;
      case arrowLeftKey:
        dCol = -1;
        dRow = 0;
        break;
      case 'q':
      case 'Q':
        cout << "Bye" << endl;
        gameOver = true;
        break;
      default:
        break;
    }

    nextRow += dRow;
    nextCol += dCol;

    if (snake.willCrash(&board, nextRow, nextCol)) {
      status = "Ouch, my nose!";
      gameOver = true;
      continue;
    }

    snake.step(&board, nextRow, nextCol);

    /*** Code for eating candy goes here. ***

    The design discussion will likely suggest that code handling the idea
    that snakes must eat candy be placed here


    Once the design discussion finishes, additional code will be included
    in the instructions. You can copy/paste it from there.
    */
    if (snake.headIntersects(myCandy.row(), myCandy.col())) {
      snake.eat(myCandy.calories(), myCandy.color());
      myCandy = myCandyMaker.newCandy(snake);
      myCandy.draw(&board);
    }
    
    ++turnCount;
  }
  
  snake.setTiles(&board);
  board.setMessage(Message(status, snake));
  board.updateConsole();
}

// ====> DJH: Great project, Khaled! And close enough to on-time.
// A couple of spots in this code are lovely, very well written! I'm happy to see that
// you've become a stronger programmer this quarter. If you add just a little bit of
// work-process discipline, earlier starts mainly, I think you're going to see a big payoff.
// Nice having you in the class!  98/100