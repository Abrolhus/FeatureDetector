#include "opencv2/imgproc.hpp"
// #include "opencv2/videoio.hpp"
// #include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>
#include <set>
using namespace cv;
using namespace std;

stringstream dataSs;
string dataStr;

void printSet(set<int> set){
    for (auto it = set.begin(); it != set.end(); ++it)
    {
        cout << *it;
    }
    cout << endl;
}    
int c;

void loadFile(string file, set<int> set)
{
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

void addToSet(vector<int> vec, set<int> set){
    cout << "before adding to set: ";
    printSet(set);
    cout << "adding to set..." << endl;
    for (int i = 0; i <vec.size(); i++)
    {
        set.insert(vec[i]);
    }
    // clear(colorsTxt); Não precisa porque agora está sendo usado SETS ao inves de VECTORS,
	// que por natureza impedem a duplicidade de elementos (como um um conjunto na matemática)
    printSet(set);
    printSet(set);

}

void save(set<int> set, string file)
{
    cout << "saving" << endl;
    for (auto it =set.begin(); it !=set.end(); ++it)
    {
        dataSs << *it << endl;
    }
    dataStr = dataSs.str();

    ofstream outFile(file.c_str(), ios::out);
    if (outFile)
    {
        outFile << dataStr << endl;
        outFile.close();
    }
    else
        cout << "could not save file" << endl;
}


Mat image0, image;
int ffillMode = 1;
int loDiff = 40, upDiff = 40;
int connectivity = 4;
int isColor = true;
bool useMask = false;
int newMaskVal = 255;
int mode;
#define FIELD 0
#define OPPONENT 1

static void onMouse(int event, int x, int y, int d, void *st)
{
    set<int>* setPoints = (set<int>*) st;
    if (event != CV_EVENT_LBUTTONDOWN)
        return;
    Point seed = Point(x, y);
    int lo = ffillMode == 0 ? 0 : loDiff;
    int up = ffillMode == 0 ? 0 : upDiff;
    int flags = connectivity + (newMaskVal << 8) +
                (ffillMode == 1 ? FLOODFILL_FIXED_RANGE : 0);
    int b,g,r;

    b = 0;   //(unsigned)theRNG() & 255;
    g = 255; //(unsigned)theRNG() & 255;
    r = 0;   //(unsigned)theRNG() & 255;

    int area;
    Rect ccomp;
    Scalar newVal = Scalar(b, g, r);
    Mat clusterized = image;
    Mat frame;
    image.copyTo(frame);

    area = floodFill(clusterized, seed, newVal, &ccomp, Scalar(lo, lo, lo),
                     Scalar(up, up, up), flags);

    imshow("image", clusterized);

    int R, G, B;
    int color;
    vector<int> colours;
    for (int y = 0; y < clusterized.rows; y++)
    {
        for (int x = 0; x < clusterized.cols; x++)
        {

            Vec3b colour = clusterized.at<Vec3b>(y, x);
            if (colour.val[0] == 0 && colour.val[1] == 255 && colour.val[2] == 0)
            {
                Vec3b colour = frame.at<Vec3b>(y, x);
                B = (int)colour.val[0];
                G = (int)colour.val[1];
                R = (int)colour.val[2];
                //cout << "B: " << B << " G: " << G << " R: " << R << endl;
                color = (B << 16) | (G << 8) | R;

                //cout << color << endl;
                colours.push_back(color);

                // char r = (cor >> 16) & 0xFF;
                // char g = (cor >> 8) & 0xFF;
                // char b = cor & 0xFF;
                // cout << "b: " << (int)b << " g: " << (int)g << " r: " << (int)r << endl;
            }
        }
    }
    // clear(colours);
    cout << "colours0: " << colours[0] << endl;
    // loadFile("clustering.txt", colors);
    // save(colours, "clustering.txt");    
    addToSet(colours, *setPoints);    
    
}

int main(int argc, char **argv)
/*  fixed: a image estava sendo declarada tanto como variavel global quanto local; 
 *  Outro problema é que a leitura da imagem (por parametro estava sendo feita na "image", não na "image0");
 */
{
    if (argc != 2)
    {
        cout << " Usage: display_image ImageToLoadAndDisplay" << endl;
        return -1;
    }

    image0;
    image0 = imread(argv[1], CV_LOAD_IMAGE_COLOR); // Read the file

    if (!image0.data) // Check for invalid input
    {
        cout << "Could not open or find the image0" << std::endl;
        return -1;
    }

    set<int> colorsTxt;
    image0.copyTo(image);
    namedWindow("image", 0);
    //createTrackbar("lo_diff", "image", &loDiff, 255, 0);
    //createTrackbar("up_diff", "image", &upDiff, 255, 0);
    cout << "vai \n";
        setMouseCallback("image", onMouse, &colorsTxt);
    for (;;)
    {
        cout<< "foi1 ";
        imshow("image", image);
        cout << "foiOto"<< endl;
        char c = (char)waitKey(0);
        if (c == 27)
        {
            cout << "Exiting ...\n";
            break;
        }
        switch (c)
        {
        case 'r':
            cout << "Original image is restored\n";
            image0.copyTo(image);
            break;
        case 's':
            cout << "Simple floodfill mode is set\n";
            ffillMode = 0;
            break;
        case 'f':
            cout << "Fixed Range floodfill mode is set\n";
            ffillMode = 1;
            break;
        case 'g':
            cout << "Gradient (floating range) floodfill mode is set\n";
            ffillMode = 2;
            break;
        case '4':
            cout << "4-connectivity mode is set\n";
            connectivity = 4;
            break;
        case '8':
            cout << "8-connectivity mode is set\n";
            connectivity = 8;
            break;
        case 'w':
            cout << "saving to file..." << endl;
            save(colorsTxt, "clustering.txt");
            break;
        case 'p':
            cout << "printing set..." << endl;
            printSet(colorsTxt);
        }
        cout<< "foi o Ultimo" << endl;
    }
    return 0;
}
