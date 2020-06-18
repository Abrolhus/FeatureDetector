#include "opencv2/imgproc.hpp" // #include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>
#include <set>
#include "src/Clusterizador.hpp"

using namespace cv;
using namespace std;

typedef struct 
{
    Mat* image;
    Clusterizador* clus;
    string* p_currentCluster;
} ClusteringParams;

typedef struct{
    string name;
    int b;
    int g;
    int r;
} ClusterConfigs;

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
    void help();
    void onMouse(int event, int x, int y, int, void* params);
    Clusterizador* Clst = new Clusterizador(40, 40);
    VideoCapture* VideoCap = new VideoCapture(video);
    string currentCluster = "campo";
    ClusteringParams clusParams = {&image, Clst, &currentCluster};
    vector<ClusterConfigs> clusterVector; // vector contendo nome e cor dos clusters a serem
                       // adicionados ao clusterizador
    clusterVector.push_back({"campo", 255, 0, 0});
    clusterVector.push_back({"jerseys", 0, 255, 255});

    namedWindow("image", 0);
    for(auto it = clusterVector.begin(); it != clusterVector.end(); ++it){
        Clst->createNewCluster(it->name, it->b, it->g, it->r);
    }
    // Clst->createNewCluster("jerseys", 0, 255, 255);
    // Clst->addToClusterViaFile("naoehazul.txt", currentCluster);
    setMouseCallback("image", onMouse, &clusParams);
    // Clst->addToClusterByImage(image, "Azul", x, y);
    help();
    
    for(int flag = 0; flag != -1;){
        flag = mainLoop(&playVideo, VideoCap, &image, "image", Clst, &currentCluster); 
    }
    Clst->printClusters();
}
void onMouse(int event, int x, int y, int, void* params)
{
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
        case 'W':
        case 'S':
            cout << "saving " << *p_currentCluster << " to file..." << endl;
            // save(*st, file);
            clust->saveClusterToFile("teste.txt", *currentCluster);           
            break;
        case 's':
        case 'w':
            cout << "Saving all Clusters to file..." << endl; 
            clust->saveAllClustersToFile();

        case 'n':
            cout << "printing clusters..." << endl;
            clust->printClusters();
            break;
        case 'u':
            cout << "undoing";
            // removeSetFromSet(st, undoSet);
            break;
        case '1' ... '9':
        { /* gambiarra para trocar de cluster de acordo com o numero (1 a 9) digitado; 
           * Note: the brackets are here to destroy the vector "aux" after this case ends;
           * https://stackoverflow.com/questions/61708267/jump-bypasses-variable-initialization-in-switch-statement
           */
            vector<string> aux = clust->getClusterNames(); 
            cout << aux.size() << endl;
            if(c <= '0' + aux.size()){
                *p_currentCluster = aux.at(c - '0' - 1); //TODO make it less dangerous. I don't think there is case where it can go wrong, but who knows;
                cout << "switched to " << *p_currentCluster << " cluster " << endl;
            }
            break;
        }
        case 'p':
        case ' ':
            *playVideo = !(*playVideo); 
    }
    return 0;
}
void help(){
    cout << "Usage: ./this video.ext" << endl <<
            "Click on image to floodfill it;" << endl << 
            "Keyboard Commands:" << endl <<
            "\t(u)ndo, (p)ause, (n) Print Clusters," << endl << 
            "\t(s)ave clusters to file" << endl <<
            "\t1 - 9: change to cluster x"<< endl << 
            "\t(q)uit" << endl;
 }
