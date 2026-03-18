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
 * @param img 输入图像矩阵的常量引用
 * @return 返回传入图像的灰度直方图矩阵
 */
cv::Mat calc_histo(const cv::Mat& img) {
  // Change BGR to GRAY
  cv::Mat _img;
  cv::cvtColor(img, _img, cv::COLOR_BGR2GRAY);

  cv::Mat hist;
  int hist_size = 256;
  float range[] = {0, 256};
  const float* hist_range = {range};

  cv::calcHist(&_img, 1, 0, cv::Mat(), hist, 1, &hist_size, &hist_range);

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
 * 对图像进行灰度线性变换。线性变换函数：y = alpha * x + beta
 *
 * @param img 输入图像矩阵的常量引用
 * @param alpha 对比度系数
 * @param beta  亮度增量
 * @return 变换后的图像
 */
cv::Mat gen_gray_lin_trans(const cv::Mat& img, double alpha, double beta) {
  cv::Mat result;

  img.convertTo(result, -1, alpha, beta);
  return result;
}

/**
 * 对图像进行灰度对数变换。线性变换函数：y = log(1+r)
 *
 * @param img 输入图像矩阵的常量引用
 * @return 变换后的图像
 */
cv::Mat gen_gray_log_trans(const cv::Mat& img) {
  cv::Mat img_f, result;
  img.convertTo(img_f, CV_32F);

  img_f += 1.0;
  cv::log(img_f, img_f);

  // 将图像归一化到 [0,255]
  img_f *= (255.0 / std::log(256.0));

  img_f.convertTo(result, CV_8U);
  return result;
}

/**
 * 对图像进行 Gamma 变换。变换函数： s = c*(r ^ gamma)
 *
 * @param img 输入图像矩阵的常量引用
 * @param gamma Gamma 值
 * @return 变换后的图像
 */
cv::Mat gen_gamma_trans(const cv::Mat& img, double gamma) {
  cv::Mat lut(1, 256, CV_8U);
  uchar* p = lut.ptr();

  for (int i = 0; i < 256; i++) {
    // s = (i/255.0)*gamma * 255.0
    double row = std::pow((double)i / 255.0, gamma) * 255.0;
    p[i] = cv::saturate_cast<uchar>(row);
  }

  cv::Mat result;
  cv::LUT(img, lut, result);

  return result;
}

/**
 * 对图像进行灰度阈值变换（二值化）
 *
 * @param img  输入图像矩阵的常量引用
 * @param thresh 阈值 (0-255)
 * @param type   阈值类型：
 *  0: cv::THRESH_BINARY (黑白二值)
 *  1: cv::THRESH_BINARY_INV (反色二值)
 *  2: cv::THRESH_OTSU (自动计算阈值)
 * @return 变换后的二值图像
 */
cv::Mat gen_threshold(const cv::Mat& img, double thresh,
                      int type = cv::THRESH_BINARY) {
  cv::Mat result, _img;

  // Change to gray
  cv::cvtColor(img, _img, cv::COLOR_BGR2GRAY);

  if (type == 2)
    cv::threshold(_img, result, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
  else
    cv::threshold(_img, result, thresh, 255, type);

  return result;
}

/**
 *对图像进行分段线性变换

 * @param img  输入图像矩阵的常量引用
 * @param r1, s1 第一个控制点
 * @param r2, s2 第二个控制点
 * @return 变换后的图像
 * 逻辑：
 * 0 <= x < r1:   y = (s1/r1) * x
 * r1 <= x < r2:  y = ((s2-s1)/(r2-r1)) * (x-r1) + s1
 * r2 <= x <= 255:y = ((255-s2)/(255-r2)) * (x-r2) + s2
 */
cv::Mat gen_piecewise_lin(const cv::Mat& img, int r1, int s1, int r2, int s2) {
  cv::Mat lut(1, 256, CV_8U);
  uchar* p = lut.ptr();

  for (int i = 0; i < 256; i++) {
    if (i < r1) {
      p[i] = cv::saturate_cast<uchar>((double)s1 / r1 * i);
    } else if (i < r2) {
      p[i] = cv::saturate_cast<uchar>((double)(s2 - s1) / (r2 - r1) * (i - r1) +
                                      s1);
    } else {
      p[i] = cv::saturate_cast<uchar>(
          (double)(255 - s2) / (255 - r2) * (i - r2) + s2);
    }
  }

  cv::Mat result;
  cv::LUT(img, lut, result);
  return result;
}

/**
 * 对图像进行直方图均衡化
 * @param img 输入图像矩阵的常量引用
 * @return 均衡化后的图像
 */
cv::Mat gen_equalize_hist(const cv::Mat& img) {
  cv::Mat _img;
  cv::cvtColor(img, _img, cv::COLOR_BGR2GRAY);

  cv::Mat result;
  cv::equalizeHist(_img, result);

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

  auto log_trans_img = gen_gray_log_trans(img);
  // cv::imshow("Log Transformed Image", log_trans_img);

  auto gamma_trans_img = gen_gamma_trans(img, 2.5);
  // cv::imshow("Gamma Transformed Image", gamma_trans_img);

  auto threshold_img = gen_threshold(img, 1, 2);
  // cv::imshow("Threshold Image", threshold_img);

  auto streched_img = gen_piecewise_lin(img, 50, 20, 200, 240);
  // cv::imshow("Streched Image", streched_img);

  auto equalized_img = gen_equalize_hist(img);
  cv::imshow("Equalized Image", equalized_img);

  cv::waitKey(0);
  return 0;
}