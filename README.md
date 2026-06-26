## Installation
Open terminal and Install the following,
```bash
sudo apt install ros-jazzy-ros2-control
sudo apt install ros-jazzy-ros2-controllers
sudo apt install ros-jazzy-robot-state-publisher
sudo apt install ros-jazzy-joint-state-publisher
sudo apt install ros-jazzy-joint-state-publisher-gui
sudo apt install ros-jazzy-joint-state-broadcaster
sudo apt install ros-jazzy-diff-drive-controller
sudo apt install libserial-dev
```

## Usage

For Launching the LiDAR:
```bash
ros2 launch rplidar_ros rplidar_a2m12_launch.py
```
For Connecting the RaspberryPi and Arduino:
```bash
ros2 launch diffdrive_arduino diffbot.launch.py
```
For Teleoperation (Original = ERRO):
```bash
ros2 run teleop_twist_keyboard teleop_twist_keyboard --ros-args -r /cmd_vel:=/diffbot_base_controller/cmd_vel_unstamped
```
For Teleoperation (Use this):
```bash
ros2 run teleop_twist_keyboard teleop_twist_keyboard --ros-args -r cmd_vel:=/diffbot_base_controller/cmd_vel -p stamped:=true -p frame_id:=base_link
```
For Navigation:
```bash
ros2 launch nav2_bringup navigation_launch.py
```
For Launching the SLAM:
```bash
ros2 launch slam_toolbox online_async_launch.py
```
Incase of any errors, Install the dependencies, for that go to the workspace folder and execute the following command:
```bash
rosdep install --from-paths src --ignore-src -r -y
```