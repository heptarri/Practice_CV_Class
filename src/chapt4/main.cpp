/**
 * @date 2026-3-19
 * @author Heptari
 * @brief 这是《数字图象处理与机器视觉》课程实验 Chapter 4 的课程代码
 */

#include <iostream>
#include <opencv4/opencv2/opencv.hpp>

cv::Mat pos_trans(const cv::Mat& img, int x, int y) {
  // Create transform matrix:
  // [1 0 x]
  // [0 1 y]
  cv::Mat m = (cv::Mat_<float>(2, 3) << 1, 0, x, 0, 1, y);
  cv::Mat result;
  cv::warpAffine(img, result, m, img.size());

  return result;
}

/**
 * @param mode 镜像模式。1：水平；2：垂直；-1：水平+垂直
 */
cv::Mat img_flip(const cv::Mat& img, int mode = 1) {
  cv::Mat result;
  cv::flip(img, result, mode);

  /*
    或者也可以使用矩阵变换实现,如利用 cv::warpAffine() 函数调用变换矩阵:

    [-1 0 w]
    [ 0 1 0]
  */
  return result;
}

cv::Mat img_transpose(const cv::Mat& img) {
  cv::Mat result;
  cv::transpose(img, result);

  return result;
}

cv::Mat img_resize(const cv::Mat& img, int w, int h,
                   int interpolation = cv::INTER_LINEAR) {
  cv::Mat result;
  cv::Size target_size(w, h);

  if (target_size == img.size()) return img.clone();
  cv::resize(img, result, target_size, 0, 0, interpolation);

  return result;
}

cv::Mat img_rotate(const cv::Mat& img, int angle) {
  cv::Mat result;
  cv::Point2f center(img.cols / 2.0f, img.rows / 2.0f);

  cv::Mat M = cv::getRotationMatrix2D(center, angle, 1.0);
  cv::warpAffine(img, result, M, img.size(), cv::INTER_LINEAR,
                 cv::BORDER_CONSTANT, cv::Scalar(0, 0, 0));

  return result;
}

int main() {
  cv::Mat img = cv::imread("static/image.png");
  if (img.empty()) {
    std::cerr << "[Error] Cannot open the file. Check the path first."
              << std::endl;
    return -1;
  }

  auto pos_trans_img = pos_trans(img, 100, 100);
  // cv::imshow("Trans Position Image", pos_trans_img);

  auto flipped_img = img_flip(img);
  // cv::imshow("Flipped Image", flipped_img);

  auto resized_img = img_resize(img, 100, 100);
  // cv::imshow("Resized Image", resized_img);

  auto rotated_img = img_rotate(img, 30);
  cv::imshow("Rotated Image", rotated_img);

  cv::waitKey(0);

  return 0;
}