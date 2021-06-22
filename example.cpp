#include <iostream>
#include <opencv2/opencv.hpp>

#include <deepmatching.hpp>


int main(int argc, char** argv)
{
	DeepMatching dm;
	std::vector<DeepMatching::Pair> pts;
	std::vector<float> scores;
	cv::Mat src1, src2, dst;

	src1 = cv::imread("climb1.png", cv::IMREAD_COLOR);
	src2 = cv::imread("climb2.png", cv::IMREAD_COLOR);
	dm.compute(src1, src2, pts, scores);

	DeepMatching::draw_matches(src1, src2, pts, scores, dst, true, 1);
	cv::namedWindow("Matches", cv::WINDOW_KEEPRATIO);
	cv::imshow("Matches", dst);
	cv::waitKey();

	return 0;
}

