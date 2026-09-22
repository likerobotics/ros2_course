import os

from ament_index_python.packages import get_package_share_directory

from launch_ros.actions import Node
from launch import LaunchDescription
from launch.substitutions import Command, PathJoinSubstitution



def generate_launch_description():
    pkg_lab = get_package_share_directory('diffdrive_arduino_hardware')
    xacro_file = os.path.join(pkg_lab, 'urdf', 'mecanum.xacro')
    controllers_file = PathJoinSubstitution(
        [pkg_lab, 'config', 'mecanum_controller.yaml']
    )
    robot_description = Command([
        'xacro ', xacro_file
    ])
    
    robot_state_publisher = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        name='robot_state_publisher',
        output='both',
        parameters=[
            {'use_sim_time': False},
            {'robot_description': robot_description},
        ]
    )

    controller = Node(
        package="controller_manager",
        executable="ros2_control_node",
        parameters=[{'use_sim_time': False}, {'robot_description': robot_description}, controllers_file],        
        output="screen"
    )

    spawn_controller = Node(
        package="controller_manager",
        executable="spawner",
        arguments=["joint_state_broadcaster", "mecanum_base_controller"],
    )

    return LaunchDescription([
        robot_state_publisher,
        controller, 
        spawn_controller,
    ])
