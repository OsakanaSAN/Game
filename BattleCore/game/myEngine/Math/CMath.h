#pragma once

class CMath {
public:
	static float PI;
	/*!
	* @brief	degree����radian�ɕϊ��B
	*/
	static float DegToRad(float deg)
	{
		return deg * (PI / 180.0f);
	}
	/*!
	* @brief	radian����degree�ɕϊ��B
	*/
	static float RadToDeg(float rad)
	{
		return rad / (PI / 180.0f);
	}
};