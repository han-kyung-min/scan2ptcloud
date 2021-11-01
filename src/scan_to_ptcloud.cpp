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

	m_ptCloudPub		= m_nh.advertise<sensor_msgs::PointCloud2>("cloud_in",1);
	m_laserScanSub  	= m_nh.subscribe("base_scan", 1, &Scan2PointCloud::scanCallback, this); // kmHan
};

Scan2PointCloud::Scan2PointCloud(const ros::NodeHandle private_nh, const ros::NodeHandle &nh):
m_nh(nh),
m_nh_private(private_nh),
m_nScanCnt(0)
{
	//m_tfListener = TransformListener(m_tfBuffer);

	m_ptCloudPub		= m_nh.advertise<sensor_msgs::PointCloud2>("cloud_in",1);
	m_laserScanSub  	= m_nh.subscribe("base_scan", 1, &Scan2PointCloud::scanCallback, this); // kmHan
};


Scan2PointCloud::~Scan2PointCloud()
{
	//m_outfilestream.close();
};


void Scan2PointCloud::scanCallback( const sensor_msgs::LaserScan::ConstPtr& scan_in )
{

	if(!m_tfListener.waitForTransform(
		scan_in->header.frame_id,
		"base_link",
		scan_in->header.stamp + ros::Duration().fromSec(scan_in->ranges.size()*scan_in->time_increment),
		ros::Duration(1.0))){
	 return;
	}

	tf::StampedTransform transform;
    try{
    	m_tfListener.lookupTransform("map", "base_link",
                               ros::Time(0), transform);
    }
    catch (tf::TransformException ex){
      //ROS_ERROR("%s",ex.what());
      //ros::Duration(1.0).sleep();
      return;
    }

	sensor_msgs::PointCloud cloud;
	m_projector.transformLaserScanToPointCloud("base_link",*scan_in, cloud, m_tfListener);

	// sensor pose
//	tf::Matrix3x3 Rm( transform.getRotation() ) ;
//	double roll, pit, yaw ;
//	Rm.getRPY(roll, pit, yaw);
//	m_outfilestream << "NODE " << transform.getOrigin().x() << " " << transform.getOrigin().y() << " "
//					<< transform.getOrigin().z() << " "
//					<< roll << " " << pit << " " << yaw << std::endl;

//	for( size_t idx=0; idx < cloud.points.size(); idx++)
//	{
//		m_outfilestream << cloud.points[idx].x << " "
//						<< cloud.points[idx].y << " "
//						<< cloud.points[idx].z << endl;
//	}
//
//	m_ofsinfo << m_nScanCnt << " " << cloud.points.size() << endl;
//
//	m_nScanCnt++;

	sensor_msgs::PointCloud2 cloud2;
	sensor_msgs::convertPointCloudToPointCloud2(cloud, cloud2);
	m_ptCloudPub.publish(cloud2);

  // Do something with cloud.
};



}

//#endif /* SRC_SCAN_TO_PTCLOUD_CPP_ */
