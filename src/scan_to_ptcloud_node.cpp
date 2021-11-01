/*
 * scan_to_ptcloud_node.cpp
 *
 *  Created on: Oct 31, 2021
 *      Author: hankm
 */


#include <ros/ros.h>
#include <ros/console.h>

#include "scan2ptcloud/scan_to_ptcloud.hpp"

using namespace scan2cloud;

int main(int argc, char** argv)
{
  ros::init(argc, argv, "scan2ptcloud");
  const ros::NodeHandle nh;
  const ros::NodeHandle private_nh("~");

  ros::WallTime start_, end_;
  if(0) //argc !=3)
  {
	ROS_ERROR("usage: %s <filepath>\n", argv[0]);
	exit(-1);
  }

  ROS_INFO("args: %s %s \n", argv[0], argv[1]);

  char coutfilepath[100], cinfofilepath[100];
  sprintf(coutfilepath, "%s/scan_msg.txt", argv[1] );
  sprintf(cinfofilepath, "%s/scan_info.txt", argv[1] );

  std::string stroutfile( coutfilepath ) ;
  std::string strinfofile( cinfofilepath ) ;
//  ofstream ofs_out( string(outfilepath) );
//  ofstream ofs_info( string(infofilepath) );
//  Scan2PointCloud oScan2PtCloud( &ofs_out, &ofs_info  );

  Scan2PointCloud oScan2PtCloud( private_nh, nh );

  while( ros::ok() )
  {
	  try{
		  ros::spinOnce();
	  }
	  catch(std::runtime_error& e)
	  {
		ROS_ERROR("scan 2 ptcloud exception: %s", e.what());
		return -1;
	  }
  }

  ROS_WARN(" Shutting down scan2ptcloud process \n");
  ros::shutdown();

  return 0;
}


