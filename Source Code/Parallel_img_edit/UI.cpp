#include "UI.h"

//pushes a coloured text element (word) into the templine object variable
void UI::add_txt_snip(std::string text, int r, int g, int b){
    sf::Text tempText;
    tempText.setString(text);
    tempText.setFillColor(sf::Color(r, g, b, 255));
    tempText.setFont(font);
    tempText.setScale(sf::Vector2f(0.5, 0.5));
    templine.line_texts.push_back(tempText);
}

//function to draw every element (word) in a line
void UI::Output_Line::draw_line(sf::RenderWindow* window) {
    for (int i = 0; i < line_texts.size(); i++) {
        window->draw(line_texts.at(i));
    }
}

//function that takes a line and positions them next to eachother in relation to Y parameter
void UI::Output_Line::position_words(float ypos) {
    line_texts.at(0).setPosition(sf::Vector2f(10, ypos)); //first word will be at [10, ypos]
    for (int i = 1; i < line_texts.size(); i++) {//every next word will be to the right of the previous word
        line_texts.at(i).setPosition(sf::Vector2f(line_texts.at(i - 1).getPosition().x + line_texts.at(i - 1).getGlobalBounds().width + 5, ypos));
    }
}

//simple custom function to put text onto a screen
void UI::addText(sf::String txt) {
    sf::Text tempText;
    tempText.setString(txt);
    tempText.setFillColor(sf::Color::Black);
    tempText.setFont(font);
    UI_texts.push_back(tempText);
}

//returns device time
std::string UI::get_time() {
    auto timenow =
        std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    char temp_str[26];
    ctime_s(temp_str, sizeof temp_str, &timenow);
    // ^ date gets copied too, but we only want time for output [11] - 19
    std::string str = "[";
    for (int i = 0; i < 8; i++) {
        str += temp_str[i + 11];
    }
    str += ']';
    return str;
}

//sets sprite to the func argument
void UI::updateSprite(std::string name) {
    
    sprite = sf::Sprite();
    texture = sf::Texture();
    texture.loadFromFile(std::string("Pictures/" + name).c_str());
    //sprite.setScale(texture.getSize().x / sprite.getGlobalBounds().width, texture.getSize().y / sprite.getGlobalBounds().height);
    sprite.setTexture(texture);

    position_picture();
}

//function that positions the picture depending on the screen size andpicture size
void UI::position_picture() {

    //Top and bottom of picture will touch edges, centre on X axi
    //if ratio width/height is smaller than the designated screen space 
    //space to project: screensize_x*0.7 x screensize_Y
    float scale_;

    if (float(texture.getSize().x) / float(texture.getSize().y) < float(screenSize_X) * 0.7f / float(screenSize_Y)) {
        scale_ = float(screenSize_Y) / float(texture.getSize().y);
        sprite.setScale(sf::Vector2f(scale_, scale_));
        sprite.setPosition(sf::Vector2f(0.3 * screenSize_X + (screenSize_X * 0.7 - sprite.getGlobalBounds().width)/2, 0));
    }
    else { 
        scale_ = float(screenSize_X) * 0.7 / float(texture.getSize().x);
        sprite.setScale(sf::Vector2f(scale_, scale_));
        sprite.setPosition(sf::Vector2f(0.3 * screenSize_X, (float(screenSize_Y) - sprite.getGlobalBounds().height) / 2));
    } 

}

//helper function to return a desired RectangleShape
sf::RectangleShape get_ColourRect(sf::Vector2f Pos, sf::Color Color, sf::Vector2f Size) {
    sf::RectangleShape tempRectShape;
    tempRectShape.setPosition(Pos);
    tempRectShape.setFillColor(Color);
    tempRectShape.setSize(Size);
    
    return tempRectShape;
}

//rescales and repositions all texts
void UI::organize_texts() {
    float scale = (float(screenSize_Y) * 0.4 * 0.8 / (UI_texts.size() - 3)) / UI_texts.at(0).getGlobalBounds().height;

    for (int i = 0; i < UI_texts.size() - 3; i++) {
        UI_texts.at(i).setPosition(20, 20 + screenSize_Y * 0.4 / (UI_texts.size() - 3) * i - UI_texts.at(i).getGlobalBounds().height/2);
        UI_texts.at(i).setScale(sf::Vector2f(scale,scale));
    }

    for (int i = 0; i < 3; i++) {
        UI_texts.at(UI_texts.size() - 3 + i).setPosition(sf::Vector2f(screenSize_X * (0.05 + i*0.1) - UI_texts.back().getGlobalBounds().width * 0.5, 0.475 * screenSize_Y - UI_texts.back().getGlobalBounds().height * 0.5));
    }
}

//some initial setups
void UI::setup(std::string c, sf::RenderWindow* window) {

    font.loadFromFile("font/arial.ttf");
    window->setFramerateLimit(60);
    srand(time(NULL));
    
    add_txt_snip("Hello! Welcome to my picture editing application", 255, 255, 255);
    pushOutput();
    add_txt_snip("Press N / C / G to choose an image processing method", 255, 255, 255);
    pushOutput();
    add_txt_snip("To clear the console, press X. To exit, press Esc", 255, 255, 255);
    pushOutput();
    add_txt_snip("You can also scroll in the console using arrow buttons UP and DOWN", 255, 255, 255);
    pushOutput();

    //Grey Menu Setup, is also a bg colour
    ColourRects.push_back(sf::RectangleShape());
    ColourRects.back().setFillColor(sf::Color(220, 220, 220, 255));

    //Output console
    ColourRects.push_back(sf::RectangleShape());
    ColourRects.back().setFillColor(sf::Color(0, 0, 0, 255));

    //picture bg
    ColourRects.push_back(sf::RectangleShape());
    ColourRects.back().setFillColor(sf::Color(165, 165, 165, 255));

    //Normal compute button
    ColourRects.push_back(sf::RectangleShape());
    ColourRects.back().setFillColor(sf::Color(100, 255, 100, 255));

    //Parallel CPU compute button
    ColourRects.push_back(sf::RectangleShape());
    ColourRects.back().setFillColor(sf::Color(255, 100, 100, 100));


    //GPU AMP compute button
    ColourRects.push_back(sf::RectangleShape());
    ColourRects.back().setFillColor(sf::Color(100, 100, 255, 100));

    // the sprite which will actually draw it
    texture.loadFromFile(c.c_str());
    sprite.setTexture(texture);

    

    
    

    addText("P - Change Picture");
    addText("S - Save Picture");
    addText("D - Image Data");
    addText("R - Revert");
    addText("1 - Cartoonify");
    addText("2 - Negative");
    addText("3 - Black and white");
    addText("4 - RGB dominant");
    addText("5 - Black Rainbow");
    addText("6 - Pixelate");

    //mode buttons. They are centered so calc and move pos needed
    addText("Normal");
    UI_texts.back().setScale(sf::Vector2f(0.6, 0.6));
    addText("CPU Farm");
    UI_texts.back().setScale(sf::Vector2f(0.6, 0.6));
    addText("GPU AMP");
    UI_texts.back().setScale(sf::Vector2f(0.6, 0.6));
    
    //resizes everything
    resize(window); 

    setComputeMode(0);
}

//Organizes the displayed lines on the individual axis
void UI::organize_output() {
    int posctr = 0;
    for (int i = OutputTopIndex; i < Output_texts_.size(); i++) {
        Output_texts_.at(i).position_words(screenSize_Y / 2 + 10 + posctr * 20);
        posctr++;
    }
}

//renders everything
void UI::render(sf::RenderWindow* window) {

    for (int i = 0; i < ColourRects.size(); i++) {
        window->draw(ColourRects.at(i));
    }
    window->draw(sprite);
    
    for (int i = 0; i < UI_texts.size(); i++) {
        window->draw(UI_texts.at(i));
    }

    for (int i = OutputTopIndex; i < Output_texts_.size(); i++) {
        Output_texts_.at(i).draw_line(window);
    }

    window->display();
}

//resizes everything to the current window size
void UI::resize(sf::RenderWindow* window) {

    screenSize_X = window->getSize().x;
    screenSize_Y = window->getSize().y;
    
    //change all positions
    ColourRects.at(0).setPosition(sf::Vector2f(0.0, 0.0));
    ColourRects.at(1).setPosition(sf::Vector2f(0.0, 0.5 * window->getSize().y));
    ColourRects.at(2).setPosition(sf::Vector2f(0.3 * window->getSize().x, 0.0));
    ColourRects.at(3).setPosition(sf::Vector2f(0.0, 0.45 * window->getSize().y));
    ColourRects.at(4).setPosition(sf::Vector2f(0.1 * window->getSize().x, 0.45 * window->getSize().y));
    ColourRects.at(5).setPosition(sf::Vector2f(0.2 * window->getSize().x, 0.45 * window->getSize().y));

    //change all sizes
    ColourRects.at(0).setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
    ColourRects.at(1).setSize(sf::Vector2f(window->getSize().x, window->getSize().y*0.5));
    ColourRects.at(2).setSize(sf::Vector2f(window->getSize().x * 0.7, window->getSize().y));
    ColourRects.at(3).setSize(sf::Vector2f(window->getSize().x * 0.1, window->getSize().y * 0.05));
    ColourRects.at(4).setSize(sf::Vector2f(window->getSize().x * 0.1, window->getSize().y * 0.05));
    ColourRects.at(5).setSize(sf::Vector2f(window->getSize().x * 0.1, window->getSize().y * 0.05));

    position_picture();
    organize_texts();
    organize_output();
}

//pushes the complete templine into the console and does some index checks
void UI::pushOutput() {
    Output_texts_.push_back(templine);
    templine.line_texts.clear();
    //std::cout << OutputTopIndex << std::endl << Output_texts_.size() << std::endl;
    if (!fullConsole) { //bool so the program doesn't have to count math below with every next push
        if (screenSize_Y / 2 < Output_texts_.size() * 20) {
            console_line_limit = Output_texts_.size();
            fullConsole = true;
            OutputTopIndex++;
        }
    }
    else {
        if (Output_texts_.size() - OutputTopIndex < console_line_limit) {
            OutputTopIndex = Output_texts_.size() - console_line_limit;
        }
        else { OutputTopIndex++; }
    }

}

//sets a time point
void UI::UI_start_ctr() {
    //the_clock::time_point start = the_clock::now();

    time_pt = the_clock::now();
}

//timer function)
void UI::UI_end_ctr_push_output(std::string name) {

    auto time_taken = duration_cast<milliseconds>(the_clock::now() - time_pt).count();

    add_txt_snip(get_time(), 255, 255, 255);
    add_txt_snip(name , 220, 255, 220);
    add_txt_snip("took ", 255, 255, 255);
    add_txt_snip(std::to_string(time_taken), 220, 220, 255);
    add_txt_snip(" ms to calculate. ", 255, 255, 255);
    pushOutput();
    organize_output();
}

void UI::setComputeMode(int mode) {
    //[3,4,5
    int alpha;
    sf::Vector2f pos;
    sf::Vector2f scale;
    for (int i = 0; i < 3; i++) {
        if (mode == i) {
            alpha = 255;
            scale = sf::Vector2f(1.0, 1.5);
            pos = sf::Vector2f(ColourRects.at(3 + i).getPosition().x, screenSize_Y * 0.425);
            
        }
        else { 
            alpha = 100;
            pos = sf::Vector2f(ColourRects.at(3 + i).getPosition().x, screenSize_Y * 0.45);
            scale = sf::Vector2f(1.0, 1.0);
        }
        ColourRects.at(3 + i).setPosition(pos);
        ColourRects.at(3 + i).setScale(scale);
        ColourRects.at(3 + i).setFillColor(sf::Color(ColourRects.at(3 + i).getFillColor().r,
                                                     ColourRects.at(3 + i).getFillColor().g,
                                                     ColourRects.at(3 + i).getFillColor().b,
                                                     alpha));
    }
    
}

int UI::getComputeMode() {
    for (int i = 0; i < 3; i++) {
        if (ColourRects.at(3 + i).getScale().y != 1.0) { //whichever is up
            return i;
        }
    }
    return -1; //fail result
}