    #include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <list>
#include <math.h>


int main()
{
    float count1 = 0.0f;
    float count2 = 1.0f;
    float temp1;
    float temp2;    

    float stickAxis[8] = {0};

    const float dist = 300.0f;
    const int nr_sprites = 64;

    sf::Texture texture;
    if (!texture.loadFromFile("gfx/axeDouble2.png"))
    {
        printf("Unable to load graphics.\n");
        exit(0);
    }
    float tex_cx = texture.getSize().x/2;
    float tex_cy = texture.getSize().y/2;

    sf::Shader default_shader;
    if (!default_shader.loadFromFile("shaders/default.vert", "shaders/default.frag"))
    {
        printf("Unable to load shader - default.\n");
        exit(0);
    }

	sf::Shader bg_shader;
    if (!bg_shader.loadFromFile("shaders/fire.glsl", sf::Shader::Fragment))
    {
        printf("Unable to load shader - fire.\n");
        exit(0);
    }


    sf::Shader color_shader;
    if (!color_shader.loadFromFile("shaders/default.vert", "shaders/color.frag"))
    {
        printf("Unable to load shader - color.\n");
        exit(0);
    }
    //color_shader.setParameter("texture", sf::Shader::CurrentTexture);//

    sf::Shader transparent_shader;
    if (!transparent_shader.loadFromFile("shaders/default.vert", "shaders/transparent.frag"))
    {
        printf("Unable to load shader - transparent.\n");
        exit(0);
    }

    sf::Shader glow_shader;
    if (!glow_shader.loadFromFile("shaders/default.vert", "shaders/glow.frag"))
    {
        printf("Unable to load shader - glow.\n");
        exit(0);
    }

    sf::Shader blur_shader;
    if (!blur_shader.loadFromFile("shaders/default.vert", "shaders/blur.frag"))
    {
        printf("Unable to load shader - blur.\n");
        exit(0);
    }


    sf::Music music;
    if (!music.openFromFile("sfx/Jogeir Liljedahl - Chronicles.ogg"))
    {
        printf("Unable to load music.\n");
        exit(0);
    }

    std::vector<sf::Sprite> sprites;

    sf::Sprite  axe_sprite;
    axe_sprite.setTexture(texture);
    axe_sprite.setOrigin(sf::Vector2f(tex_cx, tex_cy));
    axe_sprite.setScale(sf::Vector2f(0.5f, 0.5f));

    for(int i = 0; i<nr_sprites; i++)
    {
        sprites.push_back(axe_sprite);
    }

//    sf::Sprite sprite;
//    sprite.setTexture(texture);
//    sprite.setOrigin(sf::Vector2f(tex_cx, tex_cy));
//    sprite.setScale(sf::Vector2f(0.5f, 0.5f));

    //sprite.move(sf::Vector2f(5.f, 10.f)); // offset relative to the current position
    // std::cout << sf::VideoMode::getDesktopMode().width << ", " << sf::VideoMode::getDesktopMode().height;

    // Creating a fullscreen window with the best video mode supported
    sf::RenderWindow window(sf::VideoMode(sf::VideoMode::getDesktopMode()), "SFML Window");
    //sf::RenderWindow window(sf::VideoMode(sf::VideoMode::getDesktopMode()), "SFML Window", sf::Style::Fullscreen);
    window.setVerticalSyncEnabled(true);
    float cx = window.getSize().x/2;
    float cy = window.getSize().y/2;

	sf::Texture bg_tex;
	bg_tex.create(window.getSize().x, window.getSize().y);
	sf::Sprite bg_spr(bg_tex);



	// Set the resolution parameter (the resoltion is divided to make the fire smaller)
	bg_shader.setParameter("resolution", sf::Vector2f(window.getSize().x, window.getSize().y));


    music.setLoop(true);
    music.play();

	sf::Clock clk;
	clk.restart(); // start the timer

    sf::CircleShape shape(100.f);
    shape.setPosition(20,20);
    shape.setOrigin(sf::Vector2f(20.f, 20.f));
    shape.setFillColor(sf::Color::Blue);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {

            switch (event.type)
            {
            case sf::Event::JoystickConnected:
                printf("Joystick connected. ID: %d\n", event.joystickConnect.joystickId);
                break;
            case sf::Event::JoystickDisconnected:
                printf("Joystick disconnected.\n");
                break;
            case sf::Event::KeyPressed:
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) )
                    window.close();
                break;
            case sf::Event::JoystickMoved:
                {                    
                    //printf( "Stick:%d axis:%d position:%f \n", event.joystickConnect.joystickId, event.joystickMove.axis, event.joystickMove.position );
                    stickAxis[event.joystickMove.axis] =  event.joystickMove.position;
                }
                break;
            case sf::Event::JoystickButtonPressed:
                printf( "Stick:%d button:%d pressed.\n", event.joystickConnect.joystickId, event.joystickButton.button);
                break;
            case sf::Event::JoystickButtonReleased:
                printf( "Stick:%d button:%d released.\n", event.joystickConnect.joystickId, event.joystickButton.button);
                break;
            case sf::Event::Closed:
                window.close();
                break;
            }
        }

   		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		float mx = mousePos.x;
        float my = mousePos.y;

        window.clear();

        bg_shader.setParameter("time", clk.getElapsedTime().asSeconds());
		bg_shader.setParameter("location", sf::Vector2f(cx, cy));

		window.draw(bg_spr, &bg_shader);

        window.draw(shape);

        temp1 = count1;
        temp2 = count2;
        float trans;
        // Update loop
        for (auto sprite : sprites) 
        {
            sprite.setPosition(cx + dist * sin(temp1) + dist * sin(temp2), cy + dist * cos(temp1)+ dist * cos(temp2) );
            sprite.setRotation( 90.0f * sin(count1) + 90.0f * sin(temp1) + 90.0f * sin(temp2) );
//            sprite.rotate(2.6f);
            temp1 = temp1 + 0.19f;
            temp2 = temp2 + 0.14f;
            trans =  0.5 + 0.49*cos(temp2);
            transparent_shader.setUniform("transparency", trans);//
            window.draw(sprite,&transparent_shader);

        }

        sf::Vertex line[] =
        {
            sf::Vertex(sf::Vector2f(cx, cy)),
            sf::Vertex(sf::Vector2f(mx, my))
        };
        window.draw(line, 2, sf::Lines);


        for (int axis = 0; axis <8;axis++) 
        {
            float position = stickAxis[axis];
            sf::RectangleShape axis_rect(sf::Vector2f(10 , position));
            axis_rect.setPosition(40 + 20 * axis, cy );
            window.draw(axis_rect);
        }

        window.display();

        count1 = count1 + 0.016;
        if (count1 > M_PI*2)
            {
                count1 = count1 - M_PI*2;
            }

        count2 = count2 + 0.021;
        if (count2 > M_PI*2)
            {
                count2 = count2 - M_PI*2;
            }


    }

    return 0;
}


/*


list<sf::Sprite>::iterator it=walls.begin(),next;
while(it!=walls.end()) {
    next = it; next++;
    if((it->getPosition().x == x) && (it->getPosition().y == y))
        walls.erase(it);
    it = next;

for(list<sf::Sprite>::iterator it = walls.begin(); it != walls.end(); it++)
    window.draw(*it);

}



*/