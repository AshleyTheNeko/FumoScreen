all:
	g++ fumo.cpp -l X11  -l Xfixes -L lib  -l sfml-window -l sfml-graphics -l sfml-system -o fumo -I includes