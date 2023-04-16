#include "subsystems/DriverSubsystem.h"
#include <frc/XboxController.h>
#include <ctre/phoenix/motorcontrol/can/WPI_TalonFX.h>
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
    DriverSubsystem::right.SetInverted(true);
    
    DriverSubsystem::driveBase.SetSafetyEnabled(false);
    DriverSubsystem::driveBase.SetExpiration(units::time::second_t(1));

    DriverSubsystem::doom.AddInstrument(right);
    DriverSubsystem::doom.AddInstrument(left);

    if(DriverSubsystem::doom.LoadMusic("doom.chrp") != 0)
    {
        fprintf(stderr, "HELP ME!!! I RAN INTO AN ERROR!");
    }
    // DriverSubsystem::doom.Play();
}

DriverSubsystem::~DriverSubsystem()
{
    delete DriverSubsystem::driver;
}

std::mutex lol, sound2;

void DriverSubsystem::play_sound(signed short *pitch, size_t size, DriverSubsystem *subsys)
{
    try{
    sound2.lock();
    // printf("%x\n", pitch[0]);
    // subsys->left.Set(ctre::phoenix::motorcontrol::ControlMode::MusicTone, pitch[0]);

    sound2.unlock();
    }catch(const std::exception& e){
        sound2.unlock();
    }
}


void DriverSubsystem::drive(double x, double y, double rotation, DriverSubsystem *subsys)
{
    if( fabs(x) > 1 && fabs(y) > 1)
    {

        if(x == 100 && y == 100)
        {
            lol.lock();

            frc::Timer timer;
            timer.Restart();
            subsys->doom.Stop();
            subsys->doom.LoadMusic("supermariodeath.chrp");
            subsys->doom.Stop();
            subsys->doom.Play();
            while(timer.Get() < 3_s);
                // subsys->driveBase.ArcadeDrive(0,0.2, false);
            // subsys->driveBase.TankDrive(0,0, false);
            subsys->doom.Stop();
            subsys->doom.LoadMusic("doom.chrp");

#ifdef ANNOY_DEVYNN
            subsys->doom.Play();
#endif
            lol.unlock();
            return;
        }
        else if(x == 50 && y == 50)
        {
            lol.lock();

            frc::Timer timer, root;
            root.Reset();
            root.Start();
            for(int i = 0; i < 1; i++)
            {
                timer.Restart();
                while(timer.Get() < 0.25_s)
                    subsys->driveBase.ArcadeDrive(0,0.1+(0.025*i), false);
                timer.Restart();
                while(timer.Get() < 0.25_s)
                    subsys->driveBase.ArcadeDrive(0,-0.1+(-0.025*i), false);
            }
            subsys->driveBase.TankDrive(0,0, false);
#ifdef ANNOY_DEVYNN
            subsys->doom.Play();
#endif
            lol.unlock();

            return;
        }

    }
    if(fabs(x) > 1 || fabs(y) > 1)
        return;
    frc::ChassisSpeeds ch = {units::meters_per_second_t(x*0.01),units::meters_per_second_t(y*0.01),units::radians_per_second_t(rotation * M_PI* 0.01)};
    
    frc::DifferentialDriveWheelSpeeds wh = subsys->kinimatics.ToWheelSpeeds(ch);
    // DriverSubsystem::driveBase->TankDrive(wh.left, wh.right);
    // subsys->driveBase->ArcadeDrive(0,0);
    // printf("l: %f, r: %f\n", (double)y, (double)rotation);
    // subsys->left->Set(y*0.1);
    // subsys->right->Set(rotation*0.1);
    // if(y*0.15 < 0.05 && rotation*0.15 < 0.01)
    // {
    //     if(!subsys->doom.IsPlaying()){
    //         subsys->driveBase.TankDrive(0,0);
    //         subsys->doom.Play();
    //     }
    // }
    // else
    // {
    //     subsys->doom.Pause();
    // }
    auto speed = y * 0.1;
    auto rotationVelocity = subsys->rotationSlew.Calculate(rotation*0.5).value();
    if(fabs(speed) > 0.05 || fabs(rotationVelocity) > 0.1)
        subsys->driveBase.ArcadeDrive(speed, rotationVelocity, false);
    else if(!subsys->doom.IsPlaying())
    {
        subsys->driveBase.TankDrive(0,0,false);

#ifdef ANNOY_DEVYNN
        subsys->doom.Play();
#endif

    }
}

void DriverSubsystem::Periodic()
{
}


