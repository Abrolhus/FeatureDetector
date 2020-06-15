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
    Mat* image;
    Clusterizador* clus;
    string* p_currentCluster;
} ClusteringParams;

int main(int argc, char **argv)
{
    Mat image;
    string video;
    string file;
    switch (argc){
        case 2:
            video = argv[1]; 
            break;
        case 3: 
            video = argv[1]; 
            file = argv[2];
            break;
        default:     
            cout << "Usage: ./this video.ext file.txt[OPTIONAL]";     
            return -2;
            break;
    } 

    if(file.empty()) file  = "clustering.txt";
     
    bool playVideo = true;
    int mainLoop (bool* playVideo, VideoCapture* videoCap, Mat* p_image, string window, Clusterizador* clust, string* currentCluster);
    void onMouse(int event, int x, int y, int, void* params);
    Clusterizador* Clst = new Clusterizador(40, 40);
    VideoCapture* VideoCap = new VideoCapture(video);
    string currentCluster = "campo";
    ClusteringParams clusParams = {&image, Clst, &currentCluster};

    namedWindow("image", 0);
    Clst->createNewCluster("campo", 255, 0, 0);
    Clst->createNewCluster("jerseys", 0, 255, 255);
    // Clst->addToClusterViaFile("naoehazul.txt", currentCluster);
    setMouseCallback("image", onMouse, &clusParams);
    // Clst->addToClusterByImage(image, "Azul", x, y);

    for(;;){
        mainLoop(&playVideo, VideoCap, &image, "image", Clst, &currentCluster); 
    }
    Clst->printClusters();
}
void onMouse(int event, int x, int y, int, void* params)
{
    cout << "@onMouse ";
    ClusteringParams* clusParams = (ClusteringParams*) params;
    Clusterizador* cls = clusParams->clus;
    string currentCluster = *(clusParams->p_currentCluster);
    Mat img; 
    clusParams->image->copyTo(img);
    if(!img.data)
       cout << "NULL IMAGE "; 
    if (event != CV_EVENT_LBUTTONDOWN)
        return;
    else if(x> img.size().width || x < 0 || y > img.size().height || y < 0)
        return;
    cout << "end of @onMouse "; 
    cls->addToClusterByImage(img, currentCluster, x, y);
}

int mainLoop (bool* playVideo, VideoCapture* videoCap, Mat* p_image, string window, Clusterizador* clust, string* currentCluster)
{
    Mat frame;
    string* p_currentCluster = currentCluster;
    if(*playVideo)   
        *videoCap >> frame; // get a new frame from camera
    if(frame.empty() && p_image->empty()) 
        return 0; 
    else if(!frame.empty())
        frame.copyTo(*p_image);

    clust->clusterizarImagem(p_image, "jerseys");
    clust->clusterizarImagem(p_image, "campo");
    imshow(window, *p_image);
    // if(waitKey(30) >= 0) break;
    char c = (char)waitKey(30);
    if (c == 27 || c == 'q')
    {
        cout << "Exiting ...\n";
        return -1;
    }
    switch (c)
    {
        case 'r':
            cout << "Original image is restored\n";
            // image0.copyTo(image);
            break;
        case 'w':
        case 's':
            cout << "saving to file..." << endl;
            // save(*st, file);
            break;
        case 'n':
            cout << "printing st..." << endl;
            clust->printClusters();
            break;
        case 'u':
            cout << "undoing";
            // removeSetFromSet(st, undoSet);
            break;
        case '1':
            *p_currentCluster = "campo";
            break;
        case '2':
            *p_currentCluster = "jerseys";
            break;
        case 'p':
        case ' ':
            *playVideo = !(*playVideo); 
    }
    cout<< ". ";
    return 0;
}

