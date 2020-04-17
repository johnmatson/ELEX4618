#include "stdafx.h"

#include <string>
#include <iostream>
#include <thread>

#include "CControl.h"
#include "Serial.h"

// OpenCV Include
#include "opencv.hpp"

// OpenCV Library
#pragma comment(lib,".\\opencv\\lib\\opencv_world310d.lib")

// for deleting part of string: https://en.cppreference.com/w/cpp/string/basic_string/find_last_of

CControl::CControl() {}
CControl::~CControl() {}

void CControl::init_com(int comport) {

	std::string word = "COM";
	std::string comName;
	comName = word + std::to_string(comport); //attach desired number to COM

	_com.open(comName);
	_com.flush(); //add check : if (_com.isOpened() == true)
}

bool CControl::get_data(int type, int channel, int &result) {

	// TX and RX strings
	std::string tx_str = "G " + std::to_string(type) + " " + std::to_string(channel) + "\n"; //load the string with the user choice input
	std::string rx_str;

	// Send TX string
	_com.write(tx_str.c_str(), tx_str.length());
	//Sleep(10); // wait for ADC conversion, etc. May not be needed? 

	// temporary storage
	char buff[2];

	// start timeout count
	double start_time = cv::getTickCount();

	buff[0] = 0;
	// Read 1 byte and if an End Of Line then exit loop
	// Timeout after 1 second, if debugging step by step this will cause you to exit the loop
	while (buff[0] != '\n' && (cv::getTickCount() - start_time) / cv::getTickFrequency() < 1.0) //testing for a second
	{
		if (_com.read(buff, 1) > 0)
		{
			rx_str = rx_str + buff[0]; //writes to string as it comes in from com (?)			}
		}//endif
	}//endwhile

		// 3 spaces at end of string - find and delete past the space; all that will be left is value at end
		// ie what comes in is "A 1 15 482\n", want to delete everthing but value 482
		//const std::string path = "/root/config";
	auto pos = rx_str.find_last_of(' ');
	auto temp = rx_str.substr(pos + 1);

	rx_str = temp;

	result = stoi(rx_str);

	return result;
}

bool CControl::set_data(int type, int channel, int val) {

	// TX strings
	std::string tx_str = "S " + std::to_string(type) + " " + std::to_string(channel) + " " + std::to_string(val) + "\n"; // gather user input

	// Send TX string
	_com.write(tx_str.c_str(), tx_str.length());
	Sleep(10); // wait for ADC conversion, etc. May not be needed? 

	// temporary storage
	char buff[2];

	// start timeout count
	double start_time = cv::getTickCount();

	buff[0] = 0;
	// Read 1 byte and if an End Of Line then exit loop
	// Timeout after 1 second, if debugging step by step this will cause you to exit the loop
	while (buff[0] != '\n' && (cv::getTickCount() - start_time) / cv::getTickFrequency() < 1.0) //testing for a second
	{
		if (_com.read(buff, 1) > 0)
		{
			//rx_str = rx_str + buff[0]; //writes to string as it comes in from com (?)			}
		}//endif
	}//endwhile

	return true;
}


void CControl::get_button(int button, int &value) // 1 for S1, 2 for S2
{
	static int result1, result2, result3;
	static double start_tic = cv::getTickCount();
	double elapsed_time = ((cv::getTickCount() - start_tic) / cv::getTickFrequency());

	get_data(0, button, result1);

	if (result1 != result2)
	{
		bounceflag = true;
		start_tic = cv::getTickCount();
	}

	if ((bounceflag == true) && (elapsed_time >= 0.01))
	{
		get_data(0, button, result3);
		value = result3;
		bounceflag = false;
	}

	if (elapsed_time >= 1)
		bounceflag = false;

	result2 = result1;
}

void CControl::get_analog(float &outputA, float &outputB)
{
	int temp1, temp2;

	get_data(1, 9, temp1); //get y data
	outputB = temp1;
	outputB = (outputB / 1024); //give a percentage

	get_data(1, 15, temp2); //get x data
	outputA = temp2;
	outputA = (outputA / 1024); //give a percentage
}

void CControl::scale_analog(float &outputA, float &outputB)
{
	int temp;
	get_data(1, 9, temp); //get y data
	outputB = temp;
	outputB = (outputB / 1024); //give a percentage

	get_data(1, 15, temp); //get x data
	outputA = temp;
	outputA = (outputA / 1024); //give a percentage
}