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

    for (int y = 0; y < 240; y++)
    {
        for (int x = 0; x < 320; x++)
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
