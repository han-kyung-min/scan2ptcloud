/*
 * scan_to_ptcloud.cpp
 *
 *  Created on: Oct 31, 2021
 *      Author: hankm
 */

//#ifndef SRC_SCAN_TO_PTCLOUD_CPP_
//#define SRC_SCAN_TO_PTCLOUD_CPP_

#include <scan2ptcloud/scan_to_ptcloud.hpp>

namespace scan2cloud
{

//Scan2PointCloud::Scan2PointCloud(std::ofstream* poutofs, std::ofstream* pofsinfo):
//mp_outfilestream(poutofs),
//mp_ofsinfo(pofsinfo),
//m_nScanCnt(0)
//{
//	//m_tfListener = TransformListener(m_tfBuffer);
//
//	m_ptCloudPub		= m_nh.advertise<sensor_msgs::PointCloud2>("scan_ptcloud",1);
//	m_laserScanSub  	= m_nh.subscribe("base_scan", 1, &Scan2PointCloud::scanCallback, this); // kmHan
//};

Scan2PointCloud::Scan2PointCloud(const std::string& strfile_ofs, const std::string& strinfo_ofs):
m_nScanCnt(0)
{
	//m_tfListener = TransformListener(m_tfBuffer);

	m_outfilestream.open(strfile_ofs);
	m_ofsinfo.open(strinfo_ofs) ;

	m_ptCloudPub		= m_nh.advertise<sensor_msgs::PointCloud2>("scan_ptcloud",1);
	m_laserScanSub  	= m_nh.subscribe("robot1/base_scan", 1, &Scan2PointCloud::scanCallback, this); // kmHan
};

Scan2PointCloud::Scan2PointCloud(const ros::NodeHandle private_nh_, const ros::NodeHandle &nh_):
m_nh_private(private_nh_),
m_nh(nh_)
{
	m_nScanCnt = 0;
	ROS_INFO("instantiating the Scan2PointCloud node \n");
	m_ptCloudPub		= m_nh.advertise<sensor_msgs::PointCloud2>("scan_ptcloud",1);
	m_laserScanSub  	= m_nh.subscribe("scan", 1, &Scan2PointCloud::scanCallback, this); // kmHan
};


//void Scan2PointCloud::initialize()
//{
//	m_nScanCnt = 0;
//	ROS_INFO("instantiating the Scan2PointCloud node \n");
//	m_ptCloudPub		= m_nh.advertise<sensor_msgs::PointCloud2>("scan_ptcloud",1);
//	m_laserScanSub  	= m_nh.subscribe("scan", 1, &Scan2PointCloud::scanCallback, this); // kmHan
//};


Scan2PointCloud::~Scan2PointCloud()
{
	//m_outfilestream.close();
};


void Scan2PointCloud::scanCallback( const sensor_msgs::LaserScan::ConstPtr& scan_in )
{
	ROS_INFO("in scancallback \n");

//	if(!m_tfListener.waitForTransform(
//		scan_in->header.frame_id,
//		"/base_link",
//		scan_in->header.stamp + ros::Duration().fromSec(scan_in->ranges.size()*scan_in->time_increment),
//		ros::Duration(1.0))){
//	 return;
//	}


	tf::StampedTransform transform;
    try{
    	m_tfListener.lookupTransform("robot1/base_link", "robot1/base_scan",
                               ros::Time(0), transform);
    }
    catch (tf::TransformException ex)
    {
      ROS_ERROR("%s",ex.what());
      ros::Duration(1.0).sleep();
      return;
    }

	sensor_msgs::PointCloud cloud;
	m_projector.transformLaserScanToPointCloud("robot1/base_scan",*scan_in, cloud, m_tfListener);

	ROS_INFO("cloud size: %d \n", cloud.points.size());

	sensor_msgs::PointCloud2 cloud2;
	sensor_msgs::convertPointCloudToPointCloud2(cloud, cloud2);
	m_ptCloudPub.publish(cloud2);

  // Do something with cloud.
};



}

//#endif /* SRC_SCAN_TO_PTCLOUD_CPP_ */
