#include <Eigen/Core>
#include <rclcpp/rclcpp.hpp>
#include <ros2craft_interfaces/msg/mc_raw_robot_data.hpp>
#include <ros2craft_manager/MCRobot.hpp>

Eigen::IOFormat CleanFmt(4, 0, ", ", "\n", "[", "]");

class ManagerNode : public rclcpp::Node
{
private:
    Eigen::Matrix3f rotMat;

    rclcpp::Subscription<ros2craft_interfaces::msg::MCRawRobotData>::SharedPtr mcRwasDataSub_;

    void rawDataCallback(const ros2craft_interfaces::msg::MCRawRobotData::SharedPtr msg)
    {
        RCLCPP_INFO(get_logger(), "yes, got msgs");
        Eigen::Vector3f pos(msg->position.x, msg->position.y, msg->position.z);
        std::cout << pos.format(CleanFmt) << "\n";
        Eigen::Vector3f posRotated = rotMat * pos;
        std::cout << posRotated.format(CleanFmt) << "\n";
    }

public:
    ManagerNode() : Node("manager_node")
    {

        rotMat << 0, 0, 1, 1, 0, 0, 0, 1, 0;
        std::cout << rotMat.format(CleanFmt) << "\n";

        mcRwasDataSub_ =
            create_subscription<ros2craft_interfaces::msg::MCRawRobotData>("/testbot/rawData", 10, std::bind(&ManagerNode::rawDataCallback, this, std::placeholders::_1));

        MCRobot robot("testbot");
    }

    ~ManagerNode()
    {
        mcRwasDataSub_.reset();
    }
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<ManagerNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}