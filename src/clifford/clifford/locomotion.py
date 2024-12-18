import rclpy
import numpy as np

#from clifford.inverse_kinematic import InverseKinematic
from clifford.numeric_method import nodeNumericMethod

from sensor_msgs.msg import JointState
from geometry_msgs.msg import Point
from rclpy.node import Node


class CommandUserLocomotion(Node):
    def __init__(self):
        super().__init__("Nodo_de_tipo_de_locomocion")

        self.declare_parameter('modo', 'home')

        self.px = 0.0
        self.py = 0.0
        self.pz = 0.0
        self.q_list = [0.0, 0.0, 0.0, 0.0]
        self.l_list = [-0.058, -0.041, -0.013]

        self.q_pata_1 = [0.0, 0.0, 0.0, 0.0]
        self.q_pata_2 = [0.0, 0.0, 0.0, 0.0]
        self.q_pata_3 = [0.0, 0.0, 0.0, 0.0]
        self.q_pata_4 = [0.0, 0.0, 0.0, 0.0]

        self.publisher_ = self.create_publisher(JointState, '/joint_states', 10)

        self.point = Point()
        self.joint_names = JointState()
        #self.inverse_kinematic = InverseKinematic()
        self.numeric_method = nodeNumericMethod()

        self.current_leg_id = 0
        
        #self.px, self.py, self.pz = self.direct_kinematic.send_attributes()

        self.param_value = self.get_parameter('modo').get_parameter_value().string_value
        self.get_logger().info(f'el valor del parametro es {self.param_value}')

        self.joint_names = [
            "hombro_DA_joint", "codo_DA_joint", "muneca_DA_joint", "end_effector_joint",
        ]
        
        self.suscription = self.create_subscription(Point, 'trajectory_points', self.trayectory_callback, 10)

#//////////////////////////////////////////////////////////////////////////////////

    def trayectory_callback(self, point_msg):

        self.px = point_msg.x
        self.py = point_msg.y
        self.pz = point_msg.z

        #self.get_logger().info(f"valor X = {self.px}")
        #self.get_logger().info(f"valor Y = {self.py}")
        #self.get_logger().info(f"valor Z = {self.pz}")
        
        param_value = self.param_value

        self.user_commands_dog_state(param_value)

        self.values_in_joints = [
        *self.q_pata_1,
        ] 

        self.publish_joint_state()

#//////////////////////////////////////////////////////////////////////////////////
    
    def publish_joint_state(self):

        joint_state = JointState()
        joint_state.header.stamp = self.get_clock().now().to_msg()
        joint_state.name = self.joint_names
        joint_state.position = self.values_in_joints

        self.publisher_.publish(joint_state)

#/////////////////////////////////////////////////////////////////////////////////

    def user_commands_dog_state(self, param_value):
        
        current_leg_id = self.current_leg_id
        dog_state = param_value

        if dog_state == 'home':
            self.command_home(current_leg_id)
        
#//////////////////////////////////////////////////////////////////////////////////

    def command_home(self, leg_id):

        desired_points = np.array([self.px, self.py, self.pz, 0.0]) 
        #self.q_list = self.inverse_kinematic.compute_inverse_kinematic(L_list, desired_points)
        self.q_list = self.numeric_method.numeric_method(desired_points)

        if leg_id == 0:
            self.q_pata_1 = self.q_list
        elif leg_id == 1:
            self.q_pata_2 = self.q_list
        elif leg_id == 2:
            self.q_pata_3 = self.q_list
        elif leg_id == 3:
            self.q_pata_4 = self.q_list

        self.current_leg_id = (self.current_leg_id + 1)%4

#//////////////////////////////////////////////////////////////////////////////////
    
def main(args=None):
    rclpy.init(args=args)
    node = CommandUserLocomotion()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__=='__main__':
    main()