from launch import LaunchDescription
from launch_ros.actions import Node, ComposableNodeContainer
from launch_ros.descriptions import ComposableNode


def generate_launch_description():

    laser2pcd = Node(
        package="pointcloud_to_laserscan",
        executable="laserscan_to_pointcloud_node",
        remappings=[("/scan_in", "/robot70/lidar"), ("/cloud", "/cloud_in")],
    )

    octomap_server_node = Node(
        package="octomap_server",
        executable="octomap_server_node",
        name="octomap_server",
        output="screen",
        parameters=[
            {
                "frame_id": "world_origin",
                "base_frame_id": "robot70",
                "cloud_in": "/cloud_in",
            }
        ],
    )

    return LaunchDescription([laser2pcd, octomap_server_node])
