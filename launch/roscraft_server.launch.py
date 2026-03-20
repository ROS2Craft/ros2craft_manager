from launch import LaunchDescription
from launch_ros.actions import Node, ComposableNodeContainer
from launch_ros.descriptions import ComposableNode


def generate_launch_description():

    rosbridge_node = Node(
        package="rosbridge_server",
        executable="rosbridge_websocket",
        name="rosbridge_websocket",
        output="screen",
        parameters=[{"port": 9090}],
    )

    component_manager = Node(
        package="rclcpp_components",
        executable="component_container_mt",
        output="screen",
    )

    return LaunchDescription([rosbridge_node, component_manager])
