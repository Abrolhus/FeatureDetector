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

// struct Color{
    // int b;
    // int g;
    // int r;
    // Color(int pb, int pg, int pr) : b(pb), g(pg), r(pr){
    // }
// };
class Clusterizador {
    public:
        Clusterizador(int lo, int up);
        Clusterizador();
        bool createNewCluster(string name, Color color);
        bool createNewCluster(string name, int b, int g, int r);
        void printClusters();
        int addToClusterByImage(cv::Mat image, string cluster, int x, int y);
        int addToClusterViaFile(string file, string cluster);
        int clusterizarImagem(cv::Mat* img, string cluster);
        int getLoDiff();
        int getupDiff();
        void setLoDiff(int val);
        void setUpDiff(int val);
    private:
        map<string, Cluster> clusters;
        int loDiff, upDiff;
        int vec3bToInt(cv::Vec3b vec);
}; 
