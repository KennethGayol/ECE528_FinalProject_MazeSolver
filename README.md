# ECE 528 - Maze Solver Robot

## Introduction

This project utilizes the MSP432 microcontroller and the TI-RSLK MAX chassis to implement an autonomous maze-solving robot. The robot uses a reflectance sensor array to detect and follow a line-based maze.

The system is designed to identify different path conditions such as straight lines, turns, intersections, and dead ends. Based on these conditions, the robot makes decisions to navigate through the maze.

This is the final project for ECE 528 at California State University, Northridge (CSUN).

Completed by:

* Kenneth Gayol
* Manny Vartanian

---

## Background and Methodology

This project uses embedded systems concepts such as GPIO, timers, PWM motor control, and sensor interfacing. The reflectance sensor array detects the line, and the MSP432 processes this data to control the motors.

The system continuously reads sensor values and determines the robot’s position. Based on the detected pattern, the robot decides how to move. A simple rule-based approach is used, such as always turning left at intersections and correcting its path when it goes off the line. The robot also detects dead ends and stops or turns around if needed.

GPIO is used for sensor input, Timer A is used for PWM motor control, and periodic interrupts update movement in real time.

---

## Block Diagram

<img width="1118" height="415" alt="Screenshot 2026-05-06 162029" src="https://github.com/user-attachments/assets/bc61fe15-1b1c-4e51-9069-181fd8eb16d6" />


---

## Components Used

| Component                   | Quantity | Description                         |
| --------------------------- | -------- | ----------------------------------- |
| MSP432 LaunchPad            | 1        | Microcontroller board               |
| TI-RSLK MAX Chassis         | 1        | Robot platform                      |
| 8-Channel QTRX Sensor Array | 1        | Detects black line on surface       |
| DC Motors                   | 2        | Drives the robot wheels             |
| USB-A to Micro-USB Cable    | 1        | Used to power and program the board |
| Black Tape                  | 1        | Used to create the line path        |

---

## Video Demonstration

* Maze Solver Demo: https://youtu.be/gSa4Wtu__tY?feature=shared

---

## Results

In this project, we built a robot that follows a black tape path using sensors and the MSP432. The robot uses the left-hand rule, so it turns left whenever possible, otherwise it goes straight, then right, and turns around if needed. At first, the robot had trouble staying on the line and making clean turns, but after fixing code errors and adjusting speeds, it improved a lot. By the end, the robot was able to follow the line smoothly and make correct decisions in the maze.


