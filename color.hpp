#pragma once

#include <string>

class Color{
	public:
	int r;
	int g;
	int b;
	int a;
	int h;
	float s;
	float l;

	Color():r(0),g(0),b(0),a(0),h(0),s(0.0),l(0.0){};
	void GenerateHSLFromRGB();
	void GenerateRGBFromHSL();
	explicit Color(const std::string &argument);

	friend std::ostream& operator<<(std::ostream& os, const Color& color);
};
