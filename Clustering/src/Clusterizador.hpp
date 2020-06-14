#include "Cluster.hpp"
#include "opencv2/imgproc.hpp" // #include "opencv2/videoio.hpp"
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
#define NEWMASKVAL 255
#define CONNECTIVITY 4
#define CONNECTIVITY 4

class Clusterizador {
    public:
        bool createNewCluster(string name, string color);
        void printClusters();
        int addToClusterByImage(Mat image, string cluster, int x, int y);
        int getLoDiff();
        int getupDiff();
        void setLoDiff(int val);
        void setUpDiff(int val);
        static int vec3bToInt(cv::Vec3b vec);
    private:
        map<string, Cluster> clusters;
        int loDiff, upDiff;
}; 
