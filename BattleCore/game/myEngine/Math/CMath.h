#pragma once

class CMath {
public:
	static float PI;
	/*!
	* @brief	degreeからradianに変換。
	*/
	static float DegToRad(float deg)
	{
		return deg * (PI / 180.0f);
	}
	/*!
	* @brief	radianからdegreeに変換。
	*/
	static float RadToDeg(float rad)
	{
		return rad / (PI / 180.0f);
	}
};