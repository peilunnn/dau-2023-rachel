//------------------------------------------------------------------------
// GameTest.cpp
//------------------------------------------------------------------------
#include "stdafx.h"
//------------------------------------------------------------------------
#include <windows.h> 
#include <math.h>  
//------------------------------------------------------------------------
#include "App/Application/include/app.h"
//------------------------------------------------------------------------

const float THUMB_STICK_THRESHOLD = 0.5f;
const float POSITION_DELTA = 1.0f;
const float SCALE_DELTA = 0.1f;
const float ANGLE_DELTA = 0.1f;
int lastNonIdleAnimState = 6;

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
	ANIM_IDLE_LEFT,
	ANIM_IDLE_RIGHT,
	ANIM_IDLE_FORWARDS,
	ANIM_IDLE_BACKWARDS,
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
	testSprite->CreateAnimation(ANIM_FORWARDS, speed, { 24,25,26,27,28,29,30,31 });
	testSprite->CreateAnimation(ANIM_BACKWARDS, speed, { 0,1,2,3,4,5,6,7 });
	testSprite->CreateAnimation(ANIM_LEFT, speed, { 8,9,10,11,12,13,14,15 });
	testSprite->CreateAnimation(ANIM_RIGHT, speed, { 16,17,18,19,20,21,22,23 });
	testSprite->CreateAnimation(ANIM_IDLE_BACKWARDS, speed, { 0 });
	testSprite->CreateAnimation(ANIM_IDLE_LEFT, speed, { 8 });
	testSprite->CreateAnimation(ANIM_IDLE_RIGHT, speed, { 16 });
	testSprite->CreateAnimation(ANIM_IDLE_FORWARDS, speed, { 24 });
	testSprite->SetScale(1.0f);
	//------------------------------------------------------------------------
}

void UpdatePositionAndAnimation()
{
	float x, y;
	testSprite->GetPosition(x, y);
	float thumbStickX = App::GetController().GetLeftThumbStickX();
	float thumbStickY = App::GetController().GetLeftThumbStickY();
	int animationState = -1;

	if (thumbStickX > THUMB_STICK_THRESHOLD) {
		animationState = ANIM_RIGHT;
		x += POSITION_DELTA;
		lastNonIdleAnimState = ANIM_IDLE_RIGHT;
	}
	else if (thumbStickX < -THUMB_STICK_THRESHOLD) {
		animationState = ANIM_LEFT;
		x -= POSITION_DELTA;
		lastNonIdleAnimState = ANIM_IDLE_LEFT;
	}

	if (thumbStickY > THUMB_STICK_THRESHOLD) {
		animationState = ANIM_BACKWARDS;
		y -= POSITION_DELTA;
		lastNonIdleAnimState = ANIM_IDLE_BACKWARDS;
	}
	else if (thumbStickY < -THUMB_STICK_THRESHOLD) {
		animationState = ANIM_FORWARDS;
		y += POSITION_DELTA;
		lastNonIdleAnimState = ANIM_IDLE_FORWARDS;
	}

	if (fabs(thumbStickX) <= THUMB_STICK_THRESHOLD && fabs(thumbStickY) <= THUMB_STICK_THRESHOLD) {
		animationState = lastNonIdleAnimState;
	}

	testSprite->SetAnimation(animationState);
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

//------------------------------------------------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(float deltaTime)
{
	char debugMsg[256];
	sprintf_s(debugMsg, "Current Animation: %d\n", lastNonIdleAnimState);
	OutputDebugStringA(debugMsg);

	testSprite->Update(deltaTime);
	UpdatePositionAndAnimation();
	UpdateScale();
	UpdateRotation();
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