
#include <ros.h>
#include <std_msgs/Int64.h>
#include <geometry_msgs/Twist.h>
#include <Servo.h>
//#include <embedded_control/sensor_data.h>

//sensors
#define m_q_9 A1
#define m_q_2 A0
#define pir_pin 53
//sensors

//servo
#define servo_n 2
#define servo_f 3
//servo

//rotating platform
#define rot_dir 8
#define rot_brk 51
#define rot_pwm 7
//rotating platform

//flashlight
#define flash_dir 10
#define flash_pwm 9
#define flash_brk 49
//flashlight

//locomotion
#define Left_DIR 14
#define Right_DIR 16
#define Left_BRK 17
#define Right_BRK 15
#define left_PWM 12
#define right_PWM 11
//locomotion


Servo nozzle_servo;  // create servo object to control a servo
Servo flow_servo;

int angle , curr_angle; //// yeh wala bas check karne ke liye hain
int servo_test;
int servo_test_1;

ros::NodeHandle nh;
//embedded_control::sensor_data pub_msg;

void RotateRight ()
{
  analogWrite (rot_pwm , 255);
  digitalWrite(rot_dir , HIGH);
  digitalWrite(rot_brk , LOW);
}

void RotateLeft ()
{
  
  analogWrite (rot_pwm , 255);
  digitalWrite(rot_dir , LOW);
  digitalWrite(rot_brk , LOW);
}

void RotateStop()
{
   analogWrite (rot_pwm , 0);
   digitalWrite(rot_brk , HIGH);
}
void FlashOn ()
{
//  #define flash_dir 8
//#define flash_pwm 7
//#define flash_brk 51
  digitalWrite(flash_pwm,HIGH);
  digitalWrite(flash_brk,LOW);
  digitalWrite(flash_dir,HIGH);
}

void FlashOff ()
{
  digitalWrite(flash_pwm ,LOW);
  digitalWrite(flash_brk,LOW);
  digitalWrite(flash_dir,LOW);
}

void Forward(int left_pwm,int right_pwm)
{
  digitalWrite(Left_DIR,LOW);
  digitalWrite(Right_DIR,LOW);
  analogWrite(left_PWM,left_pwm);
  analogWrite(right_PWM,right_pwm);
}

void Backward(int left_pwm,int right_pwm)
{
  digitalWrite(Left_DIR,HIGH);
  digitalWrite(Right_DIR,HIGH);
  analogWrite(left_PWM,left_pwm);
  analogWrite(right_PWM,right_pwm);
}

void Right(int left_pwm,int right_pwm)
{
  digitalWrite(Left_DIR,LOW);
  digitalWrite(Right_DIR,HIGH);
  analogWrite(left_PWM,left_pwm);
  analogWrite(right_PWM,right_pwm);
}

void Left(int left_pwm,int right_pwm)
{
  digitalWrite(Left_DIR,HIGH);
  digitalWrite(Right_DIR,LOW);
  analogWrite(left_PWM,left_pwm);
  analogWrite(right_PWM,right_pwm);
}

void halt()
{
  digitalWrite(Left_BRK,HIGH);
  digitalWrite(Right_BRK,HIGH);
}


void messageCb( const geometry_msgs::Twist& vel_msg){

  int left_pwm,right_pwm;
  
  if (vel_msg.linear.x<=0 && vel_msg.linear.y<=0)
  {

  left_pwm = -(vel_msg.linear.x);
  right_pwm = -(vel_msg.linear.y);
  Backward(left_pwm,right_pwm);
  }
  
  if (vel_msg.linear.x>0 && vel_msg.linear.y>0)
  {
  left_pwm = (vel_msg.linear.x);
  right_pwm = (vel_msg.linear.y);
  Forward(left_pwm,right_pwm);
  }

  if (vel_msg.linear.x<0 && vel_msg.linear.y>0)
  {
  left_pwm = -(vel_msg.linear.x);
  right_pwm = (vel_msg.linear.y);
  Left(left_pwm,right_pwm);
  }
  
  if (vel_msg.linear.x>0 && vel_msg.linear.y<0)
  {
    
  left_pwm = (vel_msg.linear.x);
  right_pwm = -(vel_msg.linear.y);
  Right(left_pwm,right_pwm);
  }

  flow_servo.write (vel_msg.linear.z);
  servo_test = vel_msg.linear.z;
  servo_test_1 = vel_msg.angular.x;
  nozzle_servo.write (vel_msg.angular.x);

  
  if  (vel_msg.angular.y == 1)
  RotateRight();
  if  (vel_msg.angular.y == 0)
  RotateStop();
  if (vel_msg.angular.y == -1)
  RotateLeft();

  if (vel_msg.angular.z == -1)
  FlashOn();
  if (vel_msg.angular.z == 0)
  FlashOff();
   
}

//ros::Publisher arduino_pub("feedback_data", &pub_msg);
ros::Subscriber<geometry_msgs::Twist> sub("cmd_vel_joy", &messageCb );


void setup() 
{
  pinMode(Left_DIR,OUTPUT);
  pinMode(Right_DIR,OUTPUT);
  pinMode(Left_BRK,OUTPUT);
  pinMode(Right_BRK,OUTPUT);
  pinMode(left_PWM,OUTPUT);
  pinMode(right_PWM,OUTPUT);
  pinMode(flash_dir,OUTPUT);
  pinMode(flash_pwm,OUTPUT);
  pinMode(rot_pwm,OUTPUT);
  pinMode(flash_brk,OUTPUT);
  pinMode(rot_brk,OUTPUT);
  pinMode(rot_dir,OUTPUT);
  pinMode(servo_n,OUTPUT);
  pinMode(servo_f,OUTPUT);
  pinMode(pir_pin,INPUT);
  pinMode(m_q_2,INPUT);
  pinMode(m_q_9,INPUT);
  digitalWrite(Left_BRK,LOW);
  digitalWrite(Right_BRK,LOW);
  nozzle_servo.attach(servo_n);  // attaches the servo on pin 9 to the servo object 
  flow_servo.attach(servo_f);
  nh.initNode();
  nh.subscribe(sub);
 // nh.advertise(arduino_pub);
  
}




void loop()
{
  //pub_msg.servo_angle = servo_move(angle,curr_angle);
  //pub_msg.servo_angle = servo_test;
  // pub_msg.pressure = pressure ();
  //arduino_pub.publish( &pub_msg );
  nh.spinOnce();
  // delay(20); 

}

