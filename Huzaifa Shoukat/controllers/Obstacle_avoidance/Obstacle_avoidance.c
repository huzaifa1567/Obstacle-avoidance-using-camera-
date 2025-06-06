#include <webots/motor.h>
#include <webots/robot.h>
#include <webots/camera.h>
#include <webots/camera_recognition_object.h>
#include <stdio.h>
#include <string.h>



// time in [ms] of a simulation step
#define TIME_STEP 64

// entree point of the controller
int main(int argc, char **argv) {
  // initialise the Webots API
  wb_robot_init();
  WbDeviceTag camera;

  
    /* Get the camera device, enable it and the recognition */
  camera = wb_robot_get_device("camera");
  wb_camera_enable(camera, TIME_STEP);
  wb_camera_recognition_enable(camera, TIME_STEP);

  // internal variables
  int i=0;
  double left_speed = 0.0; 
  double right_speed = 0.0;

  // initialise motors
  WbDeviceTag wheels[4];
  char wheels_names[4][8] = {"wheel1", "wheel2", "wheel3", "wheel4"};
  for (i = 0; i < 4; i++) {
    wheels[i] = wb_robot_get_device(wheels_names[i]);
    wb_motor_set_position(wheels[i], INFINITY);
  }

  // feedback loop
  while (wb_robot_step(TIME_STEP) != -1) {
    // init speeds
    int num = wb_camera_recognition_get_number_of_objects(camera);
    const WbCameraRecognitionObject *objects = wb_camera_recognition_get_objects(camera);
  if (num==0)
  {
    left_speed = 6;
    right_speed = 6;
  }
  else
  {
    if (objects[0].colors[0]< 0.8 && objects[0].colors[1]< 0.8 )
    {
      if (objects[0].position_on_image[0] > 150)
      {
       left_speed = -10;
       right_speed = 10;
       printf("Obstacle detected with camera \n");
      }
      else if (objects[0].position_on_image[0] < 100)
      {
       left_speed = 10;
       right_speed = -10;
       printf("Obstacle detected with camera \n");
      }
      else
      {
       left_speed = 6;
       right_speed = 6;
      }  
   }
 }

    // write actuators inputs
    wb_motor_set_velocity(wheels[0], left_speed);
    wb_motor_set_velocity(wheels[1], right_speed);
    wb_motor_set_velocity(wheels[2], left_speed);
    wb_motor_set_velocity(wheels[3], right_speed);
  }

  // cleanup the Webots API
  wb_robot_cleanup();
  return 0;  // EXIT_SUCCESS
}
