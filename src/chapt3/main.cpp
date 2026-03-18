/**
 * @date 2026-3-18
 * @author Heptari
 * @brief 这是《数字图象处理与机器视觉》课程实验 Chapter 3 的课程代码
 */

#include <iostream>
#include <opencv4/opencv2/opencv.hpp>

/**
 * 利用 cv::calcHist 函数进行灰度直方图的计算
 *
 * @param _img 输入图像矩阵的常量引用
 * @return 返回传入图像的灰度直方图矩阵
 */
cv::Mat calc_histo(const cv::Mat& _img) {
  // Change BGR to GRAY
  cv::Mat img;
  cv::cvtColor(_img, img, cv::COLOR_BGR2GRAY);

  cv::Mat hist;
  int hist_size = 256;
  float range[] = {0, 256};
  const float* hist_range = {range};

  cv::calcHist(&img, 1, 0, cv::Mat(), hist, 1, &hist_size, &hist_range);

  // Draw
  int hist_w = 512, hist_h = 400;
  int bin_w = cvRound((double)hist_w / hist_size);
  cv::Mat hist_img(hist_h, hist_w, CV_8UC3, cv::Scalar(0, 0, 0));
  cv::normalize(hist, hist, 0, hist_img.rows, cv::NORM_MINMAX, -1, cv::Mat());

  for (int i = 1; i < hist_size; i++) {
    cv::line(
        hist_img,
        cv::Point(bin_w * (i - 1), hist_h - cvRound(hist.at<float>(i - 1))),
        cv::Point2d(bin_w * (i), hist_h - cvRound(hist.at<float>(i))),
        cv::Scalar(255, 255, 255), 2, 8, 0);
  }

  return hist_img;
}

/**
 * 对图像的灰度进行线性变换。线性变换函数：y = alpha * x + beta
 * @param input 输入灰度图
 * @param alpha 对比度系数
 * @param beta  亮度增量
 * @return 变换后的图像
 */
cv::Mat gen_gray_lin_trans(const cv::Mat& img, double alpha, double beta) {
  cv::Mat result;

  img.convertTo(result, -1, alpha, beta);
  return result;
}

int main() {
  cv::Mat img = cv::imread("static/image.png");
  if (img.empty()) {
    std::cout << "[Error] Cannot read the image. Check the path first."
              << std::endl;
    return -1;
  }

  // Show the gray histogram
  auto hist_img = calc_histo(img);
  // cv::imshow("Gray Histogram", hist_img);

  // show the linear transformed image
  auto linear_trans_img = gen_gray_lin_trans(img, 1.2, 20);
  // cv::imshow("Linear Transformed Image", linear_trans_img);

  cv::waitKey(0);
  return 0;
}