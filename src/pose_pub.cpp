#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <tf/transform_broadcaster.h>

class pose_p{
	private:
	
	ros::Subscriber pose_sub;
	std::string topic;
	std::string root;
	std::string leaf;


	float pos_x;
	float pos_y;
	float ori_x;
	float ori_y;
	float ori_z;
	float ori_w;

	public:
	pose_p(ros::NodeHandle *nh){
		topic = "/ndt_pose";
		root  = "map";
		leaf = "base_link";
		pose_sub = nh -> subscribe(topic, 1000, &pose_p::pose_callback, this);
		pos_x = 0;
		pos_y = 0;
		ori_x = 0;
		ori_y = 0;
		ori_z = 0;
		ori_w = 0;
		
	}		
	
	void pose_callback(const geometry_msgs::PoseStamped& msg){
		pos_x = msg.pose.position.x;
		pos_y = msg.pose.position.y;
		ori_x = msg.pose.orientation.x;
		ori_y = msg.pose.orientation.y;
		ori_z = msg.pose.orientation.z;
		ori_w = msg.pose.orientation.w;
		static tf::TransformBroadcaster br;
		tf::Transform transform;
		transform.setOrigin(tf::Vector3(pos_x, pos_y, 0.0));
		tf::Quaternion q(ori_x, ori_y, ori_z, ori_w);
		transform.setRotation(q);
		br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), root, leaf));
	}
};

int main(int argc, char **argv){
	ros::init(argc, argv, "pose_pub");
	ros::NodeHandle nh;
	pose_p a = pose_p(&nh);
	ros::spin();
}
