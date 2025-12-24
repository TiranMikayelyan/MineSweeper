#pragma once

struct Cell {
    bool Mine;       // Միներ
    bool Revealed;   // բացված է
    bool Flagged;    // դրոշով նշված
    int  NearMines;  // մոտակա միների քանակը

    Cell() {
        Mine = false;       // Մին չկա սկզբում
        Revealed = false;   // բացված դաշտ չկա
        Flagged = false;    // դրոշմված չկա
        NearMines = 0;      // դեռ բացվածի կողքին ական չկա
    }
};
