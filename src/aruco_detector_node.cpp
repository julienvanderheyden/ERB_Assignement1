#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>
#include <aruco_ros/aruco_ros_utils.h>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <iostream>

class ArucoMarkerDetector {
public:
    ArucoMarkerDetector() : current_id_index_(0), all_markers_detected_(false) {
        // Initialize the subscriber to the image topic
        image_sub_ = nh_.subscribe("/robot/camera1/image_raw", 1, &ArucoMarkerDetector::imageCallback, this);

        // Initialize the publisher for annotated images
        image_transport::ImageTransport it(nh_);
        image_pub_ = it.advertise("/robot/annotated_markers", 1);
    }

private:
    ros::NodeHandle nh_;
    ros::Subscriber image_sub_;
    image_transport::Publisher image_pub_;

    // Marker detection utility
    aruco::MarkerDetector marker_detector_;

    // Phase control variables
    std::unordered_set<int> detected_ids_;  // To store unique IDs seen in the first phase
    std::vector<int> sorted_ids_;           // Sorted list of IDs for publishing phase
    int current_id_index_;                  // Index for the current ID to publish
    bool all_markers_detected_;             // Flag to switch from detection to publishing phase
    int first_id_;

    void imageCallback(const sensor_msgs::ImageConstPtr& msg) {
        // Convert the ROS image message to an OpenCV image
        cv_bridge::CvImagePtr cv_ptr;
        try {
            cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
        } catch (cv_bridge::Exception& e) {
            ROS_ERROR("cv_bridge exception: %s", e.what());
            return;
        }

        // Detect markers in the image
        std::vector<aruco::Marker> markers = marker_detector_.detect(cv_ptr->image);
        
        
        if (!all_markers_detected_) {
            // Detection Phase: Collect unique marker IDs
            if(detected_ids_.empty() && !markers.empty()){
            	if(markers[0].id > 9 && markers[0].id < 100){
            		first_id_ = markers[0].id;
            	}
            	
            }
            
            for (const auto& marker : markers) {
            	if(marker.id > 9 && marker.id < 100){
                	detected_ids_.insert(marker.id);
                }
            }

            // Check if all markers have been detected by re-seeing the first detected ID
            if (!markers.empty() && detected_ids_ .size() > 2 && markers[0].id == first_id_) {
                // Sort the unique IDs to prepare for the publishing phase
                sorted_ids_ = std::vector<int>(detected_ids_.begin(), detected_ids_.end());
                std::sort(sorted_ids_.begin(), sorted_ids_.end());

                // Set the flag to move to the publishing phase
                all_markers_detected_ = true;
                current_id_index_ = 0;
                ROS_INFO("All markers detected. Switching to publishing phase. Lowest ID found  : %d. Highest ID found : %d", sorted_ids_[0], sorted_ids_.back());
            }
        } else {
            // Publishing Phase: Publish markers in order
            if (current_id_index_ < sorted_ids_.size()) {
                int target_id = sorted_ids_[current_id_index_];

                // Find the marker with the target ID
                auto marker_it = std::find_if(markers.begin(), markers.end(), [target_id](const aruco::Marker& m) {
                    return m.id == target_id;
                });

                if (marker_it != markers.end()) {
                    // Draw a circle around the detected marker
                    ROS_INFO("Publishing image related to marker %d", target_id);
                    cv::Point center(marker_it->getCenter().x, marker_it->getCenter().y);
                    cv::circle(cv_ptr->image, center, 100, cv::Scalar(0, 0, 255), 3);

                    // Publish the annotated image
                    image_pub_.publish(cv_ptr->toImageMsg());

                    // Move to the next ID for the next frame
                    current_id_index_++;
                    if(current_id_index_ >= sorted_ids_.size()){
                    	ROS_INFO("All detected markers have been published. Task is done");
                    }
                }
            }
        }
    }
};

int main(int argc, char** argv) {
    // Initialize the ROS node
    ros::init(argc, argv, "aruco_marker_detector_node");

    // Create the detector object
    ArucoMarkerDetector detector;

    // Spin to keep the node active
    ros::spin();
    return 0;
}

