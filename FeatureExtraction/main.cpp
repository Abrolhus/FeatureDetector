#include <iostream>
#include <fstream>
#include <math.h>
#include <time.h>
//#include <cv.h>
//#include <highgui.h>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "img_processing.h"
#include "line_detection.h"
#include "line_feature_detection.h"
#include "feature_extraction.h"
#include "dis_ang_translation.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "clustering.h"
#include "../Clustering/src/Clusterizador.hpp"

#define BLUE 0xff0000
#define GREEN 0x00ff00

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
	Mat img_rgb;
	loadFile("clustering.txt");
	img_rgb = imread(argv[1], CV_LOAD_IMAGE_COLOR);
	vector<field_point> result_intersections;
	vector<goalposts> goalPosts;

    Clusterizador* clus = new Clusterizador();
    clus->createNewCluster("campo", GREEN);
    clus->addToClusterViaFile("../clustering.txt", "campo");
    clus->clusterizarImagem(&img_rgb, "campo");
	// clustering(img_rgb);
    imshow("Clusterized", img_rgb);
    
	extract_features(img_rgb, result_intersections, goalPosts);
	for (int i = 0; i < goalPosts.size(); i++)
	{
		//cout << "Feature " << i << endl;
		//dis_bear test = pixel2dis_bear(normalizePixelPosition(img_rgb, goalPosts[i].root_position));
		//cout << "distance " << test.distance << endl;
		//cout << "angle " << test.bearing << endl;
		//cout << "--------------" << endl;
	}

	for (int i = 0; i < result_intersections.size(); i++)
	{
		//cout << "Intersection " << i << endl;
		//cout << result_intersections[i].position.y << endl;
		//cout << result_intersections[i].type << endl;
	}

	//imshow("va1", img_rgb);
	waitKey(0);

	return 0;
}
