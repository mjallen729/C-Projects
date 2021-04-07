#include <stdio.h>
#include <time.h>
#include <stdlib.h>

//scores:
int turnNum;
int playerScore[36];
int compScore[36];
//AI variables
int aSpotv;
int aSpoth;

void printBoard(char board[6][6]) { //Method that prints the gameboard
  printf("\t1 2 3 4 5 6\n\n");

  for (int i = 0; i < 6; i++) {
    printf("%d\t", i+1);

    for (int j = 0; j < 6; j++) {
      printf("%c ", board[i][j]);

    }
    printf("\n");

  }

}

int isOut(char board[6][6], int spotV, int spotH) { //checks if player (or ai) is out. returns 1 if out, 0 if not out
  int isOut = 1; //0 is false, 1 is true

  if ((spotH - 1 >= 0 && board[spotV][spotH-1] != '.') || (spotH + 1 <= 5 && board[spotV][spotH+1] != '.') && board[spotV][spotH+1] != 'A' && board[spotV][spotH-1] != 'A') { //left,right
    isOut = 0;
    return isOut;

  }

  if ((spotV - 1 >= 0 && board[spotV-1][spotH] != '.') || (spotV + 1 <= 5 && board[spotV+1][spotH] != '.') && board[spotV-1][spotH] != 'A' && board[spotV+1][spotH] != 'A') { //up,down
    isOut = 0;
    return isOut;

  }

  if ((spotH - 1 >= 0 && spotV - 1 >= 0 && board[spotV-1][spotH-1] != '.') || (spotH + 1 <= 5 && spotV + 1 <= 5 && board[spotV+1][spotH+1] != '.') && board[spotV-1][spotH-1] != 'A' && board[spotV+1][spotH+1] != 'A') { //upleft,downright
    isOut = 0;
    return isOut;

  }

  if ((spotH - 1 >= 0 && spotV + 1 <= 5 && board[spotV+1][spotH-1] != '.') || (spotH + 1 <= 5 && spotV - 1 >= 0 && board[spotV-1][spotH+1] != '.') && board[spotV+1][spotH-1] != 'A' && board[spotV-1][spotH+1] != 'A') { //downleft,upright
    isOut = 0;
    return isOut;

  }

  return isOut;

}

int printScore(int score[36]) { //prints the score
  int i = 0;
  int total = 0;

  while (score[i+1] != 0) {
    printf("%d+", score[i]);
    total += score[i];
    i++;

  }

  total += score[i];

  printf("%d= %d\n", score[i], total);

  return total;

}

int isValidMove(char board[6][6], int startv, int starth, int endv, int endh) { //Checks if move is valid. Returns 0 for false, 1 for true
  //sanity checks:
  if (endv > 5 || endv < 0) { //bounds v
    return 0;

  }

  if (endh > 5 || endh < 0) { //bounds h
    return 0;

  }

  if (board[endv][endh] == '.' || board[endv][endh] == 'A' || board[endv][endh] == 'P') { //if landing on dot or on other player
    return 0;

  }

  if (startv == endv && starth == endh) { //same spot
    return 0;

  }

  //pathtracing:
  /*
  Slope (y/x) can:
    -be 0 (flat)(y=0)
    -be 1 (slant)(none=0)
    -be undefined (vertical)(x=0)
  */
  int changeY = endv - startv; //positive = moved down, negative = moved up (V)
  int changeX = endh - starth; //positive = moved right, negative = moved left (H)

  //printf("$$%d,%d\n", changeX, changeY);

  if (changeX == 0 && changeY != 0) { //if slope is vertical
    if (changeY < 0) { //move up
      int go = startv - 1;

      while (go != endv) {
        if (board[go][starth] == '.' || board[go][starth] == 'A' || board[go][starth] == 'P') {
          return 0;

        }
        go--;

      }

    }

    if (changeY > 0) { //move down
      int go = startv + 1;

      while (go != endv) {
        if (board[go][starth] == '.' || board[go][starth] == 'A' || board[go][starth] == 'P') {
          return 0;

        }
        go++;

      }

    }

    return 1;

  }

  if (changeY == 0 && changeX != 0) { //if slope is flat
    if (changeX < 0) { //move left
      int go = starth - 1;

      while (go != endh) {
        if (board[startv][go] == '.' || board[startv][go] == 'A' || board[startv][go] == 'P') {
          return 0;

        }
        go--;

      }

    }

    if (changeX > 0) { //move right
      int go = starth + 1;

      while (go != endh) {
        if (board[startv][go] == '.' || board[startv][go] == 'A' || board[startv][go] == 'P') {
          return 0;

        }
        go++;

      }

    }

    return 1;

  }

  if (abs(changeX) == abs(changeY)) { //if slope is slanted
    if (changeY / changeX > 0) { //slope negative -> \.
      if (endh < starth) { //go upperleft (h-1,v-1)
        int gov = startv - 1;
        int goh = starth - 1;

        while (gov != endv && goh != endh) {
          if (board[gov][goh] == '.' || board[gov][goh] == 'A' || board[gov][goh] == 'P') {
            return 0;

          }
          gov--;
          goh--;

        }

      } else { //go lowerright (h+1,v+1)
        int gov = startv + 1;
        int goh = starth + 1;

        while (gov != endv && goh != endh) {
          if (board[gov][goh] == '.' || board[gov][goh] == 'A' || board[gov][goh] == 'P') {
            return 0;

          }
          gov++;
          goh++;

        }

      }

    }

    if (changeY / changeX < 0) { //slope positive -> /.
      if (endh > starth) { //go upperright (h+1,v-1)
        int gov = startv - 1;
        int goh = starth + 1;

        while (gov != endv && goh != endh) {
          if (board[gov][goh] == '.' || board[gov][goh] == 'A' || board[gov][goh] == 'P') {
            return 0;

          }
          gov--;
          goh++;

        }

      } else { //go lowerleft (h-1,v+1)
        int gov = startv + 1;
        int goh = starth - 1;

        while (gov != endv && goh != endh) {
          if (board[gov][goh] == '.' || board[gov][goh] == 'A' || board[gov][goh] == 'P') {
            return 0;

          }
          gov++;
          goh--;

        }

      }

    }

  } else {
    return 0;

  }

  return 1;

}

void aiMove(char board[6][6], int curv, int curh, int reservedv, int reservedh) { //Moves the AI
  //AI
  int goalV;
  int goalH;

  char seg = '\0';

  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 6; j++) {
      if (board[i][j] != '.' && board[i][j] != 'A' && board[i][j] != 'P' && i != reservedv && j != reservedh) {
        if (board[i][j] > seg && isValidMove(board, curv, curh, i, j) == 1) {
          goalV = i;
          goalH = j;
          seg = board[i][j];

        }

      }

    }

  }
  aSpotv = goalV;
  aSpoth = goalH;

  board[curv][curh] = '.';

}

void turn(char board[6][6], int spotPVert, int spotPHori) { //Main method that keeps the game going (recursive)
  /*
  .5) if player is not out:
  1) add current squares points to the score for P + A, set players positions on board
  2) print gameboard (method), print score below (P + A)
  3) ask for next move from player
  4) check if move is valid (method)
  5) if move is valid:
  6) do move, add turnNum
  7) do A move (highest point value)
  8) recurse
  */
  int spotpV = spotPVert;
  int spotpH = spotPHori;

  if (isOut(board, spotpV, spotpH) == 0 && isOut(board, spotpV, spotpH) == 0) { //if both not out
    playerScore[turnNum] = ((int)board[spotpV][spotpH]) - 48;
    compScore[turnNum] = ((int)board[aSpotv][aSpoth]) - 48;

    board[spotpV][spotpH] = 'P';
    board[aSpotv][aSpoth] = 'A';

    printBoard(board);

    printf("Your score: ");
    printScore(playerScore);
    printf("Their score: ");
    printScore(compScore);

    int nextMovev;
    int nextMoveh;

    int sc;
    do {
      printf("Where to?\n");
      sc = scanf("%d %d", &nextMovev, &nextMoveh);
      nextMovev--;
      nextMoveh--;
      scanf("%*[^\n]");

      if (sc != 2 || isValidMove(board, spotpV, spotpH, nextMovev, nextMoveh) == 0) {
        printf("INVALID MOVE!\n");

      }

    } while(sc != 2 || isValidMove(board, spotpV, spotpH, nextMovev, nextMoveh) == 0);

    board[spotpV][spotpH] = '.';
    spotpV = nextMovev;
    spotpH = nextMoveh;

    aiMove(board, aSpotv, aSpoth, spotpV, spotpH);
    printf("They went to %d,%d\n", aSpotv+1, aSpoth+1);

    turnNum++;
    turn(board, spotpV, spotpH);

  }

  if (isOut(board, spotpV, spotpH) == 0 && isOut(board, aSpotv, aSpoth) == 1) { //if ai out
    playerScore[turnNum] = ((int)board[spotpV][spotpH]) - 48;

    board[spotpV][spotpH] = 'P';
    board[aSpotv][aSpoth] = 'A';

    printBoard(board);

    printf("Your score: ");
    printScore(playerScore);
    printf("Their score: ");
    printScore(compScore);

    int nextMovev;
    int nextMoveh;

    int sc;
    do {
      printf("Where to?\n");
      sc = scanf("%d %d", &nextMovev, &nextMoveh);
      nextMovev--;
      nextMoveh--;
      scanf("%*[^\n]");

      if (sc != 2 || isValidMove(board, spotpV, spotpH, nextMovev, nextMoveh) == 0) {
        printf("INVALID MOVE!\n");
      }

    } while(sc != 2 || isValidMove(board, spotpV, spotpH, nextMovev, nextMoveh) == 0);

    board[spotpV][spotpH] = '.';
    spotpV = nextMovev;
    spotpH = nextMoveh;

    printf("AI OUT!\n");

    turnNum++;
    turn(board, spotpV, spotpH);

  }

  if (isOut(board, spotpV, spotpH) == 1 && isOut(board, aSpotv, aSpoth) == 0) { //if player out
    compScore[turnNum] = ((int)board[aSpotv][aSpoth]) - 48;

    board[spotpV][spotpH] = 'P';
    board[aSpotv][aSpoth] = 'A';

    printBoard(board);

    printf("Your score: ");
    printScore(playerScore);
    printf("Their score: ");
    printScore(compScore);

    aiMove(board, aSpotv, aSpoth, spotpV, spotpH);
    printf("They went to %d,%d\n", aSpotv+1, aSpoth+1);

    turnNum++;
    turn(board, spotpV, spotpH);

  } else {
    printf("GAME OVER!\n");

    printf("Your score: ");
    int scoreP = printScore(playerScore);
    printf("Their score: ");
    int scoreC = printScore(compScore);

    if (scoreP > scoreC) {
      printf("YOU WON!\n");

    } else if (scoreP == scoreC) {
      printf("YOU TIED!\n");

    } else {
      printf("YOU LOST!\n");

    }

    exit(0);
  }

}

int main() { //Only used to start the game off
  char board[6][6] = {
    {'1','1','1','1','1','1'},
    {'1','2','2','2','2','1'},
    {'1','2','3','3','2','1'},
    {'1','2','3','3','2','1'},
    {'1','2','2','2','2','1'},
    {'1','1','1','1','1','1'}
  };

  printBoard(board);

  int playerStartv;
  int playerStarth;

  int sc;
  do {
    printf("Where would you like to start?\n");
    sc = scanf("%d %d", &playerStartv, &playerStarth);
    playerStartv--;
    playerStarth--;
    scanf("%*[^\n]");

    if (sc != 2 || playerStartv > 5 || playerStartv < 0 || playerStarth < 0 || playerStarth > 5 || board[playerStartv][playerStarth] != '1') {
      printf("INVALID SPOT!\n");

    }

  } while(sc != 2 || playerStartv > 5 || playerStartv < 0 || playerStarth < 0 || playerStarth > 5 || board[playerStartv][playerStarth] != '1');

  srand(time(NULL));

  do {
    aSpotv = rand() % 6;
    aSpoth = rand() % 6;

  } while (aSpotv == playerStartv && aSpoth == playerStarth && board[aSpotv][aSpoth] != '1');

  printf("They went to %d,%d\n", aSpotv+1, aSpoth+1);

  turnNum = 0;
  turn(board, playerStartv, playerStarth);

}
