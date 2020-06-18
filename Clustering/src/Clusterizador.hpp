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
// using namespace cv;
// using namespace std; 
/*note: avoid using "using namespace std in include files, it may break the code 
 * https://stackoverflow.com/questions/5849457/using-namespace-in-c-headers
 */
#define NEWMASKVAL 255
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
        bool createNewCluster(std::string name, Color color);
        bool createNewCluster(std::string name, int b, int g, int r);
        void printClusters();
        int addToClusterByImage(cv::Mat image, std::string cluster, int x, int y);
        int addToClusterViaFile(std::string file, std::string cluster);
        int clusterizarImagem(cv::Mat* img, std::string cluster);
        int saveClusterToFile(std::string file, std::string cluster);
        int saveAllClustersToFile();
        int saveAllClustersToFile(string filePrefix);
        int getLoDiff();
        int getupDiff();
        vector<string> getClusterNames();
        void setLoDiff(int val);
        void setUpDiff(int val);
    private:
        map<std::string, Cluster> clusters;
        int loDiff, upDiff;
        int vec3bToInt(cv::Vec3b vec);
        bool checkIfClusterExists(string cluster);

}; 
