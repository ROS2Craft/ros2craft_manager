#ifndef MCROBOT_HPP_
#define MCROBOT_HPP_

#include <Eigen/Core>
#include <nav_msgs/msg/odometry.hpp>
#include <rclcpp/rclcpp.hpp>
#include <ros2craft_interfaces/msg/mc_raw_robot_data.hpp>
#include <string>
#include <tf2_ros/transform_broadcaster.hpp>

#include "ros2craft_manager/classForwards.h"

namespace ros2craft
{
    RC_MAN_CLASS_FORWARD(MCRobot)
    class MCRobot : public rclcpp::Node
    {
    private:
        std::string name_;
        ros2craft_interfaces::msg::MCRawRobotData rawData_;
        geometry_msgs::msg::TransformStamped t_;
        std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;
        Eigen::Matrix3f rotMat_;
        geometry_msgs::msg::Vector3 position_;
        geometry_msgs::msg::Vector3 velocities_;

        rclcpp::TimerBase::SharedPtr timer_;
        rclcpp::Subscription<ros2craft_interfaces::msg::MCRawRobotData>::SharedPtr rawSub;

        void convertRawdata();
        void rawSubCb(const ros2craft_interfaces::msg::MCRawRobotData::SharedPtr msg);
        void timerCallback();

    public:
        /*
        Create Robot definition with name
        @param[in] name
        */
        MCRobot(const rclcpp::NodeOptions &options);
        ~MCRobot();
    };
}
#endif // MCROBOT_HPP_
