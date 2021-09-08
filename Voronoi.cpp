#pragma once
#include "Voronoi.h"
#include <cmath>
#include <algorithm>

Voronoi::Voronoi (int x, int y)				// set our borders on the display
{
	x= abs(x); y= abs(y);
	 double max= pow((double) DBL_MAX/32, (double) 1/4);	// maximum value in our problem is 16|yx^3+xy^3| == 32|max(x,y)^4|
	if(max < x)
		x= max;
	if(max < y)
		y= max;
 	rightZone= x;
	downZone= y;
} // Voronoi
void Voronoi::add_point(int x, int y)
{
	 Site temp;
	temp.val.x= x; temp.val.y= y;	temp.val.multiply= 1;			// multiply for each site ALWAYS == 1 !!! because of INT
	points.insert(points.begin(), temp);					//add site
	fast_add_site(points.begin());		//find polygon only for our new site, but not for others
} // add_point
void Voronoi::refresh()					//find polygons for all sites
{
	for(auto it= points.begin(); it < points.end(); it++)
		fast_add_site(it);
} // resfresh
void Voronoi::fast_add_site(std::vector<Site>::iterator it_site)
{
	normals.clear();
	add_display_line();					// add four side lines from the display(0,0,rightZone,downZone)
	add_points_line(it_site);			// find middle normals for current site

	cross.clear();
	add_cross_points();					// find all cross points which appear after intersection of two lines

	find_vertex_Voronoi(it_site->val);		// there we keeps out useless cross points
	it_site->verticies.clear();
	fill_site(*it_site);					// we must to sort our points clockwise or counter-clockwise around our site
											// becouse it would  be easier to build our polygon
} // fast_add_site
void Voronoi::add_display_line()
{
	add_line(1, 0, 0);
	add_line(1, 0, -rightZone);
	add_line(0, 1, 0);
	add_line(0, 1, -downZone);
} // add_display_line
void Voronoi::add_points_line(std::vector<Site>::iterator it0)	// build direct line (middle normal) at the two points
{									//site.multiply == 1 always!
	for(auto it= points.begin(); it < points.end(); it++) {
		if(it == it0) continue;
		 double a, b, c;
		a= 2*(it0->val.x - it->val.x);			// |2x|
		b= 2*(it0->val.y - it->val.y);			// |2y|
		c= pow(it->val.y, 2) - pow(it0->val.y, 2) + pow(it->val.x, 2) - pow(it0->val.x, 2);		// |x^2+y^2|
		add_line(a, b, c);
	}
} // add_point_line
void Voronoi::add_line(double a, double b, double c)
{
	 Line temp;								// ax + by + c = 0
	temp.a= a; temp.b= b; temp.c= c;
	normals.push_back(temp);
} //add_line
void Voronoi::add_cross_points()								// sort out all the possible combinations of the two lines
{
	 size_t number= normals.size();
	for(size_t i= 0; i < number; i++)
		for(size_t j= i+1; j < number; j++) {
			 Point temp;
			if(cross_two_lines(normals[i], normals[j], temp))
				cross.push_back( std::make_pair(temp, 0) );
		}
} //add_cross_points
bool Voronoi::cross_two_lines(Line line1, Line line2, Point &cross)					// a1x + b1y + c1 = a2x + b2y + c2 = 0
{
	 double multiply= line1.a*line2.b - line2.a*line1.b;			// |8xy|
	if(multiply == 0)					//if lines are parallel
		return false;

	 double x= ((line2.c*line1.b)-(line1.c*line2.b)),		// |4(yx^2+y^3)|
			y= -1*((line2.c*line1.a)-(line1.c*line2.a));		// |4(x^3+xy^2)|

	if(multiply > 0) {				// if x or y is too big or small
		if((x < 0) || (y < 0) || (x > rightZone*multiply) || (y > downZone*multiply))
			return false;
	}
	else {
		if((x > 0) || (y > 0) || (x < rightZone*multiply) || (y < downZone*multiply))
			return false;
	}

	cross.x= x;
	cross.y= y;
	cross.multiply= multiply;
	return true;
} //cross_two_lines
void Voronoi::find_vertex_Voronoi(Point site)
{														// if not VORONOI vertex for our site -- delete from the cross points
	for(int i= 0; i < cross.size(); i++) {
		if(!check_vertex_Voronoi(site, cross[i].first)) {
			cross.erase(cross.begin()+i);
			i--;
		}
	}
} // find_vercities_Voronoi
bool Voronoi::check_vertex_Voronoi(Point site, Point point)			// if ax + by + c > 0 -->  vertex (x,y) in the half-plane FIRST
{																	// if ax + by + c < 0 -->  vertex (x,y) in the half-plane SECOND
	for(auto it= normals.begin(); it < normals.end(); it++) {		// if ax + by + c = 0 -->  vertex (x,y) on the line etc the half-plane SECOND/FIRST
		 short int val_site= count_val_point(site, *it),
				   val_point= count_val_point(point, *it);
		if((val_site > 0 && val_point < 0) || (val_site < 0 && val_point > 0))
			return false;
	}
	return true;
} // check_vertex_Voronoi
short int Voronoi::count_val_point(Point point, Line line)
{								// count_val_point <-----> ax + by + c ? 0
	 short int answer;
	 double left= line.a*point.x,								// |8(yx^3+xy^3)|
			right= -(line.b*point.y + line.c*point.multiply);	// |16(yx^3+xy^3)|
	if(left > right)
		answer= 1;
	if(left < right)
		answer= -1;
	if(left == right)
		answer= 0;

	if(point.multiply < 0)
		answer= -answer;
	return answer;
} // count_val_point
void Voronoi::fill_site(Site &site)
{
	sort_vertex_Voronoi_polar(site.val);
	for(auto it : cross)
		site.verticies.push_back(it.first);					// fill to save into the site<Site> -- global public variable
} // fill_site
void Voronoi::sort_vertex_Voronoi_polar(Point site)					//sort our VORONOI vertex around the site by polars
{
	for(auto it= cross.begin(); it < cross.end(); it++)
		(*it).second= count_polar_angel(site, (*it).first);
	sort(cross.begin(), cross.end(), [](std::pair<Point, double> a, std::pair<Point, double> b) {
										return a.second < b.second; });
} // sort_vertex_Voronoi_polar
double Voronoi::count_polar_angel(Point site, Point vertex)			//count polar angel for each site and its VORONOI vertex
{											// site.multiply == 1 !!always
	 double x= vertex.x - site.x*vertex.multiply,	// |X|	// point(x/multiply,y/mutiply)
			y= vertex.y - site.y*vertex.multiply;	// |Y|	// site(x,y)
	if(vertex.multiply < 0) {			// arctan= delta_y/delta_x == (multiply*delta_y)/(multiply*delta_x) if multiply > 0
		x= -x;
		y= -y;
	}
	return atan2(y, x);
} // count_polar_angel
std::vector< std::pair<double, double> > Voronoi::take_polygon(int number)			// this function is needed to transform
{																					// our global variable <SITE> to vector for answer
	 std::vector< std::pair<double, double> > answer;
	for(auto it : points[number].verticies)
		answer.push_back(std::make_pair(it.x/it.multiply, it.y/it.multiply));
	return answer;
} // take_polygon
Voronoi::~Voronoi() {} // ~Voronoi