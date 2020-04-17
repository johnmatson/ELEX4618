#pragma once
#include "stdafx.h"
#include "Serial.h"

#include <string>
#include <iostream>
#include <thread>

#include "conio.h"

// OpenCV Include
#include "opencv.hpp"

// OpenCV Library
#pragma comment(lib,".\\opencv\\lib\\opencv_world310d.lib")

class CControl { 

	private:   
		Serial _com;
		int bounceflag = false;
	public:   
		CControl();
		~CControl();

		void init_com(int comport);
		bool get_data(int type, int channel, int &result);
		bool set_data(int type, int channel, int val);
		void get_analog(float &outputA, float &outputB);
		void scale_analog(float &outputA, float &outputB);
		void get_button(int button, int &value);
};