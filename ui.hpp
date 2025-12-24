#pragma once
#include <SFML/Graphics.hpp>
#include "logic.hpp"

//  COLORS   
const sf::Color BG_COLOR(30, 30, 40);
const sf::Color CELL_UNREVEALED(80, 100, 120);
const sf::Color CELL_REVEALED(200, 200, 210);
const sf::Color CELL_HOVER(100, 120, 140);
const sf::Color HEADER_BG(40, 40, 50);
const sf::Color BORDER_COLOR(20, 20, 30);
const sf::Color BUTTON_COLOR(70, 130, 180);
const sf::Color BUTTON_HOVER(100, 160, 210);

// Number colors
const sf::Color NUM_COLORS[9] = { //0-9 թվերի գույնն է
    sf::Color::White,        // 0
    sf::Color(0, 0, 255),    // 1 - Կապույտ
    sf::Color(0, 128, 0),    // 2 - Կանաչ
    sf::Color(255, 0, 0),    // 3 - Կարմիր
    sf::Color(0, 0, 128),    // 4 - Մուգ կապույտ
    sf::Color(128, 0, 0),    // 5 - Մուգ կարմիր
    sf::Color(0, 128, 128),  // 6 - Ծովային
    sf::Color(0, 0, 0),      // 7 - Սև
    sf::Color(128, 128, 128) // 8 - Գորշ
};

//  GAME STATE 
enum class GameState {  //խաղի վիճակները, մենյուի մեջ ես, թե խաղում ես թե խաղը պրծավ
    MENU,  
    PLAYING,
    GAME_OVER
};

//  DIFFICULTY 
struct Difficulty {   //դժվարության տեսակը, պահումա մի հատ
    std::string name;
    int rows;
    int cols;
    int mines;
};

//  UI CLASS 
class GameUI {
private:
    // Game state
    GameState state; //Պահում է՝ հիմա որ վիճակում ենք (Menu/Playing/GameOver)։

    // Game logic
    std::unique_ptr<Miner> game; //unique_ptr - ով նշանակում է՝ GameUI - ն է տերը, ու memory leak չի լինի։
    int rows;  //Ընտրված դժվարության պարամետրերը պահելու համար։
    int cols; //--//--
    int mines;//--//--
    bool firstClick; // Օգտագործվում է, որ ականները տեղադրվեն միայն առաջին սեղմումից հետո

    // SFML objects
    sf::RenderWindow window; //Պատուհանը, որտեղ խաղը նկարելու ես։
    sf::Font font;               //Տառատեսակը, որ UI-ում թվեր/գրվածքներ ցույց տաս։
    sf::View gameView;     //Տեսադաշտը (camera/view)՝ որ ճիշտ մասշտաբով/տեղակայմամբ նկարես։

    // Hover state
    int hoverRow;   //Պահում է՝ մկնիկը հիմա որ բջջի վրա է կանգնած, որ hover էֆֆեկտ անես։ 
    int hoverCol;

    // Window dimensions
    int windowWidth;   //Պատուհանի չափերը։
    int windowHeight;//--//--
    int cellSize; // բջջի չափը
    int headerHeight;// վերևի տողի բարձրությունը 
    int padding; //դիզայնի բացակները


    // Menu
    std::vector<Difficulty> difficulties;  //Բոլոր դժվարությունները պահվում են վեկտորի մեջ (Easy/Medium/Hard)։
    int selectedDifficulty; //Որը ընտրված է մկնիկով։
    int hoveredButton;       //Որ կոճակի վրա է մկնիկը (hover effect-ի համար)։

    // Helper functions
    void calculateDimensions(); //Հաշվում է պատուհանի/բջիջների չափերը, որ ամեն difficulty-ի դեպքում ճիշտ տեսք ունենա։

    // Helper drawing functions
    void drawBomb(float x, float y, float size); //բոմբ նկարել
    void drawFlag(float x, float y, float size); //դրոշ նկարել
    void drawSmiley(float x, float y, float size, bool isHappy, bool isLost); //էմոջին նկարել

    // Game functions
    void handleEvents(); //վերցնումա input-ը՝ mouse click, keyboard
    void render(); //Նկարելու հիմնական ֆունկցիան՝ ամեն frame։
    void resetGame(); //Նոր խաղ սկսելու/վերադառնալու համար։

    // Draw sections
    void drawMenu(); //Մենյուի էկրանի նկարումը։
    void drawHeader(); // վերևի տողի միավոր, էմոջի , ու տենց բաները
    void drawBoard(); //լրիվ խաղատախտակը
    void drawGameOverScreen(); //խաղի ավարտը կրել կրվելուց ինչ կա

    // Button helper
    bool isMouseOverButton(int mouseX, int mouseY, float btnX, float btnY, float btnW, float btnH); //Ստուգում է՝ մկնիկը տվյալ կոճակի վրայա թե չէ

public:
    GameUI(); //կոնստրուկտորը
    void run(); //միշտ աշխատող ցիկլ՝ events + render
    void startGame(int difficultyIndex); //Սկսում է խաղը ըստ բարդության
};