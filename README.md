# ROS2CRAFT manager package

Main ros2craft ros2 package to interface with MCrobots. Main class is [MCRobot](https://github.com/ROS2Craft/ros2craft_manager/blob/main/src/MCRobot.cpp), loaded and unloaded through ros2 component system as a dynamic node by calling the respective services.
For now just he [launch file](https://github.com/ROS2Craft/ros2craft_manager/blob/main/launch/roscraft_server.launch.py) is needed, and the robots will be addded at command from MC.

20/04/2026: most of the script here are testing different functionalities.