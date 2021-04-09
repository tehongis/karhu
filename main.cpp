    #include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <list>
#include <math.h>


// float num1 = -gravity * ( 2 * mass1 + mass2) * sin(angle1);
// float num2 = -mass2 * gravity * sin(angle1-2*angle2);
// float num3 = -2 * sin(angle1 - angle2) * mass2;
// float num4 = (velocity2*velocity2)*length2+(velocity1*velocity1)*lenght1*cos(angle1-angle2);
// float den = lenght1 * (2*mass1+mass2-mass2*cos(2*angle1-2*angle2))
// acceleration1 = (num1+num2+num3*num4) / den;

// float num1 = 2 * sin(angle1-angle2);
// float num2 = ((velocity1*velocity1)*lenght1*(mass1+mass2));
// float num3 = gravity * (mass1 + mass2) * cos(angle1);
// float num4 = (velocity2 * velocity2) * lenght2 * mass2 *cos(angle1-angle2);
// float den = lenght2 * (2*mass1+mass2-mass2*cos(2*angle1-2*angle2))
// acceleration1 = num1*(num2+num3+num4) / den;

// https://youtu.be/uWzPe_S-RVE?t=1065

class pendulum {

    float length;
    float angle;
    float mass;
    float velocity;
    float acceleration;

    public:
        pendulum (float l, float a, float m) {
            length = l;
            angle = a;
            mass = m;
        }
    
        float getEndX() {
            float end_x = length * sin(angle);
            return (end_x);
        }

        float getEndY() {
            float end_y = length * cos(angle); 
            return (end_y);
        }

        void update() {


        }


};

int main()
{
    float count1 = 0.0f;
    float count2 = 1.0f;
    float temp1;
    float temp2;    

    pendulum pend1(20.0f,45.0f,10.0f);

    float stickAxis[8] = {0};

    sf::Joystick::Identification jId;
    std::string stickname;

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

	sf::Shader colorslide_shader;
    if (!colorslide_shader.loadFromFile("shaders/colorslide.glsl", sf::Shader::Fragment))
    {
        printf("Unable to load shader - colorslide.\n");
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

    sf::CircleShape shape(120.f);
    shape.setPosition(60,cy-60);
    shape.setOrigin(sf::Vector2f(60.f, 60.f));
    shape.setFillColor(sf::Color::Blue);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {

            switch (event.type)
            {
            case sf::Event::JoystickConnected:
                {
                int stickid = event.joystickConnect.joystickId;
                jId = sf::Joystick::getIdentification(stickid);
                stickname = jId.name;
                printf("%s connected. ID: %d\n", stickname,stickid);
                break;
                }
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

        int lx = cx + int(30 * pend1.getEndX());
        int ly = cy + int(30 * pend1.getEndY());

        sf::Vertex pline[] =
        {
            sf::Vertex(sf::Vector2f(cx, cy)),
            sf::Vertex(sf::Vector2f(lx, ly))
        };
        window.draw(pline, 2, sf::Lines);

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