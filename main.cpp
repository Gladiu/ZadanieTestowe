#include <cstdint>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <algorithm>
#include <regex>

class Color{
	public:
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;

	Color():r(0),g(0),b(0),a(0){}
	Color(std::string argument){

		std::regex firstColorType("^([0-9a-f]{3}|[0-9a-f]{6}|[0-9a-f]{8})$"); // ff0000ff
		std::regex secondColorType("^([0-9]{1,3},){3}[0-9]{1,3}$"); // 255,0,0,255
		
		auto words_begin = std::sregex_iterator(argument.begin(), argument.end(), firstColorType);
		auto words_end = std::sregex_iterator();

		if(std::distance(words_begin, words_end)){
			std::smatch match = *words_begin;
			std::string matchStr = match.str();
			if (matchStr.size() == 3){
				r = strtoul(matchStr.substr(-1,1).c_str(), NULL, 16);
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
		words_begin = std::sregex_iterator(argument.begin(), argument.end(), secondColorType);

		if(std::distance(words_begin, words_end)){
			std::smatch match = *words_begin;
			std::string matchStr = match.str();
			std::vector<uint8_t> rgba;
			std::string number;
			std::stringstream matchStream(matchStr);

			while(std::getline(matchStream, number, ',')){
				rgba.push_back(std::stoi(number));
			}

			this->r = rgba[0];
			this->g = rgba[1];
			this->b = rgba[2];
			this->a = rgba[3];
		}
	}

};

Color GetAverageFromList(std::list<Color> colorList){
	Color returnColor;
	std::size_t colorCount = colorList.size();
	for (auto color : colorList){
		returnColor.r = color.r/colorCount;
		returnColor.g = color.g/colorCount;
		returnColor.b = color.b/colorCount;
		returnColor.a = color.a/colorCount;
	}
	return returnColor;
}


int main(int argc, char *argv[]){

	if (argc < 3){
		std::cerr<<"Programm needs -m MODE or --mode MODE to select operating mode"<<std::endl;
		return 1;
	}

	if (std::string(argv[1]) != "--mode" and std::string(argv[1]) != "-m"){
		std::cerr<<"Incorect argument provided"<<std::endl;
		return 1;
	}

	std::list<Color> colorData;

	// Reading data from file
	
	std::string filename = "colors.txt";
	std::fstream file(filename, std::fstream::in);

	if (!file.is_open()){
		std::cerr<<"Couldnt open colors.txt"<<std::endl;
		return 1;
	}
	
	std::string line;
	while(std::getline(file, line)){
		colorData.emplace_back(line);
	}

	// Reading data from arguments
	
	for (int i = 3; i < argc; i++){
		colorData.emplace_back(std::string(argv[i]));
	}


	for(auto color : colorData){
		std::cout<<static_cast<int>(color.r)<<std::endl;
		std::cout<<static_cast<int>(color.g)<<std::endl;
		std::cout<<static_cast<int>(color.b)<<std::endl;
		std::cout<<static_cast<int>(color.a)<<std::endl;
		std::cout<<"----------------"<<std::endl;
	}
	if (std::string(argv[2]) == "lowest"){ }
	if (std::string(argv[2]) == "mix"){}
	if (std::string(argv[2]) == "mix"){}
	if (std::string(argv[2]) == "mix"){}

	file.close();

	return 0;
}
