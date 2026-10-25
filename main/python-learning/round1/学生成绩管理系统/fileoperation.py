import json
import os

def read_file(self):
    try:
        #检查路径存在
        if os.path.exists(self.filename):
            #打开文件
            with open(self.filename, 'r', encoding='utf-8') as f:
                lines = f.readlines()
             # 调用读取方法处理文件内容
            self.read_method(lines)
            
            print(f"成功获取 {len(self.students)} 名学生成绩")
        else:
            print(f"文件 {self.filename} 不存在")
            
    except Exception as e:
        print(f"读取文件失败: {e}")
        self.students = {}



#读取文件的方法
def read_method(self, lines):
    """
    文件读取的具体实现方法
    解析文件内容，提取学生信息和成绩数据
    """
    #初始化字典
    current_stu = {}
    line_num = 0
    #遍历每一行数据
    for line in lines:
        line_num += 1
        line = line.strip()# 去除首尾空白字符
        #用空行来分隔不同的学生记录：如果遇到空行就处理前一个学生
        if not line:#遇到空行
            if current_stu: # 如果当前有学生数据
                self.process(current_stu)# 处理当前学生数据
                current_stu = {}#清空，读取下一个数据

            continue #跳过空行，处理下一行
        #检查是否有冒号
        if ':' in line:
            key, value = line.split(':', 1)
            key = key.strip()
            value = value.strip()

            #放到对应位置
            if key == '学号':
                current_stu['student_id'] = value
            elif key == '姓名':
                current_stu['name'] = value
            #处理成绩
            else: # 如果scores字典不存在，则初始化
                if 'scores' not in current_stu:
                    current_stu['scores'] = {}
                #将成绩转为浮点值
                try:
                    score = float(value)
                    #将科目和成绩存入scores字典
                    current_stu['scores'][key] = score
                except ValueError:
                    print("成绩格式错误")
    #如果文件结尾没有空行，确保最后一个数据能被处理！
    if current_stu and 'student_id' in current_stu:
        self.process(current_stu)


#处理学生数据
#第二个字典：student_data
def process(self, student_data):
    """
    处理单个学生数据，计算统计信息并存储
    """
    #不存在或为空
    if 'scores' not in student_data or not student_data['scores']:
        return
    
    #获取成绩字典
    scores = student_data['scores']

    total_score = sum(scores.values())
    avg_score = total_score / len(scores)
    max_score = max(scores.values())
    min_score = min(scores.values())
    max_subjects = [subject for subject, score in scores.items() if score == max_score]
    min_subjects = [subject for subject, score in scores.items() if score == min_score]
    #完整的学生信息字典
    student_info = {
        'student_id': student_data['student_id'],
        'name': student_data.get('name', ''),# 使用get， 如果'name'不存在，默认为空而不是中断程序
        'scores': scores,
        'total_score': total_score,
        'average_score': round(avg_score, 2),
        'max_score': max_score,
        'min_score': min_score,
        'max_subjects': max_subjects,
        'min_subjects': min_subjects
    }
    #以id为键
    self.students[student_data['student_id']] = student_info
    print(f"  已保存: {student_data['name']} ({student_data['student_id']}) - 总分: {total_score}")


#保存文件
def save_file(self):

    try:# 以写入模式打开文件
        with open(self.filename, 'w', encoding='utf-8') as f:
            for student in self.students.values():
                # 写入学号
                f.write(f"学号:{student['student_id']}\n")
                # 写入姓名
                f.write(f"姓名:{student['name']}\n")
                # 写入各科成绩
                for subject, score in student['scores'].items():
                    f.write(f"{subject}:{score}\n")
                # 学生之间用空行分隔
                f.write("\n")
        #设置返回值
        print(f"成功保存 {len(self.students)} 名学生成绩到 {self.filename}")
        return True
    
    except Exception as e:
        print(f"保存文件失败: {e}")
        return False
