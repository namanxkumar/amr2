#include <ros.h>
#include <AFMotor.h>
#include <geometry_msgs/Twist.h>

// Pin variables for motors.
AF_DCMotor motor1(2);
AF_DCMotor motor2(1);
const bool left_fwd = true;
const bool right_fwd = false;

// Default_speed.
const int default_vel = 255;
int state_vel = default_vel;
enum State {FWD, BWD, RIGHT, LEFT, STOP};
State state;
const int max_vel = 255;

ros::NodeHandle  nh;


// MOTOR 1 :  RIGHT
// MOTOR 2 :  LEFT


void MoveLeft(const size_t speed) {
  motor1.setSpeed(speed);
  motor2.setSpeed(speed);
  motor1.run(FORWARD);
  motor2.run(BACKWARD);
}

void MoveRight(const size_t speed) {
  motor1.setSpeed(speed);
  motor2.setSpeed(speed);
  motor1.run(BACKWARD);
  motor2.run(FORWARD);
}

void MoveFwd(const size_t speed) {
  motor1.setSpeed(speed);
  motor2.setSpeed(speed);
  motor1.run(FORWARD);
  motor2.run(FORWARD);
}

void MoveBwd(const size_t speed) {
  motor1.setSpeed(speed);
  motor2.setSpeed(speed);
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
}

void MoveStop() {
  motor1.run(RELEASE);
  motor2.run(RELEASE);
}

void cmd_vel_cb(const geometry_msgs::Twist & msg) {
  // Read the message. Act accordingly.
  // We only care about the linear x, and the rotational z.
  const float x = msg.linear.x;
  const float z_rotation = msg.angular.z;
  //  MoveDifferential(x,z_rotation);
  if (x > 0) {
    if (state == FWD){
      state_vel +=10;
      state_vel = min(state_vel, 255);
    }
    MoveFwd(state_vel);
    state = FWD;
  }
  else if (x < 0) {
    if (state == BWD){
      state_vel +=10;
      state_vel = min(state_vel, 255);

    }
    MoveBwd(state_vel);
    state = BWD;
  }
  else if (z_rotation < 0) {
    if (state == RIGHT){
      state_vel +=10;
      state_vel = min(state_vel, 255);

    }
    MoveRight (state_vel);
    state = RIGHT;
  }  
  else if (z_rotation > 0) {
    if (state == LEFT){
      state_vel += 10;
      state_vel = min(state_vel, 255);

    }
    MoveLeft (state_vel);
    state = LEFT;
  }
  else {
    MoveStop();
    state_vel = default_vel;
    state = STOP;
  }

}


ros::Subscriber<geometry_msgs::Twist> sub("cmd_vel", &cmd_vel_cb);

void setup() {

  //Set initial speed of the motor & stop
  motor1.setSpeed(255);
  motor1.run(RELEASE);
  motor2.setSpeed(255);
  motor2.run(RELEASE);

  nh.initNode();
  nh.subscribe(sub);
}

void loop() {
  nh.spinOnce();
  delay(1);
}
