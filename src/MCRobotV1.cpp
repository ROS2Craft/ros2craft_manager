#include "ros2craft_manager/MCRobot.hpp"
#include <iostream>

MCRobot::MCRobot(std::string name) : Node(name + "_node")
{
    this->name_ = name;
    t_.child_frame_id = name_;

    tf_broadcaster_ = std::make_unique<tf2_ros::TransformBroadcaster>(*this);

    this->rawSub = create_subscription<ros2craft_interfaces::msg::MCRawRobotData>("/" + name_ + "/rawData", 1, std::bind(&MCRobot::rawSubCb, this, std::placeholders::_1));

    timer_ = this->create_wall_timer(std::chrono::milliseconds(100), std::bind(&MCRobot::timerCallback, this));

    rotMat_ << 0, 0, 1, 1, 0, 0, 0, 1, 0;
    // std::cout << rotMat.format(CleanFmt) << "\n";

    std::cout << "Robot " << name_ << " created \n";
}

MCRobot::~MCRobot()
{
    this->rawSub.reset();
    std::cout << "Robot " << name_ << " destroyed \n";
}

void MCRobot::rawSubCb(const ros2craft_interfaces::msg::MCRawRobotData::SharedPtr msg)
{
    rawData_ = *msg;
}

void MCRobot::timerCallback()
{
    convertRawdata();

    t_.header.frame_id = rawData_.frame_id;
    t_.header.stamp = this->get_clock()->now();
    t_.transform.translation = this->position_;
    tf_broadcaster_->sendTransform(t_);
}

void MCRobot::convertRawdata()
{
    Eigen::Vector3f rawPos(rawData_.position.x, rawData_.position.y, rawData_.position.z);
    Eigen::Vector3f posVec = rotMat_ * rawPos;
    this->position_.x = posVec.x();
    this->position_.y = posVec.y();
    this->position_.z = posVec.z();
    Eigen::Vector3f rawVel(rawData_.velocity.x, rawData_.velocity.y, rawData_.velocity.z);
    Eigen::Vector3f velVec = rotMat_ * rawVel;

    this->velocities_.x = velVec.x();
    this->velocities_.y = velVec.y();
    this->velocities_.z = velVec.z();
}