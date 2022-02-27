all:
	g++ fumo.cpp -l X11 -l sfml-window -l sfml-graphics -l sfml-system -lsfml-audio -l Xfixes -o fumo
