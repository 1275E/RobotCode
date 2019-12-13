// update in the autonomous.cpp as well
#define BACK_LEFT_WHEEL_PORT 1
#define BACK_RIGHT_WHEEL_PORT 9
#define FRONT_LEFT_WHEEL_PORT 5
#define FRONT_RIGHT_WHEEL_PORT 8
#define LEFT_LIFT_PORT 2
#define TILTER_PORT 10
#define RIGHT_INTAKE_MOTOR 3
#define LEFT_INTAKE_MOTOR 6

#define LIFT_DOWN_TOUCH_SENSOR 10
#define LIFT_UP_TOUCH_SENSOR 10


#include "main.h"
#include <cmath>

using namespace okapi;

double vCurve(double val);
double correct(double c);
void opcontrol() {
	
	auto drive = ChassisControllerFactory::create( {BACK_LEFT_WHEEL_PORT, -FRONT_LEFT_WHEEL_PORT}, {-BACK_RIGHT_WHEEL_PORT, FRONT_RIGHT_WHEEL_PORT}, AbstractMotor::gearset::green,{4_in, 12_in} );
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	auto intake = AsyncControllerFactory::posIntegrated({-RIGHT_INTAKE_MOTOR, LEFT_INTAKE_MOTOR});
	
  	pros::ADIDigitalIn LiftUpSensor(LIFT_UP_TOUCH_SENSOR);
	pros::ADIDigitalIn LiftDownSensor(LIFT_DOWN_TOUCH_SENSOR);

	pros::Motor leftLift(LEFT_LIFT_PORT);
	pros::Motor backLeft(BACK_LEFT_WHEEL_PORT);
	pros::Motor frontLeft(FRONT_LEFT_WHEEL_PORT, true);
	pros::Motor backRight(BACK_RIGHT_WHEEL_PORT, true);
	pros::Motor frontRight(FRONT_RIGHT_WHEEL_PORT);
	pros::Motor rightIntake(RIGHT_INTAKE_MOTOR, true);
	pros::Motor leftIntake(LEFT_INTAKE_MOTOR);
	pros::Motor tilter(TILTER_PORT, true);
	
	int liftMove, lx, ly, rx, fr, fl, bl, br;

	while (true) {
		
		// Mechanum Drive
		ly = vCurve(master.get_analog(ANALOG_LEFT_Y));
		lx = vCurve(master.get_analog(ANALOG_LEFT_X));
		rx = vCurve(master.get_analog(ANALOG_RIGHT_X));

		fr = correct(ly - lx - rx);
		br = correct(ly - lx + rx);
		fl = correct(ly + lx + rx);
		bl = correct(ly + lx - rx);

		frontRight = fr;
		frontLeft = fl;
		backLeft = bl;
		backRight = br;
		// End Mechanum Drive

		// Lift Control
		liftMove = vCurve(master.get_analog(ANALOG_RIGHT_Y));

		if (!(liftMove <= 10 && liftMove >= -10)) {
			leftLift.move(liftMove);
		}
		// End Lift Control

		//Tray Control
		auto left1 = master.get_digital(DIGITAL_L1);
		auto left2 = master.get_digital(DIGITAL_L2);

		if (left1) {
			tilter.move(100);
		} else if(left2) {
			tilter.move(-100);
		} else {
			tilter.move(0);
		}
		// End Tray Control

		// Start Intake Control
		auto right1 = master.get_digital(DIGITAL_R1);
		auto right2 = master.get_digital(DIGITAL_R2);

		if (right1) {
			leftIntake = 127;
			rightIntake = 127;
		}
		else if (right2){
			leftIntake = -127;
			rightIntake = -127;
		} else {
			leftIntake = 0;
			rightIntake = 0;
		}
		// End Intake Control
		
		// Start Backup Macro
		auto autoBackup = master.get_digital_new_press(DIGITAL_DOWN);
		if (autoBackup) {
			drive.setMaxVelocity(50);
    
    		drive.moveDistance(-4_ft);
    
    		drive.waitUntilSettled();

			drive.stop();
		}
		// End Backup Macro

		// Start dispense 1 cube macro
		auto dispense1 = master.get_digital_new_press(DIGITAL_B);
		if (dispense1) {
			intake.setMaxVelocity(50);
			
			intake.tarePosition();

			intake.setTarget(-150);

			intake.waitUntilSettled();

			intake.stop();
		}
		// End dispense 1 cube macro

		pros::delay(20);
	}
}

double vCurve(double val) {
	double modVal = pow(0.0292*val,3.0) + 0.59*val;
	if (modVal>=127) modVal=127;
	else if (modVal<=-127) modVal=-127;
	return modVal;
}
double correct(double c) {
	double x=c;
	if (c>127) x=127;
	else if (c<-127) x=-127;
	return x;
}