import os

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration, Command
from launch_ros.actions import Node


def generate_launch_description():
    pkg_lab = get_package_share_directory('my_best_model')
    xacro_file = os.path.join(pkg_lab, 'urdf', 'robot_model.xacro')

    robot_state_publisher = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        name='robot_state_publisher',
        output='both',
        parameters=[
            {'robot_description': Command(['xacro ', LaunchConfiguration('urdf_model')])},
        ]
    )

    joint_state_publisher = Node(
        package='joint_state_publisher',
        executable='joint_state_publisher',
        name='joint_state_publisher',
        output='both',
    )
    rviz = Node(
       package='rviz2',
       executable='rviz2',
       arguments=['-d', os.path.join(pkg_lab, 'config', 'rviz.rviz')],
    )

    return LaunchDescription([
        DeclareLaunchArgument(
            'urdf_model',
            default_value=xacro_file,
            description='Full path to the Xacro file'
        ),
        robot_state_publisher,
        joint_state_publisher,
        rviz,
    ])
