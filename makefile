run			:	game.exe
			./game.exe

game.exe	:	main.cpp
			g++ -Wall -o game.exe main.cpp -lsfml-system -lsfml-window -lsfml-graphics -lsfml-network -lsfml-audio
clean		:
			rm -rf game.exe
