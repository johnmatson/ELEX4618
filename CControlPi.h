#pragma once

#include <opencv2/opencv.hpp>

class CControlPi {

private:
	int bounceflag = false;
public:
	CControlPi();
	~CControlPi();

	void init_com(int comport);
	bool get_data(int type, int channel, int &result);
	bool set_data(int type, int channel, int val);
	void get_analog(float &outputA, float &outputB);
	void scale_analog(float &outputA, float &outputB);
	void get_button(int button, int &_button);
	void count_button(int button, int &result);
};
