#include "robot.hpp"
#include "devil.hpp"
#include <iostream>

int main() {
    std::string env_str =
        "(sort 1 bottle) (type small) (color blue) (at 2)\n"
        "(sort 2 desk) (type big) (at 2)\n"
        "(sort 3 microwave) (type container) (open) (at 3)\n"
        "(sort 4 bottle) (type small) (color white) (inside 3)\n"
        "(sort 5 book) (type small) (color yellow) (at 1)\n"  // 添加黄色书
        "(sort 6 cup) (type small) (color red) (at 1)\n"      // 添加红色杯子
        "(sort 7 cup) (type small) (color blue) (inside 3)\n"  // 添加蓝色杯子（在微波炉内）
        "(sort 8 human) (type big) (at 0)";                    // 添加人类
    std::string task_str =
        "(task 1) (giveme X) (item X bottle) (color X blue)\n"
        "(task 2) (puton X Y) (item X book) (color X yellow) (item Y desk)\n"
        "(task 3) (putin X Y) (item X cup) (color X red) (item Y microwave)\n"
        "(task 4) (takeout X Y) (item X cup) (color X blue) (item Y "
        "microwave)\n"
        "(task 5) (giveme X) (item X cup) (color X blue)\n"
        "(task 6) (open X) (item X microwave)\n"
        "(task 7) (close X) (item X microwave)";

    Devil devil(env_str);
    devil.readTask(task_str);
    devil.Complete();
    devil.Complete(1);
    devil.Complete(2);

    devil.Complete(1);
}
