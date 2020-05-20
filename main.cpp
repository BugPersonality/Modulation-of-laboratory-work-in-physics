#include  <SFML/Graphics.hpp>
#include <iostream>
#include <string>

using namespace std;
using namespace sf;
const float friction = 0.000001;

class Back_Groung
{
public:
    Texture texture;
    Sprite sprite;

    Back_Groung(String F)
    {
        texture.loadFromFile(F);
        Vector2u size = texture.getSize();
        sprite.setTexture(texture);
        sprite.setPosition(250, 250);
        sprite.setOrigin(size.x / 2, size.y / 2);
    }
};

class TEXT_SCORE
{
public:
    Font font;
    Text text;

    TEXT_SCORE(String F)
    {
        if (!font.loadFromFile(F))
        {
            std::cout << "Error loading file" << std::endl;

            system("pause");
        }

        text.setFont(font);
        text.setCharacterSize(15);
        text.setFillColor(sf::Color::Red);
        text.setStyle(sf::Text::Style::Bold | sf::Text::Style::Underlined);
        text.setOutlineColor(sf::Color::Yellow);
        text.setOutlineThickness(10);
    }
};

class Object
{
public:
    float x, y, w, h;
    float speed, mass;

    String File;
    Image image;
    Texture texture;
    Sprite sprite;

	Object(String F, float X, float Y, float W, float H, float _mass, float speed)
	{
        File = F;
        w = W; h = H;

        this->speed = speed;

        image.loadFromFile(File);

        image.createMaskFromColor(Color(0, 0, 0));
        texture.loadFromImage(image);
        sprite.setTexture(texture);

        x = X;
        y = Y;

        sprite.setPosition(x, y);
        sprite.setOrigin(w / 2, h / 2);

        mass = _mass;
	}

    void update(float time)
    {
        float dx = speed;
        float dy = 0;

        if (speed > 0)
        {
            x = (x + (dx * time)) - friction;
            y += dy * time;  
            speed -= friction;
        }
        else if (speed < 0)
        {
            x = (x + (dx * time)) + friction;
            y += dy * time;
            speed += friction;
        }
       
        sprite.setPosition(x, y);

        if (speed != 0)
        {
            if (speed > 0)
            {
                sprite.rotate(0.10);
            }
            else if (speed < 0)
            {
                sprite.rotate(-0.10);
            }
        }
        else
        {
            sprite.rotate(0);
        }
    }

    FloatRect getRect() 
    {
        return sprite.getGlobalBounds();
    }
};

int main() 
{
    int W = 500;
    int H = 500;

    float m_target, m_shell, start_speed;
    string modulation_type; //el or notel

    cout << "Modulation type: ";
    cin >> modulation_type;
    cout << endl << "Target mass: ";
    cin >> m_target;
    cout << endl << "Shell mass: ";
    cin >> m_shell;;
    cout << endl << "start_speed: ";
    cin >> start_speed;

    Clock clock;

    RenderWindow window(VideoMode(W, H), "lesson-3-3");

    Back_Groung background("lab_table.png");

    TEXT_SCORE shell_score("font_text_2.ttf");
    TEXT_SCORE target_score("font_text_2.ttf");

    Object target("boll_target.png", 250, 340, 50, 54, m_target, 0);
    Object shell("boll_shell.png", 96, 340, 50, 54, m_shell, start_speed);

    while (window.isOpen()) 
    {
        Event event;

        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        time = time / 800;
        
        while (window.pollEvent(event)) 
        {
            if (event.type == Event::Closed)
                window.close();
        }

        shell.update(time);
        target.update(time);

        if (target.speed == 0)
        {
            if (shell.getRect().intersects(target.getRect()))
            {
                if (modulation_type == "notel")
                {
                    target.speed = (shell.speed * shell.mass) / (shell.mass + target.mass);
                    shell.speed = target.speed;
                }
                else if (modulation_type == "el")
                {
                    target.speed = ((2 * shell.mass * shell.speed) / (shell.mass + target.mass));
                    shell.speed = ((shell.mass - target.mass) * shell.speed) / (shell.mass + target.mass);
                }
            }
        }

        float shell_pulse = shell.mass * shell.speed;
        float target_pulse = target.mass * target.speed;

        string s_speed = to_string(shell.speed);
        string t_speed = to_string(target.speed);
        
        string s_mass = to_string(shell.mass);
        string t_mass = to_string(target.mass);

        string s_pulse;
        string t_pulse;

        if (modulation_type == "notel")
        {
            string pulse = to_string((shell.mass + target.mass) * target.speed);
            s_pulse = pulse;
            t_pulse = pulse;
        }
        else if (modulation_type == "el")
        {
            s_pulse = to_string(shell_pulse);
            t_pulse = to_string(target_pulse);
        }

        shell_score.text.setString("Shell speed : " + s_speed + "\nShell mass : " + s_mass + "\nShell pulse : " + s_pulse);
        shell_score.text.setPosition(20, 20);

        target_score.text.setString("Target speed : " + t_speed + "\nTarget mass : " + t_mass + "\nTarget pulse : " + t_pulse);
        target_score.text.setPosition(250, 20);

        window.clear();
        
        window.draw(background.sprite);
        window.draw(shell.sprite);
        window.draw(target.sprite);
        window.draw(shell_score.text);
        window.draw(target_score.text);
        window.display();
    }
    return 0;
}