import rclpy
import serial 
import math
import time

from std_msgs.msg import String
from sensor_msgs.msg import JointState 
from geometry_msgs.msg import Point
from rclpy.node import Node

class ros2arduino(Node):
    def __init__(self):
        super().__init__("transform_angles_ros2arduino")

        self.data_out_old_1 = 0
        self.data_out_old_2 = 0
        self.data_out_old_3 = 0

        self.px = 0.0
        self.py = 0.0
        self.pz = 0.0

        self.position_subscriber = self.create_subscription(
            Point, 
            'trajectory_points', 
            self.trayectory_callback, 
            10
        )
        
        self.microcontroller_port = "/dev/ttyACM0"
        self.microcontroller_baudrate = 1000000
        self.serial_port = serial.Serial(
            self.microcontroller_port, 
            self.microcontroller_baudrate, 
            timeout=10
        )
         
    def write_into_serial_port(self, points):

        data_out =  f"{points};"

        self.get_logger().info(f"PUNTOS DE LA TRAYECTORIA {data_out}")
        self.serial_port.write((data_out).encode())
            
    def trayectory_callback(self, point_msg):

        self.px = point_msg.x 
        self.py = point_msg.y
        self.pz = point_msg.z

        points_trayectory = [self.px, self.py, self.pz]
             
        #self.get_logger().info(f"Los valores transformados son: {front_left_q1_in_bits}, \n {front_left_q2_in_bits}, \n {front_left_q3_in_bits}")
        self.write_into_serial_port(points_trayectory)

def main(args=None):
    rclpy.init(args=args)
    node = ros2arduino()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == "__main__":
    main()