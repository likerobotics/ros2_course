# Lab 6: ROS 2 Hardware Driver
> [!WARNING]
> It is mandatory to use a book provided by teacher instead of README.MD files If you are ITMO University student and studing Operation System ROS. 
>
> README.MD files only for NON ITMO students! 

## Objective

Integrate a physical mobile robot with ROS 2 by implementing a Python driver,
configuring command and feedback communication, and exchanging data with a
microcontroller.

## Submission contract

The `lab6` directory is a plain submission container. It contains the Python
driver package and the Arduino `ros2_control` hardware package:

```text
lab6/
├── my_ros_driver/                   # ament_python package
│   ├── launch/
│   ├── my_ros_driver/
│   ├── resource/
│   ├── package.xml
│   ├── setup.cfg
│   └── setup.py
└── diffdrive_arduino_hardware/      # ament_cmake hardware plugin
```

Requirements:

- Keep the package name `my_ros_driver` unchanged.
- Use the `ament_python` build type.
- Place Python nodes in the `my_ros_driver` module.
- Register submitted executables under `console_scripts` in `setup.py`.
- Install submitted launch files through `setup.py`.
- Do not place package metadata directly in `lab6/`.
- Keep the hardware package name `diffdrive_arduino_hardware` unchanged.

The provided `my_ros_driver` package is intentionally empty and contains no
Python driver or protocol solution.

## Driver requirements

- Receive robot motion or wheel commands through ROS 2 topics.
- Convert commands into the format expected by the microcontroller.
- Send wheel commands using the protocol `SET v_left v_right`.
- Receive encoder measurements using `ENC ticks_left ticks_right`.
- Parse encoder measurements and publish the resulting robot or wheel state.
- Implement explicit QoS settings suitable for command and feedback traffic.
- Handle serial communication errors and malformed input without terminating
  unexpectedly.
- Run against physical time with `use_sim_time` set to `false`.

## Verification

The completed package must build with `colcon`, expose its registered
executables through `ros2 pkg executables my_ros_driver`, and start through the
submitted launch file. During operation, the robot must accept commands and
provide encoder feedback through ROS 2.

If the final architecture also uses `ros2_control`, verify its configured
interfaces and controllers with:

```bash
ros2 control list_hardware_interfaces
ros2 control list_controllers
```
