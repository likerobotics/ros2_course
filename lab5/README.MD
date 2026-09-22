# Lab 5: Sensor-Equipped Robot Simulation

## Objective

Create a Gazebo scene and differential-drive robot equipped with a camera and
lidar, then implement observable robot behavior driven by sensor feedback.

## Submission structure

The `lab5` directory is a plain submission container. Submit exactly one ROS 2
package inside it:

```text
lab5/
└── my_best_robot_simulation_setup/    # ament_python package
    ├── config/
    ├── launch/simulation_final.launch.py
    ├── urdf/
    ├── worlds/
    ├── my_best_robot_simulation_setup/
    ├── resource/
    ├── package.xml
    ├── setup.cfg
    └── setup.py
```

Do not place package metadata directly in `lab5/`. The submitted
`my_best_robot_simulation_setup` package must use the `ament_python` build type.

## Scene requirements

- Provide an SDF world containing gravity, lighting, and a ground plane.
- Add at least three recognizable real-world objects of different forms, such
  as a traffic cone, tree, and table. Three primitive boxes, spheres, or
  cylinders do not satisfy this requirement.

## Robot and sensor requirements

- Use a differential-drive simulation plugin.
- Apply the base dimensions, lidar minimum/maximum range, and camera resolution
  assigned in Table 14.1 using the last digit of the student's ISU ID.
- Add both lidar and camera links/sensors to the robot model.
- Bridge or publish the required command and sensor topics to ROS 2.
- Configure RViz2 so camera and lidar output can be verified.

## Control requirements

- Implement a node that consumes sensor data and publishes commands to
  `/cmd_vel`.
- The control objective is selected by the student, but its purpose and result
  must be clear to an external observer.
- Do not reuse the prohibited door-finding example from the assignment.

## Required launch command

```bash
ros2 launch my_best_robot_simulation_setup simulation_final.launch.py
```

All runtime resources must be installed by the package. The backbone must not
provide a completed world, sensor-processing algorithm, or autonomous behavior.
