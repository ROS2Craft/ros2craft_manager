#include <rclcpp/rclcpp.hpp>
#include <ros2craft_interfaces/srv/factory_req.hpp>
#include <ros2craft_manager/MCRobot.hpp>

#include <composition_interfaces/srv/load_node.hpp>
#include <composition_interfaces/srv/unload_node.hpp>
#include <rclcpp/executor.hpp>

class ManagerNode : public rclcpp::Node
{
private:
    std::unordered_map<std::string, ros2craft::MCRobotPtr> robots;

    rclcpp::Service<ros2craft_interfaces::srv::FactoryReq>::SharedPtr factory_service;
    rclcpp::Client<composition_interfaces::srv::LoadNode>::SharedPtr load_client;
    rclcpp::Client<composition_interfaces::srv::UnloadNode>::SharedPtr unload_client;

    void fatoryReq(const std::shared_ptr<ros2craft_interfaces::srv::FactoryReq::Request> req, std::shared_ptr<ros2craft_interfaces::srv::FactoryReq::Response> res)
    {
        RCLCPP_INFO(get_logger(), "factory got request");

        switch (req->operation)
        {
            case ros2craft_interfaces::srv::FactoryReq::Request::ADD_ROBOT:
            {
                RCLCPP_INFO(get_logger(), "Adding robot: %s", req->robot_name.c_str());

                auto load_req = std::make_shared<composition_interfaces::srv::LoadNode::Request>();
                load_req->package_name = "ros2craft_manager";
                load_req->plugin_name = "ros2craft::MCRobot";
                load_req->node_name = req->robot_name;
                load_client->async_send_request(load_req);
                // auto robot_node = std::make_shared<ros2craft::MCRobot>(req->robot_name);
                // robots[req->robot_name] = robot_node;
                // executor_.add_node(robot_node);
                std::cout << "robots in factory: " << robots.size() << "\n";
                res->message = "added robot: " + req->robot_name;
                break;
            }

            case ros2craft_interfaces::srv::FactoryReq::Request::DEL_ROBOT:
                RCLCPP_INFO(get_logger(), "Removing robot: %s", req->robot_name.c_str());

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

public:
    ManagerNode() : Node("manager_node")
    {
        factory_service = this->create_service<ros2craft_interfaces::srv::FactoryReq>("/ros2craft/robot_factory",
                                                                                      std::bind(&ManagerNode::fatoryReq, this, std::placeholders::_1, std::placeholders::_2));
        load_client = this->create_client<composition_interfaces::srv::LoadNode>("ComponentManager/_container/load_node");
    }

    ~ManagerNode()
    {
        factory_service.reset();
    }
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);

    auto manager = std::make_shared<ManagerNode>();

    rclcpp::spin(manager);
    rclcpp::shutdown();
    return 0;
}