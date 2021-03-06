#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Image_.h"
#include "UI.h"
#include <thread>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>

//EXTERNAL LIBRARIES IN USE:
//SFML - https://www.sfml-dev.org/
//EasyBMP - http://easybmp.sourceforge.net/

//Declare Variables

//TO ADD YOUR OWN PICTURES: Place a BMP picture into the Pictures directory and add the name of the picture (without suffix) to the Pictures array
std::vector<std::string> Pictures{ "Flowers","Island","Tiger","Stream","City","Bear" };
int pictures_it = 0;
const int screenSize_X = sf::VideoMode::getDesktopMode().width;
const int screenSize_Y = sf::VideoMode::getDesktopMode().height;
bool keyDown = false;
bool fullscreen = false;

Image_ img;
UI Ui;
sf::Keyboard keyboard;
sf::RenderWindow window(sf::VideoMode(screenSize_X, screenSize_Y), "Picture Editor");

int main()
{

    Ui.setup("Pictures/Flowers.bmp",&window);
    bool program_running = true;        // true until the user wants to quit

    while (program_running) //main app loop
    {
        //event pump
        sf::Event evt;
        while (window.pollEvent(evt))      // while there are any events to process...
        {
            // process them. But we're only interested in the closed event
            if (evt.type == sf::Event::EventType::Closed) {
                img.clear_temp();
                program_running = false;

            }
            else if (evt.type == sf::Event::Resized) { //basic makeshift resize
                Ui.resize(&window);

            }
        }

        //INPUTS
        if (keyboard.isKeyPressed(sf::Keyboard::Escape)) {
              program_running = false;
              img.clear_temp();

        }

        //Display image data
        if (keyboard.isKeyPressed(sf::Keyboard::D)) {

            if (keyDown == false) {
                keyDown = true;
                Ui.add_txt_snip(Ui.get_time(), 255, 255, 255);
                Ui.add_txt_snip("File name:", 255, 220, 220);
                Ui.add_txt_snip(img.getImageName(), 255, 255, 210);
                Ui.pushOutput();

                Ui.add_txt_snip(Ui.get_time(), 255, 255, 255);
                Ui.add_txt_snip("Size (px):", 255, 220, 220);
                Ui.add_txt_snip(std::to_string(img.getPic()->TellWidth()) + std::string("x") + std::to_string(img.getPic()->TellHeight()), 255, 255, 210);
                Ui.pushOutput();

                Ui.add_txt_snip(Ui.get_time(), 255, 255, 255);
                Ui.add_txt_snip("Bit depth (bit/px):", 255, 220, 220);
                Ui.add_txt_snip(std::to_string(img.getPic()->TellBitDepth()), 255, 255, 210);
                Ui.pushOutput();

                Ui.add_txt_snip(Ui.get_time(), 255, 255, 255);
                Ui.add_txt_snip("Different Colours: ~", 255, 220, 220);

                //Ui.UI_start_ctr();
                //Ui.UI_end_ctr_push_output("Standard Negative");

                Ui.add_txt_snip(std::to_string(img.CountdifferentColours()), 255, 255, 210);
                Ui.pushOutput();
                
                Ui.organize_output();
            }
            
        }
        else { keyDown = false; }

        if (keyboard.isKeyPressed(sf::Keyboard::P)) {
            pictures_it = (pictures_it + 1) % Pictures.size();
            img.setPic(Pictures.at(pictures_it));
            img.setupGPU_data();
            Ui.updateSprite(img.getImageName() + ".bmp");
        }
        else { keyDown = false; }

        //console clear
        if (keyboard.isKeyPressed(sf::Keyboard::X)) {
            Ui.clear_console();
        }

        //set compute normal
        if (keyboard.isKeyPressed(sf::Keyboard::N)) {
            Ui.setComputeMode(0);
        }

        //set compute with a cpu farm
        if (keyboard.isKeyPressed(sf::Keyboard::C)) {
            Ui.setComputeMode(1);
        }

        //set with GPU amp
        if (keyboard.isKeyPressed(sf::Keyboard::G)) {
            Ui.setComputeMode(2);
        }

        //revert picture
        if (keyboard.isKeyPressed(sf::Keyboard::R)) {
            img.revert();
            Ui.updateSprite(img.getImageName() + ".bmp");
            img.setupGPU_data();
        }

        //save picture
        if (keyboard.isKeyPressed(sf::Keyboard::S)) {
            img.savePicture();
            Ui.add_txt_snip(Ui.get_time(), 255, 255, 255);
            Ui.add_txt_snip("Picture saved as ", 255, 220, 220);
            Ui.add_txt_snip("Picture_" + std::to_string(img.getSavedPics()), 255, 255, 210);
            Ui.pushOutput();
            Ui.organize_output();
        }

        //console up
        if (keyboard.isKeyPressed(sf::Keyboard::Up)) {
            if (Ui.getOutputTopIndex() != 0) {
                Ui.incrementOutputTopIndex(-1);
                Ui.organize_output();
            }
        }

        //console down
        if (keyboard.isKeyPressed(sf::Keyboard::Down)) {
            Ui.incrementOutputTopIndex(1);
            Ui.organize_output();
        }

        //edits
        if (keyboard.isKeyPressed(sf::Keyboard::Num1)) {
            
            Ui.UI_start_ctr();
            if (Ui.getComputeMode() == 0) {
                img.cartoonify(60);
                Ui.UI_end_ctr_push_output("Standard Cartoonify");
            }
            else if (Ui.getComputeMode() == 1) {
                img.cpu_farm(1);
                Ui.UI_end_ctr_push_output("Cartoonify using a parallel CPU farm");
            }
            else {
                img.cartoonify_gpu(60);
                Ui.UI_end_ctr_push_output("Cartoonify using GPU Amp");

                Ui.UI_start_ctr();
                img.cpu_farm(7);
                Ui.UI_end_ctr_push_output("But conversion back to cpu then");
            }
            Ui.updateSprite(img.getEditedImgName());
            
        }

        if (keyboard.isKeyPressed(sf::Keyboard::Num2)) {
            Ui.UI_start_ctr();
            if (Ui.getComputeMode() == 0) {
                img.negative();
                Ui.UI_end_ctr_push_output("Standard Negative");
            }
            else if (Ui.getComputeMode() == 1) {
                
                img.cpu_farm(2);
                Ui.UI_end_ctr_push_output("Negative using a parallel CPU farm");
            }
            else {
                img.negative_gpu();
                Ui.UI_end_ctr_push_output("Negative using GPU Amp");

                Ui.UI_start_ctr();
                img.cpu_farm(7);
                Ui.UI_end_ctr_push_output("But conversion back to cpu then");
            }
            Ui.updateSprite(img.getEditedImgName());
        }

        if (keyboard.isKeyPressed(sf::Keyboard::Num3)) {
            Ui.UI_start_ctr();
            if (Ui.getComputeMode() == 0) {
                img.greyify();
                Ui.UI_end_ctr_push_output("Standard Greyify");
            }
            else if (Ui.getComputeMode() == 1) {
                img.cpu_farm(3);
                Ui.UI_end_ctr_push_output("Greyify using a parallel CPU farm");
            }
            else {
                img.greyify_gpu();
                Ui.UI_end_ctr_push_output("Greyify using GPU Amp");

                Ui.UI_start_ctr();
                img.cpu_farm(7);
                Ui.UI_end_ctr_push_output("But conversion back to cpu then");
            }
            Ui.updateSprite(img.getEditedImgName());
            
        }

        if (keyboard.isKeyPressed(sf::Keyboard::Num4)) {

            Ui.UI_start_ctr();
            if (Ui.getComputeMode() == 0) {
                img.RGBdominant();
                Ui.UI_end_ctr_push_output("Standard RGB dominant");
            }
            else if (Ui.getComputeMode() == 1) {
                img.cpu_farm(4);
                Ui.UI_end_ctr_push_output("RGB dominant using a parallel CPU farm");
            }
            else {
                img.RGBdominant_gpu();
                Ui.UI_end_ctr_push_output("RGB dominant using GPU Amp");

                Ui.UI_start_ctr();
                img.cpu_farm(7);
                Ui.UI_end_ctr_push_output("But conversion back to cpu then");
            }
            Ui.updateSprite(img.getEditedImgName());
            
        }

        if (keyboard.isKeyPressed(sf::Keyboard::Num5)) {
            Ui.UI_start_ctr();
            if (Ui.getComputeMode() == 0) {
                img.black_rainbow();
                Ui.UI_end_ctr_push_output("Standard Black Rainbow");
            }
            else if (Ui.getComputeMode() == 1) {
                img.cpu_farm(5);
                Ui.UI_end_ctr_push_output("Black Rainbow using a parallel CPU farm");
            }
            else {
                img.black_rainbow_gpu();
                Ui.UI_end_ctr_push_output("Black Rainbow using GPU Amp");

                Ui.UI_start_ctr();
                img.cpu_farm(7);
                Ui.UI_end_ctr_push_output("But conversion back to cpu then");
            }
            Ui.updateSprite(img.getEditedImgName());
        }

        if (keyboard.isKeyPressed(sf::Keyboard::Num6)) {
            Ui.UI_start_ctr();
            if (Ui.getComputeMode() == 0) {
                img.pixelate(10);
                Ui.UI_end_ctr_push_output("Standard Pixelate");
            }
            else if (Ui.getComputeMode() == 1) {
                img.cpu_farm(6);
                Ui.UI_end_ctr_push_output("Pixelate using a parallel CPU farm");
            }
            else {
                img.pixelate_gpu(10);
                Ui.UI_end_ctr_push_output("Pixelate using GPU Amp");

                Ui.UI_start_ctr();
                img.cpu_farm(7);
                Ui.UI_end_ctr_push_output("But conversion back to cpu then");
            }
            Ui.updateSprite(img.getEditedImgName());
        }

        Ui.render(&window);

    }

    img.clear_temp();

}