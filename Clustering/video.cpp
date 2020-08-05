#include "opencv2/imgproc.hpp" // #include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <sstream>
#include <vector>
#include <set>
#include "src/Clusterizador.hpp"

///Colors///
//FIXED: Fix this, from RGB to BGR
#define RED 0x0000cc
#define PURPLE 0xff0080
#define BLUE 0xff0000
#define GREEN 0x009900
#define YELLOW 0x00ffff
#define LBLUE 0x999900
#define ORANGE 0x0066ff
#define LGREEN 0x00cc99
#define LILAS 0xff99cc

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

    ///functions///
    int mainLoop (bool* playVideo, VideoCapture* videoCap, Mat* p_image, string window, Clusterizador* clust, string* currentCluster);
    void help();
    void onMouse(int event, int x, int y, int, void* params);

    Clusterizador* Clst = new Clusterizador(40, 40);
    vector<int> colorList{BLUE, YELLOW, PURPLE, GREEN, ORANGE, LBLUE, LGREEN, RED, LILAS};
    Mat image;
    string video;
    string file;
    switch (argc){
        // case 2:
            // video = argv[1]; 
            // break;
        case 3 ... 11: 
        {
            video = argv[1]; 
            for(int i = 2; i < argc; i++){
                Clst->createNewCluster(string(argv[i]), colorList.at(i - 2));
                Clst->addToClusterViaFile(string(argv[i]) + ".txt", argv[i]);
            }
            file = argv[2];
            break;
        }
        default:     
            help();
            return -2;
            break;
    } 
     
    bool playVideo = true;
    VideoCapture* VideoCap = new cv::VideoCapture(video);
    string currentCluster = "";
    ClusteringParams clusParams = {&image, Clst, &currentCluster};
    vector<ClusterConfigs> clusterVector; // vector contendo nome e cor dos clusters a serem
                                         //  adicionados ao clusterizador
    // clusterVector.push_back({"campo", 255, 0, 0});
    // clusterVector.push_back({"jerseys", 0, 255, 255});

    namedWindow("image", 0);
    // for(auto it = clusterVector.begin(); it != clusterVector.end(); ++it){
        // Clst->createNewCluster(it->name, it->b, it->g, it->r);
        // Clst->addToClusterViaFile(it->name + ".txt", it->name);
    // }
    // Clst->createNewCluster("teste123", RED);
    currentCluster = Clst->getClusterNames().empty()? "": Clst->getClusterNames().at(0);
    
    //programs starts
    setMouseCallback("image", onMouse, &clusParams);
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
    Mat convertedFrame;
    string* p_currentCluster = currentCluster;
    if(*playVideo)   
        *videoCap >> frame; // get a new frame from camera
    if(frame.empty() && p_image->empty()) 
        return 0; 
    else if(!frame.empty()){
        cout << "frame type: " << frame.type() << endl;
        cout << "image type: " <<p_image->type() << endl;
        frame.convertTo(convertedFrame, p_image->type());
        cout << "convertedFrame type: " << convertedFrame.type() << endl;
        cout << "video capture retrive type:" << videoCap->get(CV_CAP_PROP_FORMAT) << endl;
        if (convertedFrame.type() == p_image->type()){
            cout << "igual ";
        }
        convertedFrame.copyTo(*p_image);
    }

    // clust->clusterizarImagem(p_image, "jerseys");
    // clust->clusterizarImagem(p_image, "campo");
    clust->clusterizarImagem(p_image);
    imshow(window, *p_image);
    // if(waitKey(30) >= 0) break;
    char c = 0;
    c = (char)waitKey(10000);

    // char c = (char)waitKey(15);
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
            cv::imwrite("img.png", *p_image);
            cout << "saved" << endl;
            break;
        case 'n':
            cout << "printing clusters..." << endl;
            clust->printClusters();
            break;
        case 'u':
            cout << "undo command not implemented yet";
            // removeSetFromSet(st, undoSet);
            break;
        case '1' ... '9':
        { /* gambiarra para trocar de cluster de acordo com o numero (1 a 9) digitado; 
           * Note: the brackets are here to destroy the vector "aux" after this case ends;
           * https://stackoverflow.com/questions/61708267/jump-bypasses-variable-initialization-in-switch-statement
           */
            vector<string> aux = clust->getClusterNames(); 
            // cout << aux.size() << endl;
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
    cout << "Usage: ./this video.ext cluster1 cluster2 cluster3 ..." << endl <<
            "Exemple: ./this video.avi field sky yellows" << endl <<
            "" << endl << 
            "Click on image to floodfill it;" << endl << 
            "Keyboard Commands:" << endl <<
            "\t(u)ndo, (p)ause, (n) Print Clusters," << endl << 
            "\t(s)ave clusters to file" << endl <<
            "\t1 - 9: change to cluster x"<< endl << 
            "\t(q)uit" << endl;
}
