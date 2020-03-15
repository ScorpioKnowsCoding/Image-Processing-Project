#include <fstream>
#include <vector>
#include <iostream>
#include <iomanip>
using namespace std;

#pragma once

//put all in a source file!!! it'll be easier


struct File {

	struct Pixel {

		unsigned char blue = 0;
		unsigned char green = 0;
		unsigned char red = 0;

		//initialization constructor

		Pixel() {

			unsigned char blue = 0;
			unsigned char green = 0;
			unsigned char red = 0;
			//int rowIndex = 0;
			//int columnIndex = 0;
		}

		/*
		Pixel(unsigned char blue, unsigned char green, unsigned char red) {
			//read rest of the file and make a new pixel for each one

			this->blue = blue;
			this->green = green;
			this->red = red;

		}
		*/

		//operator==() //compare pixels

	};

	struct Header {
		//Header INFO:
		char IDlength = 0;
		char colorMapType = 0;
		char imageType = 0;

		//Color Map Specification
		short colorMapOrigin = 0; //never changes
		short colorMapLength = 0; //never changes
		char colorMapDepth = 0; //never changes

		//Image Specification

		short xOrigin = 0; //never changes
		short yOrigin = 0; //never changes
		short imageWidth = 0;
		short imageHeight = 0;
		char pixelDepth = 0;
		char imageDescriptor = 0;

		int numOfPixels = 0;
		//Pixel* pixels = new Pixel[numOfPixels];

		Header() {

			char IDlength = 0;
			char colorMapType = 0;
			char imageType = 0;

			//Color Map Specification
			short colorMapOrigin = 0; //never changes
			short colorMapLength = 0; //never changes
			char colorMapDepth = 0; //never changes

			//Image Specification

			short xOrigin = 0; //never changes
			short yOrigin = 0; //never changes
			short imageWidth = 0;
			short imageHeight = 0;
			char pixelDepth = 0;
			char imageDescriptor = 0;

			int numOfPixels = 0;

			//write assignment operator? 

		}

		//initialization constructor;
		Header(char ID, char colorMap, char image, short origin, short length, char depth, short x, short y, short width, short height, char pDepth, char descriptor, ifstream& file) {

			IDlength = ID;
			colorMapType = colorMap;
			imageType = image;

			colorMapOrigin = origin; //never changes
			colorMapLength = length; //never changes
			colorMapDepth = depth; //never changes

			xOrigin = x; //never changes
			yOrigin = y; //never changes
			imageWidth = width;
			imageHeight = height;
			pixelDepth = pDepth;
			imageDescriptor = descriptor;
			//unsigned int count;

			numOfPixels = imageWidth * imageHeight;

			//insert a file, open it, read it, store the header data

		}

	};

	vector<Pixel> pixels;

	Header header;

	/*
	File() {

		Header header;

	}
	*/
};