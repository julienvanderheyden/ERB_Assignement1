#!/usr/bin/env python

import rospy
from geometry_msgs.msg import Twist

def rotate_robot():
    # Initialize the ROS node
    rospy.init_node('constant_angular_velocity_publisher', anonymous=True)

    # Create a publisher for the /cmd_vel topic
    cmd_vel_pub = rospy.Publisher('/cmd_vel', Twist, queue_size=10)

    # Set the rate at which to publish (in Hz)
    rate = rospy.Rate(10)  # 10 Hz

    # Create a Twist message
    twist = Twist()
    
    # Set the desired constant angular velocity (in radians per second)
    twist.angular.z = 0.5  # Adjust this value for the desired speed

    # Publish the angular velocity in a loop
    while not rospy.is_shutdown():
        cmd_vel_pub.publish(twist)
        #rospy.loginfo(f'Publishing angular velocity: {twist.angular.z} rad/s')
        rate.sleep()

if __name__ == '__main__':
    try:
        rotate_robot()
    except rospy.ROSInterruptException:
        pass

