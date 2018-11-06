#include <iostream>
#include <SFML/Graphics.hpp>
#include <math.h>
using namespace std;
//VARIABLES DE LAS QUE CONSTA LA PISTA
int const widthscreen = 1024;
int const heightscreen = 768;
int width_seg = 200;
int pista = 2000;
//DISEÑO
float camd = 0.84;

class ejeH {

public:
    float x, y, z;
    float x1,y1,w;
    float scale,curve,cl;
    float spx;
    sf::Sprite spt;
    ejeH()
    {
        curve = spx = cl= 0;
        x = 0;
        y = 0;
        z = 0;
    }
    void DrawObj(sf::RenderWindow &app){
        sf::Sprite sprite = spt;
        int scalex = sprite.getTextureRect().width, scaley = sprite.getTextureRect().height;
        float newX = x1 + scale * spx * widthscreen/2;
        float newY = 8 + y1;
        float newW = scalex * w/166;
        float newH = scaley * w/166;
        newX += newW * spx;
        newY -= newH;
        float clH = (newW + newH) - cl;
        if(0>clH)clH = 0;
        if(newH<=clH)return;
        sprite.setTextureRect(sf::IntRect(0,0,scalex,scaley-scaley*clH/newH));
        sprite.setScale(newW/scalex,newH/scaley);
        sprite.setPosition(newX, newY);
        app.draw(sprite);
    }
    void cordenates_Scre(int camx , int camy , int camz)
    {
        scale = camd/(z - camz);
        x1 = (1 + scale*(x - camx)) * widthscreen/2;
        y1 = (1 - scale*(y - camy)) * heightscreen/2;
        w =  scale * pista * widthscreen/2;
    }
};

void draw_race(sf::RenderWindow &w,  sf::Color c, int x1, int y1, int wid1, int x2, int y2, int wid2)
{
    sf::ConvexShape shape(4);
    shape.setFillColor(c);
    shape.setPoint(0, sf::Vector2f(x1 - wid1, y1));
    shape.setPoint(1, sf::Vector2f(x2 - wid2, y2));
    shape.setPoint(2, sf::Vector2f(x2 + wid2, y2));
    shape.setPoint(3, sf::Vector2f(x1 + wid1, y1));
    w.draw(shape);
}

class Game{
public:
        int pos = 0,FPS = 60;
        int posx = 0;
        int aceleracion = 0;
        int freno = 1;
        int maxy;
        int cont = -1;

    int posxmove(vector<ejeH> lines){
        if(aceleracion!=0){
            cont ++;
            if(lines[cont].curve > 0.0)return -13;
            if(lines[cont].curve < 0.0)return 13;
            return 0;
        }
    }
    void Controls(sf::RenderWindow &app){
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) app.close();
        //if(sf::Keyboard::isKeyPressed(sf::Keyboard::Tab)) aceleracion = 350;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) aceleracion-=freno;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {if(aceleracion!=0)posx+=22;}
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {if(aceleracion!=0)posx-=22;}
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) aceleracion = 200;
    }

    void Run(sf::RenderWindow &app,int &N,vector<ejeH> &lines){

        while(app.isOpen())
        {
            app.setFramerateLimit(FPS);

            if (aceleracion <= 0)freno = 0;
            else freno = 2;
            pos += aceleracion; posx += posxmove(lines);

            while(pos >= N*width_seg) pos -= N*width_seg;
            while(pos < 0) pos += N*width_seg;

            sf::Event e;
            while(app.pollEvent(e))
            {
                if(e.type == sf::Event:: Closed)
                    app.close();
            }

            Controls(app);
            app.clear(sf::Color(0, 185, 179));

            int star_pos = pos/width_seg;
            int camH = 1500 + lines[star_pos].y;

            float x = 0,dx=0;
            int maxy = heightscreen;

            for( int n = star_pos;n  < star_pos + 300; n++ )
            {
                ejeH &l = lines[n%N];
                l.cordenates_Scre(posx - x,camH,star_pos*width_seg - (n>=N?N*width_seg:0));
                x+=dx;
                dx+=l.curve;

                sf::Color grass = (n/3)%2 ? sf::Color(16,200,16) : sf::Color (0,154,0);
                sf::Color rumble = (n/3)%2 ? sf::Color(255,255,255) : sf::Color  (255,0,0);
                sf::Color road = (n/3)%2 ? sf::Color(93,93,93) : sf::Color (89,89,89);
                sf::Color lad = (n/3)%2 ? sf::Color(255,255,255) : sf::Color (89,89,89);

                l.cl = maxy;
                if(l.y1 >= maxy) continue;
                maxy = l.y1;

                ejeH p = lines[(n - 1)% N];

                draw_race(app, grass,0,p.y1,widthscreen,0, l.y1,widthscreen);
                draw_race(app, rumble, p.x1,p.y1,p.w*1.2,l.x1,l.y1,l.w*1.2);
                draw_race(app, road,p.x1,p.y1 , p.w , l.x1,l.y1,l.w);
                draw_race(app, lad,p.x1,p.y1 , p.w/18 , l.x1,l.y1,l.w/18);

                //if(p.x1>posx && posx<p.x1+p.w*1.2)aceleracion = 100;

            }

            for( int n = star_pos+200;n>star_pos; n--)lines[n%N].DrawObj(app);
            app.display();
        }
    }
};
int main()
{
    Game Poly;
    sf::RenderWindow app(sf::VideoMode(widthscreen, heightscreen), "POOL POSITION");

    sf::Texture tex[50];
    sf::Sprite obj[50];
    for(int i=1;i<=7;i++){
        tex[i].loadFromFile("Images/n.png");
        tex[i].setSmooth(true);
        obj[i].setTexture(tex[i]);
    }

    vector<ejeH> lines;
    for(int i = 0; i < 3200; i++)
    {
        ejeH line;
        line.z = i * width_seg;

        if (i>300 && i<700) line.curve=1.5;
       if (i>1100 && i<1700) line.curve=-0.7;
       if (i>1700 && i<2300) line.curve=3.5;
       if (i>2600) line.curve=-2.5;

       if (i<300 && i%20==0) {line.spx=-2.5; line.spt=obj[5];}
       if (i%17==0)          {line.spx=2.0; line.spt=obj[6];}
       if (i>300 && i%20==0) {line.spx=-0.7; line.spt=obj[4];}
       if (i>800 && i%20==0) {line.spx=-3.2; line.spt=obj[1];}
       if (i==400)           {line.spx=-2.2; line.spt=obj[7];}

       if (i>750) line.y = sin(i/30.0)*1500;

        lines.push_back(line);
    }
    int N = lines.size();
    Poly.Run(app,N,lines);
    return 0;
}
