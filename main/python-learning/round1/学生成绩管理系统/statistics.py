# 成绩统计  计算班级和各科目的统计信息
def calculate_stu(self):
    print("\n" + '='*10 + "成绩统计" + '='*10)
    if not self.students:
        print("没有学生数据!")
        return {}
     # 获取学生列表和所有科目
    stu_list = list(self.students.values())
    all_subjects = set()#方便存储,使用集合自动去重
    # 收集所有科目名称
    for student in stu_list:
        all_subjects.update(student['scores'].keys())# 添加学生所有科目
        #set.update()添加多个元素
    # 初始化统计结果字典
    result = {
        'class_avg': 0,
        'top_student': None,
        'subject_info': {}
    }

    # 班级统计
    '''total_scores = []
    for student in stu_list:
    total_scores.append(student['total_score'])'''

    total_scores = [student['total_score'] for student in stu_list]
    class_avg = sum(total_scores) / len(total_scores)
    result['class_avg'] = round(class_avg, 2)

    print(f"\n班级统计:")
    print(f"  班级平均分: {class_avg:.2f}")
    print(f"  最高总分: {max(total_scores)}")
    print(f"  最低总分: {min(total_scores)}")

    # 最好成绩学生
    top_student = None
    best_score = -1  
    for student in stu_list:
        if student['total_score'] > best_score:
            best_score = student['total_score']
            top_student = student
    
    result['top_student'] = top_student['name']
    print(f"\n最好成绩学生: {top_student['name']} (总分: {top_student['total_score']})")

    # 各科目统计
    print(f"\n各科目统计:")
    #使用sorted 返回一个新列表
    for subject in sorted(all_subjects):
        scores = []# 存储该科目所有学生的成绩
        for s in stu_list:
            if subject in s['scores']:
                scores.append(s['scores'][subject])
     # 如果有该科目的成绩数据
        if scores:
            avg = sum(scores) / len(scores)
            max_score = max(scores)
            min_score = min(scores)
            
            #创建一个新的subject键值对   存储科目统计信息
            result['subject_info'][subject] = {
                'average': round(avg, 2),
                'max': max_score,
                'min': min_score
            }
             # 输出科目统计信息
            print(f"  {subject}: 平均分{avg:.2f}, 最高分{max_score}, 最低分{min_score}")



    # 学生个人统计
    print(f"\n学生个人统计:")
    for student in stu_list:
        print(f"  {student['name']}: 平均分{student['average_score']}, "
                f"最高分{student['max_score']}({','.join(student['max_subjects'])}), "
                f"最低分{student['min_score']}({','.join(student['min_subjects'])})")

    return result