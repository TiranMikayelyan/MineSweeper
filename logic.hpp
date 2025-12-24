#pragma once
#include "Cell.hpp"
#include <vector>
#include <iostream>

class Miner {
private:
    int rows;          // տողերի քանակը
    int columns;       // սյուների քանակը
    bool lose;         // պարտված չէ սկզբում
    bool win;          // հաղթած չէ սկզբում
    int minecount;     // միների քանակը

    int TotalSafeZone; // ընդհանուր անվտանգ բջիջների թիվը = rows*columns - minecount
    int safezone;      // քանի անվտանգ բջիջ է բացվել (սա կլինի score)

    std::vector<std::vector<Cell>> board; // դաշտը (2D վեկտոր) vector -tox vector<cell> syuner board[r][c]

    void CheckWin();   // հաղթանակի ստուգում (եթե բոլոր անվտանգները բացված են)

public:
    Miner(int row, int col, int mines);

    bool Border(int row, int column);             // ստուգում ենք, սահմանը չանցնենք
    int  NeighborMines(int row, int column);      // հարևան միների քանակը
    void computeNums();                            // NearMines-ը հաշվելու համար
    void MinePlace(int row, int column);           // random միները գեներացնելու համար
    void Reveal(int row, int col);                 // դաշտը բացելու համար
    //void printBoard(bool revealAll = false);       // կոնսոլով դաշտը արտածելու համար (debug)
    void Flag(int row, int col);                   // դրոշ դնել/հանել

    bool Lose() const { return lose; }
    bool Win()  const { return win; }

    int Score() const { return safezone; }         // միավորները = քանի անվտանգ բջիջ բացվեց

    // FOR UI (SFML-ը պիտի կարողանա տեսնել board-ի վիճակը)
    const Cell& GetCell(int r, int c) const { return board[r][c]; } // ֆունկցիան վերադարձնում է խաղադաշտի կոնկրետ բջիջը՝ տրված տողի և սյան համարով։ & որ հղմամբ գա ոչ թե քոփի լինի
    int GetRows() const { return rows; } //վերադարձնում է խաղադաշտի տողերի քանակը։ UI-ն օգտագործում է այս արժեքը՝ ճիշտ բարձրությամբ խաղադաշտ նկարելու համար։
    int GetColumns() const { return columns; } //վերադարձնում է խաղադաշտի սյուների քանակը։ UI-ն օգտագործում է այս արժեքը՝ ճիշտ լայնությունը խաղադաշտ նկարելու համար։

};
