# Lab 4: Simulation and Differential-Drive Control
> [!WARNING]
> It is mandatory to use a book provided by teacher instead of README.MD files If you are ITMO University student and studing Operation System ROS. 
>
> README.MD files only for NON ITMO students! 

## Objective

Simulate a variant-specific differential-drive robot in Gazebo, control its
wheel joints through `ros2_control`, and implement the mobile-base kinematic
conversion from velocity commands to wheel velocities.

## Submission structure

The `lab4` directory is a plain submission container. Submit exactly one ROS 2
package inside it:

```text
lab4/
└── my_best_robot_simulation_control/    # ament_python package
    ├── config/
    ├── launch/simulation_control.launch.py
    ├── urdf/
    ├── worlds/
    ├── my_best_robot_simulation_control/
    ├── resource/
    ├── package.xml
    ├── setup.cfg
    └── setup.py
```

Do not place package metadata directly in `lab4/`. The submitted
`my_best_robot_simulation_control` package must use the `ament_python` build
type.

## Functional requirements

- Use the wheel radius and base dimensions assigned by the last digit of the
  student's ITMO ISU ID in Table 13.1 of the book.
- Provide a valid differential-drive robot description and Gazebo simulation.
- Define the wheel command/state interfaces required by `ros2_control`.
- Configure and start the controller manager, joint-state broadcaster, and
  wheel velocity controller.
- Implement a ROS 2 node that subscribes to `/cmd_vel`.
- Convert commanded linear and angular base velocity into left/right wheel
  angular velocity using the submitted robot's geometry.
- Publish wheel commands to `/velocity_controller/commands` in the controller's
  declared joint order.
- Install all launch, model, world, and configuration resources used at
  runtime.

## Required launch command

```bash
ros2 launch my_best_robot_simulation_control simulation_control.launch.py
```

Parameters not fixed by the assignment may be selected by the student. The
backbone must not include the kinematic conversion or variant-specific model as
a completed solution.
