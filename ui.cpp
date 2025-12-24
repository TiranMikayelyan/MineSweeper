#include "ui.hpp"

//  CONSTRUCTOR
GameUI::GameUI()
    : state(GameState::MENU), firstClick(true), hoverRow(-1), hoverCol(-1),
    selectedDifficulty(1), hoveredButton(-1)
{
    // Setup difficulties
    difficulties = {
        {"Easy", 8, 8, 10},
        {"Medium", 16, 16, 40},
        {"Hard", 25, 25, 99}
    };

    // Create window
    windowWidth = 800;
    windowHeight = 600;
    window.create(sf::VideoMode(windowWidth, windowHeight),
        "Minesweeper - SFML",
        sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);
    window.setFramerateLimit(60);

    // Setup view
    gameView = window.getDefaultView();

    // Load font
    if (!font.loadFromFile("/System/Library/Fonts/Supplemental/Arial.ttf")) {
        if (!font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
            font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");
        }
    }
}


//  CALCULATE DIMENSIONS 
void GameUI::calculateDimensions() {
    headerHeight = 80;
    padding = 20;

    // Հաշվել առավելագույն հասանելի տարածությունը
    int availableWidth = windowWidth - 2 * padding;
    int availableHeight = windowHeight - headerHeight - 2 * padding;

    // Հաշվել cell size-ը
    int cellSizeByWidth = availableWidth / cols;
    int cellSizeByHeight = availableHeight / rows;

    cellSize = std::min(cellSizeByWidth, cellSizeByHeight);
    cellSize = std::max(15, std::min(cellSize, 80));
}

//  DRAWING HELPERS 

void GameUI::drawBomb(float x, float y, float size) {
    // Bomb body
    sf::CircleShape body(size * 0.35f);
    body.setFillColor(sf::Color::Black);
    body.setPosition(x + size * 0.15f, y + size * 0.15f);
    window.draw(body);

    // Highlight
    sf::CircleShape highlight(size * 0.1f);
    highlight.setFillColor(sf::Color(255, 255, 255, 180));
    highlight.setPosition(x + size * 0.25f, y + size * 0.25f);
    window.draw(highlight);

    // Fuse
    sf::RectangleShape fuse(sf::Vector2f(size * 0.08f, size * 0.25f));
    fuse.setFillColor(sf::Color(139, 69, 19));
    fuse.setPosition(x + size * 0.46f, y + size * 0.05f);
    window.draw(fuse);

    // Spark
    sf::CircleShape spark(size * 0.08f);
    spark.setFillColor(sf::Color(255, 200, 0));
    spark.setPosition(x + size * 0.44f, y);
    window.draw(spark);
}

void GameUI::drawFlag(float x, float y, float size) {
    // Pole
    sf::RectangleShape pole(sf::Vector2f(size * 0.08f, size * 0.6f));
    pole.setFillColor(sf::Color(101, 67, 33));
    pole.setPosition(x + size * 0.3f, y + size * 0.2f);
    window.draw(pole);

    // Flag
    sf::ConvexShape flag(3);
    flag.setPoint(0, sf::Vector2f(x + size * 0.38f, y + size * 0.2f));
    flag.setPoint(1, sf::Vector2f(x + size * 0.75f, y + size * 0.35f));
    flag.setPoint(2, sf::Vector2f(x + size * 0.38f, y + size * 0.5f));
    flag.setFillColor(sf::Color(220, 20, 60));
    window.draw(flag);
}

void GameUI::drawSmiley(float x, float y, float size, bool isHappy, bool isLost) {
    // Face
    sf::CircleShape face(size);
    face.setFillColor(sf::Color(255, 220, 0));
    face.setPosition(x, y);
    face.setOutlineThickness(3);
    face.setOutlineColor(sf::Color(200, 170, 0));
    window.draw(face);

    // Eyes
    sf::CircleShape eye1(size * 0.15f);
    sf::CircleShape eye2(size * 0.15f);
    eye1.setFillColor(sf::Color::Black);
    eye2.setFillColor(sf::Color::Black);

    if (isLost) {
        eye1.setPosition(x + size * 0.4f, y + size * 0.5f);
        eye2.setPosition(x + size * 1.2f, y + size * 0.5f);
    }
    else {
        eye1.setPosition(x + size * 0.5f, y + size * 0.6f);
        eye2.setPosition(x + size * 1.1f, y + size * 0.6f);
    }
    window.draw(eye1);
    window.draw(eye2);

    // Mouth (սկզբի ձևը)
    sf::CircleShape mouth(size * 0.5f);

        mouth.setRadius(size * 0.3f);
        mouth.setPosition(x + size * 0.7f, y + size * 1.2f);
        mouth.setOutlineThickness(2);
        mouth.setOutlineColor(sf::Color::Black);
        mouth.setFillColor(sf::Color::Transparent);

    window.draw(mouth);
}

//  BUTTON HELPER  //mkniki tiruytna sahmanum
bool GameUI::isMouseOverButton(int mouseX, int mouseY, float btnX, float btnY, float btnW, float btnH) {
    return mouseX >= btnX && mouseX <= btnX + btnW &&
        mouseY >= btnY && mouseY <= btnY + btnH;
}

//  START GAME 
void GameUI::startGame(int difficultyIndex) {
    selectedDifficulty = difficultyIndex;
    rows = difficulties[difficultyIndex].rows;
    cols = difficulties[difficultyIndex].cols;
    mines = difficulties[difficultyIndex].mines;

    calculateDimensions();

    game = std::make_unique<Miner>(rows, cols, mines);
    firstClick = true;
    state = GameState::PLAYING;
}

//  RESET GAME 
void GameUI::resetGame() {
    game = std::make_unique<Miner>(rows, cols, mines);
    firstClick = true;
    state = GameState::PLAYING;
}

//  EVENT HANDLING 
void GameUI::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        // Window resize
        if (event.type == sf::Event::Resized) {
            windowWidth = event.size.width;
            windowHeight = event.size.height;

            // Update view - մեջտեղում մնալու համար
            gameView.setSize(windowWidth, windowHeight);
            gameView.setCenter(windowWidth / 2.0f, windowHeight / 2.0f);
            window.setView(gameView);

            if (state == GameState::PLAYING) {
                calculateDimensions();
            }
        }

        // Mouse click
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            int mouseX = event.mouseButton.x;
            int mouseY = event.mouseButton.y;

            // MENU STATE
            if (state == GameState::MENU) {
                // Check difficulty buttons
                for (int i = 0; i < difficulties.size(); i++) {
                    float btnW = 300;
                    float btnH = 70;
                    float btnX = windowWidth / 2 - btnW / 2;
                    float btnY = 220 + i * 100;

                    if (isMouseOverButton(mouseX, mouseY, btnX, btnY, btnW, btnH)) {
                        startGame(i);
                        break;
                    }
                }
            }

            // PLAYING STATE
            else if (state == GameState::PLAYING) {
                // Calculate board offset for centering
                int boardWidth = cols * cellSize;
                int boardHeight = rows * cellSize;
                int boardX = (windowWidth - boardWidth) / 2;
                int boardY = (windowHeight - boardHeight) / 2 + headerHeight / 2;

                // Check board click
                if (mouseY > boardY) {
                    int col = (mouseX - boardX) / cellSize;
                    int row = (mouseY - boardY) / cellSize;

                    if (row >= 0 && row < rows && col >= 0 && col < cols) {
                        if (!game->Win() && !game->Lose()) {
                            if (firstClick) {
                                game->MinePlace(row, col);
                                game->computeNums();
                                firstClick = false;
                            }
                            game->Reveal(row, col);

                            
                        }
                    }
                }

                // Check smiley click
                float smileyX = windowWidth / 2 - 20;
                float smileyY = boardY - 60;
                float smileySize = 20;
                float dist = std::sqrt(std::pow(mouseX - (smileyX + smileySize), 2) +
                    std::pow(mouseY - (smileyY + smileySize), 2));
                if (dist < smileySize * 1.5f) {
                    resetGame();
                }

                // Check back to menu button (երբ խաղը վերջացել է)
                if (game->Win() || game->Lose()) {
                    float menuBtnW = 200;
                    float menuBtnH = 50;
                    float menuBtnX = windowWidth / 2 - menuBtnW / 2;
                    float menuBtnY = windowHeight - 120;

                    if (isMouseOverButton(mouseX, mouseY, menuBtnX, menuBtnY, menuBtnW, menuBtnH)) {
                        state = GameState::MENU;
                    }
                }
            }
        }

        // Right click for flag
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
            if (state == GameState::PLAYING && !firstClick) {
                int mouseX = event.mouseButton.x;
                int mouseY = event.mouseButton.y;

                int boardWidth = cols * cellSize;
                int boardHeight = rows * cellSize;
                int boardX = (windowWidth - boardWidth) / 2;
                int boardY = (windowHeight - boardHeight) / 2 + headerHeight / 2;

                if (mouseY > boardY) {
                    int col = (mouseX - boardX) / cellSize;
                    int row = (mouseY - boardY) / cellSize;

                    if (row >= 0 && row < rows && col >= 0 && col < cols) {
                        if (!game->Win() && !game->Lose()) {
                            game->Flag(row, col);
                        }
                    }
                }
            }
        }

        // Mouse move
        if (event.type == sf::Event::MouseMoved) {
            int mouseX = event.mouseMove.x;
            int mouseY = event.mouseMove.y;

            if (state == GameState::MENU) {
                hoveredButton = -1;
                for (int i = 0; i < difficulties.size(); i++) {
                    float btnW = 300;
                    float btnH = 70;
                    float btnX = windowWidth / 2 - btnW / 2;
                    float btnY = 220 + i * 100;

                    if (isMouseOverButton(mouseX, mouseY, btnX, btnY, btnW, btnH)) {
                        hoveredButton = i;
                        break;
                    }
                }
            }
            else if (state == GameState::PLAYING) {
                int boardWidth = cols * cellSize;
                int boardHeight = rows * cellSize;
                int boardX = (windowWidth - boardWidth) / 2;
                int boardY = (windowHeight - boardHeight) / 2 + headerHeight / 2;

                if (mouseY > boardY) {
                    hoverCol = (mouseX - boardX) / cellSize;
                    hoverRow = (mouseY - boardY) / cellSize;

                    if (hoverRow < 0 || hoverRow >= rows || hoverCol < 0 || hoverCol >= cols) {
                        hoverRow = -1;
                        hoverCol = -1;
                    }
                }
                else {
                    hoverRow = -1;
                    hoverCol = -1;
                }
            }
        }
    }
}

//  DRAW MENU 
void GameUI::drawMenu() {
    // Title
    sf::Text title;
    title.setFont(font);
    title.setString("MINESWEEPER");
    title.setCharacterSize(60);
    title.setFillColor(sf::Color::White);
    title.setStyle(sf::Text::Bold);

    sf::FloatRect bounds = title.getLocalBounds();
    title.setPosition(windowWidth / 2 - bounds.width / 2, 60);
    window.draw(title);

    

    // Difficulty buttons
    for (int i = 0; i < difficulties.size(); i++) {
        float btnW = 300;
        float btnH = 70;
        float btnX = windowWidth / 2 - btnW / 2;
        float btnY = 220 + i * 100;

        // Button background
        sf::RectangleShape btn(sf::Vector2f(btnW, btnH));
        btn.setPosition(btnX, btnY);
        btn.setFillColor(hoveredButton == i ? BUTTON_HOVER : BUTTON_COLOR);
        btn.setOutlineThickness(3);
        btn.setOutlineColor(sf::Color::White);
        window.draw(btn);

        // Difficulty name
        sf::Text diffName;
        diffName.setFont(font);
        diffName.setString(difficulties[i].name);
        diffName.setCharacterSize(28);
        diffName.setFillColor(sf::Color::White);
        diffName.setStyle(sf::Text::Bold);

        bounds = diffName.getLocalBounds();
        diffName.setPosition(btnX + btnW / 2 - bounds.width / 2, btnY + 8);
        window.draw(diffName);

        // Info text
        sf::Text info;
        info.setFont(font);
        std::string infoStr = std::to_string(difficulties[i].rows) + "x" +
            std::to_string(difficulties[i].cols)  + " cells " +" " +
            std::to_string(difficulties[i].mines) + " mines";
        info.setString(infoStr);
        info.setCharacterSize(16);
        info.setFillColor(sf::Color(220, 220, 220));

        bounds = info.getLocalBounds();
        info.setPosition(btnX + btnW / 2 - bounds.width / 2, btnY + 42);
        window.draw(info);

    }

}

//  DRAW HEADER 
void GameUI::drawHeader() {
    int boardWidth = cols * cellSize;
    int boardX = (windowWidth - boardWidth) / 2;
    int boardY = (windowHeight - rows * cellSize) / 2 + headerHeight / 2;

    // Mine counter - ՁԱԽԻՑ
    sf::Text mineLabel;
    mineLabel.setFont(font);
    mineLabel.setString("Mines:");
    mineLabel.setCharacterSize(20);
    mineLabel.setFillColor(sf::Color(200, 200, 200));
    mineLabel.setPosition(boardX, boardY - 60);
    window.draw(mineLabel);

    sf::Text mineCount;
    mineCount.setFont(font);
    mineCount.setString(std::to_string(mines));
    mineCount.setCharacterSize(24);
    mineCount.setFillColor(sf::Color::White);
    mineCount.setStyle(sf::Text::Bold);
    mineCount.setPosition(boardX + 75, boardY - 62);
    window.draw(mineCount);

    // Smiley - մեջտեղում
    drawSmiley(windowWidth / 2 - 20, boardY - 60, 20,
        game->Win(), game->Lose());

    // Score - աջ կողմում
    sf::Text scoreLabel;
    scoreLabel.setFont(font);
    scoreLabel.setString("Score:");
    scoreLabel.setCharacterSize(20);
    scoreLabel.setFillColor(sf::Color(200, 200, 200));

    sf::FloatRect bounds = scoreLabel.getLocalBounds();
    scoreLabel.setPosition(boardX + boardWidth - bounds.width - 70, boardY - 60);
    window.draw(scoreLabel);

    sf::Text scoreValue;
    scoreValue.setFont(font);
    scoreValue.setString(std::to_string(game->Score()));
    scoreValue.setCharacterSize(24);
    scoreValue.setFillColor(sf::Color::White);
    scoreValue.setStyle(sf::Text::Bold);

    bounds = scoreValue.getLocalBounds();
    scoreValue.setPosition(boardX + boardWidth - bounds.width, boardY - 62);
    window.draw(scoreValue);
}

//  DRAW BOARD 
void GameUI::drawBoard() {
    int boardWidth = cols * cellSize;
    int boardHeight = rows * cellSize;
    int boardX = (windowWidth - boardWidth) / 2;
    int boardY = (windowHeight - boardHeight) / 2 + headerHeight / 2;

    // Եթե պարտվել ենք, բացել բոլոր միները
    bool revealMines = game->Lose();

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            const Cell& cell = game->GetCell(r, c);

            float x = boardX + c * cellSize;
            float y = boardY + r * cellSize;

            // Cell background
            sf::RectangleShape cellRect(sf::Vector2f(cellSize - 2, cellSize - 2));
            cellRect.setPosition(x + 1, y + 1);

            // Եթե պարտվել ենք և էս մին է, կարմրով ներկենք
            if (revealMines && cell.Mine) {
                cellRect.setFillColor(sf::Color(150, 50, 50));
            }
            else if (cell.Revealed) {
                cellRect.setFillColor(CELL_REVEALED);
            }
            else {
                if (r == hoverRow && c == hoverCol && !game->Win() && !game->Lose()) {
                    cellRect.setFillColor(CELL_HOVER);
                }
                else {
                    cellRect.setFillColor(CELL_UNREVEALED);
                }
            }

            // 3D effect
            if (!cell.Revealed && !(revealMines && cell.Mine)) {
                sf::RectangleShape highlight(sf::Vector2f(cellSize - 4, 3));
                highlight.setPosition(x + 2, y + 2);
                highlight.setFillColor(sf::Color(255, 255, 255, 40));
                window.draw(cellRect);
                window.draw(highlight);

                highlight.setSize(sf::Vector2f(3, cellSize - 4));
                window.draw(highlight);
            }
            else {
                window.draw(cellRect);
            }

            // Cell content
            if (cell.Revealed || (revealMines && cell.Mine)) {
                if (cell.Mine) {
                    drawBomb(x, y, cellSize);
                }
                else if (cell.NearMines > 0) {
                    sf::Text num;
                    num.setFont(font);
                    num.setString(std::to_string(cell.NearMines));
                    num.setCharacterSize(std::min(24, cellSize / 2));
                    num.setFillColor(NUM_COLORS[cell.NearMines]);
                    num.setStyle(sf::Text::Bold);

                    sf::FloatRect bounds = num.getLocalBounds();
                    num.setPosition(x + (cellSize - bounds.width) / 2 - bounds.left,
                        y + (cellSize - bounds.height) / 2 - bounds.top);
                    window.draw(num);
                }
            }
            else if (cell.Flagged) {
                drawFlag(x, y, cellSize);
            }
        }
    }
}

//  DRAW GAME OVER 
void GameUI::drawGameOverScreen() {
    if (!game->Win() && !game->Lose()) return;

    // Overlay
    sf::RectangleShape overlay(sf::Vector2f(windowWidth, windowHeight));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    window.draw(overlay);

    // Message
    sf::Text message;
    message.setFont(font);
    if (game->Win()) {
        message.setString("Congratulations!\n\tYou Won!");
        message.setFillColor(sf::Color(0, 255, 0));
    }
    else {
        message.setString(" Game Over! \n\tYou Lost!");
        message.setFillColor(sf::Color(255, 50, 50));
    }
    message.setCharacterSize(40);
    message.setStyle(sf::Text::Bold);

    sf::FloatRect bounds = message.getLocalBounds();
    message.setPosition(windowWidth / 2 - bounds.width / 2,
        windowHeight / 2 - 120);
    window.draw(message);

    // Score label
    sf::Text scoreLabel;
    scoreLabel.setFont(font);
    scoreLabel.setString("Your Score:");
    scoreLabel.setCharacterSize(20);
    scoreLabel.setFillColor(sf::Color(180, 180, 180));

    bounds = scoreLabel.getLocalBounds();
    scoreLabel.setPosition(windowWidth / 2 - bounds.width / 2,
        windowHeight / 2 - 20);
    window.draw(scoreLabel);

    // Score value
    sf::Text scoreValue;
    scoreValue.setFont(font);
    scoreValue.setString(std::to_string(game->Score()));
    scoreValue.setCharacterSize(36);
    scoreValue.setFillColor(sf::Color::White);
    scoreValue.setStyle(sf::Text::Bold);

    bounds = scoreValue.getLocalBounds();
    scoreValue.setPosition(windowWidth / 2 - bounds.width / 2,
        windowHeight / 2 + 10);
    window.draw(scoreValue);

    

    // Back to menu button
    float menuBtnW = 200;
    float menuBtnH = 50;
    float menuBtnX = windowWidth / 2 - menuBtnW / 2;
    float menuBtnY = windowHeight - 120;

    sf::RectangleShape menuBtn(sf::Vector2f(menuBtnW, menuBtnH));
    menuBtn.setPosition(menuBtnX, menuBtnY);
    menuBtn.setFillColor(BUTTON_COLOR);
    menuBtn.setOutlineThickness(2);
    menuBtn.setOutlineColor(sf::Color::White);
    window.draw(menuBtn);

    sf::Text menuText;
    menuText.setFont(font);
    menuText.setString("Back to Menu");
    menuText.setCharacterSize(20);
    menuText.setFillColor(sf::Color::White);

    bounds = menuText.getLocalBounds();
    menuText.setPosition(menuBtnX + menuBtnW / 2 - bounds.width / 2,
        menuBtnY + menuBtnH / 2 - bounds.height / 2 - 3);
    window.draw(menuText);

    // Click smiley text
    sf::Text clickText;
    clickText.setFont(font);
    clickText.setString("Click smiley to restart");
    clickText.setCharacterSize(16);
    clickText.setFillColor(sf::Color(150, 150, 150));

    bounds = clickText.getLocalBounds();
    clickText.setPosition(windowWidth / 2 - bounds.width / 2,
        windowHeight - 60);
    window.draw(clickText);
}

//  RENDER 
void GameUI::render() {
    window.clear(BG_COLOR);

    if (state == GameState::MENU) {
        drawMenu();
    }
    else if (state == GameState::PLAYING) {
        drawHeader();
        drawBoard();
        drawGameOverScreen();
    }

    window.display();
}

//  MAIN GAME LOOP 
void GameUI::run() {
    while (window.isOpen()) {
        handleEvents();
        render();
    }
}