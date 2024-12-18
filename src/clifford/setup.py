from setuptools import find_packages, setup
import os

from glob import glob

package_name = 'clifford'

setup(
    name=package_name,
    version='0.0.0',
    packages=['clifford'],
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),

        (os.path.join('share', package_name, 'config'), glob('config/*.yaml')),

        (os.path.join('share', package_name, 'launch'), glob('launch/*.py')),

        (os.path.join('share', package_name, 'worlds'), glob('worlds/*.world')),

        (os.path.join('share', package_name, 'description'), glob('description/*.xacro')),

        (os.path.join('share', package_name, 'description'), glob('description/*.urdf')),

        (os.path.join('share', package_name, 'description'), glob('description/*.sdf')),

        (os.path.join('share', package_name, 'description'), glob('description/*.urdf.xacro')),

        (os.path.join('share', package_name), ['README.md']),

        (os.path.join('share', package_name, 'my_robot_config'), glob('my_robot_config/*.rviz')),

        (os.path.join('share', package_name, 'meshes'), glob('meshes/*.STL')),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='jagger',
    maintainer_email='juansepc@hotmail.com',
    description='TODO: Package description',
    license='Apache-2.0',
    extras_require={
        'test': ['pytest'],
    },
    entry_points={
        'console_scripts': [
            'user_node = clifford.user:main',
            'user_node_2 = clifford.suscriber_user:main',
            'direct_node = clifford.direct_kinematic:main',
            'locomotion_node_2 = clifford.locomotion:main',
            'inverse_node = clifford.inverse_kinematic:main',  
            'trayectory_node = clifford.trayectory:main',
            'ros2arduino_node = clifford.ros2arduino:main', 
            #'numeric_node = clifford.numeric_method:main',
            'suscriber_node = clifford.suscriber_user:main',
            'visual_line_node = clifford.trayectory_visual:main',
            'change_tf_node = clifford.tf_broadcaster:main', 
        ],
    },
)