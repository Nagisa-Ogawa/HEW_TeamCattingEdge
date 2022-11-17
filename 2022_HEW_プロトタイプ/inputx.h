//=============================================================================
//
// 入力処理 [input.h]
// Author : 
//
//=============================================================================
#pragma once

#include <XInput.h>

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//ボタン一覧

// XINPUT_GAMEPAD_DPAD_UP			十字キー上
// XINPUT_GAMEPAD_DPAD_DOWN			十字キー下
// XINPUT_GAMEPAD_DPAD_LEFT			十字キー左
// XINPUT_GAMEPAD_DPAD_RIGHT		十字キー右
// XINPUT_GAMEPAD_START
// XINPUT_GAMEPAD_BACK
// XINPUT_GAMEPAD_LEFT_THUMB		Lスティック押し込み
// XINPUT_GAMEPAD_RIGHT_THUMB		Rスティック押し込み
// XINPUT_GAMEPAD_LEFT_SHOULDER		Lボタン
// XINPUT_GAMEPAD_RIGHT_SHOULDER	Rボタン
// XINPUT_GAMEPAD_A
// XINPUT_GAMEPAD_B
// XINPUT_GAMEPAD_X
// XINPUT_GAMEPAD_Y

//スティックの範囲
// X軸 左-1.0　右 1.0
// Y軸 上 1.0　下-1.0

//トリガーの範囲
// 0 ～ 255

// バイブレーションの範囲
// 0 ～ 255

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitInputX(HINSTANCE hInst, HWND hWnd);
void UninitInputX(void);
void UpdateInputX(void);

float GetThumbLeftX(int padNo);
float GetThumbLeftY(int padNo);
float GetThumbRightX(int padNo);
float GetThumbRightY(int padNo);

void SetLeftVibration(int padNo, int speed);
void SetRightVibration(int padNo, int speed);
void SetVibration(int padNo, int speed);
void StopVibration(int padNo);

int GetLeftTrigger(int padNo);
int GetRightTrigger(int padNo);

BOOL IsButtonPressedX(int padNo,DWORD button);
BOOL IsButtonTriggeredX(int padNo,DWORD button);

