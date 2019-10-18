#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>

using namespace cv;

using namespace std;

#define VERDE ((0 << 16) | (255 << 8) | 0)

int c;

int *colorsTxt = new int[256 * 256 * 256];

void loadFile(string file)
{
    for (int i = 0; i < 256 * 256 * 256; i++)
    {
        colorsTxt[i] = -1;
    }
    string line;
    ifstream inFile(file.c_str(), ios::in);
    if (inFile)
    {
        while (getline(inFile, line))
        {
            //cout << line << '\n';
            istringstream actualColor(line);
            actualColor >> c;
            //cout << colorsTxt[c] << endl;
            colorsTxt[c] = VERDE;
            //cout << colorsTxt[c] << endl;

            actualColor.str("");
        }
        inFile.close();
    }
    else
        cout << "could not open file" << endl;
}

void clustering(Mat img)
{
    int R, G, B;
    int col;
    for (int y = 0; y < img.rows; y++)
    {
        for (int x = 0; x < img.cols; x++)
        {
            Vec3b color = img.at<Vec3b>(y, x);
            B = (int)color.val[0];
            G = (int)color.val[1];
            R = (int)color.val[2];
            col = (B << 16) | (G << 8) | R;

            if (colorsTxt[col] != -1)
            {
                color.val[0] = 0;
                color.val[1] = 255;
                color.val[2] = 0;
            }
            img.at<Vec3b>(y, x) = color;
        }
    }
    imshow("Clusterized", img);
}

int main(int argc, char **argv)
{
    Mat image0;

    if (argc != 2)
    {
        cout << " Usage: display_image ImageToLoadAndDisplay" << endl;
        return -1;
    }

    Mat image;
    image = imread(argv[1], CV_LOAD_IMAGE_COLOR); // Read the file

    if (!image.data) // Check for invalid input
    {
        cout << "Could not open or find the image" << std::endl;
        return -1;
    }
    imshow("image", image0);

    //namedWindow("Display window", WINDOW_AUTOSIZE); // Create a window for display.
    //imshow("Display window", image);                // Show our image inside it.

    loadFile("clustering.txt");
    clustering(image0);

    waitKey(0); // Wait for a keystroke in the window
    return 0;
}