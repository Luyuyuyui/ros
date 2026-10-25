#!/usr/bin/env python3
import rclpy
from rclpy.action import ActionClient
from rclpy.node import Node
from test_action.action import FileUpload
import sys
import os

class FileUploadActionClient(Node):
    def __init__(self):
        super().__init__('file_upload_action_client')
        self._action_client = ActionClient(self, FileUpload, 'file_upload')

    def send_goal(self, file_paths):
        goal_msg = FileUpload.Goal()
        goal_msg.file_paths = file_paths
        goal_msg.total_files = len(file_paths)
        
        self.get_logger().info('等待服务器准备...')
        self._action_client.wait_for_server()
        
        for file_path in file_paths:
            self.get_logger().info(f'开始上传文件 {file_path}')
        
        self._send_goal_future = self._action_client.send_goal_async(
            goal_msg,
            feedback_callback=self.feedback_callback)
        
        self._send_goal_future.add_done_callback(self.goal_response_callback)

    def goal_response_callback(self, future):
        goal_handle = future.result()
        if not goal_handle.accepted:
            self.get_logger().info('上传被拒绝')
            return
        
        
        self.get_logger().info('上传开始...')
        
        self._get_result_future = goal_handle.get_result_async()
        self._get_result_future.add_done_callback(self.get_result_callback)

    def feedback_callback(self, feedback_msg):
        feedback = feedback_msg.feedback
       
        progress_text = f"进度: {feedback.current_file}/{feedback.total_files} ({feedback.percent_complete:.1f}%)"
        self.get_logger().info(progress_text)

    def get_result_callback(self, future):
        result = future.result().result
        if result.success:
            
            self.get_logger().info(f'文件上传完成: {result.message}')
        else:
            self.get_logger().info(f'上传失败: {result.message}')
        rclpy.shutdown()

def main(args=None):
    rclpy.init(args=args)
    
    
    test_files = [
        '/tmp/test1.txt',
        '/tmp/test2.txt', 
        '/tmp/test3.txt'
    ]
    
    # 创建测试文件（如果不存在）
    for file_path in test_files:
        if not os.path.exists(file_path):
            with open(file_path, 'w') as f:
                f.write(f"Test content for {file_path}")
    
    action_client = FileUploadActionClient()
    
    print(f"准备上传 {len(test_files)} 个文件")
    user_input = input("按回车键继续，或输入 'cancel' 取消上传: ")
    
    if user_input.lower() == 'cancel':
        print("上传已取消")
        return
    
    action_client.send_goal(test_files)
    
    try:
        rclpy.spin(action_client)
    except KeyboardInterrupt:
        action_client.get_logger().info('客户端被用户中断')

if __name__ == '__main__':
    main()
