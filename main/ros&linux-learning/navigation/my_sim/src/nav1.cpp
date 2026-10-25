#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <vector>

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

class Navigator {
private:
    MoveBaseClient ac;
    std::vector<std::pair<double, double>> points;  // 存储(x, y)坐标
    int current_idx;
    
public:
    Navigator() : ac("move_base", true), current_idx(0) {
        ac.waitForServer(ros::Duration(5.0));
    }
    
    // 添加目标点
    void addPoint(double x, double y) {
        points.push_back({x, y});
    }
    
    // 创建目标
    move_base_msgs::MoveBaseGoal makeGoal(double x, double y) {
        move_base_msgs::MoveBaseGoal goal;
        goal.target_pose.header.frame_id = "map";
        goal.target_pose.header.stamp = ros::Time::now();
        goal.target_pose.pose.position.x = x;
        goal.target_pose.pose.position.y = y;
        goal.target_pose.pose.orientation.w = 1.0; 
        return goal;
    }
    
    // 循环导航（可选）
  
    void runLoop() {
        while (ros::ok()) {
            for (auto& p : points) {
                ac.sendGoal(makeGoal(p.first, p.second));
                ac.waitForResult();
                
                if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED) {
                    ros::Duration(1.0).sleep();
                } else {
                    ROS_WARN("导航失败");
                }
            }
            ROS_INFO("完成一轮，继续循环");
        }
    }
};


int main(int argc, char** argv) {
    ros::init(argc, argv, "nav1");
    
    Navigator nav;
    nav.addPoint(0, -3.0);   // 原始目标点
    //在这里可以多添加点实现多点导航
    nav.runLoop();
    
    return 0;
}
