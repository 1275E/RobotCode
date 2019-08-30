#include "main.h"

#define BACK_LEFT_WHEEL_PORT 1
#define BACK_RIGHT_WHEEL_PORT 2
#define FRONT_LEFT_WHEEL_PORT 3
#define FRONT_RIGHT_WHEEL_PORT 4

void opcontrol() {
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	pros::Motor backLeft(BACK_LEFT_WHEEL_PORT);
	pros::Motor frontLeft(BACK_LEFT_WHEEL_PORT);
	pros::Motor backRight(BACK_RIGHT_WHEEL_PORT, true);
	pros::Motor frontRight(BACK_RIGHT_WHEEL_PORT, true);
	
	int left, right;

	while (true) {
		left = master.get_analog(ANALOG_LEFT_Y);
		right = master.get_analog(ANALOG_RIGHT_Y);

		backLeft.move(left);
		backRight.move(right);
		frontLeft.move(left);
		frontRight.move(right);

		pros::delay(20);
	}
}
