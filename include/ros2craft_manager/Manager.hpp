#include <rclcpp/rclcpp.hpp>
#include <ros2craft_interfaces/srv/factory_req.hpp>
#include <ros2craft_manager/MCRobot.hpp>

#include <rclcpp_components/component_manager.hpp>

namespace ros2craft
{
    class Manager : public rclcpp::Node
    {
    private:
        rclcpp_components::ComponentManager::ComponentResource cm_resources_;
        std::shared_ptr<rclcpp_components::ComponentManager> component_manager_;
        std::unordered_map<std::string, ros2craft::MCRobotPtr> robots;

        rclcpp::Service<ros2craft_interfaces::srv::FactoryReq>::SharedPtr factory_service;

        void factory_req(const std::shared_ptr<ros2craft_interfaces::srv::FactoryReq::Request> req, std::shared_ptr<ros2craft_interfaces::srv::FactoryReq::Response> res);

    public:
        Manager(const rclcpp::NodeOptions &options, std::shared_ptr<rclcpp_components::ComponentManager> cm);
        ~Manager();
    };
}