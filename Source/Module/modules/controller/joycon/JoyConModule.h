/*
  ==============================================================================

    JoyConModule.h
    Created: 23 Jan 2018 4:30:24pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#ifndef __arm__
#include "JoyShockLibrary.h"

/*
#elif JUCE_MAC
#define JOYTIME_CORE_BUILT_AS_STATIC
#define JOYTIME_INPUT_HIDAPI_BUILT_AS_STATIC
#include "joytime-input-hidapi.hpp"
*/

#endif

class JoyConModule :
	public Module,
	public Thread
{
public:
	JoyConModule();
	~JoyConModule();

#ifndef __arm__
	Array<int> controllers;
#endif

	ControllableContainer leftValues;
	ControllableContainer rightValues;

	//Left controller
	FloatParameter * leftAccelX;
	FloatParameter * leftAccelY;
	FloatParameter * leftAccelZ;
	FloatParameter * leftRoll;
	FloatParameter * leftPitch;
	Point2DParameter * leftAxis;
	BoolParameter * lStick;
	BoolParameter * down;
	BoolParameter * left;
	BoolParameter * right;
	BoolParameter * up;
	BoolParameter * zl;
	BoolParameter * l;
	BoolParameter * leftSL;
	BoolParameter * leftSR;
	BoolParameter * capture;
	BoolParameter *  minus;

	//Right controller
	FloatParameter * rightAccelX;
	FloatParameter * rightAccelY;
	FloatParameter * rightAccelZ;
	FloatParameter * rightRoll;
	FloatParameter * rightPitch;
	Point2DParameter * rightAxis;
	BoolParameter * rStick;
	BoolParameter * a;
	BoolParameter * b;
	BoolParameter * x;
	BoolParameter * y;
	BoolParameter * r;
	BoolParameter * rightSL;
	BoolParameter * rightSR;
	BoolParameter * zr;
	BoolParameter * home;
	BoolParameter * plus;


	//imu
	double lastIMUProcessTime;
	bool resetIMUOnNextPacket;
	float imuError;

	Vector3D<float> i_b;
	Vector3D<float> j_b;
	Vector3D<float> k_b;
	Vector3D<float> k_acc;
	Vector3D<float> i_b_;
	Vector3D<float> w_a, w_g;

#ifndef __arm__
	void updateController(int id);
    void onControllableFeedbackUpdateInternal(ControllableContainer *, Controllable *c) override;
#endif
    


	static JoyConModule * create() { return new JoyConModule(); }
	virtual String getDefaultTypeString() const override { return "JoyCon"; }

	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(JoyConModule)


	// Inherited via Thread
	virtual void run() override;

};
