import os
import fileoperation
import function
import statistics

class StudentSystem:
    def __init__(self, filename):
        self.filename = filename
        self.students = {}#第一个字典（总）
        self.read_file()
    
    # 文件操作
    #读文件
    def read_file(self):
        fileoperation.read_file(self)
    #保存文件：设置返回值，保证读取成功
    def save_file(self):
        return fileoperation.save_file(self)
    #读取方法
    def read_method(self, lines):
        fileoperation.read_method(self, lines)
    #处理文件放入字典
    def process(self, student_data):
        fileoperation.process(self, student_data)
    

    # 功能操作
    def add(self):
        return function.add(self)
    
    def search(self):
        function.search(self)
    
    def find(self, key):
        return function.find(self, key)
    
    def remove(self):
        return function.remove(self)
    
    def showall(self):
        function.showall(self)
    
    def update(self):
        return function.update(self)
    
    def update_stu(self, student):
        function.update_stu(self, student)
    
    # 统计方法
    def calculate(self):
        return statistics.calculate_stu(self)


def manage_scores(file_path: str) -> dict:
    # 创建系统管理器实例
    manager = StudentSystem(file_path)

    while True:
        print("\n" + "=" * 50)
        print("学生成绩管理系统")
        print("=" * 50)
        print("1. 添加学生")
        print("2. 删除学生")
        print("3. 修改学生信息")
        print("4. 显示所有学生")
        print("5. 搜索学生")
        print("6. 成绩统计")
        print("7. 保存数据")
        print("8. 退出系统")
        print("=" * 50)
        
        choice = input("请选择操作 (1-8): ").strip()
        # 根据选择执行相应操作
        if choice == '1':
            manager.add()
        elif choice == '2':
            manager.remove()
        elif choice == '3':
            manager.update()
        elif choice == '4':
            manager.showall()
        elif choice == '5':
            manager.search()
        elif choice == '6':
            manager.calculate()
        elif choice == '7':
           
            if manager.save_file():
                print("数据保存成功！")
            else:
                print("数据保存失败！")
        elif choice == '8':
            # 退出前保存数据
            if manager.save_file():
                print("数据保存成功！")
            else:
                print("数据保存失败！")

            print("感谢使用学生成绩管理系统！")
            break
        else:
            print("无效选择，请重新输入!")
    

if __name__ == "__main__":
    # file_path = r"D:\VS\.vscode\学生成绩管理系统\data.txt"
    file_path = os.path.join(os.path.dirname(__file__), "data.txt")
    manager = StudentSystem(file_path)

 # 检查文件是否存在
    if not os.path.exists(file_path):
        print(f"文件 {file_path} 不存在！")
    else: # 启动管理系统
        final_result = manage_scores(file_path)
        print(f"\n系统运行结果: {final_result}")
