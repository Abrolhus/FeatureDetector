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

class Cluster {
    public:
        Cluster(string name, string color);
        void printElements();    
        int addElement(int val);
        int removeElement(int val);
        string getName();
        string getColor();
        int getSize();
        void setName(string name);
        void setColor(string color);
    private:
        string name;
        set<int> elements;
        string color;
        bool newElementVerifier(int val);
};
