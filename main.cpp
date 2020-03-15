#include <fstream>
#include <iostream>
#include <vector>
#include "File.cpp"
#include <iomanip>
#pragma once

using namespace std;


File load(ifstream& file) {

	//ifstream& file <- previous parameter
	/*
	ifstream file(directory, ios::binary | ios::in);
	if (file.is_open() != true) {

		cout << "-1" << endl;

	}
	*/

	File loadedFile;
	//proper way to do this
	file.read((char*)& loadedFile.header.IDlength, 1);
	file.read((char*)& loadedFile.header.colorMapType, 1);
	file.read((char*)& loadedFile.header.imageType, 1);
	file.read((char*)& loadedFile.header.colorMapOrigin, 2);
	file.read((char*)& loadedFile.header.colorMapLength, 2);
	file.read((char*)& loadedFile.header.colorMapDepth, 1);
	file.read((char*)& loadedFile.header.xOrigin, 2);
	file.read((char*)& loadedFile.header.yOrigin, 2);
	file.read((char*)& loadedFile.header.imageWidth, 2);
	file.read((char*)& loadedFile.header.imageHeight, 2);
	file.read((char*)& loadedFile.header.pixelDepth, 1);
	file.read((char*)& loadedFile.header.imageDescriptor, 1);

	loadedFile.header.numOfPixels = loadedFile.header.imageWidth * loadedFile.header.imageHeight; //determines the amount of pixels we need

	//READ PIXELS
	for (int i = 0; i < loadedFile.header.numOfPixels; i++) {

		File::Pixel pixel;
		file.read((char*)& pixel.blue, 1);
		file.read((char*)& pixel.green, 1);
		file.read((char*)& pixel.red, 1);

		loadedFile.pixels.push_back(pixel);
	}
	return loadedFile;
	//file.close(); 
}

void writeFile(File file, string output) {

	ofstream outFile(output, ios::out | ios::binary);

	outFile.write((char*)& file.header.IDlength, 1);
	outFile.write((char*)& file.header.colorMapType, 1);
	outFile.write((char*)& file.header.imageType, 1);
	outFile.write((char*)& file.header.colorMapOrigin, 2);
	outFile.write((char*)& file.header.colorMapLength, 2);
	outFile.write((char*)& file.header.colorMapDepth, 1);
	outFile.write((char*)& file.header.xOrigin, 2);
	outFile.write((char*)& file.header.yOrigin, 2);
	outFile.write((char*)& file.header.imageWidth, 2);
	outFile.write((char*)& file.header.imageHeight, 2);
	outFile.write((char*)& file.header.pixelDepth, 1);
	outFile.write((char*)& file.header.imageDescriptor, 1);

	for (int i = 0; i < file.header.numOfPixels; i++) {
		outFile.write((char*)& file.pixels[i].blue, 1);
		outFile.write((char*)& file.pixels[i].green, 1);
		outFile.write((char*)& file.pixels[i].red, 1);
	}
}

void compare(string file1, string file2) {


	ifstream first(file1, ios::in | ios::binary);
	File one = load(first);
	ifstream second(file2, ios::in | ios::binary);
	File two = load(second);


	bool matches = true;

	if ((one.header.imageWidth != two.header.imageWidth) || one.header.imageHeight != two.header.imageHeight) {

		matches == false;
		cout << "Image length and width do not match." << endl;

	}
	else if ((one.header.imageWidth == two.header.imageWidth) && (one.header.imageHeight == two.header.imageHeight)) {

		for (int i = 0; i < one.header.numOfPixels; i++) {
			if (one.pixels[i].blue != two.pixels[i].blue) {
				matches == false;
				cout << "Blue Channel does not match." << endl;
			}
			if (one.pixels[i].green != two.pixels[i].green) {
				matches == false;
				cout << "Green Channel does not match." << endl;
			}
			if (one.pixels[i].red != two.pixels[i].red) {
				matches == false;
				cout << "Red Channel does not match." << endl;
			}
		}
	}

	if (matches == true) {

		cout << "Files match!" << endl;

	}
	else  if (matches == false) {

		cout << "Files do not match." << endl;

	}
}

File multiply(File layer, File pattern) {

	File result;

	//chooses the header of one of the images and sets it equal to the header of Result
	result.header.IDlength = layer.header.IDlength;
	result.header.colorMapType = layer.header.colorMapType;
	result.header.imageType = layer.header.imageType;
	result.header.colorMapOrigin = layer.header.colorMapOrigin;
	result.header.colorMapLength = layer.header.colorMapLength;
	result.header.colorMapDepth = layer.header.colorMapDepth;
	result.header.xOrigin = layer.header.xOrigin;
	result.header.yOrigin = layer.header.yOrigin;
	result.header.imageWidth = layer.header.imageWidth;
	result.header.imageHeight = layer.header.imageHeight;
	result.header.pixelDepth = layer.header.pixelDepth;
	result.header.imageDescriptor = layer.header.imageDescriptor;
	result.header.numOfPixels = layer.header.numOfPixels;


	for (int i = 0; i < layer.header.numOfPixels; i++) {

		File::Pixel multiplied;
		File::Pixel layerPixel = layer.pixels[i];
		File::Pixel patternPixel = pattern.pixels[i];


		float blue1 = layerPixel.blue / 255.0f;
		float blue2 = patternPixel.blue / 255.0f;

		float green1 = layerPixel.green / 255.0f;
		float green2 = patternPixel.green / 255.0f;

		float red1 = layerPixel.red / 255.0f; //this normalizes the value
		float red2 = patternPixel.red / 255.0f;

		multiplied.blue = (unsigned char)(((blue1 * blue2) * 255.0f) + 0.5f);
		multiplied.green = (unsigned char)(((green1 * green2) * 255.0f) + 0.5f);
		multiplied.red = (unsigned char)(((red1 * red2) * 255.0f) + 0.5f); //de-normalizes this and rounds it properly

		result.pixels.push_back(multiplied);
	}
	return result;

}

File subtract(File topLayer, File bottomLayer) {

	File result;

	//chooses the header of one of the images and sets it equal to the header of Result
	result.header.IDlength = topLayer.header.IDlength;
	result.header.colorMapType = topLayer.header.colorMapType;
	result.header.imageType = topLayer.header.imageType;
	result.header.colorMapOrigin = topLayer.header.colorMapOrigin;
	result.header.colorMapLength = topLayer.header.colorMapLength;
	result.header.colorMapDepth = topLayer.header.colorMapDepth;
	result.header.xOrigin = topLayer.header.xOrigin;
	result.header.yOrigin = topLayer.header.yOrigin;
	result.header.imageWidth = topLayer.header.imageWidth;
	result.header.imageHeight = topLayer.header.imageHeight;
	result.header.pixelDepth = topLayer.header.pixelDepth;
	result.header.imageDescriptor = topLayer.header.imageDescriptor;
	result.header.numOfPixels = topLayer.header.numOfPixels;

	for (int i = 0; i < result.header.numOfPixels; i++) {

		File::Pixel subtracted;

		float blue = (float)bottomLayer.pixels[i].blue - (float)topLayer.pixels[i].blue;
		//(unsigned char)((((bottomLayer.pixels[i].blue / 255.0f) - (topLayer.pixels[i].blue / 255.0f)) * 255.0f) + 0.5f);

		if (blue < 0) {
			subtracted.blue = 0.0f;
		}
		else if (blue > 255.0f) {
			subtracted.blue = 255.0f;
		}
		else {
			subtracted.blue = (unsigned char)blue;
		}

		float green = (float)bottomLayer.pixels[i].green - (float)topLayer.pixels[i].green;
		//(unsigned char)((((bottomLayer.pixels[i].blue / 255.0f) - (topLayer.pixels[i].blue / 255.0f)) * 255.0f) + 0.5f);

		if (green < 0) {
			subtracted.green = 0.0f;
		}
		else if (green > 255.0f) {
			subtracted.green = 255.0f;
		}
		else {
			subtracted.green = (unsigned char)green;
		}

		float red = (float)bottomLayer.pixels[i].red - (float)topLayer.pixels[i].red;
		//(unsigned char)((((bottomLayer.pixels[i].blue / 255.0f) - (topLayer.pixels[i].blue / 255.0f)) * 255.0f) + 0.5f);

		if (red < 0) {
			subtracted.red = 0.0f;
		}
		else if (red > 255.0f) {
			subtracted.red = 255.0f;
		}
		else {
			subtracted.red = (unsigned char)red;
		}
		result.pixels.push_back(subtracted);
	}
	return result;
}

File screen(File topLayer, File bottomLayer) {

	File result;

	//chooses the header of one of the images and sets it equal to the header of Result
	result.header.IDlength = topLayer.header.IDlength;
	result.header.colorMapType = topLayer.header.colorMapType;
	result.header.imageType = topLayer.header.imageType;
	result.header.colorMapOrigin = topLayer.header.colorMapOrigin;
	result.header.colorMapLength = topLayer.header.colorMapLength;
	result.header.colorMapDepth = topLayer.header.colorMapDepth;
	result.header.xOrigin = topLayer.header.xOrigin;
	result.header.yOrigin = topLayer.header.yOrigin;
	result.header.imageWidth = topLayer.header.imageWidth;
	result.header.imageHeight = topLayer.header.imageHeight;
	result.header.pixelDepth = topLayer.header.pixelDepth;
	result.header.imageDescriptor = topLayer.header.imageDescriptor;
	result.header.numOfPixels = topLayer.header.numOfPixels;

	for (int i = 0; i < result.header.numOfPixels; i++) {

		File::Pixel screened;
		File::Pixel topPixel = topLayer.pixels[i];
		File::Pixel bottomPixel = bottomLayer.pixels[i];//do this for all colors and every pixels


		float blue1 = topPixel.blue / 255.0f;
		float blue2 = bottomPixel.blue / 255.0f;

		float green1 = topPixel.green / 255.0f;
		float green2 = bottomPixel.green / 255.0f;

		float red1 = topPixel.red / 255.0f; //this normalizes the value
		float red2 = bottomPixel.red / 255.0f;

		screened.blue = (unsigned char)(((1 - (1 - blue1) * (1 - blue2)) * 255.0f) + 0.5f);
		screened.green = (unsigned char)(((1 - (1 - green1) * (1 - green2)) * 255.0f) + 0.5f);
		screened.red = (unsigned char)(((1 - (1 - red1) * (1 - red2)) * 255.0f) + 0.5f);

		result.pixels.push_back(screened);
	}
	return result;
}

File overlay(File topLayer, File bottomLayer) {

	File result;

	//chooses the header of one of the images and sets it equal to the header of Result
	result.header.IDlength = topLayer.header.IDlength;
	result.header.colorMapType = topLayer.header.colorMapType;
	result.header.imageType = topLayer.header.imageType;
	result.header.colorMapOrigin = topLayer.header.colorMapOrigin;
	result.header.colorMapLength = topLayer.header.colorMapLength;
	result.header.colorMapDepth = topLayer.header.colorMapDepth;
	result.header.xOrigin = topLayer.header.xOrigin;
	result.header.yOrigin = topLayer.header.yOrigin;
	result.header.imageWidth = topLayer.header.imageWidth;
	result.header.imageHeight = topLayer.header.imageHeight;
	result.header.pixelDepth = topLayer.header.pixelDepth;
	result.header.imageDescriptor = topLayer.header.imageDescriptor;
	result.header.numOfPixels = topLayer.header.numOfPixels;


	for (int i = 0; i < result.header.numOfPixels; i++) {

		File::Pixel overlayed;
		File::Pixel topPixel = topLayer.pixels[i];
		File::Pixel bottomPixel = bottomLayer.pixels[i];


		float blue1 = topPixel.blue / 255.0f;
		float blue2 = bottomPixel.blue / 255.0f;

		if (blue2 <= 0.5f) {

			float newBlue = 2 * blue1 * blue2;
			overlayed.blue = (unsigned char)((newBlue * 255.0f) + 0.5f);

		}
		else if (blue2 > 0.5f) {

			float newBlue = 1 - (2 * ((1 - blue1) * (1 - blue2)));
			overlayed.blue = (unsigned char)((newBlue * 255.0f) + 0.5f);
		}


		float red1 = topPixel.red / 255.0f;
		float red2 = bottomPixel.red / 255.0f;

		if (red2 <= 0.5f) {

			float newRed = 2 * red1 * red2;
			overlayed.red = (unsigned char)((newRed * 255.0f) + 0.5f);

		}
		else if (red2 > 0.5f) {

			float newRed = 1 - (2 * ((1 - red1) * (1 - red2)));
			overlayed.red = (unsigned char)((newRed * 255.0f) + 0.5f);
		}



		float green1 = topPixel.green / 255.0f;
		float green2 = bottomPixel.green / 255.0f;

		if (green2 <= 0.5f) {

			float newGreen = 2 * green1 * green2;
			overlayed.green = (unsigned char)((newGreen * 255.0f) + 0.5f);

		}
		else if (green2 > 0.5f) {

			float newGreen = 1 - (2 * ((1 - green1) * (1 - green2)));
			overlayed.green = (unsigned char)((newGreen * 255.0f) + 0.5f);
		}


		result.pixels.push_back(overlayed);
	}
	return result;
}

File taskSix(File image) {

	File result;

	result.header.IDlength = image.header.IDlength;
	result.header.colorMapType = image.header.colorMapType;
	result.header.imageType = image.header.imageType;
	result.header.colorMapOrigin = image.header.colorMapOrigin;
	result.header.colorMapLength = image.header.colorMapLength;
	result.header.colorMapDepth = image.header.colorMapDepth;
	result.header.xOrigin = image.header.xOrigin;
	result.header.yOrigin = image.header.yOrigin;
	result.header.imageWidth = image.header.imageWidth;
	result.header.imageHeight = image.header.imageHeight;
	result.header.pixelDepth = image.header.pixelDepth;
	result.header.imageDescriptor = image.header.imageDescriptor;
	result.header.numOfPixels = image.header.numOfPixels;

	for (int i = 0; i < result.header.numOfPixels; i++) {

		File::Pixel greened;

		greened.red = image.pixels[i].red;
		greened.blue = image.pixels[i].blue;

		float newGreen = (float)image.pixels[i].green + 200.0f;
		if (newGreen > 255.0f) {
			greened.green = (unsigned char)255;
		}
		else {
			greened.green = (unsigned char)newGreen;
		}

		result.pixels.push_back(greened);
	}

	return result;

}

File taskSeven(File image) {

	File result;

	result.header.IDlength = image.header.IDlength;
	result.header.colorMapType = image.header.colorMapType;
	result.header.imageType = image.header.imageType;
	result.header.colorMapOrigin = image.header.colorMapOrigin;
	result.header.colorMapLength = image.header.colorMapLength;
	result.header.colorMapDepth = image.header.colorMapDepth;
	result.header.xOrigin = image.header.xOrigin;
	result.header.yOrigin = image.header.yOrigin;
	result.header.imageWidth = image.header.imageWidth;
	result.header.imageHeight = image.header.imageHeight;
	result.header.pixelDepth = image.header.pixelDepth;
	result.header.imageDescriptor = image.header.imageDescriptor;
	result.header.numOfPixels = image.header.numOfPixels;

	for (int i = 0; i < result.header.numOfPixels; i++) {

		File::Pixel changed;

		changed.green = image.pixels[i].green;

		changed.blue = 0;
		//(image.pixels[i].blue * 0);

		float newRed = (float)image.pixels[i].red;

		float newRed2 = newRed * 4.0f;

		if (newRed2 > 255.0f) {

			changed.red = 255;

		}
		else if (newRed2 < 0) {

			changed.red = 0;

		}
		else {

			changed.red = (unsigned char)newRed2;

		}

		result.pixels.push_back(changed);

	}

	return result;

}

File taskEight1(File image) {

	File result;

	result.header.IDlength = image.header.IDlength;
	result.header.colorMapType = image.header.colorMapType;
	result.header.imageType = image.header.imageType;
	result.header.colorMapOrigin = image.header.colorMapOrigin;
	result.header.colorMapLength = image.header.colorMapLength;
	result.header.colorMapDepth = image.header.colorMapDepth;
	result.header.xOrigin = image.header.xOrigin;
	result.header.yOrigin = image.header.yOrigin;
	result.header.imageWidth = image.header.imageWidth;
	result.header.imageHeight = image.header.imageHeight;
	result.header.pixelDepth = image.header.pixelDepth;
	result.header.imageDescriptor = image.header.imageDescriptor;
	result.header.numOfPixels = image.header.numOfPixels;

	for (int i = 0; i < result.header.numOfPixels; i++) {

		File::Pixel blueP;

		blueP.blue = image.pixels[i].blue;
		blueP.green = image.pixels[i].blue;
		blueP.red = image.pixels[i].blue;

		result.pixels.push_back(blueP);

	}
	return result;
}

File taskEight2(File image) {

	File result;

	result.header.IDlength = image.header.IDlength;
	result.header.colorMapType = image.header.colorMapType;
	result.header.imageType = image.header.imageType;
	result.header.colorMapOrigin = image.header.colorMapOrigin;
	result.header.colorMapLength = image.header.colorMapLength;
	result.header.colorMapDepth = image.header.colorMapDepth;
	result.header.xOrigin = image.header.xOrigin;
	result.header.yOrigin = image.header.yOrigin;
	result.header.imageWidth = image.header.imageWidth;
	result.header.imageHeight = image.header.imageHeight;
	result.header.pixelDepth = image.header.pixelDepth;
	result.header.imageDescriptor = image.header.imageDescriptor;
	result.header.numOfPixels = image.header.numOfPixels;

	for (int i = 0; i < result.header.numOfPixels; i++) {

		File::Pixel greenP;

		greenP.blue = image.pixels[i].green;
		greenP.green = image.pixels[i].green;
		greenP.red = image.pixels[i].green;

		result.pixels.push_back(greenP);

	}
	return result;
}

File taskEight3(File image) {

	File result;

	result.header.IDlength = image.header.IDlength;
	result.header.colorMapType = image.header.colorMapType;
	result.header.imageType = image.header.imageType;
	result.header.colorMapOrigin = image.header.colorMapOrigin;
	result.header.colorMapLength = image.header.colorMapLength;
	result.header.colorMapDepth = image.header.colorMapDepth;
	result.header.xOrigin = image.header.xOrigin;
	result.header.yOrigin = image.header.yOrigin;
	result.header.imageWidth = image.header.imageWidth;
	result.header.imageHeight = image.header.imageHeight;
	result.header.pixelDepth = image.header.pixelDepth;
	result.header.imageDescriptor = image.header.imageDescriptor;
	result.header.numOfPixels = image.header.numOfPixels;

	for (int i = 0; i < result.header.numOfPixels; i++) {

		File::Pixel redP;

		redP.blue = image.pixels[i].red;
		redP.green = image.pixels[i].red;
		redP.red = image.pixels[i].red;

		result.pixels.push_back(redP);

	}
	return result;
}

File taskNine(File image1, File image2, File image3) {

	File result;

	result.header.IDlength = image1.header.IDlength;
	result.header.colorMapType = image1.header.colorMapType;
	result.header.imageType = image1.header.imageType;
	result.header.colorMapOrigin = image1.header.colorMapOrigin;
	result.header.colorMapLength = image1.header.colorMapLength;
	result.header.colorMapDepth = image1.header.colorMapDepth;
	result.header.xOrigin = image1.header.xOrigin;
	result.header.yOrigin = image1.header.yOrigin;
	result.header.imageWidth = image1.header.imageWidth;
	result.header.imageHeight = image1.header.imageHeight;
	result.header.pixelDepth = image1.header.pixelDepth;
	result.header.imageDescriptor = image1.header.imageDescriptor;
	result.header.numOfPixels = image1.header.numOfPixels;

	for (int i = 0; i < result.header.numOfPixels; i++) {

		File::Pixel together;

		together.red = image1.pixels[i].red;
		together.green = image2.pixels[i].green;
		together.blue = image3.pixels[i].blue;

		result.pixels.push_back(together);
	}
	return result;

}

File taskTen(File text) {


	File result;

	result.header.IDlength = text.header.IDlength;
	result.header.colorMapType = text.header.colorMapType;
	result.header.imageType = text.header.imageType;
	result.header.colorMapOrigin = text.header.colorMapOrigin;
	result.header.colorMapLength = text.header.colorMapLength;
	result.header.colorMapDepth = text.header.colorMapDepth;
	result.header.xOrigin = text.header.xOrigin;
	result.header.yOrigin = text.header.yOrigin;
	result.header.imageWidth = text.header.imageWidth;
	result.header.imageHeight = text.header.imageHeight;
	result.header.pixelDepth = text.header.pixelDepth;
	result.header.imageDescriptor = text.header.imageDescriptor;
	result.header.numOfPixels = text.header.numOfPixels;

	for (int i = text.header.numOfPixels - 1; i >= 0; i--) {

		File::Pixel flipped;

		flipped.blue = text.pixels[i].blue;
		flipped.red = text.pixels[i].red;
		flipped.green = text.pixels[i].green;

		result.pixels.push_back(flipped);
	}

	return result;
}

int main() {

	//~~~~~~~~~~~~~~~~~~~~~~~LOAD TEST~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	/*
	ifstream file("input/car.tga", ios_base::in | ios_base::binary);
	if (!file) {
		cout << -1 << endl;
	}
	File printFile = load(file);
	cout << printFile.header.imageHeight << endl;
	cout << printFile.header.imageWidth << endl;

	writeFile(printFile,"output/newCar.tga");

	compare("output/newCar.tga", "input/car.tga");
	*/


	/*----Test 1----*/


	ifstream layer("input/layer1.tga", ios::binary | ios::in);
	ifstream pattern("input/pattern1.tga", ios::binary | ios::in);

	File layer1 = load(layer);
	File pattern1 = load(pattern);
	File taskOneImage = multiply(layer1, pattern1);
	writeFile(taskOneImage, "output/part1.tga");

	compare("output/part1.tga", "examples/EXAMPLE_part1.tga");

	layer.close();
	pattern.close();



	/*----Test 2----*/


	ifstream twoLayer("input/layer2.tga", ios::binary | ios::in);
	ifstream car("input/car.tga", ios::binary | ios::in);

	File twoLayer1 = load(twoLayer);
	File car1 = load(car);
	File taskTwoImage = subtract(twoLayer1, car1);
	writeFile(taskTwoImage, "output/part2.tga");

	compare("output/part2.tga", "examples/EXAMPLE_part2.tga");
	twoLayer.close();
	car.close();


	/*----Test 3----*/

	ifstream threeLayer("input/layer1.tga", ios::binary | ios::in);
	ifstream threePattern("input/pattern2.tga", ios::binary | ios::in);

	File threeLayer1 = load(threeLayer);
	File threePattern1 = load(threePattern);
	File taskThreeImage1 = multiply(threeLayer1, threePattern1);
	writeFile(taskThreeImage1, "output/part3.tga");
	threeLayer.close();
	threePattern.close();

	ifstream previousMultiply("output/part3.tga", ios::binary | ios::in);
	ifstream threeText("input/text.tga", ios::binary | ios::in);
	File threeText1 = load(threeText);
	File threePrevious = load(previousMultiply);
	File taskThreeImage2 = screen(threeText1, threePrevious);
	writeFile(taskThreeImage2, "output/part3.tga");

	compare("output/part3.tga", "examples/EXAMPLE_part3.tga");

	previousMultiply.close();
	threeText.close();

	/*----Test 4----*/


	ifstream fourLayer("input/layer2.tga", ios::binary | ios::in);
	ifstream circles("input/circles.tga", ios::binary | ios::in);

	File fourLayer1 = load(fourLayer);
	File circles1 = load(circles);
	File taskFourImage1 = multiply(fourLayer1, circles1);
	writeFile(taskFourImage1, "output/part4.tga");
	fourLayer.close();
	circles.close();

	ifstream prev("output/part4.tga", ios::binary | ios::in);
	ifstream p2("input/pattern2.tga", ios::binary | ios::in);
	File fourPrev = load(prev);
	File p2Top = load(p2);
	File taskFourImage2 = subtract(p2Top, fourPrev);
	writeFile(taskFourImage2, "output/part4.tga");

	compare("output/part4.tga", "examples/EXAMPLE_part4.tga");

	prev.close();
	p2.close();


	/*----Test 5----*/

	ifstream fiveLayer("input/layer1.tga", ios::binary | ios::in);
	ifstream fivePattern("input/pattern1.tga", ios::binary | ios::in);

	File fiveLayer1 = load(fiveLayer);
	File fivePattern1 = load(fivePattern);
	File taskFiveImage = overlay(fiveLayer1, fivePattern1);
	writeFile(taskFiveImage, "output/part5.tga");

	compare("output/part5.tga", "examples/EXAMPLE_part5.tga");

	fiveLayer.close();
	fivePattern.close();


	/*----Test 6----*/

	ifstream sixCars("input/car.tga", ios::binary | ios::in);
	File sixCars1 = load(sixCars);
	File taskSixImage = taskSix(sixCars1);
	writeFile(taskSixImage, "output/part6.tga");

	compare("output/part6.tga", "examples/EXAMPLE_part6.tga");

	sixCars.close();


	/*----Test 7----*/

	ifstream sevenCars("input/car.tga", ios::binary | ios::in);
	File sevenCars1 = load(sevenCars);

	File taskSevenImage = taskSeven(sevenCars1);
	writeFile(taskSevenImage, "output/part7.tga");

	compare("output/part7.tga", "examples/EXAMPLE_part7.tga");
	sevenCars.close();


	/*----Test 8----*/

	ifstream eightCar("input/car.tga", ios::binary | ios::in);
	File eightCars1 = load(eightCar);

	File taskEightImage1 = taskEight1(eightCars1);
	File taskEightImage2 = taskEight2(eightCars1);
	File taskEightImage3 = taskEight3(eightCars1);
	writeFile(taskEightImage1, "output/part8_b.tga");
	writeFile(taskEightImage2, "output/part8_g.tga");
	writeFile(taskEightImage3, "output/part8_r.tga");


	compare("output/part8_b.tga", "examples/EXAMPLE_part8_b.tga");
	compare("output/part8_g.tga", "examples/EXAMPLE_part8_g.tga");
	compare("output/part8_r.tga", "examples/EXAMPLE_part8_r.tga");
	eightCar.close();


	/*----Test 9----*/


	ifstream layerRed("input/layer_red.tga", ios::binary | ios::in);
	ifstream layerGreen("input/layer_green.tga", ios::binary | ios::in);
	ifstream layerBlue("input/layer_blue.tga", ios::binary | ios::in);

	File layerRed1 = load(layerRed);
	File layerGreen1 = load(layerGreen);
	File layerBlue1 = load(layerBlue);

	File taskNineImage = taskNine(layerRed1, layerGreen1, layerBlue1);
	writeFile(taskNineImage, "output/part9.tga");

	compare("output/part9.tga", "examples/EXAMPLE_part9.tga");
	layerRed.close();
	layerGreen.close();
	layerBlue.close();


	/*----Test 10----*/

	ifstream text("input/text2.tga", ios::binary | ios::in);
	File text1 = load(text);

	File taskTenImage = taskTen(text1);
	writeFile(taskTenImage, "output/part10.tga");

	compare("output/part10.tga", "examples/EXAMPLE_part10.tga");
	text.close();

}
