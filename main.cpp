#include <cstdint>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <algorithm>
#include <regex>

#include "color.hpp"

Color CreateAverageFromList(const std::list<Color> &colorList){
	Color returnColor;
	std::size_t colorCount = colorList.size();
	for (auto color : colorList){
		returnColor.r = color.r/colorCount;
		returnColor.g = color.g/colorCount;
		returnColor.b = color.b/colorCount;
		returnColor.a = color.a/colorCount;
	}

	returnColor.GenerateHSLFromRGB();
	return returnColor;
}

Color CreateFromLowestValues(const std::list<Color> &colorList){
	Color returnColor;
	returnColor.r = UINT8_MAX;
	returnColor.g = UINT8_MAX;
	returnColor.b = UINT8_MAX;
	returnColor.a = UINT8_MAX;
	for (auto color : colorList){
		returnColor.r = returnColor.r > color.r ? color.r : returnColor.r;
		returnColor.g = returnColor.g > color.g ? color.g : returnColor.g;
		returnColor.b = returnColor.b > color.b ? color.b : returnColor.b;
		returnColor.a = returnColor.a > color.a ? color.a : returnColor.a;
	}

	returnColor.GenerateHSLFromRGB();
	return returnColor;
}

Color CreateFromHighestValues(const std::list<Color> &colorList){

	Color returnColor;
	returnColor.r = 0;
	returnColor.g = 0;
	returnColor.b = 0;
	returnColor.a = 0;
	for (auto color : colorList){
		returnColor.r = returnColor.r < color.r ? color.r : returnColor.r;
		returnColor.g = returnColor.g < color.g ? color.g : returnColor.g;
		returnColor.b = returnColor.b < color.b ? color.b : returnColor.b;
		returnColor.a = returnColor.a < color.a ? color.a : returnColor.a;
	}
	returnColor.GenerateHSLFromRGB();
	return returnColor;
}

Color GetAverageSaturationFromList(const std::list<Color> &colorList){

	Color returnColor;
	float average = 0;
	std::size_t colorCount = colorList.size();
	for (auto color : colorList){
		average += color.s/colorCount;
	}
	returnColor.h = average;
	returnColor.l = 1;
	returnColor.s = 1;
	returnColor.GenerateRGBFromHSL();
	return returnColor;
}

int main(int argc, char *argv[]){

	std::list<Color> colorData;
	
	enum Mode {mix, lowest, highest, mix_saturate};
	Mode selectedMode = mix;


	// Reading data from file
	
	std::string filename = "colors.txt";
	std::fstream file(filename, std::fstream::in);

	if (file.is_open()){
		std::string line;
		while(std::getline(file, line)){
			colorData.emplace_back(line);
		}
		file.close();
	}
	else{
		std::cerr<<"Couldnt open colors.txt"<<std::endl;
	}
	

	// Reading data from arguments
	
	for (int i = 1; i < argc; i++){
		std::string currentArgument(argv[i]);
		if ((currentArgument == "-m" or currentArgument == "--mode") and (i + 1) < argc){
			std::string argumentMode(argv[i+1]);
			if (argumentMode == "mix") {
				selectedMode = mix;
				i++;
			}
			else if (argumentMode == "lowest") {
				selectedMode = lowest;
				i++;
			}
			else if (argumentMode == "highest") {
				selectedMode = highest;
				i++;
			}
			else if (argumentMode == "mix-saturate") {
				selectedMode = mix_saturate;
				i++;
			}
			else{
				selectedMode = mix;
				i++;
			}
		}
		else{
			colorData.emplace_back(std::string(argv[i]));
		}
	}


	if (colorData.size() == 0){
		std::cerr<<"You have to provide colors in form of a file or program arguments "<<std::endl;
		return 1;
	}

	switch(selectedMode){
		case mix:
			std::cout<<CreateAverageFromList(colorData)<<std::endl;
			break;
		case lowest:
			std::cout<<CreateFromLowestValues(colorData)<<std::endl;
			break;
		case highest:
			std::cout<<CreateFromHighestValues(colorData)<<std::endl;
			break;
		case mix_saturate:
			std::cout<<GetAverageSaturationFromList(colorData)<<std::endl;
			break;
		default:
			std::cout<<CreateAverageFromList(colorData)<<std::endl;
			break;

	}


	return 0;
}
