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
        // Cluster(string name, string color);
        Cluster(string name, int b, int g, int r);
        void printElements();    
        void printColor();
        int addElement(int val);
        int removeElement(int val);
        bool findElement(int val);
        string getName();
        Color getColor();
        int getSize();
        void setName(string name);
        void setColor(Color color);
        set<int> getElements();
    private:
        string name;
        set<int> elements;
        Color color;
        bool newElementVerifier(int val);
};
