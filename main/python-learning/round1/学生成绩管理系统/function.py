#添加学生
def add(self):
    print("\n" + '='*10 + "添加学生" + '='*10)
    
    stu_id = input("请输入学号: ").strip()
    
    if stu_id in self.students:
        print("学号已存在!")
        return False
    
    name = input("请输入姓名: ").strip()
    if name == "":
        print("姓名不能为空！")
        return False
    
    print("\n请输入成绩（科目:分数）")
    scores = {}
    while True:
        text = input("科目:分数: ").strip()
        if text == "":
            break
        if ':' not in text:
            print("格式错误！请使用 科目:分数 的格式")
            continue
        
        subject, score_str = text.split(':', 1)
        subject = subject.strip()
        
        try:
            score = float(score_str.strip())
            scores[subject] = score
        except ValueError:
            print("请输入有效的数字！")


    #创建字典
    student_data = {
        'student_id': stu_id,
        'name': name,
        'scores': scores,
    }

    self.process(student_data)
    #学生数据存入字典
    print(f"成功添加学生: {name}")
    return True

# 搜索学生并显示详细信息,支持按学号或姓名搜索
def search(self):
    print("\n" + '='*10 + "搜索学生" + '='*10)
    if not self.students:  
        print("学生不存在!")
        return
     # 获取搜索关键词
    key = input("请输入学号或姓名: ").strip()
     # 查找匹配的学生
    found_stu = []
    for student in self.students.values():
        if (key in student['student_id'] or key in student['name']):
            found_stu.append(student)
      # 检查是否找到学生
    if not found_stu:
        print("未找到匹配的学生!")
        return
    
    print(f"找到 {len(found_stu)} 个匹配的学生:")
 # 显示找到的学生信息
    for student in found_stu:
        '''
        score_parts = []
        for subject, score in student['scores'].items():
        score_parts.append(f"{subject}:{score}")
        scores_str = ', '.join(score_parts)
        改为列表推导式：
        '''
        scores_str = ', '.join([f"{k}:{v}" for k, v in student['scores'].items()])

        print(f"  学号: {student['student_id']}, 姓名: {student['name']}, "
                f"成绩: [{scores_str}], 总分: {student['total_score']}")


#查找，（内部使用，不输出信息）
def find(self, key):
    if not self.students: 
        return None 
    
    found_stu = []
    for student in self.students.values():
        student_id1 = student['student_id']
        name1 = student['name']
        key1 = key
        
        if (key1 in student_id1 or key1 in name1):
            found_stu.append(student)
    # 根据找到的学生数量返回不同结果
    if not found_stu:
        return None # 没有找到匹配的学生，返回None
    elif len(found_stu) == 1:
        return found_stu[0] # 返回单个学生字典
    else:
        return found_stu  # 返回学生列表





#删除学生
def remove(self):
    print("\n" + '='*10 + "删除学生" + '='*10)
    if not self.students:  
        print("学生不存在！")
        return False
    # 获取要删除的学生标识
    k = input("请输入要删除的学生学号或姓名: ").strip()
     # 查找学生
    res = self.find(k)  
    if res is None:
        print("未找到该学生！")
        return False
    # 处理多个匹配结果的情况
    #检查res是不是列表 
    # if isinstance(res, list):
    #如果有多个结果
    if type(res) == list:
        print("找到多个匹配的学生:")
        for i, student in enumerate(res, 1):#enumerate(,)同时获取索引和值，列表+编号  这里显示从1开始
            print(f"{i}. {student['name']} (学号: {student['student_id']})")
        
        try:
            choice = int(input("请选择要删除的学生编号: "))
            if 1 <= choice <= len(res):
                studel = res[choice-1]#列表索引从0开始
            else:
                print("无效选择")
                return False
        except ValueError:
            print("请输入有效的数字!")
            return False
    else:#只有一个结果
        studel = res
     # 从学生字典中删除
    del self.students[studel['student_id']]
    print(f"成功删除学生: {studel['name']}")
    return True
    

#获取所有学生
def showall(self):
    if not self.students: 
        print("没有学生!")
        return
    
    print("\n" + '='*10 + "获取所有学生" + '='*10)
    #打印成表格输出
    print(f"{'学号':<12} {'姓名':<10} {'成绩':<25} {'总分':<6} {'平均分':<8} {'最高分':<12} {'最低分':<12}")
    print("="*90)
    
    for stu in self.students.values():
        scores_str = ' '.join([f"{k}:{v}" for k, v in stu['scores'].items()])
        '''
        score_parts = []
        for subject, score in stu['scores'].items():
        score_parts.append(f"{subject}:{score}")
        scores_str = ' '.join(score_parts)'''

        maxshow = f"{stu['max_score']}({','.join(stu['max_subjects'])})"
        minshow = f"{stu['min_score']}({','.join(stu['min_subjects'])})"
        
        print(f"{stu['student_id']:<12} {stu['name']:<10} {scores_str:<25} "
                f"{stu['total_score']:<6} {stu['average_score']:<8} "
                f"{maxshow:<12} {minshow:<12}")


# 修改学生信息
def update(self):
    print("\n" + '='*10 + "修改学生信息" + '='*10)
    if not self.students:  
        print("学生不存在！")
        return False
     # 查找要修改的学生
    k = input("请输入要修改的学生学号或姓名: ").strip()
    res = self.find(k)  
    
    if not res:
        print("未找到该学生！")
        return False
     # 处理多个匹配结果（同上）
    # if isinstance(res, list):
    if type(res) == list:
        print("找到多个匹配的学生:")
        for i, student in enumerate(res, 1):
            print(f"{i}. {student['name']} (学号: {student['student_id']})")
        
        try:
            choice = int(input("请选择要修改的学生编号: "))
            if 1 <= choice <= len(res):
                res = res[choice-1]
            else:
                print("无效选择")
                return False
        except ValueError:
            print("请输入有效的数字!")
            return False
    
    print(f"当前信息: 姓名: {res['name']}, 成绩: {res['scores']}")
    print("\n选择修改内容:")
    print("1. 修改姓名")
    print("2. 修改成绩")
    print("3. 添加科目成绩")
    
    try:
        choice = int(input("请选择: "))
        if choice == 1:
            new_name = input("请输入新姓名: ").strip()
            if new_name:
                res['name'] = new_name
                print("姓名修改成功!")
                return True
                
        elif choice == 2:
            subject = input("请输入要修改的科目: ").strip()
            if subject not in res['scores']:
                print("该科目不存在!")
                return False
            
            try:
                new_score = float(input(f"请输入新的{subject}成绩: "))
                
                res['scores'][subject] = new_score
                self.update_stu(res)
                print("成绩修改成功!")
                return True
            
            except ValueError:
                print("无效的成绩!")
                return False
                
        elif choice == 3:
            subject = input("请输入新科目名称: ").strip()
            if subject in res['scores']:
                print("该科目已存在!")
                return False
            
            try:
                score = float(input(f"请输入{subject}成绩: "))
                
                res['scores'][subject] = score
                self.update_stu(res)
                print("科目成绩添加成功!")
                return True
            
            except ValueError:
                print("无效成绩!")
                return False
        else:
            print("无效选择!")
            return False
            
    except ValueError:
        print("无效输入!")
        return False

# 更新学生信息（重新计算统计信息）
def update_stu(self, student):
    student_data = {
            'student_id': student['student_id'],
            'name': student['name'],
            'scores': student['scores']
        }
     # 调用 process 方法重新计算统计信息
    old_student_id = student['student_id'] # 保存旧学号用于检查
    self.process(student_data)
    if old_student_id in self.students:
        pass# 学生信息已更新

