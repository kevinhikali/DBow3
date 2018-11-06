#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{

string input_folder_path = (string)REPO_PATH + "train_images/";
string output_folder_path = (string)REPO_PATH + "vocabulary_output/";

vector<string> img_filenames = readFileList(input_folder_path);
int nImages = img_filenames.size();

vector<Mat> images;

for (int i=0; i<nImages; i++)
{
	images.push_back(imread(img_filenames[i]));
}

Ptr<Feature2D> detector = ORB::create();

vector<Mat> descriptors;

for (int i=0; i<nImages; i++)
{

}

const int K = 8;
const int L = 4;

DBoW3::WeightingType weight = DBoW3::TF_IDF;
DBoW3::ScoringType score = DBoW3::L1_NORM;

DBoW3::Vocabulary voc(K, L, weight, score);

voc.create(descriptors);

return 0;
}
