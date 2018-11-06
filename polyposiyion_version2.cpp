#include "Game.h"
#include <iostream>
#include <SFML/Graphics.hpp>

void Game::Lines(int R,int G, int B, sf::RenderWindow &app, float x,float y,float w,float h){
    sf::RectangleShape line(sf::Vector2f(w, h));
    line.setFillColor(sf::Color (R,G,B));
    line.setPosition(x,y);
    app.draw(line);
}

void Game::DrawRoad(sf::RenderWindow &app){
    float w = float(wscreen/6);
    float x = 0.0,y = float(hscreen)/3;
    float h = 0.1;
    float inc = w*2.5,prcp = 3.0,desplazo = 2.8;
    for(int lines=0;lines <= 2*hscreen/3;lines++){

        x += inc;
        h += 0.8;
        w += desplazo*1.1*h/2;

        (lines%3==0)?Lines(0,155,0,app,0,y,wscreen,h):Lines(0,255,0,app,0,y,wscreen,h);
        (lines%3==0)?Lines(255,255,255,app,x-prcp,y,w+prcp*2,h):Lines(255,0,0,app,x-prcp,y,w+prcp*2,h);
        (lines%3==0)?Lines(90,90,90,app,x-prcp/2+12,y,w+prcp-18,h):Lines(150,150,150,app,x-prcp/2+12,y,w+prcp-18,h);

        y += h;
        x -= inc;
        prcp += 2.0;
        inc -= desplazo*0.55*h/2;
    }

}
void Game::Run(){

    sf::RenderWindow app(sf::VideoMode(wscreen,hscreen),"POOL POSITION");

    while(app.isOpen()){

        app.setFramerateLimit(FPS);

        sf::Event e;
        while(app.pollEvent(e)){
                if(e.type == sf::Event:: Closed)
                    app.close();
            }
        app.clear(sf::Color(0, 185, 179));
        DrawRoad(app);
        app.display();
    }
}
