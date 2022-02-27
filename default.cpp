#include <SFML/Graphics.hpp>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <iostream>
#include <stdio.h>

#include <X11/extensions/Xfixes.h>
#include <X11/extensions/shape.h>

#define CORRECT_PATH(x)                                                        \
    std::string("/home/ashley/.config/systemd/user/assets/") + x

float get_random(void)
{
    long random_int = rand();
    float random_float = (float)random_int / (float)RAND_MAX;
    random_float += 4;
    random_float *= (rand() % 2 ? -1 : 1);
    return (random_float);
}

namespace fumo {
    struct texture {
        public:
            texture(size_t width, size_t height, size_t width_max,
                    std::string texture_path)
                : width(width), height(height), width_max(width_max)
            {
                _texture.loadFromFile(texture_path);
            };
            size_t width;
            size_t height;
            size_t width_max;
            sf::Texture _texture;
    };

    class sprite {
        public:
            sprite(fumo::texture &texture, sf::Vector2i &screen_size)
                : _texture(&texture)
            {
                _sprite = sf::Sprite(
                    texture._texture,
                    sf::IntRect(sf::Vector2i(0, 0),
                                sf::Vector2i(texture.width, texture.height)));
                _sprite.scale(sf::Vector2f(.4, .4));
                _sprite.setPosition(
                    sf::Vector2f(rand() % (screen_size.x - texture.width),
                                 rand() % (screen_size.y - texture.height)));
                _move = sf::Vector2f(get_random(), get_random());
            };

            void animate()
            {
                sf::IntRect rect_tmp = _sprite.getTextureRect();
                rect_tmp.left =
                    (rect_tmp.left + _texture->width) % _texture->width_max;
                _sprite.setTextureRect(rect_tmp);
            };

            void move(sf::Vector2i &screen_size)
            {
                if (_sprite.getPosition().x + _move.x +
                            _sprite.getGlobalBounds().width >=
                        screen_size.x ||
                    _sprite.getPosition().x + _move.x <= 0)
                    _move.x *= -1;
                if (_sprite.getPosition().y + _move.y +
                            _sprite.getGlobalBounds().height >=
                        screen_size.y ||
                    _sprite.getPosition().y + _move.y <= 0)
                    _move.y *= -1;
                _sprite.move(_move);
            };
            sf::Sprite &get_sprite(void) { return (_sprite); };

        private:
            fumo::texture *_texture;
            sf::Vector2f _move;
            sf::Sprite _sprite;
    };
}

Window get_overlay(Display *d, sf::Vector2i &screen_size)
{
    Window root = DefaultRootWindow(d);
    XSetWindowAttributes attrs;

    attrs.override_redirect = True;

    XVisualInfo vinfo;
    if (!XMatchVisualInfo(d, DefaultScreen(d), 32, TrueColor, &vinfo))
        exit(EXIT_FAILURE);
    attrs.colormap = XCreateColormap(d, root, vinfo.visual, AllocNone);
    attrs.background_pixel = 0;
    attrs.border_pixel = 0;
    attrs.event_mask = 0;

    Window overlay = XCreateWindow(
        d, root, 0, 0, screen_size.x, screen_size.y, 0, vinfo.depth,
        InputOutput, vinfo.visual,
        CWOverrideRedirect | CWColormap | CWBackPixel | CWBorderPixel, &attrs);

    XRectangle rect;
    XserverRegion region = XFixesCreateRegion(d, &rect, 1);
    XFixesSetWindowShapeRegion(d, overlay, ShapeInput, 0, 0, region);
    XFixesDestroyRegion(d, region);
    XMapWindow(d, overlay);
    XFlush(d);
    return (overlay);
}

int main()
{
    Display *d = XOpenDisplay(NULL);
    sf::Vector2i screen_size(XDisplayWidth(d, DefaultScreen(d)),
                             XDisplayHeight(d, DefaultScreen(d)));
    Window overlay = get_overlay(d, screen_size);

    sf::RenderWindow SFMLView(overlay);
    SFMLView.setFramerateLimit(60);

    sf::Clock spawn_clock;
    sf::Clock animate_clock;

    srand(time(NULL));
    std::vector<fumo::sprite> sprites;
    std::vector<fumo::texture> textures;
    textures.push_back(
        fumo::texture(498, 498, 5976, CORRECT_PATH("cirno.png")));
    textures.push_back(
        fumo::texture(498, 498, 13944, CORRECT_PATH("reimu.png")));
    textures.push_back(
        fumo::texture(498, 498, 7470, CORRECT_PATH("sakuya.png")));
    textures.push_back(
        fumo::texture(540, 540, 3780, CORRECT_PATH("remilia.png")));
    bool animate_fumo;
    while (true) {
        SFMLView.clear(sf::Color::Transparent);

        if (spawn_clock.getElapsedTime().asSeconds() > 10) {
            sprites.push_back(
                fumo::sprite(textures[rand() % textures.size()], screen_size));
            spawn_clock.restart();
        }
        animate_fumo = animate_clock.getElapsedTime().asMilliseconds() >= 50;

        for (auto iter = sprites.begin(); iter != sprites.end(); iter++) {
            iter->move(screen_size);
            if (animate_fumo)
                iter->animate();
            SFMLView.draw(iter->get_sprite());
        }
        if (animate_fumo)
            animate_clock.restart();

        SFMLView.display();
    }
    XUnmapWindow(d, overlay);
    XCloseDisplay(d);
    return (0);
}
