people
======
Leg detection library originally from Washington University.
Has been ported to work with Hydro (still uses rosbuild) and the Kinect.

https://code.google.com/p/wu-robotics/source/browse/#svn%2Ftrunk%2Fpeople2

running
======
In separate terminals launch the following ros nodes:

```
roslaunch openni_launch openni.launch
roslaunch leg_detector depth_to_laser.launch
roslaunch leg_detector leg_detector.launch
```


