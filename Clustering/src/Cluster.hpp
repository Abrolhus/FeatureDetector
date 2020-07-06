#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>
#include <set>
using namespace std;
using namespace cv;

struct Color{
    int b;
    int g;
    int r;
    Color(int pb, int pg, int pr) : b(pb), g(pg), r(pr){
    }
    Color(){

        b = 0;
        g = 0;
        r = 0;
    }
};
class Cluster {
    public:
        Cluster(string name, Color color);
        int b();
        int g();
        int r();
        // Cluster(string name, string color);
        Cluster(string name, int b, int g, int r);
        Cluster(string name, int hexColor);
        void printElements();    
        std::string getColorString();
        int addElement(int val);
        int removeElement(int val);
        bool findElement(int val);
        string getName();
        int getColor();
        int getSize();
        void setName(string name);
        // void setColor(Color color);
        void setColor(int hexColor);
        // set<int> getElements();
        bool isInRange(int val);
    private:
        string name;
        // set<int> elements;
        int upperLimit;
        int lowerLimit;
        // Color color;
        int color;
        Color minColor;
        Color maxColor;
        bool newElementVerifier(int val);
        int bgrToInt(int b, int g, int r);
        Color intToColor(int hexColor);
        
};

