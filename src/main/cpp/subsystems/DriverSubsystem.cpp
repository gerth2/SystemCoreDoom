#include "subsystems/DriverSubsystem.h"
#include <frc/XboxController.h>
#include <frc/drive/DifferentialDrive.h>
#include <frc/kinematics/DifferentialDriveKinematics.h>
#include <frc/kinematics/DifferentialDriveOdometry.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>

#define ANNOY_DEVYNN

DriverSubsystem::DriverSubsystem(frc::XboxController *controller)
{
    DriverSubsystem::driver = controller;

}

DriverSubsystem::~DriverSubsystem()
{
}


void DriverSubsystem::play_sound(signed short *pitch, size_t size, DriverSubsystem *subsys)
{

}


void DriverSubsystem::drive(double x, double y, double rotation, DriverSubsystem *subsys)
{
 
}

void DriverSubsystem::Periodic()
{
}


