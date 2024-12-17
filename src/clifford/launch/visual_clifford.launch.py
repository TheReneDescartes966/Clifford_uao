import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch_ros.actions import Node
import xacro
def generate_launch_description():
    # Ruta al archivo URDF
    pkg_name = 'clifford'
    file = 'description/clifford.urdf.xacro'

    xacro_file = os.path.join(get_package_share_directory(pkg_name),file)   
    robot_description_raw = xacro.process_file(xacro_file).toxml()

    world_file_path = os.path.join(
        os.getenv('HOME'),
        'clifford_ws', 'src', 'clifford', 'my_robot_config', 'my_robot_config.rviz'
    )
     
    # Nodo para publicar el modelo URDF en el tópico /robot_description
    node_robot_state_publisher = Node(
            package='robot_state_publisher',
            executable='robot_state_publisher',
            name='robot_state_publisher',
            output='screen',
            parameters=[{'robot_description':robot_description_raw, 
                         'use_sim_time': True}]
        )
    
    node_robot_state_publisher_gui = Node(
        package= "joint_state_publisher_gui",
        executable= "joint_state_publisher_gui",
        )
    
    # Nodo para lanzar RViz con la configuración predeterminada
    rviz = Node(
            package='rviz2',
            executable='rviz2',
            name='rviz2',
            output='screen',
            arguments=['-d', world_file_path]
            
        )
    
    transform_y_matrix = Node(
        package='tf2_ros',
        executable='static_transform_publisher',
        arguments=['--x', '0', '--y', '0', '--z', '0', '--roll', '1.57', '--pitch', '0', '--yaw', '0', '--frame-id', 'world', '--child-frame-id', 'base_footprint'],
        name='world_to_base_footprint_tf',
    )

    transform_y_matrix_2 = Node(
        package='tf2_ros',
        executable='static_transform_publisher',
        arguments=['--x', '0', '--y', '0', '--z', '0', '--roll', '1.57', '--pitch', '0', '--yaw', '0', '--frame-id', 'base_footprint', '--child-frame-id', 'base_link'],
        name='base_footprint_to_base_link_tf',
    )

    transform_y_matrix_3 = Node(
        package='tf2_ros',
        executable='static_transform_publisher',
        arguments=['--x', '0', '--y', '0', '--z', '0', '--roll', '1.57', '--pitch', '0', '--yaw', '0', '--frame-id', 'base_link', '--child-frame-id', 'hombro_DA_link'],
        name='base_link_to_hombro_DA_link_tf',
    )

    transform_y_matrix_4 = Node(
        package='tf2_ros',
        executable='static_transform_publisher',
        arguments=['--x', '0', '--y', '0', '--z', '0', '--roll', '1.57', '--pitch', '0', '--yaw', '0', '--frame-id', 'hombro_DA_link', '--child-frame-id', 'codo_DA_link'],
        name='hombro_DA_link_to_codo_DA_link_tf',
    )

    transform_y_matrix_5 = Node(
        package='tf2_ros',
        executable='static_transform_publisher',
        arguments=['--x', '0', '--y', '0', '--z', '0', '--roll', '1.57', '--pitch', '0', '--yaw', '0', '--frame-id', 'codo_DA_link', '--child-frame-id', 'muneca_DA_link'],
        name='codo_DA_link_to_muneca_DA_link_tf',
    )

    transform_y_matrix_6 = Node(
        package='tf2_ros',
        executable='static_transform_publisher',
        arguments=['--x', '0', '--y', '0', '--z', '0', '--roll', '1.57', '--pitch', '0', '--yaw', '0', '--frame-id', 'muneca_DA_link', '--child-frame-id', 'end_effector_link'],
        name='muneca_DA_link_to_end_effector_link_tf',
    )
    
    return LaunchDescription([
        node_robot_state_publisher,
        rviz,
        node_robot_state_publisher_gui,
        #transform_y_matrix,
        #transform_y_matrix_2,
        #transform_y_matrix_3,
        #transform_y_matrix_4,
        #transform_y_matrix_5,
        #transform_y_matrix_6

    ])
