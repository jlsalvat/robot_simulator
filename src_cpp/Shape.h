#pragma once
#include <iostream>
#include <vector>
#include <ostream>
#include <string>
#include <SDL2/SDL.h>
#include <cmath>
#include "../src_cpp/Graphics.h"
#include "../src_cpp/Utils.h"
#include "../src_cpp/Parameters.h"
#include <algorithm>
// Define Infinite (Using INT_MAX caused overflow problems) 
#define INF 10000 

namespace sides {
	enum Side {
		TOP,
		BOTTOM,
		LEFT,
		RIGHT,
		NONE
	};

	inline Side getOppositeSide(Side side) {
		return
			side == TOP ? BOTTOM :
			side == BOTTOM ? TOP :
			side == LEFT ? RIGHT :
			side == RIGHT ? LEFT :
			NONE;
	}
}

struct Color {
	int red;
	int green;
	int blue;
	int alpha;
	Color(int r, int g, int b, int a = 255) :red(r), green(g), blue(b), alpha(a) {}
	Color() :Color(0, 0, 0, 0) {}
};


//classe mère
struct Shape {
	int x, y; //position de la forme
	int width, height;
	Color color;
	Shape(int pos_x, int pos_y, int w, int h, Color c) :x(pos_x), y(pos_y), width(w), height(h), color(c) {}
	Shape(Color c) :Shape(0, 0, 0, 0, c) {}
	Shape(int pos_x = 0, int pos_y = 0, int w = 0, int h=0 ) :x(pos_x), y(pos_y), width(w), height(h), color() {}

	virtual void draw(const Graphics & graphics) {
		_sdl_renderer = graphics.gRenderer();
		SDL_SetRenderDrawColor(_sdl_renderer, color.red, color.green, color.blue, color.alpha);
	}
protected:
	SDL_Renderer *_sdl_renderer;
};


struct Rectangle : public Shape {
	Rectangle(Rectangle * rect):Shape(rect->x, rect->y, rect->width, rect->height, rect->color){}
	Rectangle(const Rectangle  &rect) :Shape(rect.x, rect.y, rect.width, rect.height, rect.color) {}
//	Rectangle(SDL_Rect rect, Color c) : Shape(rect.x, rect.y, rect.w , rect.h, c) {}
//	Rectangle(SDL_Rect rect) : Shape(rect.x, rect.y, rect.w, rect.h) {}
	Rectangle(int pos_x, int pos_y, int w, int h, Color c) : Shape(pos_x, pos_y, w, h, c) {}
	Rectangle(int pos_x, int pos_y, int w, int h) :Shape(pos_x, pos_y, w, h) {}
	Rectangle( ) :Rectangle(0,0,0,0) {}
	const int getCenterX() const { return this->x + this->width / 2; }
	const int getCenterY() const { return this->y + this->height / 2; }
	const int getLeft() const { return this->x; }
	const int getRight() const { return this->x + this->width; }
	const int getTop() const { return this->y; }
	const int getBottom() const { return this->y + this->height; }
	//afin de limiter les erreurs d'intersection on diminue la surface à tester
	const int getMinLeft() const { return this->x+2; }
	const int getMinRight() const { return this->x + this->width-2; }
	const int getMinTop() const { return this->y+2; }
	const int getMinBottom() const { return this->y + this->height-2; }
	const int getSide(const sides::Side side) const {
		return
			side == sides::LEFT ? this->getLeft() :
			side == sides::RIGHT ? this->getRight() :
			side == sides::TOP ? this->getTop() :
			side == sides::BOTTOM ? this->getBottom() :
			sides::NONE;
	}
	//bool collidesWith
	//Takes in another Rectangle and checks if the two are colliding
	const bool collidesWith(const Rectangle &other) const {
		return
			this->getRight()  >   other.getMinLeft() &&
			this->getMinLeft() < other.getRight() &&
			this->getMinTop() < other.getBottom() &&
			this->getBottom() > other.getMinTop(); //on diminue la taille du rectangle à tester de 1
	}
	const bool intersect(const Rectangle &other) const {
		return
			this->getRight() >= other.getLeft() &&
			this->getLeft() <= other.getRight() &&
			this->getTop() <= other.getBottom() &&
			this->getBottom() >= other.getTop(); //on diminue la taille du rectangle à tester de 1
	}
	//this est la tuile et other est le rectangle à l'intérieur
	const bool isInside(const Rectangle &other) const {
		int iGetTop = this->getTop();
		int minTop = other.getMinTop();
		int iGetBottom = this->getBottom();
		int minBottom = other.getMinBottom();
		int iGetLeft= this->getLeft();
		int minLeft= other.getMinLeft();
		int iGetRigth = this->getRight();
		int minright = other.getMinRight();
		return
			this->getRight() > other.getMinRight() && this->getTop() < other.getMinTop() // en haut à droite
			&& this->getRight() > other.getMinRight() && this->getBottom() > other.getMinBottom() // en bas à droite
			&& this->getLeft() < other.getMinLeft() && this->getTop() < other.getMinTop() // en haut à gauche
			&& this->getLeft() < other.getMinLeft() && this->getBottom() > other.getMinBottom(); // en bas à gauche
	}

	const bool isValidRectangle() const {
		return (this->x >= 0 && this->y >= 0 && this->width >= 0 && this->height >= 0);
	}

	const inline SDL_Rect getRect() const {
		return SDL_Rect{ x,y,width,height };
	}

	virtual void draw(const Graphics & graphics) {
		Shape::draw(graphics);
		SDL_Rect r;
		r.x = x;
		r.y = y;
		r.w = width;
		r.h = height;
		SDL_RenderFillRect(_sdl_renderer, &r);
	}

	//Side getCollisionSide
	//Determine which side the collision happened on
	const sides::Side getCollisionSide(Rectangle &other) const;
	const int getCollisionValue(const SDL_Point &p1, const SDL_Point &p2) const;
};




struct Polyline {
	Color color;
	std::vector<SDL_Point*> points;
	SDL_Point  corner_left, corner_right;
	int indice_position_left, indice_position_right;
	Polyline(Color c):color(c) {}
	Polyline(int pos_x, int pos_y, Color c) : color(c) { points.push_back(new SDL_Point{ pos_x,pos_y }); }
	Polyline(int pos_x, int pos_y) :color(){ points.push_back(new SDL_Point{ pos_x,pos_y }); }
	Polyline(std::vector<SDL_Point*> p, Color c):color(c) {
			for (int i = 0; i < p.size(); i++) {
				SDL_Point * tmp_point = new SDL_Point;
				tmp_point->x= p[i]->x;
				tmp_point->y = p[i]->y;
				points.push_back(tmp_point);
		}
	}
	virtual void draw(const Graphics & graphics) {
		_sdl_renderer = graphics.gRenderer();
		SDL_SetRenderDrawColor(_sdl_renderer, color.red, color.green, color.blue, color.alpha);
		if (color.alpha != 0) // if no color is choose in tmx objectgroup -> don't display Polyline
		for (int i = 0; i < points.size()-1; i++) {  
			drawLine(points[i]->x, points[i]->y,points[i+1]->x, points[i+1]->y);
		}
		//on reboucle
		drawLine(points[points.size() - 1]->x, points[points.size() - 1]->y, points[0]->x, points[0]->y);
	}
	void add_point(SDL_Point *p) {
		points.push_back(p);
	}
	void add_point(int x, int y) {
		SDL_Point *p = new SDL_Point{ x,y };
		points.push_back(p);
	}
	void printLines() {
		for (int i = 0; i < points.size()-1; i++) {
			std::cout << points[i]->x << "," << points[i]->y << "->" << points[i+1]->x << "," << points[i+1]->y<< "\n";
		}
	}
	void substract(const int &x, const int &y) {
		for (int i = 0; i < points.size(); i++) {
			points[i]->x -= x;
			points[i]->y -= y;
		}

	}
	void erase_all() {
		for (int i = 0; i < points.size(); i++)
			delete points[i];
		points.clear();
	}
	void move(int x, int y) {
		for (int i = 0; i < points.size(); i++) {
			points[i]->x += x;
			points[i]->y +=y;
		}

	}
	~Polyline() {
		//erase_all();
	}
	virtual void findCorners() {
		xmax = xmin = points[0]->x;
		ymax = ymin = points[0]->y;
//première passe on cherches les 4 valeurs min et max en x et y
		for (int i = 1; i<points.size(); i++) {
			if (points[i]->x > xmax)
				xmax = points[i]->x;
			if (points[i]->y > ymax)
				ymax = points[i]->y;
			if (points[i]->x < xmin)
				xmin = points[i]->x;
			if (points[i]->y <ymin)
				ymin = points[i]->y;
		}
//deuxième passe on cherche les bords droit, gauche haut et bas de la polyligne
		for (int i = 0; i<points.size(); i++) {
			if (points[i]->x == xmax && points[i]->y == ymin) {
				corner_right.x = points[i]->x;
				corner_right.y = points[i]->y;
				indice_position_right = i;
			}
			else if (points[i]->x == xmax && points[i]->y == ymax) {
				corner_right.x = points[i]->x;
				corner_right.y = points[i]->y;
				indice_position_right = i;
			}
			if (points[i]->x == xmin && points[i]->y == ymin) {
				corner_left.x = points[i]->x;
				corner_left.y = points[i]->y;
				indice_position_left = i;
			}
			else if (points[i]->x == xmin && points[i]->y == ymax) {
				corner_left.x = points[i]->x;
				corner_left.y = points[i]->y;
				indice_position_left = i;
			}
		}
	}
	//return Rectangle where Polyline is included
	Rectangle getRectangle(Color c) {
		if(corner_left.y<corner_right.y)// point en haut à gauche
			return Rectangle(corner_left.x, corner_left.y, corner_right.x - corner_left.x, corner_right.y - corner_left.y,c);
		else
			return Rectangle(corner_left.x, corner_right.y, corner_right.x - corner_left.x, corner_left.y - corner_right.y, c);
	}
	const sides::Side getCollisionSide(Rectangle &other) const;
private :
	int xmax, ymax, xmin, ymin;

	SDL_Renderer *_sdl_renderer;
	void drawLine(int x0, int y0, int x1, int y1);
};

struct Cross : public Shape {
	Cross(Color c) :Shape(c) {
		for (int i = 0; i < 4; i++) {
			SDL_Point tmp_point;
			points.push_back(tmp_point);
		}
		points_base.push_back({ -2,0 });
		points_base.push_back({ 0,2 });
		points_base.push_back({ 2,0 });
		points_base.push_back({ 0,-2 });
	}
	Cross() :Cross({ 255,255,255,255 }) {}

	Rectangle getRectangle() {
		Rectangle rect(x,y,width,height);
		return rect;
	}

	void update(int x, int y) {
		for (int i = 0; i < points.size(); i++) {
			points[i].x = points_base[i].x + x;
			points[i].y = points_base[i].y  + y;
		}
		this->x = points[0].x;
		this->y = points[3].y;
		width = points[2].x - points[0].x;
		height = points[1].y - points[3].y;
	}
	//testé OK
    const float collidesWith(const Rectangle &rect) const {
		//attention les coordonnées sont à partir du haut gauche
		SDL_Point p_left, p_right, p_top, p_bottom;//points extrèmes de la croix
		SDL_Point p_left_top, p_left_bottom, p_right_top, p_right_bottom;//points diagonales
		int value_inside = 0;
		int indice = 0;
		float cpt_values_inside = 0;
		static int first = 1;
		p_left = points[0];
		p_right = points[2];
		p_bottom= points[1];
		p_top = points[3];
		p_right_top = { p_right.x - 1,p_right.y - 1 };
		p_right_bottom = { p_right.x - 1,p_right.y + 1 };
		p_left_top = { p_left.x + 1,p_right.y - 1 };
		p_left_bottom = { p_left.x + 1,p_left.y + 1 };
		//a l'intérieur : les 14 points, c'est a dire 5 verticaux, 5 horizontaux et 4 diagonaux sont à l'intérieur du rect
		if (rect.getTop() < p_top.y && rect.getBottom() > p_bottom.y && rect.getRight() > p_right.x && rect.getLeft() < p_left.x)
			value_inside = 13;
		else {//sinon
			//  *
			// ***
			//*****
			// ***
			//  *
			//
			//test point extreme à gauche
			if (rect.getTop() < p_left.y && rect.getBottom() > p_left.y && rect.getRight() > p_left.x && rect.getLeft() < p_left.x)
				value_inside++;
			//test colonne à gauche de 3 points
			if (rect.getTop() > p_left_top.y && rect.getTop() < p_left_bottom.y  && rect.getLeft() < p_left_top.x && rect.getRight() > p_left_top.x)
				value_inside += p_left_bottom.y - rect.getTop() ;
			if (rect.getBottom() < p_left_bottom.y && rect.getBottom() > p_left_top.y   && rect.getLeft() < p_left_top.x && rect.getRight() > p_left_top.x)
				value_inside += rect.getBottom() - p_left_top.y ;
			if (rect.getRight() > p_left_top.x && 	rect.getLeft() < p_left_top.x && rect.getTop() < p_left_top.y && rect.getBottom() > p_left_bottom.y)
				value_inside += 3;
			//test colonne au centre de 5 points
			if (rect.getTop() > p_top.y && rect.getTop() < p_bottom.y  && rect.getLeft() < p_top.x && rect.getRight() > p_top.x)
				value_inside += p_bottom.y - rect.getTop() ;
			if (rect.getBottom() < p_bottom.y && rect.getBottom() > p_top.y   && rect.getLeft() < p_top.x && rect.getRight() > p_top.x)
				value_inside += rect.getBottom() - p_top.y + first;
			if (rect.getRight() > p_top.x && 	rect.getLeft() < p_top.x && rect.getTop() < p_top.y && rect.getBottom() > p_bottom.y)
				value_inside += 5;
			//test colonne à droite de 3 points
			if (rect.getTop() > p_right_top.y && rect.getTop() < p_right_bottom.y  && rect.getLeft() < p_right_top.x && rect.getRight() > p_right_top.x)
				value_inside += p_left_bottom.y - rect.getTop() ;
			if (rect.getBottom() < p_right_bottom.y && rect.getBottom() > p_right_top.y   && rect.getLeft() < p_right_top.x && rect.getRight() > p_right_top.x)
				value_inside += rect.getBottom() - p_left_top.y ;
			if (rect.getRight() > p_right_top.x && 	rect.getLeft() < p_right_top.x && rect.getTop() < p_right_top.y && rect.getBottom() > p_right_bottom.y)
				value_inside += 3;
			//test point extreme à droite
			if (rect.getTop() < p_right.y && rect.getBottom() > p_right.y && rect.getRight() > p_right.x && rect.getLeft() < p_right.x)
				value_inside++;
		}
		return (float)value_inside;
	}
	//testé OK
	const float collidesWithBorder(const Rectangle &rect) const {
		//attention les coordonnées sont à partir du haut gauche
		SDL_Point p_left, p_right, p_top, p_bottom;//points extrèmes de la croix
		SDL_Point p_left_top, p_left_bottom, p_right_top, p_right_bottom;//points diagonales
		int value_inside = 0;
		int indice = 0;
		float cpt_values_inside = 0;
		static int first = 1;
		p_left = points[0];
		p_right = points[2];
		p_bottom = points[1];
		p_top = points[3];
		p_right_top = { p_right.x - 1,p_right.y - 1 };
		p_right_bottom = { p_right.x - 1,p_right.y + 1 };
		p_left_top = { p_left.x + 1,p_right.y - 1 };
		p_left_bottom = { p_left.x + 1,p_left.y + 1 };
		//a l'intérieur : les 14 points, c'est a dire 5 verticaux, 5 horizontaux et 4 diagonaux sont à l'intérieur du rect
		if (rect.getTop() < p_top.y && rect.getBottom() > p_bottom.y && rect.getRight() > p_right.x && rect.getLeft() < p_left.x)
			value_inside = 13;
		else {//sinon
			//  *
			// ***
			//*****
			// ***
			//  *
			//
			//test point extreme à gauche
			if (rect.getTop() <= p_left.y && rect.getBottom() >= p_left.y && rect.getRight() >= p_left.x && rect.getLeft() <= p_left.x)
				value_inside++;
			//test colonne à gauche de 3 points
			if (rect.getTop() >= p_left_top.y && rect.getTop() <= p_left_bottom.y  && rect.getLeft() <= p_left_top.x && rect.getRight() >= p_left_top.x)
				value_inside += p_left_bottom.y - rect.getTop();
			if (rect.getBottom() <= p_left_bottom.y && rect.getBottom() >= p_left_top.y   && rect.getLeft() <= p_left_top.x && rect.getRight() >= p_left_top.x)
				value_inside += rect.getBottom() - p_left_top.y;
			if (rect.getRight() >= p_left_top.x && 	rect.getLeft() <= p_left_top.x && rect.getTop() <= p_left_top.y && rect.getBottom() >= p_left_bottom.y)
				value_inside += 3;
			//test colonne au centre de 5 points
			if (rect.getTop() >= p_top.y && rect.getTop() <= p_bottom.y  && rect.getLeft() <= p_top.x && rect.getRight() >= p_top.x)
				value_inside += p_bottom.y - rect.getTop();
			if (rect.getBottom() <= p_bottom.y && rect.getBottom() >= p_top.y   && rect.getLeft() <= p_top.x && rect.getRight() >= p_top.x)
				value_inside += rect.getBottom() - p_top.y + first;
			if (rect.getRight() >= p_top.x && 	rect.getLeft() <= p_top.x && rect.getTop() <= p_top.y && rect.getBottom() >= p_bottom.y)
				value_inside += 5;
			//test colonne à droite de 3 points
			if (rect.getTop() >= p_right_top.y && rect.getTop() <= p_right_bottom.y  && rect.getLeft() <= p_right_top.x && rect.getRight() >= p_right_top.x)
				value_inside += p_left_bottom.y - rect.getTop();
			if (rect.getBottom() <= p_right_bottom.y && rect.getBottom() >= p_right_top.y   && rect.getLeft() <= p_right_top.x && rect.getRight() >= p_right_top.x)
				value_inside += rect.getBottom() - p_left_top.y;
			if (rect.getRight() >= p_right_top.x && 	rect.getLeft() <= p_right_top.x && rect.getTop() <= p_right_top.y && rect.getBottom() >= p_right_bottom.y)
				value_inside += 3;
			//test point extreme à droite
			if (rect.getTop() <= p_right.y && rect.getBottom() >= p_right.y && rect.getRight() >= p_right.x && rect.getLeft() <= p_right.x)
				value_inside++;
		}
		return (float)value_inside;
	}
	//testé ok.
	const bool isInside(const Rectangle rect) const {
		SDL_Point p_left, p_right, p_top, p_bottom;
		p_left = points[0];
		p_right = points[2];
		p_bottom = points[1];
		p_top = points[3];
		return (rect.getRight() > p_right.x &&
			rect.getLeft() < p_left.x &&
			rect.getTop() < p_top.y &&
			rect.getBottom() > p_bottom.y);
	}
	const float collidesWith(const Polyline &poly) const; 
	const float collidesWithBorder(const Polyline &poly) const;


private :
	std::vector<SDL_Point> points_base;
	std::vector<SDL_Point> points;
	//https://www.geeksforgeeks.org/how-to-check-if-a-given-point-lies-inside-a-polygon/

	// Given three colinear points p, q, r, the function checks if 
	// point q lies on line segment 'pr' 
	bool onSegment(const SDL_Point &p,const  SDL_Point &q,const SDL_Point &r) const
	{
		if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
			q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y))
			return true;
		return false;
	}

	// To find orientation of ordered triplet (p, q, r). 
	// The function returns following values 
	// 0 --> p, q and r are colinear 
	// 1 --> Clockwise 
	// 2 --> Counterclockwise 
	int orientation(const SDL_Point &p,const  SDL_Point &q,const  SDL_Point &r) const 
	{
		int val = (q.y - p.y) * (r.x - q.x) -
			(q.x - p.x) * (r.y - q.y);

		if (val == 0) return 0; // colinear 
		return (val > 0) ? 1 : 2; // clock or counterclock wise 
	}

	// The function that returns true if line segment 'p1q1' 
	// and 'p2q2' intersect. 
	bool doIntersect(const SDL_Point &p1, const SDL_Point &q1, const SDL_Point &p2, const SDL_Point &q2) const;
	bool doIntersectBorder(const SDL_Point &p1, const SDL_Point &q1, const SDL_Point &p2, const SDL_Point &q2) const;
	// Returns true if the point p lies inside the polygon[] with n vertices 
	bool isInside(const Polyline &poly, SDL_Point &p) const;
	bool isInsideBorder(const Polyline &poly, SDL_Point &p) const;
	
};

struct Circle : public Shape {
	int thick=1;
	std::vector<SDL_Rect*> circles;
	Circle(Point center, float rayon_cm) :Shape(), circles{} {centre = Parameters::conv_point_to_SDL_Point(center); rayon = Parameters::convWidthCmToPix(rayon_cm); }
	Circle(int pos_x, int pos_y, int w, int h, Color c) : Shape(pos_x, pos_y, w, h, c),circles{} { calcul_rayon(); calcul_centre(); }
	Circle(int pos_x, int pos_y, int w, int h) :Circle(pos_x, pos_y, w, h, { 0,200,200,200 }) {}
	Circle():Circle(0,0,0,0){}
	virtual void draw(const Graphics & graphics) {
		Shape::draw(graphics);
		if (color.alpha != 0) // if no color is choose in tmx objectgroup -> don't display Rectangle
		for (int angle = 0; angle <= 270; angle += 90) {  //dessine un cercle
			drawQuartercircle(angle);
		}
	}
	void printRect() {
		for (int i = 0; i < circles.size(); i++) {
			std::cout << circles[i]->x << "," << circles[i]->y << "," << circles[i]->w << "," << circles[i]->h << "\n";
		}
	}
	void erase_all() {
		for (int i = 0; i < circles.size(); i++)
			delete circles[i];
		circles.clear();
	}
	void move(int dx, int dy) {
		centre.x += dx;
		centre.y += dy;
	}
	//getter
	SDL_Point center()const { return centre; }
	//setter
	void center(SDL_Point &center) { centre.x = center.x; centre.y = center.y; }
	//getter
	int radius()const { return rayon; }
	//setter
	void radius(int rayon) { this->rayon = rayon; }
	~Circle() {
		erase_all();
	}
	int  drawQuartercircle(int angle_debut);
	friend std::ostream& operator<<(std::ostream& os, const Circle& circle);
	bool collideWith(Rectangle &rect)
	{
		int left, top, right, bottom;
		left = rect.getLeft();
		bottom = rect.getBottom();
		top = rect.getTop();
		right = rect.getRight();
		float closestX = (centre.x < left ? left : (centre.x > right ? right : centre.x));
		float closestY = (centre.y < top ? top : (centre.y > bottom ? bottom : centre.y));
		float dx = closestX - centre.x;
		float dy = closestY - centre.y;

		return (dx * dx + dy * dy) <= rayon * rayon;
	}
private:

	SDL_Point centre;
	int rayon;
	void calcul_rayon();
	void calcul_centre();
};


std::ostream& operator<<(std::ostream& os, const Color& color);
std::ostream& operator<<(std::ostream& os, const Shape& shape);
std::ostream& operator<<(std::ostream& os, const Rectangle& rect);
std::ostream& operator<<(std::ostream& os, const Circle& circle);
std::ostream& operator<<(std::ostream& os, const Polyline& lines);

