#pragma once
#include <string>

const double PI = 3.14159265;

bool is_equal(double d1, double d2, double epsilon=1.e-13);
bool is_greater(double d1, double d2, double epsilon=1.e-13);
bool is_greater_or_equal(double d1, double d2, double epsilon=1.e-13);
bool is_smaller(double d1, double d2, double epsilon=1.e-13);
bool is_smaller_or_equal(double d1, double d2, double epsilon=1.e-13);
double round_down(double d);
double f_rand(double f_min, double f_max);
long get_time();
bool file_exists(const std::string &path);
void display_warning(const std::string &text);
void display_error(const std::string &text);
void parser_error_unknown_command(const std::string &command);
void parser_assert_command(const std::string &command, const std::string &expected_command);

//class rayPlot
//{
//public:
	//static void drawEllipse(Vector center, double radius);
	//static void drawCam(Vector position);
	//static void drawRect(Vector center, Vector lineTo0, Vector lineTo1);
	//static void drawLine(Vector p, Vector q, double anteil=1);
	//
	//static bool active;
	//static double left;
	//static double right;
	//static double top;
	//static double botton;
	//static QImage image;
	//static QPainter painter;
//};

//enum Compare {KL = -1, GL = 0, GR = 1};

//Compare CompareD(double d1, double d2, double tol = .0000000001);
//bool ladeBitmap(const std::string &path, QImage &image);
//int Abgerundet(double d);

//bool loadGeo(	std::ifstream &in, Cam &cam, const std::string path);
//bool loadGeo(	std::ifstream &in, Cam &cam, double &fps, int &bitrate, const std::string path,
						 //std::vector < Ball* > &balls, std::vector < Ebene* > &planes, std::vector < Cube* > &cubes);
