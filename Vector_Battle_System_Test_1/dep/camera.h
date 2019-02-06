#pragma once
/*The purpose of this header file is to be epic.
Additionally, I was tired of having so many globals that could all fit into one class.
Essentially it makes the cursor and the camera into classes
It also contains some other settings*/
#ifndef __camera_h
#define __camera_h

#include "fcolor.h"
#include "geometry.h"

//Controls all the settings for a given camera
class camera {
public:
	float LookPointX = 10.0f / 2.0f;
	float LookPointY = 6.0f / 2.0f;
	float LookPointZ = 0.0f / 2.0f;
	const float PerspectiveRiseMax = PI / 2.0f - 0.0005f;
	float PerspectiveOrbit = 0.0f;
	float PerspectiveRise = PerspectiveRiseMax;
	float PerspectiveDist = 7.5f;

};

//Controls settings and information about the 'cursor' (which is actually a construct)
class cursor {
public:
	float X = 1.0f / 2.0f;
	float Y = 6.0f / 2.0f;
	point Position = point(5.0f, 3.0f);
	const float Sensitivity = 0.3f;
	float Snap = 0.0f;
	const float Tilt = PI / 4.0f;
	float Rot = 0.0f;
	const float Yaw = 0.0f; //0.2;
	const float SpreadDefault = PI / 8.0f; // PI / 12;
	float Spread = SpreadDefault;
	const float Size = 0.4f;
	float RotSpeed = 2.0f * PI; //0.05;
	fcolor color = fcolor(1.0f,1.0f,1.0f);

	float Red() { return color.get_level('r'); }
	float Green() { return color.get_level('g'); }
	float Blue() { return color.get_level('b'); }
};




#endif