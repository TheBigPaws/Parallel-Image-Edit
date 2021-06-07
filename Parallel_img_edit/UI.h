#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <string>
#include <chrono>
#include <ctime>



// timing setup
using std::chrono::duration_cast;
using std::chrono::milliseconds;

typedef std::chrono::steady_clock the_clock;

class UI
{
	//custom class to build console output. It's a vector of words
	struct Output_Line {
			std::vector<sf::Text> line_texts{};
			void draw_line(sf::RenderWindow* window);
			void position_words(float ypos);
	};

	private:
		int screenSize_X = sf::VideoMode::getDesktopMode().width;
		int screenSize_Y = sf::VideoMode::getDesktopMode().height;
		int OutputTopIndex = 0;

		//variables for the console
		Output_Line templine;
		int console_line_limit;
		bool fullConsole = false;

		//---------UI elements
		//texts
		std::vector<sf::Text> UI_texts;
		std::vector<Output_Line> Output_texts_;
		sf::Font font;

		//background menu colours
		std::vector<sf::RectangleShape> ColourRects;

		//picture
		sf::Texture texture;
		sf::Sprite sprite; 
		

		//timer
		the_clock::time_point time_pt;
		the_clock::time_point start;
		the_clock::time_point end;
	public:
		 //variable to make scrolling in Output possible

		//timer functions
		void UI_start_ctr();
		void UI_end_ctr_push_output(std::string name);
		
		//console functions
		void add_txt_snip(std::string text, int r, int g, int b);
		void pushOutput();
		void organize_output();
		void clear_console() {
			OutputTopIndex = 0;
			Output_texts_.clear();
		}

		//UI texts(not console)
		void addText(sf::String txt);
		void organize_texts();
		
		//picture functions
		void position_picture();
		void updateSprite(std::string name);

		//basic loop functions
		void setup(std::string c, sf::RenderWindow* window);
		void render(sf::RenderWindow* window);
		void resize(sf::RenderWindow* window);
		
		//Getters, setters
		std::string get_time();
		void setComputeMode(int mode);
		int getComputeMode();
		int getOutputTopIndex() {
			return OutputTopIndex;
		}
		void incrementOutputTopIndex(int inc) {
			OutputTopIndex += inc;
		}
};

