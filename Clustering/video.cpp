#include "opencv2/imgproc.hpp" // #include "opencv2/videoio.hpp"
// #include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>
#include <set>

#define LODIFF 40
#define UPDIFF 40
#define CONNECTIVITY 4 
#define ISCOLOR true
#define USEMASK false
#define NEWMASKVAL 255
#define FFILLMODE 1
#define MASK_COLOR 255, 0, 0

#define FIELD 0
#define OPPONENT 1

using namespace cv;
using namespace std;

Mat image;
// set<int> colorsTxt;

int main(int argc, char **argv)
{
    void onMouse(int event, int x, int y, int d, void*);
    void clustering(Mat* img, set<int> set);
    void printSet(set<int> set);
    void loadFile(string file, set<int> set);
    void addToSet(vector<int> vec, set<int>* set);
    void save(set<int> set, string file);
    int mainLoop (string window,  set<int> *set, VideoCapture* cap, bool* playVideo);
    // static void onMouse(int event, int x, int y, int d, void *st);

    bool playVideo = true;
    set<int> colorsTxt;
    int flag;
    // Mat image;
    // VideoCapture* cap = new VideoCapture("daviSabbagRitual.webm");
    VideoCapture* cap = new VideoCapture("/home/abrolhus/Rinobot/data/jerseys/rinobot/lac2019/video.avi");
    // cap->open("daviSabbagRitual.webm"); // open video
    if(!cap->isOpened())  // check if we succeeded
        return -1;
    // Mat edges;
    namedWindow("frame",0);
    setMouseCallback("frame", onMouse, &colorsTxt);
        
    for (;;){
        flag = mainLoop("frame", &colorsTxt, cap, &playVideo);
        if(flag == -1) return -1;
    }
    return 0;
}



void printSet(set<int> set){ //prints set
    cout << "P ";
    for (auto it = set.begin(); it != set.end(); ++it)
    {
        cout << *it << " ";
    }
    cout << endl;
}    

void loadFile(string file, set<int> set)
{
    int c;
    cout<< "Loading file..." << endl;
    string line;
    ifstream inFile(file.c_str(), ios::in);
    if (inFile)
    {
        while (getline(inFile, line))
        {
            //cout << line << '\n';
            istringstream actualColor(line);
            actualColor >> c;
            //cout << c << endl;
            set.insert(c);
            actualColor.str("");
        }
        inFile.close();
    }
    else
        cout << "could not open file, creating a new one..." << endl;
}

void addToSet(vector<int> vec, set<int>* set){ // Adds vector elements to set;
    cout << "before adding to set: ";
    // printSet(*set);
    cout << "adding to set..." << endl;
    for (int i = 0; i <vec.size(); i++)
    {
        set->insert(vec[i]);
    }
    // clear(p_colorsTxt); Não precisa porque agora está sendo usado SETS ao inves de VECTORS,
	// que por natureza impedem a duplicidade de elementos (como um um conjunto na matemática)
    printSet(*set);
    cout << "done addin";
}

void save(set<int> set, string file)
{
    printSet(set);
    stringstream dataSs;
    string dataStr;
    cout << "saving" << endl;
    for (auto it =set.begin(); it !=set.end(); ++it)
    {
        dataSs << *it << endl;
    }
    dataStr = dataSs.str();
    cout << "data___________: " << dataStr << endl;

    ofstream outFile(file.c_str(), ios::out);
    if (outFile)
    {
        outFile << dataStr << endl;
        outFile.close();
    }
    else
        cout << "could not save file" << endl;
}
void clustering(Mat* img, set<int> set)
{
    int R, G, B;
    int col;
    int step = 1; // increment value for x and y. Higher the numeber faster the code.
    for (int y = 0; y < img->rows; y+= step)
    {
        for (int x = 0; x < img->cols; x+= step)
        {
            Vec3b color = img->at<Vec3b>(y, x);
            B = (int)color.val[0];
            G = (int)color.val[1];
            R = (int)color.val[2];
            col = (B << 16) | (G << 8) | R;

            if (set.find(col) != set.end())
            {
                // color.val[0] = 0;
                // color.val[1] = 255;
                // color.val[2] = 0;
                color = Vec3b(MASK_COLOR);
                img->at<Vec3b>(y, x) = color;
            }
        }
    }

}
int mainLoop (string window,  set<int> *set, VideoCapture* cap, bool* playVideo){

        Mat frame;
        if(*playVideo)   
            *cap >> frame; // get a new frame from camera
        if(frame.empty() && image.empty()) return 0; 
        else if(!frame.empty())
            frame.copyTo(image);
        // cvtColor(frame, image, CV_BGR2GRAY);
        // GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
        // Canny(edges, edges, 0, 30, 3);
        clustering(&image, *set);
        imshow("frame",image);
        // if(waitKey(30) >= 0) break;
        char c = (char)waitKey(30);
        if (c == 27)
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
        // case 's':
            // cout << "Simple floodfill mode is set\n";
            // ffillMode = 0;
            // break;
        // case 'f':
            // cout << "Fixed Range floodfill mode is set\n";
            // ffillMode = 1;
            // break;
        // case 'g':
            // cout << "Gradient (floating range) floodfill mode is set\n";
            // ffillMode = 2;
            // break;
        // case '4':
            // cout << "4-connectivity mode is set\n";
            // connectivity = 4;
            // break;
        // case '8':
            // cout << "8-connectivity mode is set\n";
            // connectivity = 8;
            // break;
        case 'w':
        case 's':
            cout << "saving to file..." << endl;
            save(*set, "clustering.txt");
            break;
        case 'n':
            cout << "printing set..." << endl;
            printSet(*set);
            break;
        case 'p':
        case ' ':
            *playVideo = !(*playVideo); 
        }
        cout<< ". ";
        return 0;
}
void onMouse(int event, int x, int y, int , void* st)
{
    // Mat imageCopy; image.copyTo
    cout << "foii ";
    set<int>* setPoints = (set<int>*) st;
    if (event != CV_EVENT_LBUTTONDOWN)
        return;
    cout << "rrr";
    Point seed = Point(x, y);
    int lo = FFILLMODE == 0 ? 0 : LODIFF;
    int up = FFILLMODE == 0 ? 0 : UPDIFF;
    int flags = CONNECTIVITY + (NEWMASKVAL << 8) +
                (FFILLMODE == 1 ? FLOODFILL_FIXED_RANGE : 0);
    int b,g,r;

    b = 0;   //(unsigned)theRNG() & 255;
    g = 255; //(unsigned)theRNG() & 255;
    r = 0;   //(unsigned)theRNG() & 255;

    int area;
    Rect ccomp;
    Scalar newVal = Scalar(MASK_COLOR);
    Mat clusterized;
    Mat quadro;
    image.copyTo(quadro);
    image.copyTo(clusterized);

    area = floodFill(clusterized, seed, newVal, &ccomp, Scalar(lo, lo, lo),
                     Scalar(up, up, up), flags);


    int R, G, B;
    int color;
    vector<int> colours;
    unsigned char aux[] = {MASK_COLOR};
    // cout << "aaaaaaaaaaaaaaaa" << aux[0];
    for (int y = 0; y < clusterized.rows; y++)
    {
        for (int x = 0; x < clusterized.cols; x++)
        {

            Vec3b colour = clusterized.at<Vec3b>(y, x);
            // if (colour.val[0] == 255 && colour.val[1] == 0 && colour.val[2] == 0)
            if (colour.val[0] == aux[0] && colour.val[1] ==  aux[1] && colour.val[2] == aux[2] )
            // if (colour == aux);
            {
                Vec3b colour = quadro.at<Vec3b>(y, x);
                B = (int)colour.val[0];
                G = (int)colour.val[1];
                R = (int)colour.val[2];
                //cout << "B: " << B << " G: " << G << " R: " << R << endl;
                color = (B << 16) | (G << 8) | R;
                cout << color << " ";
                //cout << color << endl;
                colours.push_back(color);

                // char r = (cor >> 16) & 0xFF;
                // char g = (cor >> 8) & 0xFF;
                // char b = cor & 0xFF;
                // cout << "b: " << (int)b << " g: " << (int)g << " r: " << (int)r << endl;
            }
        }
    }
    addToSet(colours, setPoints);    
}

