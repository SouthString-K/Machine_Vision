#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    string path = argc > 1 ? argv[1] : "images/coins.jpg";
    Mat src = imread(path, IMREAD_COLOR);
    if (src.empty()) { cerr << "cannot read " << path << endl; return -1; }

    Mat gray, blur, bin;
    cvtColor(src, gray, COLOR_BGR2GRAY);
    GaussianBlur(gray, blur, Size(5, 5), 0);
        // estimate uneven background by opening with a kernel larger than a coin,
    // then subtract so the dim coin and bright coins separate cleanly
    Mat bg, diff;
    morphologyEx(blur, bg, MORPH_OPEN, getStructuringElement(MORPH_ELLIPSE, Size(171, 171)));
    subtract(blur, bg, diff);
    threshold(diff, bin, 0, 255, THRESH_BINARY | THRESH_OTSU);

    Mat k = getStructuringElement(MORPH_ELLIPSE, Size(7, 7));
    morphologyEx(bin, bin, MORPH_CLOSE, k);
    morphologyEx(bin, bin, MORPH_OPEN, k);

    // external contours only -> fills coin interior holes
    vector<vector<Point>> contours;
    findContours(bin, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    Mat out = src.clone();
    int count = 0;
    const double minArea = 1500; // reject noise
    for (auto& c : contours)
    {
        if (contourArea(c) < minArea) continue;
        count++;
        Point2f ctr; float rad;
        minEnclosingCircle(c, ctr, rad);
        circle(out, ctr, (int)rad, Scalar(0, 0, 255), 2);
        putText(out, to_string(count), Point((int)ctr.x - 10, (int)ctr.y + 8),
                FONT_HERSHEY_SIMPLEX, 0.8, Scalar(0, 255, 0), 2);
    }
    putText(out, "Count: " + to_string(count), Point(10, 30),
            FONT_HERSHEY_SIMPLEX, 1.0, Scalar(255, 0, 0), 2);

    cout << "Coin count: " << count << endl;
    imwrite("images/coins_binary.png", bin);
    imwrite("images/coins_result.png", out);
    if (argc > 2) return 0; // no GUI when 2nd arg given
    imshow("result", out);
    waitKey(0);
    return 0;
}
