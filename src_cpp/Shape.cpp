#include "Shape.h"


const sides::Side Rectangle::getCollisionSide(Rectangle &other) const {
	int amtRight, amtLeft, amtTop, amtBottom;
	amtRight = this->getRight() - other.getLeft();
	amtLeft = other.getRight() - this->getLeft();
	amtTop = other.getBottom() - this->getTop();
	amtBottom = this->getBottom() - other.getTop();

	int vals[4] = { abs(amtRight), abs(amtLeft), abs(amtTop), abs(amtBottom) };
	int lowest = vals[0];
	for (int i = 1; i < 4; i++) {
		if (vals[i] < lowest) {
			lowest = vals[i];
		}
	}
	return
		lowest == abs(amtRight) ? sides::RIGHT :
		lowest == abs(amtLeft) ? sides::LEFT :
		lowest == abs(amtTop) ? sides::TOP :
		lowest == abs(amtBottom) ? sides::BOTTOM :
		sides::NONE;
}
const int Rectangle::getCollisionValue(const SDL_Point &p1, const SDL_Point &p2) const {
	int amtRight, amtLeft, amtTop, amtBottom;
	int left = p1.x;
	int right = p2.x;
	int top = p1.y;
	int bottom = p2.y;
	amtRight = this->getRight() - left;
	amtLeft = right - this->getLeft();
	amtTop = bottom - this->getTop();
	amtBottom = this->getBottom() - top;
	int vals[4] = { abs(amtRight), abs(amtLeft), abs(amtTop), abs(amtBottom) };
	int lowest = vals[0];
	for (int i = 1; i < 4; i++) {
		if (vals[i] < lowest) {
			lowest = vals[i];
		}
	}
	return lowest;
}

const sides::Side Polyline::getCollisionSide(Rectangle &other) const {
	//https://math.stackexchange.com/questions/274712/calculate-on-which-side-of-a-straight-line-is-a-given-point-located
	return sides::BOTTOM;
}

void Circle::calcul_rayon() {
	rayon= this->width / 2;
}
void Circle::calcul_centre() {
	centre.x = this->x + this->width / 2;
	centre.y = this->y + this->height / 2;
}


int Circle::drawQuartercircle( int angle_debut)
{
	//https://en.wikipedia.org/wiki/Midpoint_circle_algorithm
	SDL_Point p;
	SDL_Rect *rect_temp;
	//init point de demarrage
	int x1, y1;
	int R2;
	x1 = rayon;
	y1 = 0;
	R2 = rayon * rayon;
	int rayon_large = rayon + thick;
	int R_large2 = rayon_large * rayon_large;
	int x2 = rayon_large;
	int y2 = 0;
	int i = 0;
	int y_av = 0;
	int w, h;

	//calcul octants
	while (y1 <= x1)
	{
		y1++;
		y2++;
		int Res = 2 * (x1*x1 + y1 * y1 - R2 + 2 * y1 + 1) + 1 - 2 * x1;
		if (Res > 0)
		{
			w = x2 - x1;
			h = y2 - y_av;
			if (w > 0 && h > 0)
			{
				switch (angle_debut)
				{
				case 90:
					rect_temp = new SDL_Rect;
					rect_temp->x = x1 + centre.x;
					rect_temp->y = -y1 + centre.y;
					rect_temp->w = w;
					rect_temp->h = h;		
					circles.push_back(rect_temp);
					SDL_RenderFillRect(_sdl_renderer,rect_temp);
					rect_temp = new SDL_Rect;
					rect_temp->x = +y1 - h + centre.x;
					rect_temp->y = -x2 + centre.y;
					rect_temp->w = h;
					rect_temp->h = w;
					circles.push_back(rect_temp);
					SDL_RenderFillRect(_sdl_renderer, rect_temp);
					break;
				case 0:
					rect_temp = new SDL_Rect;
					rect_temp->x = x1 + centre.x;
					rect_temp->y = y_av + centre.y;
					rect_temp->w = w;
					rect_temp->h = h;
					circles.push_back(rect_temp);
					SDL_RenderFillRect(_sdl_renderer, rect_temp);
					rect_temp = new SDL_Rect;
					rect_temp->x = y_av + centre.x;
					rect_temp->y = x1 + centre.y;
					rect_temp->w = h;
					rect_temp->h = w;
					circles.push_back(rect_temp);
					SDL_RenderFillRect(_sdl_renderer, rect_temp);
					break;
				case 180:
					rect_temp = new SDL_Rect;
					rect_temp->x = -x2 + centre.x;
					rect_temp->y = -y_av - h + centre.y;
					rect_temp->w = w;
					rect_temp->h = h;
					circles.push_back(rect_temp);
					SDL_RenderFillRect(_sdl_renderer, rect_temp);
					rect_temp = new SDL_Rect;
					rect_temp->x = -y_av - h + centre.x;
					rect_temp->y = -x2 + centre.y;
					rect_temp->w = h;
					rect_temp->h = w;
					circles.push_back(rect_temp);
					SDL_RenderFillRect(_sdl_renderer, rect_temp);
					break;
				case 270:
					rect_temp = new SDL_Rect;
					rect_temp->x = -x2 + centre.x;
					rect_temp->y = +y_av + centre.y;
					rect_temp->w = w;
					rect_temp->h = h;
					circles.push_back(rect_temp);
					SDL_RenderFillRect(_sdl_renderer, rect_temp);
					rect_temp = new SDL_Rect;
					rect_temp->x = -y_av - h + centre.x;
					rect_temp->y = +x1 + centre.y;
					rect_temp->w = h;
					rect_temp->h = w;
					circles.push_back(rect_temp);
					SDL_RenderFillRect(_sdl_renderer, rect_temp);
					break;

				}
			}
			y_av = y1;
			x1--;
		}
		Res = 2 * (x2*x2 + y2 * y2 - R_large2 + 2 * y2 + 1) + 1 - 2 * x2;
		if (Res > 0)
		{
			w = x2 - x1;
			h = y2 - y_av;
			if (w > 0 && h > 0)
			{
				switch (angle_debut)
				{
				case 90:
					rect_temp = new SDL_Rect;
					rect_temp->x = x1 + centre.x;
					rect_temp->y = -y1 + centre.y;
					rect_temp->w = w;
					rect_temp->h = h;
					circles.push_back(rect_temp);
					SDL_RenderFillRect(_sdl_renderer, rect_temp);
					rect_temp = new SDL_Rect;
					rect_temp->x = +y1 - h + centre.x;
					rect_temp->y = -x2 + centre.y;
					rect_temp->w = h;
					rect_temp->h = w;
					circles.push_back(rect_temp);
					SDL_RenderFillRect(_sdl_renderer, rect_temp);
					break;
				case 0:
					rect_temp = new SDL_Rect;
					rect_temp->x = x1 + centre.x;
					rect_temp->y = y_av + centre.y;
					rect_temp->w = w;
					rect_temp->h = h;
					circles.push_back(rect_temp);
					SDL_RenderFillRect(_sdl_renderer, rect_temp);
					rect_temp = new SDL_Rect;
					rect_temp->x = y_av + centre.x;
					rect_temp->y = x1 + centre.y;
					rect_temp->w = h;
					rect_temp->h = w;
					circles.push_back(rect_temp);
					SDL_RenderFillRect(_sdl_renderer, rect_temp);
					break;
				case 180:
					rect_temp = new SDL_Rect;
					rect_temp->x = -x2 + centre.x;
					rect_temp->y = -y_av - h + centre.y;
					rect_temp->w = w;
					rect_temp->h = h;
					circles.push_back(rect_temp);
					SDL_RenderFillRect(_sdl_renderer, rect_temp);
					rect_temp = new SDL_Rect;
					rect_temp->x = -y_av - h + centre.x;
					rect_temp->y = -x2 + centre.y;
					rect_temp->w = h;
					rect_temp->h = w;
					circles.push_back(rect_temp);
					SDL_RenderFillRect(_sdl_renderer, rect_temp);
					break;
				case 270:
					rect_temp = new SDL_Rect;
					rect_temp->x = -x2 + centre.x;
					rect_temp->y = +y_av + centre.y;
					rect_temp->w = w;
					rect_temp->h = h;
					circles.push_back(rect_temp);
					SDL_RenderFillRect(_sdl_renderer, rect_temp);
					rect_temp = new SDL_Rect;
					rect_temp->x = -y_av - h + centre.x;
					rect_temp->y = +x1 + centre.y;
					rect_temp->w = h;
					rect_temp->h = w;
					circles.push_back(rect_temp);
					SDL_RenderFillRect(_sdl_renderer, rect_temp);

					break;

				}
			}
			x2--;
			y_av = y1;
		}
	}
	//on complète
	//x1=y1
	for (int j = w; j >= 0; j--)
	{
		switch (angle_debut)
		{
		case 90:
			rect_temp = new SDL_Rect;
			rect_temp->x = x1 + centre.x;
			rect_temp->y = -(y1 + w - j) + centre.y;
			rect_temp->w = j;
			rect_temp->h = 1;
			circles.push_back(rect_temp);
			SDL_RenderFillRect(_sdl_renderer, rect_temp);
			break;
		case 0:
			rect_temp = new SDL_Rect;
			rect_temp->x = x1 + centre.x;
			rect_temp->y = y1 + w - j + centre.y;
			rect_temp->w = j;
			rect_temp->h = 1;
			circles.push_back(rect_temp);
			SDL_RenderFillRect(_sdl_renderer, rect_temp);
			break;
		case 180:
			rect_temp = new SDL_Rect;
			rect_temp->x = -x2 + (w - j) + centre.x;
			rect_temp->y = -y1 - (w - j) + centre.y;
			rect_temp->w = j;
			rect_temp->h = 1;
			circles.push_back(rect_temp);
			SDL_RenderFillRect(_sdl_renderer, rect_temp);
			break;
		case 270:
			rect_temp = new SDL_Rect;
			rect_temp->x = -x2 + (w - j) + centre.x;
			rect_temp->y = +y1 + (w - j) + centre.y;
			rect_temp->w = j;
			rect_temp->h = 1;
			circles.push_back(rect_temp);
			SDL_RenderFillRect(_sdl_renderer, rect_temp);
			break;
		}

	}
	return (i - 1);
}

void Polyline::drawLine(int x0, int y0, int x1, int y1)
{
	//https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
	if (y0 == y1)//on est sur une ligne horizontale
		SDL_RenderDrawLine(_sdl_renderer, x0, y0, x1, y1);
	else
	{
		int dx = abs(x1 - x0);
		int sx = x0 < x1 ? 1 : -1;
		int dy = -abs(y1 - y0);
		int sy = y0 < y1 ? 1 : -1;
		int err = dx + dy;  /* error value e_xy */
		while (1)   /* loop */
		{
			SDL_RenderDrawPoint(_sdl_renderer, x0, y0);
			if (x0 == x1 && y0 == y1) break;
			int e2 = 2 * err;
			if (e2 >= dy)  /* e_xy+e_x > 0 */
			{
				err += dy;
				x0 += sx;
			}
			if (e2 <= dx)  /* e_xy+e_y < 0 */
			{
				err += dx;
				y0 += sy;
			}
		}
	}
}

std::ostream& operator<<(std::ostream& os, const Color& color) {
	return os << "Color:" << color.red << "," << color.green << "," << color.red << "," << color.alpha;
}

std::ostream& operator<<(std::ostream& os, const Shape& shape) {
	return os << shape.color << " Position:" << shape.x << "," << shape.y;
}

std::ostream& operator<<(std::ostream& os, const Rectangle& rect) {
	return os << "Rectangle->" << static_cast<const Shape &>(rect)  ;
}

std::ostream& operator<<(std::ostream& os, const Circle& circle) {
	return os << "Circle->" << static_cast<const Shape &>(circle) << " Rayon:" << circle.rayon << " thick:" << circle.thick ;
}
std::ostream& operator<<(std::ostream& os, const Polyline& lines) {
	for (int i = 0; i < lines.points.size() - 1; i++) {
		os << lines.points[i]->x << "," << lines.points[i]->y << "->" << lines.points[i + 1]->x << "," << lines.points[i + 1]->y << "\n";
	}
	return os;
}

bool Cross::isInside(const Polyline &poly, SDL_Point &p) const 
{
	// Create a point for line segment from p to infinite 
	SDL_Point extreme = { INF, p.y };

	// Count intersections of the above line with sides of polygon 
	int count = 0, i = 0;
	int n = poly.points.size();
	do
	{
		int next = (i + 1);
		if (next == n) //on est a la fin du test
			break;

		// Check if the line segment from 'p' to 'extreme' intersects 
		// with the line segment from 'polygon[i]' to 'polygon[next]' 
		if (doIntersect(*poly.points[i], *poly.points[next], p, extreme))
		{
			// If the point 'p' is colinear with line segment 'i-next', 
			// then check if it lies on segment. If it lies, return true, 
			// otherwise false 
			//if (orientation(*poly.points[i], p, *poly.points[next]) == 0)
			//	return onSegment(*poly.points[i], p, *poly.points[next]);
			count++;
			//petit ajout, bug si la ligne de capteur est à la même hauteur que le point du polyline.
			if (p.y == poly.points[next]->y) {
				i = next;
				next = (i + 1);
				if (next == n) //on est a la fin du test
					break;
			}
		}
		i = next;
	} while (i != n);

	// Return true if count is odd, false otherwise 
	return count & 1; // Same as (count%2 == 1) 
}
bool Cross::isInsideBorder(const Polyline &poly, SDL_Point &p) const
{
	// Create a point for line segment from p to infinite 
	SDL_Point extreme = { INF, p.y };

	// Count intersections of the above line with sides of polygon 
	int count = 0, i = 0;
	int n = poly.points.size();
	do
	{
		int next = (i + 1) ;
		if (next == n) //on est a la fin du test
			break;
		// Check if the line segment from 'p' to 'extreme' intersects 
		// with the line segment from 'polygon[i]' to 'polygon[next]' 
		if (doIntersect(*poly.points[i], *poly.points[next], p, extreme))
		{
			// If the point 'p' is colinear with line segment 'i-next', 
			// then check if it lies on segment. If it lies, return true, 
			// otherwise false 
			if (orientation(*poly.points[i], p, *poly.points[next]) == 0)
				return onSegment(*poly.points[i], p, *poly.points[next]);
			count++;
			//petit ajout, bug si la ligne de capteur est à la même hauteur que le point du polyline.
			if (p.y == poly.points[next]->y) {
				i = next;
				next = (i + 1);
				if (next == n) //on est a la fin du test
					break;
			}
		}
		i = next;
	} while (i != n);
	// Return true if count is odd, false otherwise 
	return count & 1; // Same as (count%2 == 1) 
}
bool Cross::doIntersect(const SDL_Point &p1, const SDL_Point &q1, const SDL_Point &p2, const SDL_Point &q2) const
{
	// Find the four orientations needed for general and 
	// special cases 
	int o1 = orientation(p1, q1, p2);
	int o2 = orientation(p1, q1, q2);
	int o3 = orientation(p2, q2, p1);
	int o4 = orientation(p2, q2, q1);

	// General case 
	if (o1 != o2 && o3 != o4)
		return true;
	return false; // Doesn't fall in any of the above cases 
}
bool Cross::doIntersectBorder(const SDL_Point &p1, const SDL_Point &q1, const SDL_Point &p2, const SDL_Point &q2) const
{
	// Find the four orientations needed for general and 
	// special cases 
	int o1 = orientation(p1, q1, p2);
	int o2 = orientation(p1, q1, q2);
	int o3 = orientation(p2, q2, p1);
	int o4 = orientation(p2, q2, q1);

	// General case 
	if (o1 != o2 && o3 != o4)
		return true;

	// Special Cases 
	//on ne le compte pas il faut être à l'intérieur de la forme ( a tester)
	// p1, q1 and p2 are colinear and p2 lies on segment p1q1 
	if (o1 == 0 && onSegment(p1, p2, q1)) return true;

	// p1, q1 and p2 are colinear and q2 lies on segment p1q1 
	if (o2 == 0 && onSegment(p1, q2, q1)) return true;

	// p2, q2 and p1 are colinear and p1 lies on segment p2q2 
	if (o3 == 0 && onSegment(p2, p1, q2)) return true;

	// p2, q2 and q1 are colinear and q1 lies on segment p2q2 
	if (o4 == 0 && onSegment(p2, q1, q2)) return true;

	return false; // Doesn't fall in any of the above cases 
}
const float Cross::collidesWith(const Polyline &poly) const {
	//on cherche à savoir sur la ligne Cross combien de points sont à l'intérieure du polygone...
	int count = 0;
	SDL_Point p_left, p_right, p_top, p_bottom;//points extrèmes de la croix
	SDL_Point p_left_top, p_left_bottom, p_right_top, p_right_bottom;//points diagonales
	p_left = points[0];
	p_right = points[2];
	p_bottom = points[1];
	p_top = points[3];
	p_right_top = { p_right.x - 1,p_right.y - 1 };
	p_right_bottom = { p_right.x - 1,p_right.y + 1 };
	p_left_top = { p_left.x + 1,p_right.y - 1 };
	p_left_bottom = { p_left.x + 1,p_left.y + 1 };

	SDL_Point point_courant;//point à gauche
	point_courant.x = p_left.x;
	point_courant.y = p_left.y;
	for (int i = 0; i < 5; i++) { // notre croix a 5 points.
		if (isInside(poly,point_courant))
			count++;
		point_courant.x++;
	}
	point_courant.x = p_top.x;//point en haut
	point_courant.y = p_top.y;
	for (int i = 0; i < 4; i++) { // notre croix a 4 points (on enlève le point milieu
		if (isInside(poly, point_courant))
			count++;
		if (i == 1)
			point_courant.y += 2;
		else
			point_courant.y++;
	}
	//on teste les 4 points en diagonale
	if (isInside(poly, p_right_top))
		count++;
	if (isInside(poly, p_right_bottom))
		count++;
	if (isInside(poly, p_left_top))
		count++;
	if (isInside(poly, p_left_top))
		count++;
	return count; 
}

const float Cross::collidesWithBorder(const Polyline &poly) const {
	//on cherche à savoir sur la ligne Cross combien de points sont à l'intérieure du polygone...
	int count = 0;
	SDL_Point p_left, p_right, p_top, p_bottom;//points extrèmes de la croix
	SDL_Point p_left_top, p_left_bottom, p_right_top, p_right_bottom;//points diagonales
	p_left = points[0];
	p_right = points[2];
	p_bottom = points[1];
	p_top = points[3];
	p_right_top = { p_right.x - 1,p_right.y - 1 };
	p_right_bottom = { p_right.x - 1,p_right.y + 1 };
	p_left_top = { p_left.x + 1,p_right.y - 1 };
	p_left_bottom = { p_left.x + 1,p_left.y + 1 };

	SDL_Point point_courant;//point à gauche
	point_courant.x = p_left.x;
	point_courant.y = p_left.y;
	for (int i = 0; i < 5; i++) { // notre croix a 5 points.
		if (isInsideBorder(poly, point_courant))
			count++;
		point_courant.x++;
	}
	point_courant.x = p_top.x;//point en haut
	point_courant.y = p_top.y;
	for (int i = 0; i < 4; i++) { // notre croix a 4 points (on enlève le point milieu
		if (isInsideBorder(poly, point_courant))
			count++;
		if (i == 1)
			point_courant.y += 2;
		else
			point_courant.y++;
	}
	//on teste les 4 points en diagonale
	if (isInsideBorder(poly, p_right_top))
		count++;
	if (isInsideBorder(poly, p_right_bottom))
		count++;
	if (isInsideBorder(poly, p_left_top))
		count++;
	if (isInsideBorder(poly, p_left_top))
		count++;
	return count;
}