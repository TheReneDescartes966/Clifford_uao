import rclpy
import serial 
import math
import time

from std_msgs.msg import String
from sensor_msgs.msg import JointState 
from rclpy.node import Node

class ros2arduino(Node):
    def __init__(self):
        super().__init__("transform_angles_ros2arduino")
        self.data_out_old_1 = 0
        self.data_out_old_2 = 0
        self.data_out_old_3 = 0
        self.position_subscriber = self.create_subscription(
            JointState, 
            '/joint_states', 
            self.joint_states_callback, 
            10
        )

        # Serial port's information
        self.microcontroller_port = "/dev/ttyUSB0"
        self.microcontroller_baudrate = 115200
        self.serial_port = serial.Serial(
            self.microcontroller_port, 
            self.microcontroller_baudrate, 
            timeout=10
        )
        self.mapped_msg = String()


    def map_position_into_bits(self, value, in_min, in_max, out_min, out_max):
        mapped_data_list = (value - in_min) * (out_max - out_min) / ((in_max - in_min) + out_min)
        return round(mapped_data_list)
         
    def write_into_serial_port(self, mapped_data_list):
        #self.mapped_msg.data = str(mapped_data_list)
        #encoded_mapped_msg = self.mapped_msg.data.encode()
        #data_out = f"{encoded_mapped_msg};"


        data_out = f"2:c{mapped_data_list[0]},{mapped_data_list[1]},{mapped_data_list[2]}"
        self.get_logger().info(data_out)

        
        self.serial_port.write((data_out).encode())
          

        """if theta3 <= np.radians(-92):
                    theta3 = np.radians(-92)

                if theta3 >= np.radians(26):
                    theta3 = np.radians(26)

                if theta5 <= np.radians(-107):
                    theta5 = np.radians(-107)            

                if theta5 >= np.radians(41):
                    theta5 = np.radians(40)"""
            
        
    def joint_states_callback(self, joint_states_msg):
        print("JointState received!")
        
        front_left_q1_in_degrees = int(math.degrees(joint_states_msg.position[0]))
        front_left_q2_in_degrees = int(math.degrees(joint_states_msg.position[1]))
        front_left_q3_in_degrees = int(math.degrees(joint_states_msg.position[2]))

        """
        front_left_q1_in_bits = self.map_position_into_bits(front_left_q1_in_degrees, -150, 150, 0, 1023)
        front_left_q2_in_bits = self.map_position_into_bits(front_left_q2_in_degrees, 150, -150, 0, 1023)
        front_left_q3_in_bits = self.map_position_into_bits(front_left_q3_in_degrees, -150, 150, 0, 1023)
        """

        mapped_data_list = [
            front_left_q1_in_degrees,
            front_left_q2_in_degrees,
            front_left_q3_in_degrees,
            #back_left_q1_in_bits,
            #back_left_q2_in_bits,
            #back_left_q3_in_bits
            # front_right_q1_in_bits,
            # front_right_q2_in_bits,
            # front_right_q3_in_bits,
            # back_right_q1_in_bits,
            # back_right_q2_in_bits,
            # back_right_q3_in_bits3
            
        ]
        
        #self.get_logger().info(f"Los valores transformados son: {front_left_q1_in_bits}, \n {front_left_q2_in_bits}, \n {front_left_q3_in_bits}")
        self.write_into_serial_port(mapped_data_list)

def main(args=None):
    rclpy.init(args=args)
    node = ros2arduino()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == "__main__":
    main()