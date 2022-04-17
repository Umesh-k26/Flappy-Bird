#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>

using namespace std;
using namespace sf;
int main()
{
  srand(static_cast<unsigned>(time(0)));
  RenderWindow window(sf::VideoMode(1024, 768), "Flappy Bird!");
  window.setFramerateLimit(60);

  // Background
  Texture background;
  background.loadFromFile("sprites/background-day.png");
  Sprite BG(background);
  BG.setPosition(0, 0);

  // Bird
  Texture bird_still, bird_up, bird_down;
  bird_still.loadFromFile("sprites/bluebird-midflap.png", sf::IntRect(0, 2, 34, 24));
  bird_up.loadFromFile("sprites/bluebird-upflap.png", sf::IntRect(0, 2, 34, 24));
  bird_down.loadFromFile("sprites/bluebird-downflap.png", sf::IntRect(0, 2, 34, 24));
  Sprite Bird(bird_still);
  float x = 300;
  float y = 400;
  Bird.setPosition(x, y);

  // Sounds
  Sound sound;
  Sound sound1;
  Sound sound2;
  SoundBuffer wing;
  SoundBuffer swoosh;
  SoundBuffer die;
  SoundBuffer hit;
  wing.loadFromFile("audio/wing.ogg");
  swoosh.loadFromFile("audio/swoosh.ogg");
  die.loadFromFile("audio/die.ogg");
  hit.loadFromFile("audio/hit.ogg");

  // Score
  float currentscore = 0;
  Texture number;
  number.loadFromFile("sprites/0.png");
  Sprite zero(number);
  Texture number1;
  number1.loadFromFile("sprites/1.png");
  Sprite one(number1);
  Texture number2;
  number2.loadFromFile("sprites/2.png");
  Sprite two(number2);
  Texture number3;
  number3.loadFromFile("sprites/3.png");
  Sprite three(number3);
  Texture number4;
  number4.loadFromFile("sprites/4.png");
  Sprite four(number4);
  Texture number5;
  number5.loadFromFile("sprites/5.png");
  Sprite five(number5);
  Texture number6;
  number6.loadFromFile("sprites/6.png");
  Sprite six(number6);
  Texture number7;
  number7.loadFromFile("sprites/7.png");
  Sprite seven(number7);
  Texture number8;
  number8.loadFromFile("sprites/8.png");
  Sprite eight(number8);
  Texture number9;
  number9.loadFromFile("sprites/9.png");
  Sprite nine(number9);
  map<int, Sprite> m;
  m[0] = zero;
  m[1] = one;
  m[2] = two;
  m[3] = three;
  m[4] = four;
  m[5] = five;
  m[6] = six;
  m[7] = seven;
  m[8] = eight;
  m[9] = nine;
  for (int i = 0; i < 10; i++)
  {
    m[i].setPosition(0, 50);
  }

  // Game-over display
  Texture over;
  over.loadFromFile("sprites/gameover.png");
  Sprite gameover(over);

  // Pipes
  Texture pipe;
  pipe.loadFromFile("sprites/pipe-green.png", sf::IntRect(0, 4, 52, 768));

  // Game-over flag
  int f = 0;

  // base
  Texture base;
  base.loadFromFile("sprites/base.png");
  Sprite Base(base);
  Base.setPosition(0, 700);

  // Speeds
  float V_x = 1;
  float V_y = 0;
  // float a_x = 0;
  float gravity = 0.2f;

  // View
  View view1(FloatRect(0.f, 0.f, 1024.f, 768.f));
  view1.setCenter(512, 384);

  // Pipes vector
  vector<Sprite> pipes;

  // SpawnTimer
  int PipeSpawnTimer = 300;

  int l = 0;
  while (window.isOpen())
  {
    window.setView(view1);
    Event event;
    while (window.pollEvent(event))
    {
      if (event.type == Event::Closed)
        window.close();
      else if (event.type == Event::KeyPressed)
      {
        if (event.key.code == Keyboard::Up)
        {
          Bird.setTexture(bird_up);
          Bird.rotate(-45);
          sound.setBuffer(wing);
          sound.play();
          V_y = -10;
        }
      }
      else if (event.type == Event::KeyReleased)
      {
        if (event.key.code == Keyboard::Up)
        {
          Bird.setTexture(bird_still);
          Bird.rotate(45);
          V_y = 0;
          Bird.setRotation(0);
        }
      }
    }
    V_y = V_y + gravity;
    if (Bird.getPosition().y < 0.f)
      Bird.setPosition(Bird.getPosition().x, 0.f);

    if (Bird.getPosition().y >= 690.f)
    {
      f = 1;
      sound1.setBuffer(hit);
      sound1.play();
      Bird.move(Bird.getPosition().x, 690 - Bird.getPosition().x);
      sound2.setBuffer(die);
      sound2.play();
    }
    if (PipeSpawnTimer < 300)
      PipeSpawnTimer++;
    if (PipeSpawnTimer >= 300 && f == 0)
    {
      Sprite pup(pipe);
      float x = 730.f + Bird.getPosition().x;
      float y = 350.f + float(rand() % 150);

      pup.rotate(180);
      pup.setPosition(x, y - 40);
      pipes.push_back(pup);

      Sprite pd(pipe);
      pd.setPosition(x - 52, y + 80);
      pipes.push_back(pd);
      PipeSpawnTimer = 0;
      for (size_t i = 0; i != pipes.size(); i++)
      {
        if (Bird.getPosition().x - pipes[i].getPosition().x > 500)
        {
          pipes.erase(pipes.begin() + i);
        }
      }
    }
    for (size_t i = 0; i < pipes.size(); i = i + 2)
    {
      if (pipes[i].getPosition().x - 20 == Bird.getPosition().x)
      {
        currentscore++;
      }
    }

    int n = (int)currentscore;
    window.clear();

    window.draw(BG);
    window.draw(Bird);
    for (size_t i = 0; i < pipes.size(); i++)
    {
      window.draw(pipes[i]);
    }
    vector<int> temp;
    while (n > 0)
    {
      temp.push_back(n % 10);
      n = n / 10;
    }
    l = temp.size();
    if (currentscore > 0)
      for (int i = l - 1; i >= 0; i--)
      {
        m[temp[i]].setPosition(view1.getCenter().x + 24 * (l - 1 - i), 50);
        window.draw(m[temp[i]]);
      }
    if (currentscore == 0)
    {
      m[0].setPosition(view1.getCenter().x, 50);
      window.draw(m[0]);
    }
    if (f == 1)
    {
      gameover.setPosition(view1.getCenter());
      window.draw(gameover);
    }
    window.draw(Base);
    window.display();
    if (f == 0)
    {
      Bird.move(V_x, V_y);
      view1.move(V_x, 0);
      BG.move(V_x, 0);
      Base.move(V_x, 0);
      ;
    }
    if (f == 1)
    {
      Event e;
      while (window.pollEvent(e))
      {
        if (e.type == Event::KeyPressed)
        {
          if (e.key.code == Keyboard::Enter)
          {
            window.close();
            main();
          }
          if (e.key.code == Keyboard::Escape)
          {
            window.close();
          }
        }
      }
    }
    for (size_t i = 0; i < pipes.size(); i++)
    {
      if ((pipes[i].getGlobalBounds().intersects(Bird.getGlobalBounds())) && f == 0)
      {

        sound1.setBuffer(hit);
        sound1.play();
        // Bird.move(Bird.getPosition().x, 690-Bird.getPosition().x);
        sound2.setBuffer(die);
        sound2.play();
        f = 1;
      }
    }
  }

  return 0;
}
