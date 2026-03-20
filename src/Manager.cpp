#include <ros2craft_manager/Manager.hpp>

namespace ros2craft
{
    Manager::Manager(const rclcpp::NodeOptions &options, std::shared_ptr<rclcpp_components::ComponentManager> cm) : Node("manager_node", options), component_manager_(cm)
    {
        factory_service = this->create_service<ros2craft_interfaces::srv::FactoryReq>("/ros2craft/robot_factory",
                                                                                      std::bind(&Manager::factory_req, this, std::placeholders::_1, std::placeholders::_2));

        cm_resources_ = component_manager_->get_component_resources("ros2craft_manager")[0];
    }

    Manager::~Manager()
    {
    }

    void Manager::factory_req(const std::shared_ptr<ros2craft_interfaces::srv::FactoryReq::Request> req, std::shared_ptr<ros2craft_interfaces::srv::FactoryReq::Response> res)
    {
        RCLCPP_INFO(get_logger(), "factory got request");

        switch (req->operation)
        {
            case ros2craft_interfaces::srv::FactoryReq::Request::ADD_ROBOT:
            {
                RCLCPP_INFO(get_logger(), "Adding robot: %s", req->robot_name.c_str());
                // auto cmr = rclcpp_components::ComponentManager::ComponentResource();
                // std::pair<std::string, std::string> resource("", "");
                component_manager_->create_component_factory(cm_resources_);

                // auto robot_node = std::make_shared<ros2craft::MCRobot>(req->robot_name);
                // robots[req->robot_name] = robot_node;
                // executor_.add_node(robot_node);
                std::cout << "robots in factory: " << robots.size() << "\n";
                res->message = "added robot: " + req->robot_name;
                break;
            }

            case ros2craft_interfaces::srv::FactoryReq::Request::DEL_ROBOT:
                RCLCPP_INFO(get_logger(), "Removing robot: %s", req->robot_name.c_str());
                // executor_.remove_node(robots[req->robot_name]);
                robots.erase(req->robot_name);
                std::cout << "robots in factory: " << robots.size() << "\n";
                res->message = "removed robot: " + req->robot_name;
                break;

            default:
                RCLCPP_INFO(get_logger(), "Unknow operation: ");
                break;
        }

        res->success = true;
    }
}
