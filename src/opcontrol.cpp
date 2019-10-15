#define BACK_LEFT_WHEEL_PORT 9
#define BACK_RIGHT_WHEEL_PORT 8
#define FRONT_LEFT_WHEEL_PORT 7
#define FRONT_RIGHT_WHEEL_PORT 2
#define LEFT_LIFT_PORT 1
#define RIGHT_LIFT_PORT 10
#define CLAW_PORT 5

#include "main.h"
#include <cmath>

double vCurve(double val);
double correct(double c);
void opcontrol() {

	pros::Controller master(pros::E_CONTROLLER_MASTER);

	pros::Motor leftLift(LEFT_LIFT_PORT, true);
	pros::Motor rightLift(RIGHT_LIFT_PORT);
	pros::Motor backLeft(BACK_LEFT_WHEEL_PORT);
	pros::Motor frontLeft(FRONT_LEFT_WHEEL_PORT);
	pros::Motor backRight(BACK_RIGHT_WHEEL_PORT, true);
	pros::Motor frontRight(FRONT_RIGHT_WHEEL_PORT, true);
	pros::Motor claw(CLAW_PORT, true);
	
	int y, x, left, right, liftMove;

	while (true) {
		
		// Arcade Drive
		y = vCurve(master.get_analog(ANALOG_LEFT_Y));
    	x = vCurve(master.get_analog(ANALOG_LEFT_X));
		
		left = correct(y + x);
    	right = correct(y - x);
		
		backLeft.move(left);
		backRight.move(right);
		frontLeft.move(left);
		frontRight.move(right);
		// End Arcade Drive

		// Lift Control
		liftMove = vCurve(master.get_analog(ANALOG_RIGHT_Y));

		if (!(liftMove <= 10 && liftMove >= -10)) {
		rightLift.move(liftMove);
		leftLift.move(liftMove);
		}
		// End Lift Control

		// Claw Control
		auto r2Status = master.get_digital(DIGITAL_R2);
		if (r2Status) {
			claw.move(127);
		}		
		else {
			claw.move(0);
		}
		// End Claw Control

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