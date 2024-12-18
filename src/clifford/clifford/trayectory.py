import rclpy

from rclpy.node import Node
from geometry_msgs.msg import Point
from sensor_msgs.msg import JointState

import numpy as np
import matplotlib.pyplot as plt

class trayectoriaElipse(Node):
    def __init__(self):
        super().__init__("trajectory_node")

        # Parámetros de la elipse y la trayectoria
        self.a = -0.050
        self.h = 0.005   

        self.z0 = -0.162  
        self.y_amplitud = 0.0

        # Generación de la trayectoria
        self.num_puntos = 50
        self.t_vals = np.linspace(0, 50, self.num_puntos)  
        self.current_index = 0

        self.value_y = 0.0

        self.publisher_ = self.create_publisher(Point, 'trajectory_points', 10)
        self.timer = self.create_timer(0.5, self.publish_points)

    def compute_trajectory(self):

        # Cálculo de las coordenadas x(t), y(t) y z(t)

        self.x_trayectoria = self.a*(self.t_vals/50)
        self.z_trayectoria = self.z0 + self.h * np.sin(np.pi*(self.t_vals/50))
        #self.y_trayectoria = self.a * np.cos(self.t_vals)
    
    def publish_points(self):

        self.compute_trajectory()

        if self.current_index < len (self.x_trayectoria):

            punto = Point()
            punto.x = self.x_trayectoria[self.current_index]
            punto.y = self.value_y
            punto.z = self.z_trayectoria[self.current_index]

            #self.get_logger().info(f"Trayectoria X: {self.x_trayectoria}")
            #self.get_logger().info(f"Trayectoria Y: {self.y_trayectoria}")
            #self.get_logger().info(f"Trayectoria Z: {self.z_trayectoria}")


            self.publisher_.publish(punto)
            self.current_index = (self.current_index + 1)%101
            #self.get_logger().info(f"Los valores a publicar son: \n X = {punto.x}, \n Y = {punto.y}, \n  Z = {punto.z}")

        else:
            #self.get_logger().info("TODOS LOS VALORES HAN SIDO PUBLICADOS")
            self.timer.cancel()
        
    
def main(args=None):
    rclpy.init(args=args)
    node = trayectoriaElipse()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()