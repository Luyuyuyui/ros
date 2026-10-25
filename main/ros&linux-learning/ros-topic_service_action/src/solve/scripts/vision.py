#!/usr/bin/env python3
import rospy
from solve.srv import Location, LocationResponse

class vision:
    """
    vision 节点：视觉，提供 get_location 服务
    """
    def __init__(self):
        rospy.init_node('vision')
        
        # 创建服务,名称为 get_location,类型为 Location,回调函数为 solve_req
        self.service = rospy.Service('get_location', Location, self.solve_req)
        
    def solve_req(self, req):
        """
        处理 get_location 服务请求，根据 req.target 返回对应的坐标
        如果 target 不在 locations 中，返回 (0.0, 0.0)
        """
        target = req.target.strip()    
        object = {}                

        locations = [
            ("ball_1",2.5,3.0),
            ("three_point_line",10.0,7.5),
            ("hoop_red",14.0,7.5),
            ("ball_2",7.0,8.5),
            ("free_throw_line",8.0,7.5),
            ("hoop_blue",0.0,7.5),
        ]

        for i in locations:
            name = i[0]
            x = i[1]
            y = i[2]
            object[name] = (x, y)

        res = LocationResponse() # 创建响应对象
        if target in object: #查坐标：如果请求的 target 在 locations 中，返回对应坐标
            x, y = object[target]
            res.name = target
            res.x = x
            res.y = y
        else:
            res.name = target
            res.x = 0.0
            res.y = 0.0
        return res
    
    def run(self):
        rospy.spin()
 
if __name__ == '__main__':
    v = vision()
    v.run()