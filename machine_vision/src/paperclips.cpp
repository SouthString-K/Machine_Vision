#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    string path = argc > 1 ? argv[1] : "images/paperclips.jpg";
    Mat src = imread(path, IMREAD_COLOR);
    if (src.empty()) { cerr << "cannot read " << path << endl; return -1; }

    Mat gray, blur, bin;
    cvtColor(src, gray, COLOR_BGR2GRAY);
    GaussianBlur(gray, blur, Size(5, 5), 0);
    // dark clips on bright background -> inverse Otsu
    threshold(blur, bin, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);

    // close small gaps, remove specks
    Mat k = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
    morphologyEx(bin, bin, MORPH_CLOSE, k);
    morphologyEx(bin, bin, MORPH_OPEN, k);

    vector<vector<Point>> contours;
    findContours(bin, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    Mat out = src.clone();
    int count = 0;
    const double minArea = 2000; // reject noise
    for (auto& c : contours)
    {
        double a = contourArea(c);
        if (a < minArea) continue;
        Rect b = boundingRect(c);
        // skip scan-border artifact touching image edge
        if (b.x <= 2 || b.y <= 2 || b.x + b.width >= src.cols - 2 || b.y + b.height >= src.rows - 2) continue;
        count++;
        Rect r = boundingRect(c);
        rectangle(out, r, Scalar(0, 0, 255), 2);
        putText(out, to_string(count), Point(r.x, r.y - 5),
                FONT_HERSHEY_SIMPLEX, 0.8, Scalar(0, 200, 0), 2);
    }
    putText(out, "Count: " + to_string(count), Point(20, 40),
            FONT_HERSHEY_SIMPLEX, 1.2, Scalar(255, 0, 0), 3);

    cout << "Paper clip count: " << count << endl;
    imwrite("images/paperclips_binary.png", bin);
    imwrite("images/paperclips_result.png", out);
    if (argc > 2) return 0; // no GUI when 2nd arg given
    imshow("result", out);
    waitKey(0);
    return 0;
}
