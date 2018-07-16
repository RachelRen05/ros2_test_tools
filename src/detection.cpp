// Copyright 2016 Open Source Robotics Foundation, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "rclcpp/rclcpp.hpp"
#include "object_msgs/msg/objects_in_boxes.hpp"
#include "std_msgs/msg/string.hpp"
using std::placeholders::_1;

class MinimalSubscriber : public rclcpp::Node
{
public:
  MinimalSubscriber()
  : Node("detection")
  {
    subscription_ = this->create_subscription<object_msgs::msg::ObjectsInBoxes>(
      "/movidius_ncs_stream/detected_objects", std::bind(&MinimalSubscriber::topic_callback, this, _1));
  }

private:
  void topic_callback(const object_msgs::msg::ObjectsInBoxes::SharedPtr msg)
  {
    struct timespec time_start={0, 0};
    clock_gettime(CLOCK_REALTIME, &time_start);
    static double last_sec = 0;
    static double last_nsec = 0;
    static int count = 0;
    double interval = 0;
    double current_sec = time_start.tv_sec;
    double current_nsec = time_start.tv_nsec;
    double msg_sec = msg->header.stamp.sec;
    double msg_nsec = msg->header.stamp.nanosec;

    count++;
    interval = (current_sec - last_sec) + ((current_nsec - last_nsec)/1000000000);
    if(last_sec == 0)
    {
       last_sec = current_sec;
       last_nsec = current_nsec;
       return;
    }

    if(interval >= 1.0)
    {
      double latency = (current_sec - msg_sec) + ((current_nsec - msg_nsec)/1000000000);
      double fps = count/interval;
      count = 0;
      last_sec = current_sec;
      last_nsec = current_nsec;
      RCLCPP_INFO(this->get_logger(), "fps %.3f hz, latency %.3f sec", fps, latency)
    }
  }
  rclcpp::Subscription<object_msgs::msg::ObjectsInBoxes>::SharedPtr subscription_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MinimalSubscriber>());
  rclcpp::shutdown();
  return 0;
}
