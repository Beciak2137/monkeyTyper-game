#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int WORD_COUNT = 35;
float SPEED = 50.0f;
int MAX_MISSED_WORDS = 10;
int CHARACTER_SIZE = 24;
const int HORIZONTAL_SPACING = 300;
const int VERTICAL_SPACING = 50;

std::vector<sf::Font> Fonts;
int currentFontIndex = 0;
std::vector<std::string> FontsName;

struct MovingWord {
    sf::Text text;
    float speed;
    bool active;
};
bool isOverlapping(const sf::Text& text, const std::vector<MovingWord>& words) {
    for (const auto& word : words) {
        if (text.getGlobalBounds().intersects(word.text.getGlobalBounds())) {
            return true;
        }
    }
    return false;
}

void initializeWords(std::vector<MovingWord>& words, const sf::Font& font, const std::vector<std::string>& wordList) {
    std::srand(static_cast<unsigned>(std::time(0)));
    int min = -2000;
    int max = 0;

    std::vector<std::string> usedWords;

    while (words.size() < WORD_COUNT) {
        std::string newWord = wordList[std::rand() % wordList.size()];


        if (std::find(usedWords.begin(), usedWords.end(), newWord) == usedWords.end()) {
            sf::Text text;
            text.setFont(font);
            text.setString(newWord);
            text.setCharacterSize(CHARACTER_SIZE);
            text.setFillColor(sf::Color::White);

            int random_x, random_y;
            bool overlap;
            do {
                random_x = min + HORIZONTAL_SPACING + std::rand() % (max - min + 1);
                random_y = VERTICAL_SPACING + std::rand() % (WINDOW_HEIGHT / 2);

                if (random_y >= WINDOW_HEIGHT) {
                    random_y = std::rand() % (WINDOW_HEIGHT - VERTICAL_SPACING);
                }

                text.setPosition(static_cast<float>(random_x), static_cast<float>(random_y));
                overlap = isOverlapping(text, words);

            } while (overlap);

            words.push_back({text, SPEED, true});
            usedWords.push_back(newWord);
        }
    }
}
void regenerateWords(std::vector<MovingWord>& words, const sf::Font& font, const std::vector<std::string>& wordList) {
    words.clear();


    initializeWords(words, font, wordList);
}
void drawMenu(sf::RenderWindow& window, const sf::Text& title, const sf::RectangleShape& startButton, const sf::Text& startText, const sf::RectangleShape& optionsButton, const sf::Text& optionsText, const sf::RectangleShape& scoreboardButton, const sf::Text& scoreboardText) {
    window.draw(startButton);
    window.draw(optionsButton);
    window.draw(scoreboardButton);
    window.draw(title);
    window.draw(startText);
    window.draw(optionsText);
    window.draw(scoreboardText);
}

void updateWords(std::vector<MovingWord>& words, sf::Time deltaTime, int& missedWords) {
    std::vector<sf::FloatRect> wordBounds;

    for (auto& word : words) {
        word.text.move(word.speed * deltaTime.asSeconds(), 0);

        if (word.text.getPosition().x > WINDOW_WIDTH) {
            int random_x = -2000 + std::rand() % (0 - (-2000) + 1);
            int random_y = std::rand() % WINDOW_HEIGHT;

            word.text.setPosition(static_cast<float>(random_x), static_cast<float>(random_y));

            if (word.active) {
                ++missedWords;
            }
        }

        wordBounds.push_back(word.text.getGlobalBounds());
    }
}

void handleInput(std::string& input, std::vector<MovingWord>& words, int& score) {
    for (auto it = words.begin(); it != words.end(); ++it) {
        std::string wordStr = it->text.getString();
        if (input == wordStr) {
            it = words.erase(it);
            input.clear();
            score += 10;
            break;
        }
    }
}

void highlightMatchingPart(sf::Text& text, const std::string& input) {
    std::string wordStr = text.getString();
    size_t matchLength = input.length();
    if (wordStr.substr(0, matchLength) == input) {
        text.setFillColor(sf::Color::Green);
    } else {
        text.setFillColor(sf::Color::White);
    }
}

void saveSize(){
    std::ofstream file("Size.txt");
    if(file.is_open()){
        file<<CHARACTER_SIZE;
        file.close();
    }
}
void saveSpeed(){
    std::ofstream file("Speed.txt");
    if(file.is_open()){
        file<<SPEED;
        file.close();
    }
}
void saveMissedWords(){
    std::ofstream file("MissedWords.txt");
    if(file.is_open()){
        file<<MAX_MISSED_WORDS;
        file.close();
    }
}
int loadMissedWords(){
    int missedwords;
    std::ifstream file("MissedWords.txt");
    if (file.is_open()) {
        file >> missedwords;
        file.close();
    }
    return missedwords;
}
int loadSpeed(){
    int speed;
    std::ifstream file("Speed.txt");
    if (file.is_open()) {
        file >> speed;
        file.close();
    }
    return speed;
}
int loadSize(){
    int size;
    std::ifstream file("Size.txt");
    if (file.is_open()) {
        file >> size;
        file.close();
    }
    return size;
}
void saveHighScore(int score) {
    std::ofstream file("rekord.txt");
    if (file.is_open()) {
        file << score;
        file.close();
    }
}

int loadHighScore() {
    int highScore = 0;
    std::ifstream file("rekord.txt");
    if (file.is_open()) {
        file >> highScore;
        file.close();
    }
    return highScore;
}
bool areAllWordsFound(const std::vector<MovingWord>& words) {
    for (const auto& word : words) {
        if (word.active) {
            return false;
        }
    }
    return true;
}

void loadFonts() {
    sf::Font font1, font2, font3,font4,font5,font6;
    std::string name1,name2,name3,name4, name5, name6;
    name1="arial";
    name2="arial";
    name3 = "COURIER";
    name4 = "FRE30F9X";
    name5 = "SURFS___";
    name6 = "DIGITALDREAM";
    font1.loadFromFile("../arial.ttf");
    font2.loadFromFile("../arial.ttf");
    font3.loadFromFile("../COURIER.ttf");
    font4.loadFromFile("../FRE3OF9X.ttf");
    font5.loadFromFile("../SURFS___.ttf");
    font6.loadFromFile("../DIGITALDREAM.ttf");

    Fonts.push_back(font1);
    Fonts.push_back(font2);
    Fonts.push_back(font3);
    Fonts.push_back(font4);
    Fonts.push_back(font5);
    Fonts.push_back(font6);
    FontsName.push_back(name1);
    FontsName.push_back(name2);
    FontsName.push_back(name3);
    FontsName.push_back(name4);
    FontsName.push_back(name5);
    FontsName.push_back(name6);
}

void applyFontToText(sf::Text& text, const sf::Font& font) {
    text.setFont(font);
}
void applyFontToAllTexts(const sf::Font& font, sf::Text& title, sf::Text& startText, sf::Text& optionsText, sf::Text& scoreboardText, sf::Text& inputText, sf::Text& scoreText, sf::Text& missedWordsText, sf::Text& speedLabel, sf::Text& speedValue, sf::Text& skipWordsLabel, sf::Text& skipWordsValue, sf::Text& wordSizeLabel, sf::Text& wordSizeValue, sf::Text& highScoreText, sf::Text& highScore) {
    applyFontToText(title, font);
    applyFontToText(startText, font);
    applyFontToText(optionsText, font);
    applyFontToText(scoreboardText, font);
    applyFontToText(inputText, font);
    applyFontToText(scoreText, font);
    applyFontToText(missedWordsText, font);
    applyFontToText(speedLabel, font);
    applyFontToText(speedValue, font);
    applyFontToText(skipWordsLabel, font);
    applyFontToText(skipWordsValue, font);
    applyFontToText(wordSizeLabel, font);
    applyFontToText(wordSizeValue, font);
    applyFontToText(highScoreText, font);
    applyFontToText(highScore, font);
}

void drawEndGameMessage(sf::RenderWindow& window, const sf::Font& font) {
    sf::Text endGameText("Koniec gry!", font, 64);
    endGameText.setFillColor(sf::Color::White);
    sf::FloatRect textBounds = endGameText.getLocalBounds();
    endGameText.setOrigin(textBounds.left + textBounds.width / 2.0f,
                          textBounds.top + textBounds.height / 2.0f);
    endGameText.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
    window.draw(endGameText);
}
int main() {
    MAX_MISSED_WORDS=loadMissedWords();
    SPEED = loadSpeed();
    CHARACTER_SIZE = loadSize();
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    int startPressed = 0;
    int score = 0;
    int missedWords = 0;
    loadFonts();
    sf::Font& currentFont = Fonts[currentFontIndex];
    std::string& currentFontName = FontsName[currentFontIndex];

    std::string input;

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Monkey Typer");

    //sf::Font font;
    //font.loadFromFile("../arial.ttf");

    sf::Text title("Monkey Typer", currentFont, 64);
    title.setFillColor(sf::Color::White);
    title.setPosition(200, 50);

    sf::RectangleShape startButton(sf::Vector2f(350, 50));
    startButton.setPosition({225, 250});
    startButton.setFillColor(sf::Color(0, 200, 100));

    sf::Text startText("START", currentFont, 32);
    startText.setFillColor(sf::Color::White);
    startText.setPosition(350, 250);

    sf::RectangleShape optionsButton(sf::Vector2f(350, 50));
    optionsButton.setPosition({225, 350});
    optionsButton.setFillColor(sf::Color(0, 200, 100));

    sf::Text optionsText("Ustawienia", currentFont, 32);
    optionsText.setFillColor(sf::Color::White);
    optionsText.setPosition(325, 350);

    sf::RectangleShape scoreboardButton(sf::Vector2f(350, 50));
    scoreboardButton.setPosition({225, 450});
    scoreboardButton.setFillColor(sf::Color(0, 200, 100));

    sf::Text scoreboardText("Tabela Wynikow", currentFont, 32);
    scoreboardText.setFillColor(sf::Color::White);
    scoreboardText.setPosition(280, 450);

    sf::Text inputText("", currentFont, 24);
    inputText.setFillColor(sf::Color::Green);
    inputText.setPosition(10, WINDOW_HEIGHT - 40);

    sf::Text scoreText("Wynik: 0", currentFont, 24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(WINDOW_WIDTH - 160, 10);

    std::vector<MovingWord> words;
    std::vector<std::string> wordList = {"marchew", "ziemniak", "pomidor", "ogorek", "kapusta", "cebula","jablko", "banan", "pomarancza", "truskawka", "gruszka", "malina","kot", "pies", "krowa", "kon", "owca", "kura","stol", "krzeslo", "szafa", "lozko", "komoda", "kanapa","telefon", "komputer", "ksiazka", "dlugopis", "klawiatura", "aparat","laptop","ekran","c++","java","lenovo"};
    initializeWords(words, currentFont, wordList);

    sf::Text speedLabel("Predkosc:", currentFont, 24);
    speedLabel.setPosition(50, 50);

    sf::Text speedValue(std::to_string(SPEED), currentFont, 24);
    speedValue.setPosition(250, 50);

    sf::RectangleShape increaseSpeedButton(sf::Vector2f(30, 30));
    increaseSpeedButton.setPosition(600, 50);
    increaseSpeedButton.setFillColor(sf::Color::Green);

    sf::Text plus1 ("+",currentFont,40);
    plus1.setPosition(601,40);
    plus1.setFillColor(sf::Color::Black);

    sf::RectangleShape decreaseSpeedButton(sf::Vector2f(30, 30));
    decreaseSpeedButton.setPosition(400, 50);
    decreaseSpeedButton.setFillColor(sf::Color::Red);

    sf::Text minus1 ("-",currentFont,40);
    minus1.setPosition(407,39);
    minus1.setFillColor(sf::Color::Black);

    sf::Text skipWordsLabel("Liczba slow do pominiecia:", currentFont, 24);
    skipWordsLabel.setPosition(50, 100);

    sf::Text skipWordsValue(std::to_string(MAX_MISSED_WORDS), currentFont, 24);
    skipWordsValue.setPosition(350, 100);

    sf::Text FontChange("Czcionka:",currentFont,24);
    FontChange.setPosition(50,200);

    sf::Text FontNamee(currentFontName, currentFont, 24);
    FontNamee.setPosition(250, 200);

    sf::RectangleShape increaseFontChange(sf::Vector2f(30, 30));
    increaseFontChange.setPosition(600, 200);
    increaseFontChange.setFillColor(sf::Color::Green);

    sf::Text plus4 ("+",currentFont,40);
    plus4.setPosition(601,190);
    plus4.setFillColor(sf::Color::Black);

    sf::RectangleShape decreaseFontChange(sf::Vector2f(30, 30));
    decreaseFontChange.setPosition(400, 200);
    decreaseFontChange.setFillColor(sf::Color::Red);

    sf::Text minus4 ("-",currentFont,40);
    minus4.setPosition(407,189);
    minus4.setFillColor(sf::Color::Black);

    sf::RectangleShape increaseSkipWordsButton(sf::Vector2f(30, 30));
    increaseSkipWordsButton.setPosition(600, 100);
    increaseSkipWordsButton.setFillColor(sf::Color::Green);

    sf::Text plus2 ("+",currentFont,40);
    plus2.setPosition(601,90);
    plus2.setFillColor(sf::Color::Black);

    sf::RectangleShape decreaseSkipWordsButton(sf::Vector2f(30, 30));
    decreaseSkipWordsButton.setPosition(400, 100);
    decreaseSkipWordsButton.setFillColor(sf::Color::Red);

    sf::Text minus2 ("-",currentFont,40);
    minus2.setPosition(407,89);
    minus2.setFillColor(sf::Color::Black);

    sf::Text wordSizeLabel("Wielkosc slow:", currentFont, 24);
    wordSizeLabel.setPosition(50, 150);

    sf::Text wordSizeValue(std::to_string(CHARACTER_SIZE), currentFont, 24);
    wordSizeValue.setPosition(250, 150);

    sf::RectangleShape increaseWordSizeButton(sf::Vector2f(30, 30));
    increaseWordSizeButton.setPosition(600, 150);
    increaseWordSizeButton.setFillColor(sf::Color::Green);

    sf::Text plus3 ("+",currentFont,40);
    plus3.setPosition(601,140);
    plus3.setFillColor(sf::Color::Black);

    sf::RectangleShape decreaseWordSizeButton(sf::Vector2f(30, 30));
    decreaseWordSizeButton.setPosition(400, 150);
    decreaseWordSizeButton.setFillColor(sf::Color::Red);

    sf::Text minus3 ("-",currentFont,40);
    minus3.setPosition(407,139);
    minus3.setFillColor(sf::Color::Black);

    sf::Text missedWordsText("Pominiete slowa: 0", currentFont, 24);
    missedWordsText.setFillColor(sf::Color::White);
    missedWordsText.setPosition(WINDOW_WIDTH - 600, WINDOW_HEIGHT -30);

    sf::Text SaveSeting("Zapisz",currentFont,24);
    SaveSeting.setPosition(360,550);

    sf::RectangleShape saveSettingss(sf::Vector2f(100, 50));
    saveSettingss.setPosition({347, 540});
    saveSettingss.setFillColor(sf::Color(0, 200, 100));

    int highestScore = loadHighScore();
    sf::Text highScoreText("Rekord: " + std::to_string(highestScore), currentFont, 24);
    highScoreText.setFillColor(sf::Color::White);
    highScoreText.setPosition(WINDOW_WIDTH-200, WINDOW_HEIGHT -30);

    sf::Text highScore("Rekord: " + std::to_string(highestScore), currentFont, 30);
    highScore.setFillColor(sf::Color::White);
    highScore.setPosition(40, 50);





    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Escape) {
                        startPressed = 0;
                        scoreText.setString("Wynik: " + std::to_string(score));
                    }
                    break;
                case sf::Event::TextEntered:
                    if (startPressed == 1) {
                        if (event.text.unicode < 128) {
                            if (event.text.unicode == 8) { // Backspace
                                if (!input.empty()) {
                                    input.pop_back();
                                }
                            } else if (event.text.unicode == 13) { // Enter
                                handleInput(input, words, score);
                            } else {
                                input += static_cast<char>(event.text.unicode);
                            }
                        }
                    }
                    break;
                case sf::Event::MouseButtonPressed:
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        auto mousePos = sf::Mouse::getPosition(window);
                        if (startButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                            startPressed = 1;
                            missedWords = 0;
                            score = 0;
                            input.clear();
                            initializeWords(words, currentFont, wordList);
                        } else if (optionsButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                            startPressed = 2;
                        } else if (scoreboardButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                            startPressed = 3;
                        }
                    }
                    if (increaseSpeedButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        SPEED += 5.0f;
                        //zrobic funkcje set speed by obnizala wszedzie predkosc i tak wszedzie w sumie
                        speedValue.setString(std::to_string(SPEED));
                        regenerateWords(words, currentFont, wordList);
                    }
                    else if (increaseSkipWordsButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        MAX_MISSED_WORDS += 1;
                        skipWordsValue.setString(std::to_string(MAX_MISSED_WORDS));
                    }
                    else if (increaseWordSizeButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        CHARACTER_SIZE +=1;
                        wordSizeValue.setString(std::to_string(CHARACTER_SIZE));
                        regenerateWords(words, currentFont, wordList);
                    }
                    else if (decreaseSpeedButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        if(SPEED!=0){SPEED -= 5.0f;
                        speedValue.setString(std::to_string(SPEED));
                        regenerateWords(words, currentFont, wordList);}
                    }
                    else if (decreaseSkipWordsButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        if(MAX_MISSED_WORDS!=0){
                        MAX_MISSED_WORDS -= 1;
                        skipWordsValue.setString(std::to_string(MAX_MISSED_WORDS));}
                    }
                    else if (decreaseWordSizeButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        if(CHARACTER_SIZE!=0){
                            CHARACTER_SIZE -=1;
                            wordSizeValue.setString(std::to_string(CHARACTER_SIZE));
                            regenerateWords(words, currentFont, wordList);
                        }
                    }
                    else if (decreaseFontChange.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        if(currentFontIndex!=0){
                            currentFontIndex--;
                            currentFontName=FontsName[currentFontIndex];
                            currentFont=Fonts[currentFontIndex];
                            FontNamee.setString(currentFontName);
                        }
                        else {
                            currentFontIndex = 0;
                            currentFontName=FontsName[currentFontIndex];
                            currentFont=Fonts[currentFontIndex];
                            FontNamee.setString(currentFontName);

                        }
                        regenerateWords(words,currentFont,wordList);
                    }
                    else if (increaseFontChange.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        if(currentFontIndex!=5){
                            currentFontIndex++;
                            currentFontName=FontsName[currentFontIndex];
                            currentFont=Fonts[currentFontIndex];
                            FontNamee.setString(currentFontName);
                        }
                        else {
                            currentFontIndex = 5;
                            currentFontName=FontsName[currentFontIndex];
                            currentFont=Fonts[currentFontIndex];
                            FontNamee.setString(currentFontName);

                        }
                        regenerateWords(words,currentFont,wordList);
                    }
                    else if (saveSettingss.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        saveMissedWords();
                        saveSize();
                        saveSpeed();
                        startPressed=0;
                    }
                    break;

            }
        }

        sf::Time deltaTime = clock.restart();
        window.clear(sf::Color::Black);

        if (startPressed == 0) {
            drawMenu(window, title, startButton, startText, optionsButton, optionsText, scoreboardButton, scoreboardText);
        } else if (startPressed == 1) {
            updateWords(words, deltaTime, missedWords);
            if (missedWords >= MAX_MISSED_WORDS) {
                startPressed = 4;
                scoreText.setString("Wynik: " + std::to_string(score));
            }
            if (score > highestScore) {
                highestScore = score;
                highScoreText.setString("Rekord: " + std::to_string(highestScore));
                saveHighScore(highestScore);
            }
            if (areAllWordsFound(words)) {
                startPressed=4;
            }
            missedWordsText.setString("Pominiete slowa: "+std::to_string(missedWords));
            handleInput(input, words, score);
            scoreText.setString("Wynik: " + std::to_string(score));
            for (auto& word : words) {
                highlightMatchingPart(word.text, input);
                window.draw(word.text);
            }
            inputText.setString(input);
            highlightMatchingPart(inputText, input);
            window.draw(inputText);
            window.draw(scoreText);
            window.draw(highScoreText);
            window.draw(missedWordsText);
        } else if(startPressed == 2){
            window.clear(sf::Color::Black);
            window.draw(speedLabel);
            window.draw(speedValue);
            window.draw(increaseSpeedButton);
            window.draw(plus1);
            window.draw(decreaseSpeedButton);
            window.draw(minus1);
            window.draw(skipWordsLabel);
            window.draw(skipWordsValue);
            window.draw(increaseSkipWordsButton);
            window.draw(plus2);
            window.draw(decreaseSkipWordsButton);
            window.draw(minus2);
            window.draw(wordSizeLabel);
            window.draw(wordSizeValue);
            window.draw(increaseWordSizeButton);
            window.draw(plus3);
            window.draw(decreaseWordSizeButton);
            window.draw(minus3);
            window.draw(FontChange);
            window.draw(FontNamee);
            window.draw(increaseFontChange);
            window.draw(decreaseFontChange);
            window.draw(minus4);
            window.draw(plus4);
            window.draw(saveSettingss);
            window.draw(SaveSeting);

        }
        else if(startPressed ==3){
            window.draw(highScore);
        } else if(startPressed==4){
            window.clear(sf::Color::Black);
            drawEndGameMessage(window, currentFont);
        }

        window.display();
    }

    return 0;
}
