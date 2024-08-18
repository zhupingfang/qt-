//
// Created by zhupi on 2024/8/15.

#include "main_window.h"
#include <QApplication>
#include <QDebug>
#include <QElapsedTimer>
#include "SARibbonBar.h"
#include <opencv2/opencv.hpp>

// 重定向qdebug的打印

//using namespace boost::asio;
//using namespace boost::filesystem;
//namespace po = boost::program_options;

#include "test.h"

int main(int argc, char* argv[])
{
	QMessageLogContext text;
	QApplication a(argc, argv);
	MainWindow w;
	w.show();

	MyPythonTest test;
	test.TestPython();

	return (a.exec());


//		// 创建一个空的黑色图像
//		cv::Mat image = cv::Mat::zeros(400, 400, CV_8UC3);
//
//		// 在图像上绘制一个白色的圆
//		cv::circle(image, cv::Point(200, 200), 100, cv::Scalar(255, 255, 255), -1);
//
//		// 显示图像
//		cv::imshow("Test Image", image);
//		cv::waitKey(0);
//
//		std::cout << "OpenCV is working correctly!" << std::endl;
}
