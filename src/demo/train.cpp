/*
++++++++++++++++++++++++++++++++++++++++++++++++++
author: kevinhikali
email:  kevinhikali@hotmail.com
++++++++++++++++++++++++++++++++++++++++++++++++++
*/
#include <iostream>
#include <ostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>

#include "DBoW3.h"
#include "res_path.h"
#include "cpp_lib_read_filename.hpp"

using namespace std;
using namespace cv;

template <class Type>
bool writeSimilarityMatrix(const vector<vector<Type> > similarity_matrix, string filename)
{
    ofstream on;
    if (filename.empty()) {
        cerr << __FUNCTION__ << " - Error opening " << filename <<endl;
        return false;
    }

    on.open(filename, ios::out);

    if (similarity_matrix.empty() || similarity_matrix[0].empty()) {
        cerr << __FUNCTION__ << " - Error similarity matrix" <<endl;
        return false;
    }

    int nRow = similarity_matrix.size();
    int nCol = similarity_matrix[0].size();

    for (int i=0; i<nRow; i++) {
        for (int j=0; j<nCol; j++) {
            on << similarity_matrix[i][j] << " ";
        }
        on << endl;
    }

    return true;
}

template <class Type>
string num2string(Type number)
{
    stringstream ss;
    ss << number;
    string str = ss.str();
    return str;
}

int main(int argc, char** argv)
{
    string input_folder_path = (string)CONFIG_PATH + "../images/";
    string output_folder_path = (string)OUTPUT_PATH + "vocabulary_output/";

    vector<string> img_filenames = KEVINHIKALI::readFileList(input_folder_path);
    KEVINHIKALI::sortFilenames(img_filenames);
    int nImages = img_filenames.size();

    vector<Mat> images;
    for (int i=0; i<nImages; i++) {
        string filename = input_folder_path + img_filenames[i];
        Mat image = imread(filename);
        if (image.empty()) {
            cerr<< __FUNCTION__ << " - Error reading filename " << filename <<endl;
            return -1;
        }
        images.push_back(image);
    }

    Ptr<Feature2D> detector = ORB::create();
    vector<Mat> descriptors;

    cout << "Extracting features..." << endl;
    for (int i=0; i<nImages; i++) {
        vector<cv::KeyPoint> keypoints;
        cv::Mat descriptor;
        detector->detectAndCompute(images[i], cv::Mat(), keypoints, descriptor);
        descriptors.push_back(descriptor);
    }

    const int K = 8;
    const int L = 4;
    DBoW3::WeightingType weight = DBoW3::TF_IDF;
    DBoW3::ScoringType score = DBoW3::L1_NORM;

    DBoW3::Vocabulary voc(K, L, weight, score);

    cout << "Creating " << K << "^" << L << " vocabulary..." << endl;
    voc.create(descriptors);
    cout << "done!" << endl;

    cout << "Vocabulary information: " << endl << voc << endl << endl;

    vector<vector<double> > similarity_matrix;

    for (int i=0; i<nImages; i++) {
        vector<double> line;
        for (int j=0; j<nImages; j++) {
            line.push_back(0);
        }
        similarity_matrix.push_back(line);
    }

    DBoW3::BowVector v1, v2;
    for (int i=0; i<nImages; i++) {
        voc.transform(descriptors[i], v1);
        for (int j=i+1; j<nImages; j++) {
            voc.transform(descriptors[j], v2);
            double score = voc.score(v1, v2);

            similarity_matrix[i][j] = score;

            cout << "Image " << i << " vs Image " << j << "= " << score << endl;
        }
    }

    // save the vocabulary to disk
    cout << endl << "Saving vocabulary..." << endl;
    voc.save("VOC_K_" + num2string(K) + "_L_" + num2string(L) + ".yml.gz");
    cout << "Done" << endl;

    // save similarity matrix

    writeSimilarityMatrix(similarity_matrix,
                          "VOC_K_" + num2string(K) + "_L_" + num2string(L) + ".txt");

    return 0;
}
/*
++++++++++++++++++++++++++++++++++++++++++++++++++
author: kevinhikali
email:  kevinhikali@hotmail.com
++++++++++++++++++++++++++++++++++++++++++++++++++
*/
