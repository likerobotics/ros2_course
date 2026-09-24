# ROS 2 Course

> [!NOTE]
> README for russian stidents available [here](https://github.com/likerobotics/mobile_robot_ros_driver/blob/main/README_RU.md).


> [!WARNING]
> It is mandatory to use a book provided by teacher instead of README.MD files If you are ITMO University student and studing Operation System ROS. 
>
> README.MD files only for NON ITMO University students! 


Course materials, starter packages, practical tasks, and structured LAB
submissions for learning robot software development with ROS 2.

The course materials based on **ROS 2 Jazzy**, **Ubuntu 24.04**, and **Gazebo Harmonic**.
It progresses from ROS communication fundamentals to robot modeling,
simulation, feedback control, sensors data processing, and physical hardware integration.

## Course topics

- Ubuntu, ROS 2 installation, workspaces, packages, and `colcon`
- Nodes, topics, messages, parameters, namespaces, and launch files
- Quality of Service, ROS time, logging, and command-line diagnostics
- Custom messages, services, and actions
- TF2, URDF, Xacro, `robot_state_publisher`, and RViz2
- Gazebo simulation and `ros_gz_bridge`
- Differential-drive kinematics and `ros2_control`
- Camera and lidar simulation and sensor-data processing
- Arduino communication, encoder feedback, and hardware drivers

## LABS

1. **ROS 2 package and controller** — launch two turtlesim systems in separate
   namespaces, guide the first turtle through assigned points, and make the
   second turtle follow its pose.
2. **Custom services and actions** — create a student-named interface package
   and implement service/action clients and servers in `lab2_controller`.
3. **Robot model** — describe a variant-specific mobile robot and manipulator
   in Xacro and visualize its model and TF tree in RViz2.
4. **Simulation and control** — simulate a differential-drive robot, control
   its joints through `ros2_control`, and convert `/cmd_vel` into wheel speeds.
5. **Sensors and autonomous behavior** — add a camera and lidar, construct a
   populated simulation world, and implement behavior using sensor feedback.
6. **Hardware integration** — implement a Python ROS driver and integrate the
   Arduino hardware plugin, command transport, and encoder feedback.

Variant-specific dimensions, target points, sensor parameters, and assessment
requirements are defined in the course book. A student's variant depended
on the last digit of their ITMO ISU ID. If you are non ITMO student, you can use variant 0.

## Repository structure

```text
ros2_course/
├── lab1/
│   └── mybestcontroller/                  # ament_python
├── lab2/
│   ├── lab2_controller/                   # provided ament_python package
│   └── <student_interface_package>/       # student-created ament_cmake package
├── lab3/
│   └── my_best_model/                     # ament_python
├── lab4/
│   └── my_best_robot_simulation_control/  # ament_python
├── lab5/
│   └── my_best_robot_simulation_setup/    # ament_python
├── lab6/
│   ├── my_ros_driver/                     # ament_python
│   └── diffdrive_arduino_hardware/        # ament_cmake
├── tasks/                                 # independent practical assignments
├── docker_ros2/                           # course container environment
├── install_ros2_jazzy.html                # English installation guide
├── install_ros2_jazzy_ru.html             # Russian installation guide
└── ros2_cheatsheet.html                   # ROS 2 command reference
```

Each `labN` directory is a plain submission container rather than a ROS
package. Package metadata must remain inside the package directories shown
above. The required names and paths form part of the automated-checking
contract.

## Package inventory

| Lab | Package | Build type | Purpose |
| --- | --- | --- | --- |
| 1 | `mybestcontroller` | `ament_python` | Namespaced turtlesim controller |
| 2 | `lab2_controller` | `ament_python` | Service/action client and server nodes |
| 2 | Student-selected name | `ament_cmake` | Custom `.srv` and `.action` interfaces |
| 3 | `my_best_model` | `ament_python` | Xacro model and RViz2 launch resources |
| 4 | `my_best_robot_simulation_control` | `ament_python` | Gazebo and differential-drive control |
| 5 | `my_best_robot_simulation_setup` | `ament_python` | Sensor-equipped Gazebo simulation |
| 6 | `my_ros_driver` | `ament_python` | ROS-to-microcontroller driver |
| 6 | `diffdrive_arduino_hardware` | `ament_cmake` | `ros2_control` hardware plugin |

Detailed requirements are available in each laboratory's `README.MD` file.
Starter packages define structure and resource locations; students remain
responsible for their assigned interfaces, variants, nodes, algorithms, and
hardware behavior.

## Installation

Use one of the standalone guides:

- [Install ROS 2 Jazzy — English](install_ros2_jazzy.html)
- [Установка ROS 2 Jazzy — русский](https://likerobotics.ru/courses/ros2/ustanovka-ros-2-jazzy/)
- [Docker environment](docker_ros2/README.md)

The native target is Ubuntu 24.04 with ROS 2 Jazzy and Gazebo Harmonic. The
Docker environment provides the same course dependencies and mounts this
repository at `/workspace`.

## Building packages

From the repository root:

```bash
source /opt/ros/jazzy/setup.bash
rosdep update
rosdep install --from-paths . --ignore-src --rosdistro jazzy -y
colcon build --symlink-install
source install/setup.bash
```

Build one package and its dependencies when working on an individual lab:

```bash
colcon build --symlink-install --packages-up-to my_best_model
```

List the packages detected in the repository:

```bash
colcon list --base-paths lab1 lab2 lab3 lab4 lab5 lab6
```

## Running laboratory packages

Examples using the fixed launch contracts:

```bash
ros2 launch mybestcontroller lab1.launch.py
ros2 launch my_best_model rviz.launch.py
ros2 launch my_best_robot_simulation_control simulation_control.launch.py
ros2 launch my_best_robot_simulation_setup simulation_final.launch.py
```

Lab 2 and Lab 6 launch commands depend on the executables and launch files
completed by the student. Consult the corresponding lab README or course book.

## Practical tasks and reference material

The [`tasks/`](tasks/) directory contains one Markdown file for each independent
assignment block from the course book. These are practical exercises and are
separate from the structured laboratory submissions.

The standalone [ROS 2 command cheat sheet](ros2_cheatsheet.html) covers package
creation, graph inspection, topics, services, actions, parameters, launch,
rosbag2, TF2, Xacro, `ros2_control`, Gazebo, and diagnostics.

## Submission rules

- Submit source files only under the corresponding `labN` directory.
- Preserve all required directory, package, interface, and launch filenames.
- Keep ROS package metadata inside its package directory.
- Do not commit `build/`, `install/`, `log/`, caches, or generated Python files.
- Declare dependencies in `package.xml` and package build metadata.
- Confirm that packages build from a clean workspace.
- Test required launch commands and ROS interfaces before submission.
- Submit original work; starter content is not a completed laboratory solution.
