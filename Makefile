all:
	g++ fumo.cpp -L lib -l X11 -l Xfixes -l sfml-window -l sfml-graphics -l sfml-system -o fumo -I includes