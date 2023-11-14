//------------------------------------------------------------------------
// GameTest.cpp
//------------------------------------------------------------------------
#include "stdafx.h"
//------------------------------------------------------------------------
#include <windows.h> 
#include <math.h>  
//------------------------------------------------------------------------
#include "app\app.h"
//------------------------------------------------------------------------

const float THUMB_STICK_THRESHOLD = 0.5f;
const float POSITION_DELTA = 1.0f;
const float SCALE_DELTA = 0.1f;
const float ANGLE_DELTA = 0.1f;

//------------------------------------------------------------------------
// Eample data....
//------------------------------------------------------------------------
CSimpleSprite *testSprite;
enum
{
	ANIM_FORWARDS,
	ANIM_BACKWARDS,
	ANIM_LEFT,
	ANIM_RIGHT,
};
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{
	//------------------------------------------------------------------------
	// Example Sprite Code....
	testSprite = App::CreateSprite(".\\TestData\\Test.bmp", 8, 4);
	testSprite->SetPosition(400.0f, 400.0f);
	float speed = 1.0f / 15.0f;
	testSprite->CreateAnimation(ANIM_BACKWARDS, speed, { 0,1,2,3,4,5,6,7 });
	testSprite->CreateAnimation(ANIM_LEFT, speed, { 8,9,10,11,12,13,14,15 });
	testSprite->CreateAnimation(ANIM_RIGHT, speed, { 16,17,18,19,20,21,22,23 });
	testSprite->CreateAnimation(ANIM_FORWARDS, speed, { 24,25,26,27,28,29,30,31 });
	testSprite->SetScale(1.0f);
	//------------------------------------------------------------------------
}

void UpdatePositionAndAnimation()
{
	float x, y;
	testSprite->GetPosition(x, y);
	float thumbStickX = App::GetController().GetLeftThumbStickX();
	float thumbStickY = App::GetController().GetLeftThumbStickY();

	if (thumbStickX > THUMB_STICK_THRESHOLD) {
		testSprite->SetAnimation(ANIM_RIGHT);
		x += POSITION_DELTA;
	}
	else if (thumbStickX < -THUMB_STICK_THRESHOLD) {
		testSprite->SetAnimation(ANIM_LEFT);
		x -= POSITION_DELTA;
	}

	if (thumbStickY > THUMB_STICK_THRESHOLD) {
		testSprite->SetAnimation(ANIM_BACKWARDS);
		y -= POSITION_DELTA;
	}
	else if (thumbStickY < -THUMB_STICK_THRESHOLD) {
		testSprite->SetAnimation(ANIM_FORWARDS);
		y += POSITION_DELTA;
	}

	testSprite->SetPosition(x, y);
}

void UpdateScale()
{
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_UP, false)) {
		testSprite->SetScale(testSprite->GetScale() + SCALE_DELTA);
	}
	else if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_DOWN, false)) {
		testSprite->SetScale(testSprite->GetScale() - SCALE_DELTA);
	}
}

void UpdateRotation()
{
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_LEFT, false)) {
		testSprite->SetAngle(testSprite->GetAngle() + ANGLE_DELTA);
	}
	else if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_RIGHT, false)) {
		testSprite->SetAngle(testSprite->GetAngle() - ANGLE_DELTA);
	}
}

void ResetAnimationOnButtonA()
{
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_A, true))
	{
		testSprite->SetAnimation(-1);
	}
}

//------------------------------------------------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(float deltaTime)
{
	testSprite->Update(deltaTime);
	UpdatePositionAndAnimation();
	UpdateScale();
	UpdateRotation();
	ResetAnimationOnButtonA();
}

//------------------------------------------------------------------------
// Add your display calls here (DrawLine,Print, DrawSprite.) 
// See App.h 
//------------------------------------------------------------------------
void Render()
{	
	testSprite->Draw();
}
//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{	
	//------------------------------------------------------------------------
	// Example Sprite Code....
	delete testSprite;
	//------------------------------------------------------------------------
}