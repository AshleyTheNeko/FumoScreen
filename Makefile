all:
	g++ fumo.cpp -L lib -l X11 -l sfml-window -l sfml-graphics -l sfml-system -l Xfixes -o fumo -I includes