#include "stdio.h"
#include "stdlib.h"
#include "string.h"

/*
int board[25] = {
  :,:,:,:,:,
  :,O,-,X,:,
  :,X,-,-,:,
  :,-,-,-,:,
  :,:,:,:,:,

}
*/

//Declaration in less messy enumarator
enum { NOUGHTS, CROSSES, BORDER, EMPTY };
enum { HUMANWIN, COMPWIN, DRAW };

const int Directions[4] = { 1, 5, 4, 6};

//Array representing the board in a 25 squares with
//Borders invisible on the run
const int ConvertTo25[9] = {
    6,7,8,
    11,12,13,
    16,17,18
};

//Gets the difference between the numbers at certain directions
int GetNumForDir(int startSq, const int dir, const int *board, const int us)
{
    int found = 0;
    while(board[startSq] != BORDER)
    {
        if (board[startSq] != us) {
            break;
        }
        found ++;
        startSq += dir;
    }
    return found;

}

int FindThreeInARow(const int *board, const int ourindex, const int us)
{
    int DirIndex = 0;
    int Dir = 0;
    int threeCount = 1;

    for(DirIndex = 0; DirIndex < 4; ++DirIndex)
    {
        Dir = Directions[DirIndex];
        threeCount += GetNumForDir(ourindex + Dir, Dir, board, us);
        threeCount += GetNumForDir(ourindex + Dir * -1, Dir * -1, board, us);
        if(threeCount == 3)
        {
            break;
        }
        threeCount = 1;
    }
    return threeCount;
}

//Initialises the game board
void InitialiseBoard(int *board)
{
    int index = 0;

    for(index = 0; index < 25 ; ++index)
    {
        board[index] = BORDER;
    }

    for(index = 0; index < 9; ++index)
    {
        board[ConvertTo25[index]] = EMPTY;
    }

}

//Printing the running board
void PrintBoard(const int *board)
{
    int index = 0;
    char pceChairs[] = "OX|-";
    printf("\nBoard:\n\n");
    for(index = 0; index < 9; ++index)
    {
        if(index !=0 && index%3==0)
        {
            printf("\n\n");
        }
        printf("%4c",pceChairs[board[ConvertTo25[index]]]);
    }
    printf("\n");
}

//Checks if there are any empty squares on the board (used for Draw result)
int HasEmpty(const int *board)
{
    int index = 0;
    for(index = 0; index < 9; ++index)
    {
        if(board[ConvertTo25[index]] == EMPTY) return 1;
    }
    return 0;
}

//Returns the move made
void MakeMove(int *board, const int sq, const side)
{
    board[sq] = side;
}

int GetWinningMove(int *board, const int side)
{
    int ourMove = -1;
    int winFound = 0;
    int index = 0;

    for(index = 0; index < 9; ++index)
    {
        if(board[ConvertTo25[index]] == EMPTY)
        {
            ourMove = ConvertTo25[index];
            board[ourMove] = side;

            if(FindThreeInARow(board, ourMove, side) == 3)
            {
                winFound = 1;
            }
            board[ourMove] = EMPTY;
            if(winFound == 1)
            {
                break;
            }
            ourMove = -1;
        };
    }
    return ourMove;
}

//Returns an integer randomly depending on the positions available
//GetWinningMove function is used here for making computers decion making better
int GetComputerMove(int *board, const int side) {
    int index = 0;
    int numFree = 0;
    int availableMoves[9];
    int randMove = 0;

    randMove = GetWinningMove(board, side);
    if(randMove != -1)
    {
        return randMove;
    }

    randMove = 0;
    for(index = 0; index < 9; ++index) {
        if( board[ConvertTo25[index]] == EMPTY)
        {
            availableMoves[numFree++] = ConvertTo25[index];
        };
    }

    randMove = (rand() % numFree);
    return availableMoves[randMove];
}

//Returns an the integer index number of internal board (used for PLayers chosen Move)
int GetHumanMove(const int *board, int mode, int p)
{
    char userInput[4];

    int moveOk = 0;
    int move = -1;

    while (moveOk == 0)
    {
        if(mode==1)
            printf("Please enter a move from 1 to 9: ");
        else
            printf("(Player %d) Please enter a move from 1 to 9: ", p);

        fgets(userInput, 3, stdin);
        fflush(stdin);

        if(strlen(userInput) != 2)
        {
            printf("Invalid strlen()\n");
            continue;
        }

        if ( sscanf(userInput, "%d", &move) != 1)
        {
            move = -1;
            printf("Invalid sscanf()\n");
            continue;
        }

        if ( move < 1 || move > 9)
        {
            move = -1;
            printf("Ivalid range\n");
            continue;
        }

        move--; // zero Indexing

        if( board[ConvertTo25[move]] !=EMPTY)
        {
            move=-1;
            printf("Square not available\n");
            continue;
        }
        moveOk = 1;
    }
    printf("Making Move...%d\n", (move+1));
    return ConvertTo25[move];
}

//Function whith logical and iterative statements which
//interacts with the running process of the game and its rules
void RunGame()
{
    int GameOver = 0;
    int Side = NOUGHTS;
    int LastMoveMade = 0;
    int board[25];

    char gameModeInput[4];
    int modeChoice = -1;
    int mode = 2;

    printf("Enter 1 for AI opponent or 2 for two human players:");
    fgets(gameModeInput, 3, stdin);

    if(strlen(gameModeInput) != 2 || sscanf(gameModeInput, "%d", &modeChoice) != 1)
    {
        modeChoice = -1;
    }

    if (modeChoice == 1)
    {
        mode = 1;
    }

    InitialiseBoard(&board[0]);
    PrintBoard(&board[0]);

    while(!GameOver)
    {
        if(Side==NOUGHTS) {
            LastMoveMade = GetHumanMove(&board[0], mode, 1);
            MakeMove(&board[0], LastMoveMade,NOUGHTS);
            Side = CROSSES;
            if(mode==2)
                PrintBoard(&board[0]);
        } else {
            if(mode==1)
            {
                LastMoveMade = GetComputerMove(&board[0], Side);
                MakeMove(&board[0],LastMoveMade,Side);
            }

            else
            {
                LastMoveMade = GetHumanMove(&board[0], mode, 2);
                MakeMove(&board[0], LastMoveMade,CROSSES);
            }

            Side=NOUGHTS;
            PrintBoard(&board[0]);
        }

        //if three in a row exists Game is Over
        if(FindThreeInARow(board, LastMoveMade, Side ^ 1) == 3)
        {
            printf("Game Over!\n");
            GameOver = 1;
            if(Side==NOUGHTS)
            {
                if(mode==1)
                    printf("Computer Wins\n");
                else
                    printf("Player 2 Wins\n");
                break;
            }
            else
            {
                if(mode==1)
                    printf("Human Wins\n");
                else
                    printf("Player 1 Wins\n");
                break;
            }
        }

        //if no more moves, game is drawc
        if(!HasEmpty(board))
        {
            printf("Game Over!\n");
            GameOver = 1;
            printf("Its a draw\n");
        }
    }

    PrintBoard(&board[0]);
}

int main()
{

    srand(time(NULL));
    RunGame();

    return 0;
}
