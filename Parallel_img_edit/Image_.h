#pragma once
#include "EasyBMP.h"
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <chrono>
#include <queue>

#include <amp.h>
#include <amp_math.h>

using namespace concurrency;
using std::chrono::duration_cast;
using std::chrono::milliseconds;

typedef std::chrono::steady_clock the_clock;

class task_ {
public:
	virtual ~task_()
	{
	}
	task_() {

	}
	/** Perform the task_. Subclasses must override this. */
	virtual void run() = 0;
};

class writeBack_task : public task_ {
private:
	int row;
	BMP* picture;
	int* gpu_arr;
public:
	writeBack_task(BMP* a, int b, int* c) {
		picture = a; row = b; gpu_arr = c;
	}
	void run();
};

class greyify_task : public task_ {
private:
	int row;
	BMP* picture;
public:
	greyify_task(BMP* a, int b) {
		picture = a; row = b;
	}
	void run();
};

class black_rainbow_task : public task_ {
private:
	int row;
	BMP* picture;
public:
	black_rainbow_task(BMP* a, int b) {
		picture = a; row = b;
	}
	void run();
};

class cartoonify_task : public task_ {
private:
	int sharpness;
	int row;
	BMP* picture;
public:
	cartoonify_task(BMP* a, int b, int c) {
		picture = a; row = b; sharpness = c;
	}
	void run();
};

class pixelate_task : public task_ {
private:
	int pixelsize;
	int row;
	BMP* picture;
public:
	pixelate_task(BMP* a, int b, int c) {
		picture = a; row = b; pixelsize = c;
	}
	void run();
};


class rgb_dominant_task : public task_ {
private:
	int row;
	BMP* picture;
public:
	rgb_dominant_task(BMP* a, int b) {
		picture = a; row = b;
	}
	void run();
};


class negative_task : public task_ {
private:
	int row;
	BMP* picture;
public:
	negative_task(BMP* a, int b) {
		picture = a; row = b;
	}
	void run();
};

//CUSTOM CLASS TO SIMPLIFY WORKING WITH IMAGES
class Image_
{
	private:
		BMP EditedPic; //picture we are working with

		std::string lastEdit;
		std::string imageName = "Flowers";

		//cpu farm vars
		std::queue <task_*> taskQ;
		std::vector <std::thread> threadVect;
		std::mutex mutexLock;

		//gpu array
		int* dynArr;

		//vars to aid saving and cleanup of junk pics
		int savedPics = 0;
		std::vector <std::string> tempnames;

		bool SavedCls[16777216] = { false }; //hashtable to save seen colours

	public:
		//Constructor
		Image_() {

			//initial picture
			EditedPic.ReadFromFile(std::string("Pictures/" + imageName + ".bmp").c_str());
			//const int size = EditedPic.TellHeight() * EditedPic.TellWidth() * 3;
			//dynArr = new int[size];
			setupGPU_data();

		}

		int CountdifferentColours(); //hash function

		//setters, getters
		int getSavedPics() { return savedPics; }
		void setPic(std::string name);
		BMP* getPic() { return &EditedPic; }
		std::string getImageName() { return imageName; }
		std::string getEditedImgName() { return "temp/" + imageName + lastEdit; }

		//gpu funcs
		void setupGPU_data();

		void clear_temp() {
			for (int i = 0; i < tempnames.size(); i++) {
				remove(("Pictures/" + tempnames.at(i)).c_str());
			}
		}

		void savePicture() {
			clear_temp();
			
			savedPics++;
			EditedPic.WriteToFile(std::string("Pictures/Final/Picture_" + std::to_string(savedPics) + ".bmp").c_str());
		}

		//edit functions
		void revert();

		//general function
		void cpu_farm(int calc);

		void greyify();
		void greyify_gpu();

		void negative();
		void negative_gpu();

		void cartoonify(int sharpness);
		void cartoonify_gpu(int sharpness);

		void RGBdominant();
		void RGBdominant_gpu();

		void pixelate(int pixelSize);
		void pixelate_gpu(int pixelsize);

		void black_rainbow();
		void black_rainbow_gpu();
		
		
};

