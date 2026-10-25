"""
python_advanced_assessment.py
学生作业文件
"""
import os
# ========== 题目 1 ==========
def manage_scores(file_path: str) -> dict:
    """
    实现学生成绩管理系统。
    可自行设计交互或内部逻辑。
    """
    filename = file_path
    student_info = {}

    if os.path.exists(filename):
        # 打开文件
        with open(filename, 'r', encoding='utf-8') as f:
            lines = f.readlines()
        all_scores=[]
        # 读取文件
        for line in lines:
            line = line.strip()
            if not line:
                continue
            content = line.split(',')#按，分割内容
            name = content[0].strip()#捕获名字
            score_info = []#存放单人成绩

            for i in content[1:]:
                try:
                    score = float(i.strip())#转换成浮点数
                    score_info.append(score)
                    all_scores.append(score)
                except ValueError:#跳过不合法的成绩
                    continue
            #计算成绩平均分和最高分
            if score_info:
                total_score = sum(score_info)
                avg_score = total_score / len(score_info)
                max_score = max(score_info)

                student_info[name] = {
                    "avg": round(avg_score, 2),
                    "max": max_score
                }
        
        # 计算班级统计信息
        top_student_name = ""
        top_avg_score = 0

        for name, info in student_info.items():

            if info["avg"] > top_avg_score:
                top_avg_score = info["avg"]
                top_student_name = name

        class_avg = 0.0
        #根据测试用例，班级平均分应该是所有原始成绩的平均值？但是没有标明科目相同还是不同（）
        #but这样算感觉最终结果有点脱离实际。。也许是我理解错了？（）
        if all_scores:
            class_avg = round(sum(all_scores) / len(all_scores), 2)

        return {
            "students": student_info,
            "class_avg": class_avg,
            "top_student": top_student_name
        }
    else:
        return {
            "students": {},
            "class_avg": 0.0,
            "top_student": ""
        }

"""
    提交的文件中名为    “学生成绩管理系统”  是重新设计了交互系统的版本（花了很多时间和精力才写完，功能不完善的地方见谅），
    添加了   添加学生，删除学生，修改学生信息，显示所有学生，搜索学生的功能
    添加了   具体科目名称，学号，学科平均分，总分平均分以及最高分，最低分
"""
    
pass

from collections import deque
# ========== 题目 2 ==========
def solve_maze(maze: list[list[int]]) -> list[tuple[int, int]]:
    """
    自动求解迷宫路径。
    """
    if not maze or not maze[0]:
        return []
    
    n=len(maze)
    start=(0,0)
    goal=(n-1,n-1)

    if maze[0][0] == 1 or maze[n-1][n-1] == 1:
        return []
    if n==1:
        return [(0,0)]
    
    visited=[]
    for i in range(n):
        visited.append([False]*n)
    
    parent=[]
    for i in range(n):
        parent.append([(-1,-1)]*n)

    q=deque()

    q.append(start)
    visited[0][0] = True
    parent[0][0] = start
    found=False

    dxy = [(0, 1), (1, 0), (0, -1), (-1, 0)]

    while q and not found:
        x,y=q.popleft()

        if x == goal[0] and y == goal[1]:
            found = True
            break

        for dx, dy in dxy:
            nx, ny =  x+ dx, y + dy
            if (0 <= nx < n and 0 <= ny < n and maze[nx][ny] == 0 and not visited[nx][ny]):
                visited[nx][ny] = True
                parent[nx][ny] = (x, y)
                q.append((nx, ny))

    if found:
        path=[]
        current=goal

        while current!=start:
            path.append(current)
            current=parent[current[0]][current[1]]

        path.append(start)
        path.reverse()
        return path
    else:
        return []

    pass

# ========== 示例测试 ==========
if __name__ == "__main__":
    # --- 题目 1 测试 ---
    import tempfile, os
    test_file = os.path.join(tempfile.gettempdir(), "scores.txt")
    with open(test_file, "w", encoding="utf-8") as f:
        f.write("Alice,90,85,95\nBob,80,70,88\nCharlie,100,90,98\n")

    print("题目1结果：")
    print(manage_scores(test_file))

    # --- 题目 2 测试 ---
    print("\n题目2结果：")
    maze = [
        [0, 1, 0, 0, 0],
        [0, 1, 0, 1, 0],
        [0, 0, 0, 1, 0],
        [1, 1, 0, 0, 0],
        [0, 0, 0, 1, 0],
    ]
    print(solve_maze(maze))