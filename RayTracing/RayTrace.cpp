#include <cmath>
#include <float.h>
#include <DxLib.h>
#include "RayTrace.h"

RayTrace::RayTrace(Size& _size)
{
	_screenSize = _size;
}

RayTrace::~RayTrace()
{
}

void RayTrace::Update(void)
{
	RayTracing(Vector3f(0, 100, 300), Sphere(100, Position3f(0, 100, -50)));
}

bool RayTrace::IsHitRayAndObject(const Position3f& eye, const Vector3f& ray, const Sphere& sp, Vector3f& normal, Vector3f& hitPos)
{
	auto distance = [&](float a, float b) {
		return sqrt(pow(b, 2) - pow(a, 2));
	};
	auto sight = sp._pos - eye;
	auto baseRange = Dot(sight, ray);
	auto base = ray * baseRange;
	auto vec = base - sight;
	if (sp._radius >= vec.Magnitude())
	{
		auto t = baseRange - distance(vec.Magnitude(), sp._radius);
		hitPos = eye + ray * t;
		normal = (hitPos - sp._pos).Normalized();
		return true;
	}
	return false;
}

void RayTrace::DrawPixelWithFloat(int x, int y, float r, float g, float b)
{
	DrawPixel(x, y, GetColor((int)(r * 0xff), (int)(g * 0xff), (int)(b * 0xff)));
}

void RayTrace::RayTracing(const Position3f& eye, const Sphere& sp)
{
	Vector3f light = Vector3f(-1, 1, 1).Normalized();

	for (int y = 0; y < _screenSize.height; y++)
	{
		for (int x = 0; x < _screenSize.width; x++)
		{
			// 視線ベクトル
			auto sight = Vector3f(x - _screenSize.width / 2, _screenSize.height / 2 - y, 0) - eye;
			sight.Normalize();
			Vector3f normal;
			Vector3f hitPos;
			auto plane = Plane{ Vector3f(0,1,0), -10 };

			if (IsHitRayAndObject(eye, sight, sp, normal, hitPos))
			{
				// 基本色
				Color albedo = { 1.f,0.7f,0.7f };
				// 拡散反射
				auto diffuse = Dot(light, normal);
				diffuse = (0 <= diffuse ? diffuse: 0);
				albedo *= diffuse;
				// 反射
				auto reflect = 2.f * normal * diffuse - light;
				// 反射光(スペキュラ)
				float specular = 1 * pow(Dot(reflect, sight), 20) * 1;
				Color color = { Clamp(albedo.x + specular),Clamp(albedo.y + specular) ,Clamp(albedo.z + specular)};

				// 鏡面反射
				auto ref = ReflectedVector(sight, normal);
				Color refColor = {1,1,1};
				Vector3f RefPos = hitPos;
				if (Dot(plane._normal, ref) < 0)
				{
					RefPos = hitPos + ref * (plane._offset - Dot(hitPos, plane._normal)) / Dot(ref, plane._normal);
					refColor = GetFloorColor(RefPos);
				}
				color = color * Color(Clamp(refColor.x), Clamp(refColor.y), Clamp(refColor.z));
				DrawPixelWithFloat(x, y, color.x, color.y, color.z);
			}
			else
			{
				// 床との判定
				if (Dot(plane._normal, sight) < 0)
				{
					auto t = (plane._offset - Dot(eye, plane._normal)) / Dot(sight, plane._normal);
					hitPos = eye + sight * t;
					
					auto col = GetFloorColor(hitPos);
					DrawPixelWithFloat(x, y, col.x,col.y,col.z);
				}
				else
				{
					DrawPixelWithFloat(x, y, 0.4f, 0.9f, 1.f);
				}
			}
		}
	}
}

float RayTrace::Clamp(float in, const float minval, const float maxval)
{
	return max(min(in, maxval), minval);
}

Color RayTrace::GetFloorColor(Vector3f& pos)
{
	if (static_cast<int>((pos.x + _screenSize.width * 10) / 100) % 2 == 0)
	{
		if (static_cast<int>((pos.z + _screenSize.width * 10) / 100) % 2 == 1)
		{
			return Color(1.f, 1.f, 1.f);
		}
		else
		{
			return Color(1.f, 0.07f, 0.5f);
		}
	}
	else
	{
		if (static_cast<int>((pos.z + _screenSize.width* 10) / 100) % 2 == 0)
		{
			return Color(1.f, 1.f, 1.f);
		}
		else
		{
			return Color(1.f, 0.07f, 0.5f);
		}
	}
}
