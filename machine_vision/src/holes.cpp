#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    string path = argc > 1 ? argv[1] : "images/holes.jpg";
    Mat src = imread(path, IMREAD_COLOR);
    if (src.empty()) { cerr << "cannot read " << path << endl; return -1; }

    Mat gray, blur, bin;
    cvtColor(src, gray, COLOR_BGR2GRAY);
    GaussianBlur(gray, blur, Size(7, 7), 0);
    // holes are darker than the metal surface -> inverse Otsu
    threshold(blur, bin, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);

    Mat k = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
    morphologyEx(bin, bin, MORPH_OPEN, k);
    morphologyEx(bin, bin, MORPH_CLOSE, k);

    vector<vector<Point>> contours;
    findContours(bin, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    Mat out = src.clone();
    int count = 0;
    for (auto& c : contours)
    {
        double area = contourArea(c);
        if (area < 800 || area > 20000) continue;           // size of the small holes
        Rect b = boundingRect(c);
        if (b.x <= 2 || b.y <= 2 || b.x + b.width >= src.cols - 2 ||
            b.y + b.height >= src.rows - 2) continue;       // incomplete (cut by border)
        double per = arcLength(c, true);
        double circ = 4 * CV_PI * area / (per * per);
        if (circ < 0.8) continue;                           // keep round shapes only

        count++;
        Point2f ctr; float rad;
        minEnclosingCircle(c, ctr, rad);
        circle(out, ctr, (int)rad + 4, Scalar(0, 0, 255), 3);
        putText(out, to_string(count), Point((int)ctr.x - 8, (int)ctr.y + 8),
                FONT_HERSHEY_SIMPLEX, 0.8, Scalar(0, 255, 0), 2);
        cout << count << ": center(" << (int)ctr.x << "," << (int)ctr.y
             << ") r=" << (int)rad << " circ=" << circ << endl;
    }
    putText(out, "Count: " + to_string(count), Point(20, 40),
            FONT_HERSHEY_SIMPLEX, 1.2, Scalar(255, 0, 0), 3);

    cout << "Hole count: " << count << endl;
    imwrite("images/holes_binary.png", bin);
    imwrite("images/holes_result.png", out);
    if (argc > 2) return 0; // no GUI when 2nd arg given
    imshow("result", out);
    waitKey(0);
    return 0;
}
