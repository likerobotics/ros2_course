# Lab 2: Custom Services and Actions

## Objective

Define custom ROS 2 interfaces and implement separate client and server nodes
for turtle control.

## Submission contract

The submitted `lab2` directory must contain exactly two ROS 2 packages:

```text
lab2/
├── lab2_controller/       # provided empty ament_python package
└── <interface_package>/   # student-created ament_cmake package
```

Requirements:

- Keep the provided controller package name `lab2_controller` unchanged.
- Implement all Python clients, servers, and launch files in
  `lab2_controller`.
- Create one additional interface package using `ament_cmake`.
- The interface-package name is selected by the student.
- Store all custom `.srv` and `.action` definitions only in the interface
  package.
- Declare the interface package as a dependency of `lab2_controller` after it
  is created.
- Do not place `.srv` or `.action` files in `lab2_controller`.
- Do not place `package.xml`, `setup.py`, or `CMakeLists.txt` directly in the
  `lab2` directory.

The provided `lab2_controller` package intentionally contains no nodes,
interfaces, launch files, or completed solution logic.

## Part 1: Service

- Create `AISUID.srv` in the student-created interface package, replacing
  `ISUID` with the student's six-digit ITMO ISU ID.
- The request must contain current coordinates `Xcurrent` and `Ycurrent`.
- The response must contain target distance `Distance` and angular error
  `Angle`. An additional completion flag may be used.
- Implement the service server and client in `lab2_controller`.
- The server owns the target-point list and calculates the errors. The client
  must not know the target coordinates.
- The client obtains the turtle pose, requests errors while the turtle is
  stationary, and performs closed-loop motion.
- The server advances to the next assigned target when the current target is
  reached, using the tolerances specified in the course book.
- Provide `service_server.launch.py` and `service_client.launch.py`. The server
  launch file must also start turtlesim.
- Verify the server using both `ros2 service call` and the custom client.

## Part 2: Action (optional advanced task)

- Create `AISUID.action` in the same interface package.
- The goal contains a target position, feedback reports distance error, and the
  result reports successful completion.
- Implement the action server and client in `lab2_controller`.
- The server controls the turtle. The client sends a goal, displays feedback,
  and reports completion.
- Provide `action_server.launch.py` and `action_client.launch.py`.
- Verify the server with `ros2 action send_goal` and the custom client.

Package names, interface filenames, and required launch filenames are part of
the automated-checking contract.
