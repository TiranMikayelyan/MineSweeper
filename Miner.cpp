#include "logic.hpp"
#include <random>

// ---------------- Constructor ----------------
Miner::Miner(int row, int col, int mines)
{
    rows = row;
    columns = col;
    minecount = mines;

    lose = false;
    win = false;

    safezone = 0;
    TotalSafeZone = rows * columns - minecount;

    // Ստեղծում ենք 2D array
    board.resize(rows);
    for (int i = 0; i < rows; i++) {
        board[i].resize(columns);
    }
}

// ---------------- Border check ----------------
bool Miner::Border(int row, int column) {
    return (row >= 0 && row < rows && column >= 0 && column < columns);
}

// ---------------- Count neighbor mines ----------------
int Miner::NeighborMines(int row, int column) {
    int count = 0;

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0)
                continue;

            int nearrow = row + i; //մեր կանգնած դիրքին + շեղումը //Սա հաշվում է հարևան բջջի տողի ինդեքսը։
            int nearcol = column + j; //մեր կանգնած դիրքին + շեղումը// Սա հաշվում է հարևան բջջի սյան ինդեքսը։

            if (Border(nearrow, nearcol) && board[nearrow][nearcol].Mine) // սահմանում է , ու մին կա թե չէ , եթե հա ++
                count++;
        }
    }
    return count;
}

// ---------------- Computing numbers ----------------
void Miner::computeNums() {   //յուրաքանչյուր բջջի համար հաշվել ու պահել, քանի ական կա նրա շուրջը
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if (board[i][j].Mine) //եթե բջիջը ական է , ուրեմն հարևան հաշվելու միտք չկա
                board[i][j].NearMines = 0; 
            else
                board[i][j].NearMines = NeighborMines(i, j); //եթե ական չի , ուրեմն կամչում ենք նախորդ ֆունկցիան , ու հաշվում կողի ականնեերի քանակը
        }
    }
}

// ---------------- Mines Place ----------------
void Miner::MinePlace(int row, int column) {  //row և column պարամետրերը  առաջին սեղմված բջիջներն են
    std::random_device random; //Սա ստեղծում է պատահականության աղբյուր (seed)՝ համակարգից։
    std::mt19937 gen(random()); // Սա պատահական թվերի գեներատորն է։ Այն օգտագործումա վերևի random - ը որպես seed։

    std::uniform_int_distribution<int> rownum(0, rows - 1); //Սա սահմանում է, որ գեներացվող տողի ինդեքսը լինի 0-ից մինչև row-1
    std::uniform_int_distribution<int> colnum(0, columns - 1);//նույն վերևինը սյունով

    int placed = 0; //0 տեղադրված ական
    while (placed < minecount) { // էս աշխատումա էնքան ժամանակ,մինչև տեղադրենք minecount հատ ական։
        int r = rownum(gen); //պատահական տող
        int c = colnum(gen);//սյուն

        // 1-ին քայլից չպարտվելու համար
        if ((r == row && c == column) || board[r][c].Mine)
            continue;

        board[r][c].Mine = true;
        placed++;
    }
}

//// ---------------- Board's Output (console debug) ----------------
//void Miner::printBoard(bool revealAll) {   //revealAll == True ուրեմն սաղ ցույց կտա
//    std::cout << "\n   ";
//    for (int i = 0; i < columns; i++)    //տողերնա համարակալում
//        std::cout << i << " ";
//    std::cout << std::endl;
//
//    for (int j = 0; j < rows; j++) {
//        std::cout << j << " |";    //սյուները
//        for (int k = 0; k < columns; k++) {
//            const Cell& cell = board[j][k]; //տվյալ բջիջը պահում cell -ի մեջ , մեր ստրակտի տիպնա, ու դրա համար սկսում ստուգումներ
//
//            if (revealAll) {   //աշխատում է անկախ բջջի վիճակից
//                if (cell.Mine)               std::cout << "* ";  
//                else if (cell.NearMines > 0) std::cout << cell.NearMines << " ";
//                else                          std::cout << ". ";
//                continue;
//            }
//
//            if (!cell.Revealed) {
//                if (cell.Flagged)             std::cout << "F ";
//                else                          std::cout << "# ";
//            }
//            else {   // կախված է բջջի վիճակից
//                if (cell.Mine)               std::cout << "* ";
//                else if (cell.NearMines > 0) std::cout << cell.NearMines << " ";
//                else                          std::cout << ". ";
//            }
//        }
//        std::cout << std::endl;
//    }
//}

// ---------------- Flags ----------------
void Miner::Flag(int row, int col) //ստանումա դիրքը
{
    if (!Border(row, col)) //սահմանից դուրսա ուրեմն դուրսա գալիս
        return;

    Cell& cell = board[row][col];

    if (cell.Revealed)
        return;

    cell.Flagged = !cell.Flagged;  //եթե true կդառնա false, կամ հակառակը
}

//հաղթանակի ստուգում
void Miner::CheckWin() {
    if (safezone == TotalSafeZone) {
        win = true;
    }
}

// ---------------- Opening (Reveal) ----------------
void Miner::Reveal(int row, int col) { //ստանումա ընտրված տողն ու սյունը
    if (!Border(row, col)) //ստուգումա սահմանումա թե չէ
        return;

    Cell& cell = board[row][col]; //վերցնումա բացվածը

    if (cell.Revealed || cell.Flagged) //եթե արդեն բացա կամ դրոշով բան չանի
        return;

    // Եթե մին է
    if (cell.Mine) {
        cell.Revealed = true;  //եթե մինա Lose դառնա true 
        lose = true;
        return;
    }

    // Բացված
    cell.Revealed = true; //եթե ուղղակի բացա , score ++
    safezone++;

    // Եթե կողքին միներ կան
    if (cell.NearMines > 0) { //եթե կողը միներ կան, նավյսակի ստուգում ենք կարողա հաղթել ենք
        CheckWin();
        return;
    }

    // դատարկ մասերի ավտոմատ բացում
    for (int dr = -1; dr <= 1; ++dr) {
        for (int dc = -1; dc <= 1; ++dc) {  //սաղ կոորդինատների վրայով անցնումա
            if (dr == 0 && dc == 0) continue; //մեր դիրքի վրայով ուղղակի անցնենք
            Reveal(row + dr, col + dc); //ռեկուրսիայա անում , ու հարևան ազատ բջիջները բացումա
        }
    }

    CheckWin(); //ստուգումա հաղթանակը
}