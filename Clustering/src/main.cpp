#include "opencv2/imgproc.hpp" // #include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>
#include <set>
#include "Clusterizador.hpp"

using namespace cv;
using namespace std;

typedef struct 
{
    Mat image;
    Clusterizador* clus;
} ClusteringParams;

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        cout << " Usage: display_image ImageToLoadAndDisplay" << endl;
        return -1;
    }

    int x = 130, y = 50; 
    Mat image;
    image = imread(argv[1], CV_LOAD_IMAGE_COLOR); // Read the file
    if (!image.data) // Check for invalid input
    {
        cout << "Could not open or find the image" << std::endl;
        return -1;
    }

    void onMouse(int event, int x, int y, int, void* params);
    Clusterizador* Clst = new Clusterizador(40, 40);
    ClusteringParams clusParams = {image, Clst};

    namedWindow("image", 0);
    Clst->createNewCluster("Azul", 0, 0, 255);
    setMouseCallback("image", onMouse, &clusParams);
    // Clst->addToClusterByImage(image, "Azul", x, y);

    for(;;){

        Clst->clusterizarImagem(&image, "Azul");
        imshow("image", image);
        char c = (char)waitKey(60);
        if(c == 'q' || c == 27)
            break;
    }
    Clst->printClusters();
}
void onMouse(int event, int x, int y, int, void* params)
{

    ClusteringParams* clusParams = (ClusteringParams*) params;
    Clusterizador* cls = clusParams->clus;
    Mat img = clusParams->image;

    if (event != CV_EVENT_LBUTTONDOWN)
        return;
    else if(x> img.size().width || x < 0 || y > img.size().height || y < 0)
        return;
    cls->addToClusterByImage(img, "Azul", x, y);
}
