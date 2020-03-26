#pragma once
#include "geometry/Geometry.h"

// レイトレーイングを実際に行うクラス
class RayTrace
{
public:
	RayTrace(Size& _size);
	~RayTrace();
	void Update(void);
private:
	bool IsHitRayAndObject(const Position3f& eye, const Vector3f& ray, const Sphere& sp, Vector3f& normal, Vector3f& hitPos);
	void DrawPixelWithFloat(int x, int y, float r, float g, float b);
	void RayTracing(const Position3f& eye, const Sphere& sp);
	float Clamp(float in, const float minval = 0.f, const float maxval = 1.f);
	Color GetFloorColor(Vector3f& pos);
	Size _screenSize;
};

