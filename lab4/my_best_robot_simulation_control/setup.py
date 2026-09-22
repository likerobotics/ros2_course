from glob import glob
import os

from setuptools import find_packages, setup


package_name = 'my_best_robot_simulation_control'


setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        (
            'share/ament_index/resource_index/packages',
            ['resource/' + package_name],
        ),
        ('share/' + package_name, ['package.xml']),
        (
            os.path.join('share', package_name, 'config'),
            glob(os.path.join('config', '*')),
        ),
        (
            os.path.join('share', package_name, 'launch'),
            glob(os.path.join('launch', '*launch.[pxy][yma]*')),
        ),
        (
            os.path.join('share', package_name, 'urdf'),
            glob(os.path.join('urdf', '*')),
        ),
        (
            os.path.join('share', package_name, 'worlds'),
            glob(os.path.join('worlds', '*')),
        ),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='student',
    maintainer_email='student@example.com',
    description=(
        'Python simulation and control package skeleton for laboratory work 4.'
    ),
    license='Apache-2.0',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [],
    },
)
