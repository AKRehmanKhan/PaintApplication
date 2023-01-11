#include <iostream>
#include <vector>
#include <fstream>
#include "GP142.H"
#include "GP142LIB.H"
using namespace std;




struct Point
{
	double x;
	double y;
};

char key;
int x = 0, y = 0;
Point p1;
int pill = 0;

class shapes
{
protected:
	
	int color;
	int n;
	int shape;
	bool style;
	int coloredfilled;
	int len;
	char* arr;
	

public:
	
	vector <Point> coordinates;
	
	vector<Point> getCoordinates()
	{
		return coordinates;
	}


	virtual void draw() = 0;  // virtual draw function

	virtual bool contain(Point p) = 0;// virtual contain function
	
	void changecolor(int col)
	{
		color = col;
	}

	void erasecol(int c, Point z)
	{
		coloredfilled = c;

	}
	
	void erasecol()
	{
		coloredfilled = 0;
	}
	void set(int & a, int & b, int & c,bool &s, int &f , vector<Point> & d,int & l,char* &array)
	{
		a = color;
		b = n;
		c = shape;
		s = style;
		f = coloredfilled;
		d = coordinates;
		if (c == 6)
		{
			l = len;
			array = new char[l];
			strcpy(array, arr);
		}
	}

	void setcolorfilled(int colorf)
	{
		coloredfilled = colorf;
	}

	void setpoint(vector <Point>  & a,int & b)
	{
		a = coordinates;
		b = n;
	}
	
	
	~shapes()
	{
		coordinates.clear();
	}
	
		
	


};


class polygons :public shapes
{

public:
	bool onSegment(Point pa, Point qa, Point ra)
	{
		if (qa.x <= max(pa.x, ra.x) && qa.x >= min(pa.x, ra.x) &&
			qa.y <= max(pa.y, ra.y) && qa.y >= min(pa.y, ra.y))
			return true;

		return false;
	}
	int orientation(Point pa, Point q, Point r){
		int val = (q.y - pa.y) * (r.x - q.x) -
			(q.x - pa.x) * (r.y - q.y);
		if (val == 0) return 0;  // colinear
		return (val > 0) ? 1 : 2; // clock or counterclock wise
	}
	bool doesIntersect(Point p1, Point q1, Point p2, Point q2){
		int o1 = orientation(p1, q1, p2);
		int o2 = orientation(p1, q1, q2);
		int o3 = orientation(p2, q2, p1);
		int o4 = orientation(p2, q2, q1);
		if (o1 != o2 && o3 != o4)
			return true;
		if (o1 == 0 && onSegment(p1, p2, q1)) return true;
		if (o2 == 0 && onSegment(p1, q2, q1)) return true;
		if (o3 == 0 && onSegment(p2, p1, q2)) return true;
		if (o4 == 0 && onSegment(p2, q1, q2)) return true;
		return false;
	}
	bool polygonchecker(Point ch, int no, vector<Point>&p){
		Point u;
		int count = 0;
		for (int d = 0; d < no - 1; d++){
			u.x = 255, u.y = -210;
			if (doesIntersect(p[d], p[d + 1], ch, u)){
				count++;
			}
		}
		if (doesIntersect(p[no - 1], p[0], ch, u))	count++;

		if (count % 2 != 0) return true;
		else{
			count = 0;
			for (int d = 0; d < no - 1; d++){
				u.x = 255, u.y = 200;
				if (doesIntersect(p[d], p[d + 1], ch, u)){
					count++;
				}
			}
			if (doesIntersect(p[no - 1], p[0], ch, u))	count++;
		}

		if (count % 2 != 0) return true;
		else{
			count = 0;
			for (int d = 0; d < no - 1; d++){
				u.x = -225, u.y = 200;
				if (doesIntersect(p[d], p[d + 1], ch, u)){
					count++;
				}
				if (doesIntersect(p[no - 1], p[0], ch, u))	count++;
			}
		}

		if (count % 2 != 0) return true;
		else{
			count = 0;
			for (int d = 0; d < no - 1; d++){
				u.x = -225, u.y = -210;
				if (doesIntersect(p[d], p[d + 1], ch, u)){
					count++;
				}
			}
			if (doesIntersect(p[no - 1], p[0], ch, u))	count++;
		}
		if (count % 2 != 0) return true;
		else return false;

	}
	
};
class polygon :public polygons
{
public:
	polygon(int col, int colorf)
	{
		int quit = FALSE;
		shape = 3;
		color = col;
		n = 1;
		coloredfilled = colorf;
		GP142_await_event(&x, &y, &key);
		p1.x = x, p1.y = y;
		coordinates.push_back(p1);
		while (quit != TRUE){
			int	event = GP142_await_event(&x, &y, &key);
			switch (event) {
			case GP142_MOUSE:
				p1.x = x;
				p1.y = y;
				coordinates.push_back(p1);
				n++;
				break;
			default:
				quit = TRUE;
				break;
			}
		}
	}
	polygon(int a, int b, int c,int f, vector<Point> d)
	{
		color = a;
		n = b;
		shape = c;
		coloredfilled = f;
		coordinates = d;
	}
	void draw()
	{
		for (int i = 0; i < n - 1; i++){
			GP142_lineXY(color, coordinates[i].x, coordinates[i].y, coordinates[i + 1].x, coordinates[i + 1].y, 2);
		}
		GP142_lineXY(color, coordinates[0].x, coordinates[0].y, coordinates[n - 1].x, coordinates[n - 1].y, 2);
	}
	bool contain(Point p)
	{
		if (polygonchecker(p, n, coordinates))
		{
			return true;
		}
		else
		{
			return false;
		}
			
	}
};
class rectangle :public polygons
{
public:
	rectangle(int col, int colf)
	{
		n = 2;
		color = col;
		shape = 4;
		coloredfilled = colf;
		for (int i = 0; i < n; i++){
			GP142_await_event(&x, &y, &key);
			p1.x = x, p1.y = y;
			coordinates.push_back(p1);
		}
	}
	rectangle(int a, int b, int c, int f, vector <Point> d)
	{
		color = a;
		n = b;
		c = shape;
		f = coloredfilled;

		coordinates = d;
	}
	void draw()
	{
		GP142_lineXY(color, coordinates[0].x, coordinates[0].y, coordinates[1].x, coordinates[0].y, 2);
		GP142_lineXY(color, coordinates[0].x, coordinates[0].y, coordinates[0].x, coordinates[1].y, 2);
		GP142_lineXY(color, coordinates[1].x, coordinates[0].y, coordinates[1].x, coordinates[1].y, 2);
		GP142_lineXY(color, coordinates[1].x, coordinates[1].y, coordinates[0].x, coordinates[1].y, 2);
	}
	bool contain(Point p)
	{
		if (p.x <= coordinates[0].x && p.x >= coordinates[1].x&&p.y <= coordinates[0].y&&p.y >= coordinates[1].y)
		{
			return true;
			}
		else if (p.x >= coordinates[0].x&&p.x <= coordinates[1].x&&p.y >= coordinates[0].y&&p.y <= coordinates[1].y)
		{
			return true;
		}
		else if (p.x >= coordinates[0].x&&p.x <= coordinates[1].x&&p.y <= coordinates[0].y&&p.y >= coordinates[1].y)
		{
			return true;
		}
		else	if (p.x < coordinates[0].x&&p.x > coordinates[1].x&&p.y > coordinates[0].y&&p.y < coordinates[1].y)
		{
			return true;
		}

		else
		{
			return false;
		}
	}
};



class openshapes :public shapes
{
protected:

public:
	virtual void changecolor(int col)=0;
	float Distance(int x1, int y1, int  x2, int y2)
	{
		float distancex = 0;
		float distancey = 0;
		distancex = (x2 - x1)*(x2 - x1);
		distancey = (y2 - y1) *(y2 - y1);
		return  (sqrt(distancex + distancey));
	}
	bool pointcheck(int x1, int y1, int x2, int y2, int x3, int y3)
	{
		float g = (Distance(x1, y1, x3, y3) + Distance(x2, y2, x3, y3)) - Distance(x1, y1, x2, y2);
		if (g <= 0.4 && g >= -0.4)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
};
class curve :public openshapes
{
public:
	
	curve(int col, int colorf)
	{
		int quit = FALSE;
		shape = 2;
		color = col;
		n = 1;
		coloredfilled = colorf;
		GP142_await_event(&x, &y, &key);
		p1.x = x, p1.y = y;
		coordinates.push_back(p1);
		while (quit != TRUE){
			int	event = GP142_await_event(&x, &y, &key);
			switch (event)
			{
			case GP142_MOUSE:
				p1.x = x;
				p1.y = y;
				coordinates.push_back(p1);
				n++;
				break;
			default:
				quit = TRUE;
				break;
			}
		}
	} 
	curve(int a, int b, int c,int f, vector <Point> d)
	{
		color = a;
		n = b;
		shape = c;
		coloredfilled = f;
		coordinates = d;
	}
	void changecolor(int col)
	{
		color = col;
	}
	void draw()
	{
		for (int i = 0; i < n - 1; i++){
			GP142_lineXY(color, coordinates[i].x, coordinates[i].y, coordinates[i + 1].x, coordinates[i + 1].y, 2);
		}
	}
	bool contain(Point p)
	{
		for (int d = 0; d < n - 1; d++){
			if (pointcheck(coordinates[d].x, coordinates[d].y, coordinates[d + 1].x, coordinates[d + 1].y, p.x, p.y))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}

};

class line :public openshapes
{

	
  public:
	line(int c, bool s, int colorf)
	{
		 n = 2;
		 style = s;
		 color = c;
		 shape = 1;
		 coloredfilled = colorf;
		 for (int i = 0; i < n; i++)
		 {
			 GP142_await_event(&x, &y, &key);
			 p1.x = x, p1.y = y;
			 coordinates.push_back(p1);
		 }
	}
	line(int a,int b,int c, int f,vector<Point>d,bool s)
	{
		n = b;
		color = a;
		shape=c;
	    style = s;
	    coloredfilled=f;
		coordinates = d;
		
	}
	void changecolor(int col)
	{
		color = col;
	}
	
	
    void draw()
	{
		
		if (style == true)
			GP142_lineXY(color, coordinates[0].x, coordinates[0].y, coordinates[1].x, coordinates[1].y,2);
		else {

			int totalx = coordinates[1].x - coordinates[0].x;
			int totaly = coordinates[1].y - coordinates[0].y;
			int steps;
			if (abs(totalx) > abs(totaly))
				steps = abs(totalx);
			else
				steps = abs(totaly);
			float Xincrement = totalx / (float)steps;
			float Yincrement = totaly / (float)steps;
			Point p = coordinates[0];
			for (int i = 0; i < steps; i++){
				p.x = p.x + Xincrement;
				p.y = p.y + Yincrement;
				if (i % 3 == 0)
				{
					GP142_pixelXY(color, round(p.x), round(p.y));
				}
					
			}
		}
	}

	bool contain(Point p)
	{
		
		if (pointcheck(coordinates[0].x, coordinates[0].y, coordinates[1].x, coordinates[1].y, p.x, p.y))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	bool  returnstyle()
	{
		return style;
	}
	
	
};

class circle :public shapes
{
  protected:

   int fillcolor;
   double radius;


  public:
	void fill(int color)
	{
		fillcolor = color;
	}
	double distance(int x1, int y1, int  x2, int y2)
	{
		float distancex = 0;
		float distancey = 0;
		distancex = (x2 - x1)*(x2 - x1);
		distancey = (y2 - y1) *(y2 - y1);
		return  (sqrt(distancex + distancey));
	}
	circle(int col)
	{
		n = 2;
		color = col;
		shape = 5;
		GP142_await_event(&x, &y, &key);
		p1.x = x, p1.y = y;
		coordinates.push_back(p1);
		GP142_await_event(&x, &y, &key);
		p1.x = x, p1.y = y;
		coordinates.push_back(p1);
		radius = (distance(coordinates[0].x, coordinates[0].y, coordinates[1].x, coordinates[1].y));
	}
	circle(int a, int b, int c, vector <Point> d)
	{
		color = a;
		n = b;
		shape = c;
		coordinates = d;
		radius = (distance(coordinates[0].x, coordinates[0].y, coordinates[1].x, coordinates[1].y));
	}
	void draw()
	{
		GP142_circleXY(color, coordinates[0].x, coordinates[0].y, radius, 2);
	}
	bool contain(Point p)
	{
		int distancex = 0;
		int distancey = 0;
		distancex = (coordinates[0].x - p.x)*(coordinates[0].x - p.x);
		distancey = (coordinates[0].y - p.y) *(coordinates[0].y - p.y);

		if ((sqrt(distancex + distancey)) <= radius)
		{
			return true;
		}
			
		else
		{
			return false;
		}
			
	}

};


class text :public shapes
{

public:
	int returnlength(){
		return len;
	}
	char* returntext(){
		return arr;
	}
	text(int x, int y, char* l, int col)
	{
		p1.x = x; p1.y = y;
		n = 1;
		color = col;
		shape = 6;
		coordinates.push_back(p1);
		len = strlen(l);
		arr = new char[len];
		strcpy(arr, l);
	}
	text(int a, int b, int c, vector<Point> d, char * t, int l)
	{
		color = a;
		n = b;
		shape = c;
		coordinates= d;
		len = l;
		arr = new char[len];
		strcpy(arr, t);
	}
	void draw()
	{
		const char *str = arr;
		int startOfline = coordinates[0].x;
		for (int i = coordinates[0].x, j = 0; j < strlen(str); i+=15,j++)
		{
			if (str[j] == '\r')
			{
				coordinates[0].y -= 20;
				i = startOfline-20;
			}
			GP142_printfXY(color,i, coordinates[0].y, 20, "%c", str[j]);
			
		}
	}
	bool contain(Point p)
	{
		if (p.x <= ((coordinates[0].x + 18) + 18) && p.x >= (coordinates[0].x + (len * 18)) && p.y <= (coordinates[0].y + 18) && p.y >= coordinates[0].y)
		{
			return true;
		}
		else	if (p.x >= (coordinates[0].x + 18) && p.x <= (coordinates[0].x + (len * 18)) && p.y >= (coordinates[0].y + 18) && p.y <= coordinates[0].y)
		{
			return true;
		}
		else	if (p.x >= (coordinates[0].x + 18) && p.x <= (coordinates[0].x + (len * 18)) && p.y <= (coordinates[0].y + 18) && p.y >= coordinates[0].y)
		{
			return true;
		}
		else	if (p.x <= (coordinates[0].x + 18) && p.x >= (coordinates[0].x + (len * 18)) && p.y >= (coordinates[0].y + 18) && p.y <= coordinates[0].y)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	

};




double distance(int x1, int y1, int  x2, int y2)
{
	float distancex = 0;
	float distancey = 0;
	distancex = (x2 - x1) * (x2 - x1);
	distancey = (y2 - y1) * (y2 - y1);
	return  (sqrt(distancex + distancey));
}




void  displayline(int colu, int x1, int y1, int x2, int y2,int color,int width){

	GP142_lineXY(colu, x1, y1, x2, y1, width);
	GP142_lineXY(colu, x1, y1, x1, y2, width);
	GP142_lineXY(colu, x2, y1, x2, y2, width);
	GP142_lineXY(colu, x2, y2, x1, y2, width);
	if (x1 > x2) swap(x1, x2);
	if (y1 > y2) swap(y1, y2);
	for (int i = x1; i < x2; i++)
		GP142_lineXY(colu, i, y1, i, y2, width);
}
void displayrectangle(int x1, int y1, int x2, int y2,int color,int width){
	GP142_lineXY(color, x1, y1, x2, y1, width);
	GP142_lineXY(color, x1, y1, x1, y2, width);
	GP142_lineXY(color, x2, y1, x2, y2, width);
	GP142_lineXY(color, x2, y2, x1, y2, width);
}



void Display(int color, int width) {
	width = 2;
	color = 19;
	int col1 = 1;
	displayline(0, -292, 240, 268, 200, color, width);
	displayline(0, 257, 240, 292, -240, color, width);
	displayline(0, -350, 240, -226, -240, color, width);
	displayline(0, -230, -212, 292, -240, color, width);
	// drawing space
	displayrectangle(255, -210, -225, 200, color, width);

	for (int i = -223; i < 253; i++)
	{
		for (int j = -208; j < 198; j++)
		{
			GP142_pixelXY(0, i, j);
		}
	}

	int coordinates = 210;
	int i = 0;
	//drawing tools selection buttons
	while (i < 11)
	{
		displayrectangle(-307, coordinates + 5, -255, coordinates - 20, color, width);
		coordinates = coordinates - 35;
		i++;
	}
	coordinates = 210;

	displayline(RED, -292, coordinates - 8, -270, coordinates - 8, RED, 2);
	coordinates = coordinates - 35;
	GP142_textXY(BLUE, -300, (coordinates + (coordinates - 35)) / 2, 25, "-");
	GP142_textXY(BLUE, -290, (coordinates + (coordinates - 35)) / 2, 25, "-");
	GP142_textXY(BLUE, -280, (coordinates + (coordinates - 35)) / 2, 25, "-");
	GP142_textXY(BLUE, -270, (coordinates + (coordinates - 35)) / 2, 25, "-");
	coordinates = coordinates - 35;
	GP142_textXY(GREEN, -280, (coordinates + (coordinates - 30)) / 2, 45, ".");
	GP142_textXY(GREEN, -290, (coordinates + (coordinates - 40)) / 2, 45, ".");
	GP142_textXY(GREEN, -300, (coordinates + (coordinates - 50)) / 2, 45, ".");
	coordinates = coordinates - 35;
	GP142_textXY(ORANGE, -305, (coordinates + (coordinates - 30)) / 2, 10, "P");
	GP142_textXY(ORANGE, -300, (coordinates + (coordinates - 30)) / 2, 10, "o");
	GP142_textXY(ORANGE, -293, (coordinates + (coordinates - 30)) / 2, 10, "l");
	GP142_textXY(ORANGE, -289, (coordinates + (coordinates - 30)) / 2, 10, "y");
	GP142_textXY(ORANGE, -281, (coordinates + (coordinates - 30)) / 2, 10, "g");
	GP142_textXY(ORANGE, -275, (coordinates + (coordinates - 30)) / 2, 10, "o");
	GP142_textXY(ORANGE, -268, (coordinates + (coordinates - 30)) / 2, 10, "n");
	coordinates = coordinates - 35;
	displayrectangle(-290, coordinates - 5, -280, coordinates - 15, color, 2);
	coordinates = coordinates - 35;
	GP142_textXY(YELLOW, -293, (coordinates + (coordinates - 35)) / 2, 20, "O");
	coordinates = coordinates - 35;
	GP142_textXY(11, -302, (coordinates + (coordinates - 30)) / 2, 15, "T");
	GP142_textXY(11, -295, (coordinates + (coordinates - 30)) / 2, 15, "e");
	GP142_textXY(11, -285, (coordinates + (coordinates - 30)) / 2, 15, "x");
	GP142_textXY(11, -275, (coordinates + (coordinates - 30)) / 2, 15, "t");
	coordinates = coordinates - 35;
	for (int i = -295; i < -270; i++)
	{
		for (int j = coordinates - 15; j < coordinates - 1; j++)
		{
			GP142_pixelXY(WHITE, i, j);
		}
	}
	//for bucket / fill colour
	coordinates = coordinates - 35;
	for (int i = -303; i < -297; i++)
	{
		for (int j = coordinates - 15; j < coordinates - 1; j++)
		{
			GP142_pixelXY(PURPLE, i, j);
		}
	}
	for (int i = -297; i < -292; i++)
	{
		for (int j = coordinates - 15; j < coordinates - 1; j++)
		{
			GP142_pixelXY(RED, i, j);
		}
	}
	for (int i = -292; i < -284; i++)
	{
		for (int j = coordinates - 15; j < coordinates - 1; j++)
		{
			GP142_pixelXY(BLUE, i, j);
		}
	}
	for (int i = -284; i < -278; i++)
	{
		for (int j = coordinates - 15; j < coordinates - 1; j++)
		{
			GP142_pixelXY(BROWN, i, j);
		}
	}
	for (int i = -278; i < -272; i++)
	{
		for (int j = coordinates - 15; j < coordinates - 1; j++)
		{
			GP142_pixelXY(WHITE, i, j);
		}
	}
	for (int i = -272; i < -266; i++)
	{
		for (int j = coordinates - 15; j < coordinates - 1; j++)
		{
			GP142_pixelXY(YELLOW, i, j);
		}
	}
	for (int i = -266; i < -260; i++)
	{
		for (int j = coordinates - 15; j < coordinates - 1; j++)
		{
			GP142_pixelXY(GREEN, i, j);
		}
	}
	coordinates = coordinates - 35;
	GP142_textXY(BLUE, -305, (coordinates + (coordinates - 30)) / 2, 15, "S");
	GP142_textXY(BLUE, -295, (coordinates + (coordinates - 30)) / 2, 15, "a");
	GP142_textXY(BLUE, -285, (coordinates + (coordinates - 30)) / 2, 15, "v");
	GP142_textXY(BLUE, -275, (coordinates + (coordinates - 30)) / 2, 15, "e");
	coordinates = coordinates - 35;
	GP142_textXY(RED, -302, (coordinates + (coordinates - 30)) / 2, 15, "Q");
	GP142_textXY(RED, -290, (coordinates + (coordinates - 30)) / 2, 15, "u");
	GP142_textXY(RED, -282, (coordinates + (coordinates - 30)) / 2, 15, "i");
	GP142_textXY(RED, -278, (coordinates + (coordinates - 30)) / 2, 15, "t");
	GP142_textXY(RED, -275, (coordinates + (coordinates - 30)) / 2, 15, "!");

	coordinates = coordinates - 35;

	coordinates = -210;

	for (int i = 0; i < 24; i++)
	{

		// drawing paint colours outline circle
		displayrectangle( coordinates+3, 227, coordinates-13, 210,19,0);
		// filling colours in above circles
		//displayrectangle(coordinates , 220, coordinates - 10, 213, i, 0);
		for (int k = coordinates -12; k < coordinates +3 ; k++)
		{
			for (int j = 211; j < 227; j++)
			{
				GP142_pixelXY(i, k, j);

			}
		}
		coordinates = coordinates + 20;
	}


}




int ColReturner(int c){
	     if (c == 0)        return 0;
	else if (c == 16777215) return 1;
	else if (c == 255)      return 2;          
	else if (c == 65280)    return 3;
	else if (c == 16711680) return 4;
	else if (c == 65535)    return 5;
	else if (c == 16711935) return 6;
	else if (c == 16776960) return 7;
	else if (c == 13369497) return 8;
	else if (c == 10040064) return 9;
	else if (c == 10053273) return 10;
	else if (c == 16777113) return 11;
	else if (c == 13434777) return 12;
	else if (c == 26367)    return 13;
	else if (c == 3368601)  return 14;
	else if (c == 13408767) return 15;
	else if (c == 10092543) return 16;
	else if (c == 3381708)  return 17;
	else if (c == 6724095)  return 18;
	else if (c == 3368448)  return 19;
	else if (c == 10079283) return 20;
	else if (c == 3381657)  return 21;
	else if (c == 8421504)  return 22;
	else if (c == 13421772) return 23;
}

bool checkbox(int x1, int y1, int x2, int y2, int x, int y) {
	if (x <= x1 && x >= x2 && y <= y1 && y >= y2) return true;
	else	if (x >= x1 && x <= x2 && y >= y1 && y <= y2) return true;
	else	if (x >= x1 && x <= x2 && y <= y1 && y >= y2) return true;
	else	if (x <= x1 && x >= x2 && y >= y1 && y <= y2) return true;
	else return false;
}

void boxselector(int x1, int y1, int x2, int y2, int co, int width) {


	displayrectangle(x1 + 1, y1 - 1, x2 - 1, y2 + 1, co, width);

}

void colorchecker(int mousex, int mousey, int & color){
	if (checkbox(-223, 227, - 207, 210, mousex, mousey)){
		color = 0;

	}
	else 	if (checkbox(-203, 227,- 187, 210, mousex, mousey)) {
		color = 1;

	}
	else 	if (checkbox(-183 , 227, -167, 210, mousex, mousey)) {
		color = 2;
	}
	else 	if (checkbox(-163, 227, -147, 210, mousex, mousey)) {
		color = 3;
	}
	else 	if (checkbox(-143, 227, -127, 210, mousex, mousey)) {
		color = 4;
	}
	else 	if (checkbox(-123, 227, -107, 210, mousex, mousey)) {
		color = 5;
	}
	else 	if (checkbox(-103, 227, -87, 210, mousex, mousey)) {
		color = 6;
	}
	else 	if (checkbox(-83, 227, -67, 210, mousex, mousey)) {
		color = 7;
	}
	else 	if (checkbox(-63, 227, -47, 210, mousex, mousey)) {
		color = 8;
	}
	else 	if (checkbox(-43, 227, -27, 210, mousex, mousey)) {
		color = 9;
	}
	else 	if (checkbox(-23, 227, -7, 210, mousex, mousey)) {
		color = 10;
	}
	else 	if (checkbox(-3, 227, 13, 210, mousex, mousey)) {
		color = 11;
	}
	else 	if (checkbox(17, 227, 33, 210, mousex, mousey)) {
		color = 12;
	}
	else 	if (checkbox(37, 227, 53, 210, mousex, mousey)) {
		color = 13;
	}
	else 	if (checkbox(57, 227, 73, 210, mousex, mousey)) {
		color = 14;
	} 
	else 	if (checkbox(77, 227, 93, 210, mousex, mousey)) {
		color = 15;
	}
	else 	if (checkbox(97, 227, 113, 210, mousex, mousey)) {
		color = 16;
	}
	else 	if (checkbox(117, 227, 133, 210, mousex, mousey)) {
		color = 17;
	}
	else 	if (checkbox(137, 227, 153, 210, mousex, mousey)) {
		color = 18;
	}
	else 	if (checkbox(157, 227, 173, 210, mousex, mousey)) {
		color = 19;
	}
	else 	if (checkbox(177, 227, 193, 210, mousex, mousey)) {
		color = 20;
	}
	else 	if (checkbox(197, 227, 213, 210, mousex, mousey)) {
		color = 21;
	}
	else 	if (checkbox(217, 227, 233, 210, mousex, mousey)) {
		color = 22;
	}
	else 	if (checkbox(237, 227, 253, 210, mousex, mousey)) {
		color = 23;
	}
}





void LoadFile(vector <shapes *> & all)
{
	int mousex;
	int mousey;
	char keypressed;


	GP142_textXY(BLUE, -105, -20, 25, "L");
	GP142_textXY(BLUE, -92, -20, 25, "o");
	GP142_textXY(BLUE, -78, -20, 25, "a");
	GP142_textXY(BLUE, -65, -20, 25, "d");
	GP142_rectangleXY(BLUE, -115, 10, -45, -20, 1);
	GP142_textXY(RED, 50, -20, 25, "N");
	GP142_textXY(RED, 68, -20, 25, "e");
	GP142_textXY(RED, 80, -20, 25, "w");
	GP142_rectangleXY(RED, 110, 10, 38, -20, 1);
	GP142_await_event(&mousex, &mousey, &keypressed);
	if (mousex >= -115 && mousex <= -45 && mousey <= 10 && mousey >= -20)
	{
		GP142_clear();
		
		Display(20, 1);
		ifstream fin("draw.txt");
		int a;
		int b;
		int c;
		int f;
		bool s = true;
		double x;
		double y;
		vector <Point> d;
		char * t=nullptr;
		int l;
		while (fin.eof() != true)
		{
			fin >> a;
			fin >> b;
			fin >> c;

			if (c == 6)
			{
				fin >> l;

				t = new char[l + 1];
				for (int i = 0; i < l; i++)
				{
					fin >> t[i];
				}
				t[l] = '\0';

			}

			

			else if (c == 1)
			{
				fin >> s;
			}

			fin >> f;

			d.resize(b);
			for (int i = 0; i < b; i++)
			{
				fin >> d[i].x;
				fin >> d[i].y;

			}
			

			
		   if (c == 1)
		  {
			 line* obj = new line(a, b, c, f, d, s);
			 obj->draw();
			 all.push_back(obj);

		   } 

			 if (c == 2)
			{
				curve* obj = new curve(a, b, c, f, d);
				obj->draw();
				all.push_back(obj);
			}
			else if (c == 3)
			{
				polygon* obj = new polygon(a, b, c, f, d);
				obj->draw();
				for (double k = -255; k < 255; k++)
				{
					for (double j = -210; j < 220; j++)
					{
						Point p;
						p.x = k;
						p.y = j;

						if (obj->contain(p) == true)
						{
							GP142_pixelXY(f, k, j);
						}
					}
				}
				all.push_back(obj);
			}
			else if (c == 4)
			{
				rectangle* obj = new rectangle(a, b, c, f, d);
				obj->draw();
				if (f != 0)
				{
					for (int i = d[0].x + 1; i < d[1].x; i++)
					{
						for (int j = d[1].y + 1; j < d[0].y; j++)
						{
							GP142_pixelXY(f, i, j);

						}
					}
				}
				all.push_back(obj);
			}
			else if (c == 5)
			{
				circle* obj = new circle(a, b, c, d);
				obj->draw();
				if (f != 0)
				{
					for (double i = 0; i < distance(d[0].x, d[0].y, d[1].x, d[1].y) - 1; i++)

						GP142_circleXY(f, d[0].x, d[0].y, i, 0);
				}
				all.push_back(obj);
			}
			else if (c == 6)
			{
				text* obj = new text(a, b, c, d, t, l);
				obj->draw();
				all.push_back(obj);
			}
		}

		
	}
	
		
	else
	{
		GP142_clear();
		ofstream fout("draw.txt", std::ofstream::out | std::ofstream::trunc);
		fout.close();
		Display(20, 1);
	}
}

void ToolSelector(int mousex, int mousey, char& keypressed, int& quit, int color, int colorf, int width, vector<shapes*>& all, int& index, int& size)
{
    width = 1;
	color = RED;
	colorf = 0;
	int x = 0, y = 0;
	int x1 = 0, y1 = 0;
	int x2 = 0, y2 = 0;
	int coord = 210;
	double rad = 0;
	//Line
	if (checkbox(-307, 210, -255, 195, mousex, mousey)){
		boxselector(-307, 210, -255, 195, color, width);
		bool sty = true;
		GP142_await_event(&mousex, &mousey, &keypressed);
		colorchecker(mousex, mousey, color);
		width = 1;
	
		
		line  *a = new line(color, sty, colorf);
		all.push_back(a);
		all[index]->draw();
		index++;
		
		
		boxselector(-307, 210, -255, 195, 0,width);
	}
	//Doted Line 
	else if (checkbox(-307, 175, -255, 160, mousex, mousey))
	{
		boxselector(-307, 175, -255, 160, color,width);
		bool sty= false;
		
		GP142_await_event(&mousex, &mousey, &keypressed);
		colorchecker(mousex, mousey, color);
		width = 1;
		x++;
	
		line* a = new line(color, sty, colorf);
		all.push_back(a);
		all[index]->draw();
		index++;
		boxselector(-307, 175, -255, 160, 0,width);
	}
	// Curve
	else if (checkbox(-307, 140, -255, 125, mousex, mousey))
	{
		boxselector(-307, 140, -255, 125, color, width);
		GP142_await_event(&mousex, &mousey, &keypressed);
		colorchecker(mousex, mousey, color);

		curve* a = new curve(color, colorf);
		a->draw();
		all.push_back(a);
		index++;
		boxselector(-320, 140, -242, 125, 0, width);
	}
	//Polygon
	else if (checkbox(-307, 105, -255, 90, mousex, mousey))
	{
		boxselector(-307, 105, -255, 90, color,width);
		GP142_await_event(&mousex, &mousey, &keypressed);
		colorchecker(mousex, mousey, color);
	
		polygon *  a = new polygon(color, colorf);
		a->draw();
		all.push_back(a);
		index++;
	
		boxselector(-307, 105, -255, 90,0,width);
	}
	// Rectangle
	else 	if (checkbox(-307, 70, -255, 55, mousex, mousey)){
		boxselector(-307, 70, -255, 55, color,width);
		GP142_await_event(&mousex, &mousey, &keypressed);
		colorchecker(mousex, mousey, color);
		
		rectangle *  a = new rectangle(color, colorf);
		a->draw();
		all.push_back(a);
		index++;
		boxselector(-307, 70, -255, 55, 0,width);
	}
	//Circle
	else 	if (checkbox(-307, 35, -255, 20, mousex, mousey)){
		boxselector(-307, 35, -255, 20,color,width);
		GP142_await_event(&mousex, &mousey, &keypressed);
		colorchecker(mousex, mousey, color);
		
		circle *  a = new circle(color);
		a->draw();
		all.push_back(a);
		index++;
		boxselector(-307, 35, -255, 20, 0,width);
	}
	// Text
	else 	if (checkbox(-307, 0, -255, -15, mousex, mousey)){
		boxselector(-307, 0, -255, -15, color,width);
		GP142_await_event(&mousex, &mousey, &keypressed);
		colorchecker(mousex, mousey, color);
		GP142_await_event(&mousex, &mousey, &keypressed);
		x = mousex;
		y = mousey;
		char a[200];
		int i = 0;
		int event;
		quit = FALSE;
		while (!quit) {
			event = GP142_await_event(&mousex, &mousey, &key);
			switch (event) {
			case GP142_KBD:
				a[i] = key;
				i++;
				break;
			default:
				quit = TRUE;
				break;
			}
		}
		quit = FALSE;
		a[i] = '\0';

		text* b = new text(x, y, a, color);
		b->draw();
		all.push_back(b);
		index++;
		boxselector(-307, 0, -255, -15, 0,width);
	}
	//Eraser
	else 	if (checkbox(-307, -35, -255, -50, mousex, mousey)){
		boxselector(-307, -35, -255, -50, color,width);
		mousex = 0, mousey = 0;
		GP142_await_event(&mousex, &mousey, &key);
		while (!checkbox(-320, -35, -255, -50, mousex, mousey))
		{
			if (all.size() == 0)
			{
				break;
			}
			int x = 0;
			Point z;
			z.x = mousex;
			z.y = mousey;
			for (int i = 0; i<all.size(); i++) 
			{
				if (all[i]->contain(z)==true)
				{
					all[i]->erasecol();
					all[i]->changecolor(0);
					all[i]->draw();
					x = i;
					int a;
					int b;
					int c;
					int f;
					vector <Point> d;
					char* array = 0;
					bool s = true;
					int l;

							
					all[i]->set(a, b, c, s, f, d, l, array);

					if (c == 3)
					{
	    				for (double k = -255; k < 255; k++)
						{
						   for (double j = -210; j < 220; j++)
						   {
								Point p;
								p.x = k;
								p.y = j;

					    		if (all[i]->contain(p) == true)
								{
											GP142_pixelXY(a, k, j);
								}
							}
						}
					 break;
					}

					else if (c == 4)
					{

						for (int i = d[0].x + 1; i < d[1].x - 1; i++)
					    {
							for (int j = d[1].y + 2; j < d[0].y; j++)
						    {
							  GP142_pixelXY(f, i, j);

							}
						}
					 break;
					}

					else if (c == 5)
					{
						for (double i = 0; i < distance(d[0].x, d[0].y, d[1].x, d[1].y) - 1.42; i++)
						{
							GP142_circleXY(f, d[0].x, d[0].y, i, 0);
						}
					 break;
					}


					else if (c == 6)
					{
					all[i]->draw();
					}

					break;
				}
			}
			if (all[x]->contain(z) == true)
			{
				
				all.erase(all.begin() + x);
				index--;
			}
			
		
			GP142_await_event(&mousex, &mousey, &key);
		}

		boxselector(-307, -35, -255, -50, 0,width);
	}
	//Fill Color
	else 	if (checkbox(-307, -70, -255, -85, mousex, mousey))
    {
		boxselector(-307, -70, -255, -85, color,width);
		mousex = 0, mousey = 0;
		GP142_await_event(&mousex, &mousey, &keypressed);
		colorchecker(mousex, mousey, colorf);
		GP142_await_event(&mousex, &mousey, &keypressed);
		Point z;
		z.x = mousex;
		z.y = mousey;
		z.x = mousex;
		z.y = mousey;
		int a;
		int b;
		int c;
		int f=colorf;
		vector <Point> d;
		char* array = 0;
		bool s=true;
		int l;
		for (int i = 0,j=0; i < all.size(); i++)
		{
			if (all[i]->contain(z) == true)
			{
				all[i]->setcolorfilled(f);
				all[i]->set(a,b,c,s,f,d,l,array);
				
			   if (c == 1)
			  {
                
				all[i]->changecolor(colorf);
				all[i]->draw();
				j++;
   			    }

			   else if (c == 2)
			   { 
			     all[i]->changecolor(colorf);
				 all[i]->draw();
				 j++;
		 	    }

			   else if (c == 3)

			   {
				   for (double k = -255; k < 255; k++)
				   {
					   for (double j = -210; j < 220; j++)
					   {
						   Point p;
						   p.x = k;
						   p.y = j;

						   if (all[i]->contain(p) == true)
						   {
							   GP142_pixelXY(colorf, k, j);
						   }
					   }
				   }
			   }

		        else if (c == 4)
				{
					
					for (int i = d[0].x + 1; i < d[1].x-1; i++)
					{
						for (int j = d[1].y+2; j < d[0].y; j++)
						{
							GP142_pixelXY(colorf, i, j);

						}
					}
					break;
				}

				else if (c == 5)
				{
					for(double i=0;i<distance(d[0].x, d[0].y, d[1].x, d[1].y)-1.42;i++)

					GP142_circleXY(colorf,d[0].x , d[0].y, i, 0);
				}
				
				
			   else if (c == 6)
			   {
				   all[i]->changecolor(colorf);
				   all[i]->draw();
			   }
				


				

			}

		}
		
		
		boxselector(-307, -70, -255, -85,0,width);
	}
	// Save
	else 	if (checkbox(-307, -105, -255, -120, mousex, mousey)){
		boxselector(-307, -105, -255, -120, color,width);
		ofstream fout("draw.txt", std::ios_base::app);
		int a;
		int b;
		int c;
		int f;
		bool sa=0;
		vector <Point> d;
		char * array = 0;
		int l;
		for (int i = 0; i < all.size(); i++)
		{
			
			all[i]->set(a, b, c,sa,f,d,l,array);
			fout << a << endl;
			fout << b << endl;
			fout << c << endl;
		
			
			if (c == 1)
			{
				fout << sa << endl;
			}
			
			if (c == 6)
			{
				fout << l << endl;
				fout << array << endl;
			}
			fout << f << endl;
			for (int i = 0; i < b; i++)
			{
				fout << d[i].x << endl;
				fout << d[i].y << endl;
			}
			
		}
		fout.close();
		boxselector(-307, -105, -255, -120, 0,width);
	}
	else 	if (checkbox(-307, -140, -255, -155, mousex, mousey)){
		boxselector(-307, -140, -255, -155, color,width);
		quit = TRUE;
		boxselector(-307, -140, -255, -155, 0,width);
	}
	
	
}



int main()
{
	int size = 0;
	int color = 0;
	vector<shapes*>allshapes;
	int colorindex = 0;
	int index = 0;
	int colorf=0;
	int width = 1;
	int quit=false;
	int event;
	int mousex;
	int mousey;
	char key;
	int x = 0, y = 0;
	int x1 = 0, y1 = 0;
	int x2 = 0, y2 = 0;
	double radius = 0;

	GP142_open();
	Display(color,width);
	LoadFile(allshapes);
	
	quit = false;
	while (quit!=true)
	{
		
		event = GP142_await_event(&mousex, &mousey, &key);
		switch (event) {
		case GP142_QUIT:
			quit = true;
			break;

		case GP142_MOUSE:
			
		 ToolSelector(mousex, mousey, key, quit,color, colorf,width,allshapes,index,size);
			break;

		case GP142_KBD:
			break;

		default:
			break;
		}
	}

	
	GP142_close();
	return 0;
}
