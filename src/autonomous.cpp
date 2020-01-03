#include "main.h"
#include "globals.hpp"

// update in the autonomous.cpp as well
#define BACK_LEFT_WHEEL_PORT 1
#define BACK_RIGHT_WHEEL_PORT 9
#define FRONT_LEFT_WHEEL_PORT 5
#define FRONT_RIGHT_WHEEL_PORT 8
#define LEFT_LIFT_PORT 7
#define TILTER_PORT 10
#define RIGHT_INTAKE_MOTOR 3
#define LEFT_INTAKE_MOTOR 6


using namespace okapi;  

auto drive = ChassisControllerFactory::create( {BACK_LEFT_WHEEL_PORT, -FRONT_LEFT_WHEEL_PORT}, {-BACK_RIGHT_WHEEL_PORT, FRONT_RIGHT_WHEEL_PORT}, AbstractMotor::gearset::green,{4_in, 12_in} );
auto intake = AsyncControllerFactory::posIntegrated({-RIGHT_INTAKE_MOTOR, LEFT_INTAKE_MOTOR});
auto lift = AsyncControllerFactory::posIntegrated(LEFT_LIFT_PORT);
auto tilter = AsyncControllerFactory::posIntegrated(TILTER_PORT);
	
// Other Commands
void strafeLeft();
void strafeRight();
void dropStack();
void intakeCubes(int targetToIntakeTo);
void deploy();

// Autons
void sevenStack(int isRight);

// 1 : Blue Protected
// 2 : Blue Unprotected
// 3 : Red Protected
// 4 : Red Unprotected
// 5 : Experimental
// 6 : Skills

void autonomous() {
    deploy();
    switch (autonomousPreSet) {
        case 1:
            // Blue Protected
            sevenStack(1);
            break;
        case 2:
            // Blue Unprotected
            break;
        case 3:
            // Red Protected
            sevenStack(-1);
            break;
        case 4:
            // Red Unprotected
            break;
        case 5:
            // Experimental
            break;
        case 6:
            // Skills
            break;
        default:
            // No Selection
            break;
    }
};

// Autons

void sevenStack( int isRight) {
    
    pros::delay(1500);

    // Slow max velocity
    drive.setMaxVelocity(20);

    // Drive 4ft, intaking cubes
    drive.moveDistanceAsync(4_ft);
    intakeCubes(1000000000);
    
    // Wait to settle
    pros::delay(1500);

    // while, move the lift up.
    lift.setMaxVelocity(200);
    lift.tarePosition();
    lift.setTarget(2000);
    
    // once finished,
    drive.waitUntilSettled();
    
    // move the lift down, all while intaking cubes
    lift.setMaxVelocity(70);
    lift.setTarget(0);
    
    // now turn to the left (or right)
    drive.turnAngle(130 * isRight);    
    drive.waitUntilSettled();

    // drive to the edge of the stack.
    drive.moveDistance(2.75_ft);
    intake.stop();
    
    // Drop the stack
    dropStack();
};

// If the bot needs to turn right, then set isRight to 1, otherwise, set it to -1.

// Commands

void strafeLeft(double seconds) {
    
};

void strafeRight(double seconds) {

};

void intakeCubes(int targetToIntakeTo) {
    intake.setMaxVelocity(50);
			
	intake.tarePosition();

	intake.setTarget(targetToIntakeTo);
};

void dropStack() {
    tilter.setMaxVelocity(10);
    tilter.setTarget(700);

    drive.setMaxVelocity(20);
    drive.moveDistance(-2_ft);
};

void deploy() {
    intake.setMaxVelocity(50);
			
	intake.tarePosition();

	intake.setTarget(-500);
}