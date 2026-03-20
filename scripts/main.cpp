#include <rclcpp/rclcpp.hpp>
#include <rclcpp_components/component_manager.hpp>
#include <ros2craft_manager/Manager.hpp>

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    auto exec = std::make_shared<rclcpp::executors::MultiThreadedExecutor>();
    auto component_manager = std::make_shared<rclcpp_components::ComponentManager>(exec);
    exec->add_node(component_manager);
    auto manager = std::make_shared<ros2craft::Manager>(rclcpp::NodeOptions(), component_manager);
    exec->add_node(manager);
    exec->spin();
    rclcpp::shutdown();
    return 0;
}