#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "geometry_msgs/Quaternion.h"
#include "sensor_msgs/Imu.h"
#include "triangle_node/motor_control.h"
#include <math.h>

float omega1, omega2, omega3; // angular velocities
float r = 0.0525; // Wheel radius (105mm/2)
float h = 0.18;   // Distance from the center of the body to the wheel (180mm)

float v_x = 0;    // Global translation speed in x (m/s)
float v_y = 0;    // Global translation speed in y (m/s)
float omega_body = 0;  // Rotational speed of the body (rad/s)
float theta = 3.1415;  // Orientation of the body (rad)

ros::Publisher motor_control_pub;
ros::Publisher imu_pub;


int	omega2pwm(float omega) {
	/*	
		omega ... angular velocity ( in rad/s )
		rpm = omega*9.5493; // conversion from rad/s to rpm	 ( 1/(2*pi)*60 = 9.5493 )
		pwm = 2.4307*rpm + 36.2178; // conversion of rpm to pwm values
	*/
	if(fabs(omega) <= 0.05) return 0;

	return (int)(2.43*(fabs(omega)*9.55) + 36.22);
}

int sign(float number){
	if(number>=0) return 1; else return 0;
}

void imu_callback(const sensor_msgs::Imu & msg){
	imu_pub.publish(msg.orientation);	
}


void cmd_vel_callback(const geometry_msgs::Twist & msg){
	/*
		omega1	...	rotation speed of motor 1	(in rad/s)
		omega2	...	rotation speed of motor 2	(in rad/s)
		omega3	...	rotation speed of motor 3	(in rad/s)
	*/
	ROS_INFO("Msg received");

	geometry_msgs::Twist out_msg;
	v_x = msg.linear.x;
	v_y = msg.linear.y;
	omega_body = msg.angular.z;

	omega1 = 1/r * (v_x * cosf(theta) + v_y * sinf(theta) + omega_body * h);
	omega2 = 1/r * (cosf(theta) * v_y/3 -  sinf(theta) * v_x/3 +  sqrt(3) * sinf(theta) * v_y/3 +  sqrt(3) * cosf(theta) * v_x /3 + omega_body * h);
	omega3 = 1/r * (-sqrt(3) * sinf(theta) * v_y/3 + cosf(theta) * v_y/3 -  sqrt(3)  * cosf(theta) * v_x/3 - sinf(theta) * v_x/3 + omega_body * h);

	// pwm signal 
	out_msg.linear.x = omega2pwm(omega1); // convert from rad/s to pwm signal
	out_msg.linear.y = omega2pwm(omega2);
	out_msg.linear.z = omega2pwm(omega3);

	// motor direction
	out_msg.angular.x = sign(omega1); // set direction bit depending on the rotation speed
	out_msg.angular.y = sign(omega2);
	out_msg.angular.z = sign(omega3);

	motor_control_pub.publish(out_msg);
}


int main(int argc, char **argv){

	ros::init(argc, argv, "triangle_control");

	ros::NodeHandle n;
	
	motor_control_pub = n.advertise<geometry_msgs::Twist>("motor_control_data", 1000);

	ros::Subscriber imu_sub = n.subscribe("imu", 1000, imu_callback);
	
	imu_pub = n.advertise<geometry_msgs::Quaternion>("orientation", 1000);

	ros::Subscriber sub = n.subscribe("cmd_vel", 1000, cmd_vel_callback);

	ros::spin();

	return 0;

}
