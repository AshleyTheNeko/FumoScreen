#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <iostream>
#include <stdio.h>

#include <X11/extensions/Xfixes.h>
#include <X11/extensions/shape.h>

float get_random(void)
{
    long random_int = rand();
    float random_float = (float)random_int / (float)RAND_MAX;
    return (random_float + 4);
}

int main()
{
    Display *d = XOpenDisplay(NULL);
    Window root = DefaultRootWindow(d);
    int default_screen = XDefaultScreen(d);

    XSetWindowAttributes attrs;
    attrs.override_redirect = True;

    XVisualInfo vinfo;
    if (!XMatchVisualInfo(d, DefaultScreen(d), 32, TrueColor, &vinfo)) {
        printf("No visual found supporting 32 bit color, terminating\n");
        exit(EXIT_FAILURE);
    }
    attrs.colormap = XCreateColormap(d, root, vinfo.visual, AllocNone);
    attrs.background_pixel = 0;
    attrs.border_pixel = 0;
    attrs.event_mask = 0;

    Window overlay = XCreateWindow(
        d, root, 0, 0, 1920, 1080, 0, vinfo.depth, InputOutput, vinfo.visual,
        CWOverrideRedirect | CWColormap | CWBackPixel | CWBorderPixel, &attrs);

    XRectangle rect;
    XserverRegion region = XFixesCreateRegion(d, &rect, 1);
    XFixesSetWindowShapeRegion(d, overlay, ShapeInput, 0, 0, region);
    XFixesDestroyRegion(d, region);

    XMapWindow(d, overlay);
    XFlush(d);

    // Show our windows
    sf::RenderWindow SFMLView(overlay);
    SFMLView.setFramerateLimit(60);

    sf::Music music;
    music.openFromFile("/home/ashley/.config/systemd/user/cirno.mp3");

    sf::Texture texture;
    texture.loadFromFile("/home/ashley/.config/systemd/user/cirno.png");
    sf::Sprite new_sprite(
        texture, sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(498, 498)));
    new_sprite.scale(sf::Vector2f(0.4, 0.4));
    sf::IntRect rect_tmp;
    sf::Clock move_clock;
    sf::Clock animate_clock;

    music.setLoop(true);
    music.play();

    srand(time(NULL));
    std::vector<sf::Sprite> sprites;
    std::vector<sf::Vector2f> moves;
    bool IsRunning = true;
    while (IsRunning) {
        SFMLView.clear(sf::Color::Transparent);

        if (move_clock.getElapsedTime().asSeconds() > 5) {
            sprites.push_back(new_sprite);
            moves.push_back(sf::Vector2f(get_random(), get_random()));
            move_clock.restart();
        }
        for (size_t i = 0; i < sprites.size(); i++) {
            if (sprites[i].getPosition().x + moves[i].x +
                        sprites[i].getGlobalBounds().width >=
                    1920 ||
                sprites[i].getPosition().x + moves[i].x <= 0)
                moves[i].x *= -1;
            if (sprites[i].getPosition().y + moves[i].y +
                        sprites[i].getGlobalBounds().height >=
                    1080 ||
                sprites[i].getPosition().y + moves[i].y <= 0)
                moves[i].y *= -1;
            if (animate_clock.getElapsedTime().asMilliseconds() >= 50) {
                rect_tmp = sprites[i].getTextureRect();
                rect_tmp.left = (rect_tmp.left + 498) % 5976;
                sprites[i].setTextureRect(rect_tmp);
            }
            sprites[i].move(moves[i]);
            SFMLView.draw(sprites[i]);
        }
        if (animate_clock.getElapsedTime().asMilliseconds() >= 50)
            animate_clock.restart();

        SFMLView.display();
    }
    music.stop();
    XUnmapWindow(d, overlay);
    XCloseDisplay(d);
}
