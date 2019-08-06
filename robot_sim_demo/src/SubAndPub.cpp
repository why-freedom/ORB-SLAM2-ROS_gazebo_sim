#include <ros/ros.h>
#include <ros/time.h>
#include <std_msgs/Header.h>
#include <std_msgs/Float32.h>
#include <nav_msgs/Odometry.h>

#include <iostream>
#include <iterator>
#include <random>

typedef boost::shared_ptr<nav_msgs::Odometry const> OdomConstPtr;
ros::Time current_frame_time_ , time_begin_;
using namespace ros;
using namespace std;
class SubAndPub
{
public:
    SubAndPub()
    {
        pub_ = n_.advertise<nav_msgs::Odometry>("/odom_noise", 1);

        sub_ = n_.subscribe("/odom", 1, &SubAndPub::callback, this);
    }

    double GussNoise(const OdomConstPtr &odom)
    {
        // 此处添加高斯噪声处理
        const double mean = 0.0; // 均值
        const double stddev = 0.0; // 标准差
        double stddev_v;
        // 以下设置纯属经验及根据测试
        // 60s之后开始产生高斯噪声
        if(odom->header.stamp.sec < 60)
            stddev_v = 0;
        else
        stddev_v = (stddev + 0.3)*sqrt(odom->header.stamp.sec-60); // 乘以时间系数
//        cout << "stddev_v is " << stddev_v << endl;

        std::default_random_engine generator;
        std::normal_distribution<double> dist(mean, stddev_v);

        double noise = dist(generator);
        cout << "noise is " << noise << endl;
        return -noise;
    }

    void callback(const OdomConstPtr &odom)
    {

        nav_msgs::Odometry odometry;
        std_msgs::Header header;
        odometry.header = header;

        odometry.header.stamp = odom->header.stamp;
        odometry.header.frame_id = "odom_noise";
        odometry.child_frame_id = "base_footprint";

        odometry.pose.pose.position.x = odom->pose.pose.position.x + GussNoise(odom);
        odometry.pose.pose.position.y = odom->pose.pose.position.y + GussNoise(odom);
        odometry.pose.pose.position.z = odom->pose.pose.position.z;
        odometry.pose.pose.orientation.x = odom->pose.pose.orientation.x;
        odometry.pose.pose.orientation.y = odom->pose.pose.orientation.y;
        odometry.pose.pose.orientation.z = odom->pose.pose.orientation.z;
        odometry.pose.pose.orientation.w = odom->pose.pose.orientation.w;
        odometry.twist.twist.linear.x = 0.0;
        odometry.twist.twist.linear.y = 0.0;
        odometry.twist.twist.linear.z = 0.0;

        pub_.publish(odometry);
    }

private:
    ros::NodeHandle n_;
    ros::Publisher pub_;
    ros::Subscriber sub_;

};


int main(int argc, char **argv)
{
  //用于解析ROS参数，第三个参数为本节点名
  ros::init(argc, argv, "pub_sub");



  // 实例化SubAndPub
  SubAndPub SP;

  ros::spin();
  return 0;
} 

