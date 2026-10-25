#!/usr/bin/env python
import rospy
from solve.msg import match

class VoiceLaunch:
    """
    VoiceLaunch 节点：语音，发布任务到task 话题
    """
    def __init__(self):
        rospy.init_node('voice')

        self.pub = rospy.Publisher('task', match, queue_size=10)
        self.rate_hz = 1.0

        #任务列表
        self.tasks = [
            (1, "ball_1", "pick_up"),
            (2, "three_point_line", "dribble"),
            (3, "hoop_red", "shoot"),
            (4, "ball_2", "pick_up"),
            (5, "free_throw_line", "dribble"),
            (6, "hoop_blue", "shoot"),
        ]

    def run(self):
        """
        按顺序发布任务
        """
        rate = rospy.Rate(self.rate_hz)
        idx = 0

        #只要没有任何订阅者连接到Publisher，就一直循环等
        while not rospy.is_shutdown() and self.pub.get_num_connections() == 0:
            rospy.sleep(0.1)
     
        while not rospy.is_shutdown():
            try:
                if not self.tasks:
                    # 没有任务可发布了，定期提示并继续等待
                    rospy.logwarn_throttle(5.0, "no task to publish")
                    rate.sleep()
                    continue
                
                if idx >= len(self.tasks):# 所有任务发布完了，退出循环
                    rospy.spin() 
                    break

                id, target, action = self.tasks[idx]
                msg = match() #创建消息对象
                msg.id = id
                msg.target = target
                msg.action = action
                self.pub.publish(msg)#发布消息
                idx += 1
            except Exception as e:
                rospy.logerr("Error publishing task: %s", e)

            rate.sleep()


if __name__ == '__main__':
    try:
        voice = VoiceLaunch()
        voice.run()
    except Exception as e:
        rospy.logerr("VoiceLaunch node error: %s", e)