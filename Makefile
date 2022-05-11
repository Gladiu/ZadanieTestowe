.PHONY default: main.cpp color.cpp
	g++ main.cpp color.cpp -o main -Wall -Wextra -fsanitize=address
