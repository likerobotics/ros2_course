# Laboratory Work 1: ROS 2 Package and Controller

> [!WARNING]
> It is mandatory to use a book provided by teacher instead of README.MD files If you are ITMO University student and studing Operation System ROS. 
>
> README.MD files only for NON ITMO students! 

## Objective

Create an `ament_python` ROS 2 package, launch multiple nodes in isolated
namespaces, and implement an encapsulated feedback controller for turtlesim.

## Submission structure

Submit exactly one package in this directory:

```text
lab1/
└── mybestcontroller/
    ├── launch/lab1.launch.py
    ├── mybestcontroller/lab1_controller.py
    ├── package.xml
    ├── setup.cfg
    └── setup.py
```

Do not rename the package or the two required Python files.

## Functional requirements

- Define namespaces `ns1_ISUID` and `ns2_ISUID`, replacing `ISUID` with the
  student's ITMO ISU ID.
- Launch one turtlesim instance and one instance of the same controller in each
  namespace.
- The controller must determine its behavior from its current namespace.
- Turtle 1 must visit the points assigned by the last digit of the student's
  ISU ID. Point order is unrestricted.
- Measure the elapsed time from the start until Turtle 1 reaches its final
  point, then publish it on a topic named `result_ISUID`.
- Turtle 2 must follow Turtle 1 using Turtle 1's pose feedback. It must not use
  Turtle 1's target coordinates, command topic, or launch-file remapping.
- Use relative or private resource names inside reusable controller code.
- Complete the task in less than five minutes without random motion.

## Required launch command

```bash
ros2 launch mybestcontroller lab1.launch.py
```

The package skeleton contains no controller or launch solution. Replace all
`TODO(student)` markers with the student's implementation.

