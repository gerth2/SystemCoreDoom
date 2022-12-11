// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include <frc/smartdashboard/SmartDashboard.h>
#include "subsystems/DoomSubsystem.h"
#include "Robot.h"

DoomSubsystem::DoomSubsystem(frc::XboxController *controller)
{
	this->mat = cv::Mat(SCREEN_HEIGHT, SCREEN_WIDTH, CV_8UC3);
	this->doomRunning = false;
	this->driver = controller;
	
}

void DoomSubsystem::Periodic()
{
	static int periods = 0;

	// TODO - collect joystick movements

	frc::SmartDashboard::PutNumber("periods", ++periods);
}

void DoomSubsystem::StartEngine()
{
	printf("Starting DOOM Engine\n");

	this->doomRunning = true;
	// frc::CameraServer::StartAutomaticCapture();

	this->cvSink = frc::CameraServer::GetVideo();
	this->videoStream = frc::CameraServer::PutVideo("doom", SCREEN_WIDTH, SCREEN_HEIGHT);

	std::thread doomThread(LaunchDoom);
	doomThread.detach();
}

void DoomSubsystem::StopEngine()
{
	printf("Stopping DOOM Engine\n");

	this->doomRunning = false;
}

void DoomSubsystem::LaunchDoom()
{
	D_SetLoopHook(&DoomSubsystem::OnDoomLoop);
	myargc = 0;
	myargv = {};
	// myargv[0] = strdup("-regdev");

	D_DoomMain();
}

void sendStick(double x, double y, bool button1, bool button2, bool button3, bool button4)
{
	// x = x < 0 ? 0 : x;
	// y = y > 0 ? 0 : y;
	event_t event = {ev_joystick, 0, 0, 0};
	event.data1 |= button1 ? 1 : 0;
	event.data1 |= button2 ? 2 : 0;
	event.data1 |= button3 ? 4 : 0;
	event.data1 |= button3 ? 8 : 0;
	event.data2 = ((int)(x * SCREEN_WIDTH)/10); // Probably wrong
	event.data3 = ((int)(y * SCREEN_HEIGHT)/10);
	D_PostEvent(&event);
}

void DoomSubsystem::sendKeyDown(int key)
{
	// Keys are processed between Menu and game, menu is in M_Responder, game is in G_Responder.
	event_t event = {ev_keydown, key, 0, 0};
	D_PostEvent(&event);
	
}

void DoomSubsystem::sendKeyUp(int key)
{
	event_t event = {ev_keyup, key, 0, 0};
	D_PostEvent(&event);
	// Actual events occur in G_BuildTiccmd g_game.c
}

void DoomSubsystem::OnDoomLoop()
{

	printf("Loop hook called!\n");
	// Update the screen
	Robot::m_doomSubsystem.UpdateMat();

	
	// TODO Craft joystick input into event_t and send to D_PostEvent
	// sendStick
	// TODO Send joystick inputs to the engine
}

void DoomSubsystem::UpdateMat()
{	
	sendStick(this->driver->GetRightX() + this->driver->GetLeftX(), this->driver->GetLeftY() + this->driver->GetRightY(),
		this->driver->GetRightBumper() || this->driver->GetAButton() || (this->driver->GetRightTriggerAxis() > 0.5), this->driver->GetBButton(), this->driver->GetXButton(), this->driver->GetYButton());

	// Get the current screen buffer
	byte palette[256 * 3];
	byte screen[SCREEN_WIDTH * SCREEN_HEIGHT];

	I_ReadPalette(palette);
	I_ReadScreen(screen);

	// Iterate through the pixels in the Mat
	for (int y = 0; y < mat.rows; y++)
	{
		for (int x = 0; x < mat.cols; x++)
		{
			// Calculate index into color palette
			int imgIndex = y * SCREEN_WIDTH + x;

			int paletteIndex = screen[imgIndex];

			// Get pixel
			cv::Vec3b &color = mat.at<cv::Vec3b>(y, x);

			// Copy the values from the screen buffer (BGR order bc ¯\_(ツ)_/¯)
			color[0] = palette[paletteIndex * 3 + 2];
			color[1] = palette[paletteIndex * 3 + 1];
			color[2] = palette[paletteIndex * 3 + 0];

			// Set pixel
			mat.at<cv::Vec3b>(cv::Point(x, y)) = color;
		}
	}

	// Send the updated Mat to the video stream
	this->videoStream.PutFrame(this->mat);
}

int IsDoomRunning()
{
	return Robot::m_doomSubsystem.doomRunning;
}
