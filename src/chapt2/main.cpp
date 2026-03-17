/**
 * @date 2026-3-17
 * @author Heptari
 * @brief 这是《数字图象处理与机器视觉》课程实验 Chapter 2 的课程代码
 */

#include <iostream>
#include <opencv4/opencv2/opencv.hpp>

/**
 * 通过命令行输出该图片的具体信息，如宽度、高度、通道数等
 *
 * @param img 输入图像矩阵的常量引用
 */
void get_info(const cv::Mat& img) {
  std::cout << "Basic information:" << std::endl;
  std::cout << "Width: " << img.cols << " px" << std::endl;
  std::cout << "Height:" << img.rows << " px" << std::endl;
  std::cout << "Chan Num: " << img.channels() << std::endl;
  std::cout << "Type:" << img.type() << std::endl;
}

/**
 * 检查该图片是否有效，从内容是否为空、通道数、长宽值等方面考虑
 *
 * @param img 输入图像矩阵的常量引用
 * @return 该对象是否有效
 */
bool check_valid(const cv::Mat& img) {
  if (img.empty() || img.data == nullptr || (img.cols <= 0 || img.rows <= 0))
    return false;

  int c = img.channels();
  if (c != 1 && c != 2 && c != 3 && c != 4) return false;

  return true;
}

/**
 * 获取图像中指定位置的像素值。
 *
 * 支持单通道（灰度）和多通道（彩色）图像。对于多通道图像，返回 BGR
 * 格式的像素值。 如果图像无效或位置越界，将输出错误信息并返回默认的
 * cv::Scalar()。
 *
 * @param img 输入图像矩阵的常量引用
 * @param row 像素的行索引
 * @param col 像素的列索引
 * @return cv::Scalar 像素值。对于单通道图像，返回灰度值；对于多通道图像，返回
 * BGR 向量。 如果出错，返回 cv::Scalar()（全零值）。
 */
cv::Scalar get_pixel_value(const cv::Mat& img, int row, int col) {
  if (!check_valid(img)) {
    std::cerr << "[Error] The image is invalid." << std::endl;
    return cv::Scalar();
  }

  if (row < 0 || row >= img.rows || col < 0 || col >= img.cols) {
    std::cerr << "[Error] Pixel position out of bounds." << std::endl;
    return cv::Scalar();
  }

  if (img.channels() > 1)
    return img.at<cv::Vec3b>(row, col);
  else
    return cv::Scalar(img.at<uchar>(row, col));
}

bool set_pixel_value(cv::Mat& img, int row, int col, const cv::Scalar& value) {
  if (!check_valid(img)) {
    std::cerr << "[Error] The image is invalid." << std::endl;
    return false;
  }

  if (row < 0 || row >= img.rows || col < 0 || col >= img.cols) {
    std::cerr << "[Error] Pixel position out of bounds." << std::endl;
    return false;
  }

  if (img.channels() == 1) {
    img.at<uchar>(row, col) = static_cast<uchar>(value[0]);
  } else if (img.channels() == 3) {
    img.at<cv::Vec3b>(row, col) =
        cv::Vec3b(static_cast<uchar>(value[0]), static_cast<uchar>(value[1]),
                  static_cast<uchar>(value[2]));
  }

  return true;
}

bool resize_image(const cv::Mat& src, cv::Mat& dst, int width, int height,
                  int interpolation = cv::INTER_LINEAR) {
  if (!check_valid(src)) {
    std::cerr << "[Error] The source image is invalid." << std::endl;
    return false;
  }

  if (width <= 0 || height <= 0) {
    std::cerr << "[Error] Invalid target size." << std::endl;
    return false;
  }

  cv::resize(src, dst, cv::Size(width, height), 0, 0, interpolation);
  return true;
}

cv::Mat create_blank_image(int width, int height, const cv::Scalar& color) {
  cv::Mat img(height, width, CV_8UC3, color);
  return img;
}

bool is_grayscale(const cv::Mat& img) {
  if (img.channels() != 1) return false;
  return true;
}

bool is_binary(const cv::Mat& img) {
  if (!is_grayscale(img)) return false;
  if (img.type() != CV_8UC1) return false;

  cv::Mat mask = (img != 0) & (img != 255);
  return cv::countNonZero(mask) == 0;
}

int main(int argc, char** argv) {
  // Open a image as a destination
  cv::Mat img = cv::imread("static/image.png");
  if (!check_valid(img)) {
    std::cerr << "[Error] Invalid image." << std::endl;
    return -1;
  }

  get_info(img);

  auto pixel = get_pixel_value(img, 297, 260);
  std::cout << "Pixel value: " << pixel << std::endl;

  cv::Scalar new_value(0, 255, 255);
  for (int i = 0; i < 100; i++)
    for (int j = 0; j < 100; j++) {
      if (set_pixel_value(img, i, j, new_value)) {
        // std::cout << "Pixel set successfully." << std::endl;
      }
    }

  cv::Mat resized_image;
  if (resize_image(img, resized_image, 320, 240))
    cv::imshow("Resized Image", resized_image);

  // Show the processed image
  // cv::namedWindow("Processed Image", cv::WINDOW_AUTOSIZE);
  // cv::imshow("Processed Image", img);
  cv::waitKey(0);

  return 0;
}