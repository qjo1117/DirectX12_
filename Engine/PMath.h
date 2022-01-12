#pragma once




class PMath
{
public:
	template<typename T>
	static T Lerp(const T& a, const T& b, float alpha) {
		return a + ((a - b) * alpha);
	}
	static int32 Lerp(const int32& a, const int32& b, float alpha) {
		return a + ((a - b) * alpha);
	}
	static float Lerp(const float& a, const float& b, float alpha) {
		return a + ((a - b) * alpha);
	}

	template<typename T>
	static T Clamp(const T& x, const T& low, const T& high)
	{
		return x < low ? low : (x > high ? high : x);
	}
	static float Clamp(const float& x, const float& low, const float& high)
	{
		return x < low ? low : (x > high ? high : x);
	}
	static float Clamp(const int32& x, const int32& low, const int32& high)
	{
		return x < low ? low : (x > high ? high : x);
	}


};

