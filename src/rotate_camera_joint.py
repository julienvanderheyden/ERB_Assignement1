#!/usr/bin/env python
import rospy
from std_msgs.msg import Float64

def rotate_camera_joint():
    rospy.init_node('rotate_camera_joint', anonymous=True)
    
    # Publisher to send velocity command messages
    camera_joint_velocity_pub = rospy.Publisher('/robot/camera_joint_velocity_controller/command', Float64, queue_size=10)
    
    rate = rospy.Rate(10)  # Set a rate of 10 Hz
    velocity = 0.5  # Define the rotational velocity (radians per second)

    while not rospy.is_shutdown():
        # Create a velocity command message
        camera_joint_velocity_command = Float64()
        
        # Set the velocity command
        camera_joint_velocity_command.data = velocity
        
        # Publish the velocity command
        camera_joint_velocity_pub.publish(camera_joint_velocity_command)
        
        rate.sleep()

if __name__ == '__main__':
    try:
        rotate_camera_joint()
    except rospy.ROSInterruptException:
        pass

