import os

from ament_index_python.packages import get_package_share_directory

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution, Command

from launch_ros.actions import Node


def generate_launch_description():
    pkg_ros_gz_sim = get_package_share_directory('ros_gz_sim')
    pkg_lab = get_package_share_directory('my_best_robot_simulation_setup')

    xacro_file = os.path.join(pkg_lab, 'urdf', 'robot_model.xacro')

    
    gz_sim = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(pkg_ros_gz_sim, 'launch', 'gz_sim.launch.py')),
        launch_arguments={'gz_args': PathJoinSubstitution([
            pkg_lab,
            'worlds',
            'empty.world'
        ])}.items(),
    )

    robot_state_publisher = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        name='robot_state_publisher',
        output='both',
        parameters=[
            {'use_sim_time': True},
            {'robot_description': Command(['xacro ', LaunchConfiguration('urdf_model')])},
        ]
    )

    rviz = Node(
       package='rviz2',
       executable='rviz2',
       parameters=[{'use_sim_time': True}],
       arguments=['-d', os.path.join(pkg_lab, 'config', 'rviz.rviz')],
    )

    spawn = Node(
        package='ros_gz_sim', 
        executable='create', 
        arguments=[ '-name', 'diff_drive', '-topic', 'robot_description', '-x', '1', '-y', '1'], 
        output='screen'
    )

    bridge = Node(
        package='ros_gz_bridge',
        executable='parameter_bridge',
        parameters=[{
            'config_file': os.path.join(pkg_lab, 'config', 'ros_gz_bridge.yaml'),
            'qos_overrides./tf_static.publisher.durability': 'transient_local',
        }],
        output='screen'
    )
    spawn_controller = Node(
        package="controller_manager",
        executable="spawner",
        name=f"spawn_controller_left_wheel_controller",
        arguments=["velocity_controller"],
        output="screen"
    )


    return LaunchDescription([
        gz_sim,
        DeclareLaunchArgument(
            'urdf_model',
            default_value=xacro_file,
            description='Full path to the Xacro file'
        ),
        bridge,
        spawn,
        robot_state_publisher,
        rviz,
        spawn_controller,
    ])
