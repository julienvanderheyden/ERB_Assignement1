# Experimental Robotics Laboratory - Assignment 1

This is a ROS package implementing the first assignment of the course **"Experimental Robotics Laboratory"**. The task involves simulating a mobile robot equipped with a camera in an environment containing 5 ArUco markers. The goal is to make the robot identify the marker with the smallest ID and process the markers in ascending order of their IDs.

---

## **Features**

1. **Robot Description:**
   - The robot is modeled using `robot4.xacro`, located in the `urdf` folder.
   - The robot has a camera and is capable of:
     - Rotating on its axis (differential drive).
     - Rotating its camera independently.

2. **Environment Description:**
   - The simulation environment is implemented in `aruco_test.world` (located in the `worlds` folder).
   - It contains 5 ArUco markers arranged in a circular pattern.

3. **Task Objectives:**
   - Locate the ArUco marker with the smallest ID.
   - Annotate each ArUco marker with a circle.
   - Publish the annotated images on the `/annotated_markers` topic  in ascending order of their IDs.

---

## **How to Run the Package**

### **Step 1: Launch the Simulation**

Start the ROS launch file to initialize the environment and robot. You can choose between two modes:

1. **Robot Turns with Differential Drive**:
   ```bash
   roslaunch assignment1_erb assignment1_erb.launch camera_joint:=false
   ```
In this mode, the robot rotates on its axis to identify the markers.

2. **Camera Rotated Independtly**:
  ```bash
  roslaunch assignment1_erb assignment1_erb.launch camera_joint:=true
  ```
In this mode, the camera rotates on its axis while the robot remains stationary.

### **Step 2: Start the Gazebo Simulation**
Once the launch file is running, start the simulation in Gazebo to enable the robot to operate in the environment.


---

## **Dependencies**

Ensure the following dependencies are installed to run the package:

### **Core ROS Dependencies**
- `roscpp`: Core library for C++ nodes.
- `std_msgs`: Standard message types for communication in ROS.
- `sensor_msgs`: For working with sensor data, such as camera images.

### **Robot Model Dependencies**
- `urdf`: Parses and handles the robot model in URDF format.
- `xacro`: Processes `.xacro` files used for robot description.

### **Computer Vision Dependencies**
- `cv_bridge`: Converts between ROS image messages and OpenCV image formats.
- `image_transport`: Publishes and subscribes to image topics efficiently.

### **ArUco Marker Detection**
- `aruco_ros`: ROS package for detecting and processing ArUco markers.

### **Build Tool**
- `catkin`: The build tool for compiling the ROS package (usually pre-installed).

---

## **Topics**

- `/annotated_markers`: Publishes images of the ArUco markers with annotations (circle around the marker).
