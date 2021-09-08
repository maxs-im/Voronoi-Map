#pragma once

#include <vector>

class Voronoi		// O(n^4)
{
	public:
		struct Point    // struct for coordinates of each point formated (X; Y)<=>(X= x/multiply; Y= y/multiply)
		{
			double x, y,
				   multiply;
		};
		struct Site					// struct for each site with its VORONOI verticies
		{
			Point val;
			std::vector<Point> verticies;
		};
		std::vector <Site> points;

		void add_point(int, int);
		std::vector< std::pair<double, double> > take_polygon(int);
		void refresh();

		Voronoi (int, int);
		~Voronoi();
	private:
		struct Line					// struct for lines in format ax + by + c = 0
		{
			double a, b, c;
		};
		std::vector <Line> normals;
		std::vector < std::pair<Point, double> > cross;				//first -- (X,Y) and double -- polar angel (rad)
		int rightZone, downZone;

		void fast_add_site(std::vector<Site>::iterator);
		void add_display_line();
		void add_line(double, double, double);
		void add_points_line(std::vector<Site>::iterator);
		void add_cross_points();
		bool cross_two_lines(Line, Line, Point &);
		void find_vertex_Voronoi(Point);
		bool check_vertex_Voronoi(Point, Point);
		short int count_val_point(Point, Line);
		void fill_site(Site &);
		void sort_vertex_Voronoi_polar(Point);
		double count_polar_angel(Point, Point);
};
