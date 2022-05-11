#include "color.hpp"

#include <cstdlib>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <algorithm>
#include <regex>

void Color::GenerateHSLFromRGB(){

	float fR = r / 255.0;
	float fG = g / 255.0;
	float fB = b / 255.0;
	float fCMin = std::min({ fR, fG, fB}, [](float numberA, float numberB) {
	      return numberA<numberB;
	});

	float fCMax = std::max({ fR, fG, fB}, [](float numberA, float numberB) {
	      return numberA<numberB;
	});


	l = 50 * (fCMin + fCMax);

	if (fCMin == fCMax) {
		s = 0;
		h = 0;
		return;

	}
	else if (l < 50) {
		s = 100 * (fCMax - fCMin) / (fCMax + fCMin);
	}
	else {
		s = 100 * (fCMax - fCMin) / (2.0 - fCMax - fCMin);
	}

	if (fCMax == fR) {
		h = 60 * (fG - fB) / (fCMax - fCMin);
	}
	if (fCMax == fG) {
		h = 60 * (fB - fR) / (fCMax - fCMin) + 120;
	}
	if (fCMax == fB) {
		h = 60 * (fR - fG) / (fCMax - fCMin) + 240;
	}
	if (h < 0) {
		h = h + 360;
	}
	l /= 100;
	s /= 100;
}

void Color::GenerateRGBFromHSL(){

	// https://en.wikipedia.org/wiki/HSL_and_HSV#To_RGB

	float c = (1 - std::abs(2*l - 1)) * s;
	int hprime = h / 60;
	float x = c * (1 - std::abs(hprime % 2 - 1));
	Color tempColor;
	if ( h > 0 and h < 1){
		tempColor.r = c;
		tempColor.g = x;
		tempColor.b = 0;
	}
	if ( h > 1 and h < 2){
		tempColor.r = x;
		tempColor.g = c;
		tempColor.b = 0;
	}
	if ( h > 2 and h < 3){
		tempColor.r = 0;
		tempColor.g = c;
		tempColor.b = x;
	}
	if ( h > 3 and h < 4){
		tempColor.r = 0;
		tempColor.g = x;
		tempColor.b = c;
	}
	if ( h > 4 and h < 5){
		tempColor.r = x;
		tempColor.g = 0;
		tempColor.b = c;
	}
	if ( h > 5 and h < 6){
		tempColor.r = c;
		tempColor.g = 0;
		tempColor.b = x;
	}

	float m = l - c/2;
	r = tempColor.r + m;
	g = tempColor.g + m;
	b = tempColor.b + m;

}
Color::Color(const std::string &argument){

	std::regex firstColorType("^([0-9a-f]{3}|[0-9a-f]{6}|[0-9a-f]{8})$"); // ff0000ff
	std::regex secondColorType("^([0-9]{1,3},){3}[0-9]{1,3}$"); // 255,0,0,255
	
	auto wordsBeginFirstMatch = std::sregex_iterator(argument.begin(), argument.end(), firstColorType);
	auto wordsEndFirstMatch = std::sregex_iterator();

	auto wordsBeginSecondMatch = std::sregex_iterator(argument.begin(), argument.end(), secondColorType);
	auto wordsEndSecondMatch = std::sregex_iterator();

	if(std::distance(wordsBeginFirstMatch, wordsEndFirstMatch) != 0){
		std::smatch match = *wordsBeginFirstMatch;
		std::string matchStr = match.str();
		if (matchStr.size() == 3){
			r = strtoul(matchStr.substr(0,1).c_str(), NULL, 16);
			g = strtoul(matchStr.substr(1,1).c_str(), NULL, 16);
			b = strtoul(matchStr.substr(2,1).c_str(), NULL, 16);
			a = 255;
		}
		if (matchStr.size() == 6){
			r = strtoul(matchStr.substr(0,2).c_str(), NULL, 16);
			g = strtoul(matchStr.substr(2,2).c_str(), NULL, 16);
			b = strtoul(matchStr.substr(4,2).c_str(), NULL, 16);
			a = 255;
		}
		if (matchStr.size() == 8){
			r = strtoul(matchStr.substr(0,2).c_str(), NULL, 16);
			g = strtoul(matchStr.substr(2,2).c_str(), NULL, 16);
			b = strtoul(matchStr.substr(4,2).c_str(), NULL, 16);
			a = strtoul(matchStr.substr(6,2).c_str(), NULL, 16);
		}
	}


	if(std::distance(wordsBeginSecondMatch, wordsEndSecondMatch)){
		std::smatch match = *wordsBeginSecondMatch;
		std::string matchStr = match.str();
		std::vector<int> rgba;
		std::string number;
		std::stringstream matchStream(matchStr);

		while(std::getline(matchStream, number, ',')){
			rgba.push_back(std::stoi(number));
		}

		r = rgba[0];
		g = rgba[1];
		b = rgba[2];
		a = rgba[3];
	}

	if(!std::distance(wordsBeginFirstMatch, wordsEndFirstMatch)
	   and !std::distance(wordsBeginSecondMatch, wordsEndSecondMatch)){
		std::cerr<<"Invalid Color Format: "<<argument<<std::endl;
	}
	GenerateHSLFromRGB();
}

std::ostream& operator<<(std::ostream& os, const Color& color){
	os<<"Red: "<<color.r<<std::endl;
	os<<"Green: "<<color.g<<std::endl;
	os<<"Blue: "<<color.b<<std::endl;
	os<<"Alpha: "<<color.a<<std::endl;
	os<<"Hex: "<<"#"<<std::hex<<color.r<<" "<<color.g<<" "<<color.b<<" "<<color.a<<std::endl;
	os<<"Hue: "<<color.h<<std::endl;
	os<<"Saturation: "<<color.s<<std::endl;
	os<<"Lightness: "<<color.l<<std::endl;
	return os;
}
