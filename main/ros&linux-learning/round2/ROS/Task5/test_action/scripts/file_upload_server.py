#!/usr/bin/env python3
import rclpy
from rclpy.action import ActionServer
from rclpy.node import Node
from test_action.action import FileUpload
import os
import time

class FileUploadActionServer(Node):
    def __init__(self):
        super().__init__('file_upload_action_server')
        self._action_server = ActionServer(
            self,
            FileUpload,
            'file_upload',
            self.execute_callback)
        
  
        self.upload_dir = 'uploaded_files'
        if not os.path.exists(self.upload_dir):
            os.makedirs(self.upload_dir)
            
        self.get_logger().info('文件上传服务器已启动')

    def show_upload_progress(self, current, total, aborted=False):
        
        percent = (current / total) * 100
        if aborted:
            return f"上传中止: {current}/{total} 文件 ({percent:.1f}%)"
        elif current == total:
            return f"上传完成: {total}/{total} 文件 (100%)"
        else:
            return f"进度: {current}/{total} 文件 ({percent:.1f}%)"

    def execute_callback(self, goal_handle):
        file_paths = goal_handle.request.file_paths
        total_files = len(file_paths)
        
        self.get_logger().info(f'开始上传 {total_files} 个文件')
        
        feedback_msg = FileUpload.Feedback()
        result = FileUpload.Result()
        
        try:
            for i, file_path in enumerate(file_paths, 1):
                
                if goal_handle.is_cancel_requested:
                    goal_handle.canceled()
                    self.get_logger().info(self.show_upload_progress(i-1, total_files, True))
                    result.success = False
                    result.message = "上传被用户取消"
                    return result
                
               
                self.get_logger().info(f'处理文件: {os.path.basename(file_path)}')
                time.sleep(1.0) 
                
                # 更新进度
                feedback_msg.current_file = i
                feedback_msg.total_files = total_files
                feedback_msg.percent_complete = float(i) / float(total_files) * 100.0
                
             
                progress_text = self.show_upload_progress(i, total_files)
                self.get_logger().info(progress_text)
                
                
                goal_handle.publish_feedback(feedback_msg)
            
           
            goal_handle.succeed()
            self.get_logger().info(self.show_upload_progress(total_files, total_files))
            
            result.success = True
            result.message = "所有文件上传成功"
            return result
            
        except Exception as e:
            self.get_logger().error(f'上传错误: {str(e)}')
            goal_handle.abort()
            result.success = False
            result.message = f"上传失败: {str(e)}"
            return result

def main(args=None):
    rclpy.init(args=args)
    file_upload_action_server = FileUploadActionServer()
    
    try:
        rclpy.spin(file_upload_action_server)
    except KeyboardInterrupt:
        file_upload_action_server.get_logger().info('服务器关闭')
    finally:
        file_upload_action_server.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()
