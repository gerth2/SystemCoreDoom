#pragma once

#include <frc2/command/SubsystemBase.h>
#include <frc/drive/DifferentialDrive.h>
#include <frc/XboxController.h>
#include <frc/kinematics/DifferentialDriveKinematics.h>
#include <frc/kinematics/DifferentialDriveOdometry.h>
#include <frc/filter/SlewRateLimiter.h>

class DriverSubsystem :  public frc2::SubsystemBase
{
private:
	/* data */
	frc::XboxController* driver;
	frc::DifferentialDriveOdometry odometry{frc::Rotation2d(), 0_m, 0_m, frc::Pose2d()};
	double maxSpeed = .01;
public:
	frc::SlewRateLimiter<units::scalar> rotationSlew{0.3 / 1_s};

	frc::DifferentialDriveKinematics kinimatics{1_ft};

	DriverSubsystem(frc::XboxController* controller);
	static void drive(double x, double y, double rotation, DriverSubsystem *subsys);
	~DriverSubsystem();
	DriverSubsystem* getInstance();
	static void play_sound(signed short *pitches, size_t size, DriverSubsystem *subsys);
	 /**
   * Will be called periodically whenever the CommandScheduler runs.
   */
  void Periodic() override;
};

