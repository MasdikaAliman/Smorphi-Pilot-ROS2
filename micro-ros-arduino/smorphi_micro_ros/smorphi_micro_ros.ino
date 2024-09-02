#include <Arduino.h>
#include <micro_ros_arduino.h>

#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <std_msgs/msg/int32.h>
#include <geometry_msgs/msg/point.h>
#include <smorphi.h>


rcl_node_t node;
rclc_support_t support;
rcl_allocator_t allocator;
;
// subscriber
rcl_subscription_t shape_sub;
std_msgs__msg__Int32 shape_msg;
rclc_executor_t executor_sub;

rcl_subscription_t cmd_vel_sub;
geometry_msgs__msg__Point cmd_vel_msg;


#define LED_PIN 2

Smorphi my_robot;

#define RCCHECK(fn) \
  { \
    rcl_ret_t temp_rc = fn; \
    if ((temp_rc != RCL_RET_OK)) { \
      error_loop(); \
    } \
  }
#define RCSOFTCHECK(fn) \
  { \
    rcl_ret_t temp_rc = fn; \
    if ((temp_rc != RCL_RET_OK)) { \
    } \
  }

/**
 * @brief loop to indicate error with blinking LED
 *
 */
void error_loop() {
  while (1) {
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    delay(100);
  }
}

void shapeCB(const void *msgin) {
  std_msgs__msg__Int32 *msg = (std_msgs__msg__Int32 *)msgin;
  if (msg->data == 0) {
    //Shape I
    my_robot.I();
    msg->data = 10;

  } else if (msg->data == 1) {
    //SHAPE J
    my_robot.J();
    msg->data = 10;

  } else if (msg->data == 2) {
    //SHAPE L
    my_robot.L();
    msg->data = 10;

  } else if (msg->data == 3) {
    //SHAPE O
    my_robot.O();
    msg->data = 10;

  } else if (msg->data == 4) {
    //SHAPE S
    my_robot.S();
    msg->data = 10;

  } else if (msg->data == 5) {
    //SHAPE T
    my_robot.T();
    msg->data = 10;

  } else if (msg->data == 5) {
    //SHAPE Z
    my_robot.Z();
    msg->data = 10;
  }

  my_robot.sm_reset_M1();
  my_robot.sm_reset_M2();
  my_robot.sm_reset_M3();
  my_robot.sm_reset_M4();
}

void cmd_vel_CB(const void *msgin) {
  const geometry_msgs__msg__Point *msg = (const geometry_msgs__msg__Point *)msgin;
  my_robot.sm_velocity_handler(msg->y, -msg->x, msg->z);
  my_robot.sm_pwm_handler(2);
  my_robot.MotorDirection();
  // my_robot.sm_reset_M1();
  // my_robot.sm_reset_M2();
  // my_robot.sm_reset_M3();
  // my_robot.sm_reset_M4();
}

void setup() {
  Serial.begin(921600);
  set_microros_transports();
  my_robot.BeginSmorphi();
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  delay(2000);

  allocator = rcl_get_default_allocator();

  // create init_options
  RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));

  // create node
  RCCHECK(rclc_node_init_default(&node, "smorphi_esp32_node", "", &support));

  // create subscriber
  RCCHECK(rclc_subscription_init_default(
    &shape_sub,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
    "/shape_cmd"));

  RCCHECK(rclc_subscription_init_default(
    &cmd_vel_sub,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Point),
    "/velocity_cmd"));


  RCCHECK(rclc_executor_init(&executor_sub, &support.context, 2, &allocator));
  RCCHECK(rclc_executor_add_subscription(&executor_sub, &shape_sub, &shape_msg, &shapeCB, ON_NEW_DATA));
  RCCHECK(rclc_executor_add_subscription(&executor_sub, &cmd_vel_sub, &cmd_vel_msg, &cmd_vel_CB, ON_NEW_DATA));
}

void loop() {
  RCCHECK(rclc_executor_spin_some(&executor_sub, RCL_MS_TO_NS(10)));
}