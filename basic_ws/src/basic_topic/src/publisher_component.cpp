#include "basic_topic/publisher_component.hpp"

using namespace std::chrono_literals;

namespace basic_topic
{

    PublisherComponent::PublisherComponent(const rclcpp::NodeOptions& options) :
        Node("publisher_node", options)
    {
        publisher_ = this->create_publisher<geometry_msgs::msg::Quaternion>("quaternion", 10);

        timer_ = this->create_wall_timer(
            500ms,
            [this]() { this->publish_quaternion(); });
    }

    double PublisherComponent::normalize_angle(double angle)
    {
        angle = std::fmod(angle + kPi, 2.0 * kPi);
        if (angle < 0.0)
            angle += 2.0 * kPi;
        return angle - kPi;
    }

    geometry_msgs::msg::Quaternion PublisherComponent::rpy_to_quaternion(double roll, double pitch, double yaw)
    {
        geometry_msgs::msg::Quaternion q;
        const double cy = std::cos(yaw * 0.5);
        const double sy = std::sin(yaw * 0.5);
        const double cp = std::cos(pitch * 0.5);
        const double sp = std::sin(pitch * 0.5);
        const double cr = std::cos(roll * 0.5);
        const double sr = std::sin(roll * 0.5);

        q.w = cr * cp * cy + sr * sp * sy;
        q.x = sr * cp * cy - cr * sp * sy;
        q.y = cr * sp * cy + sr * cp * sy;
        q.z = cr * cp * sy - sr * sp * cy;
        return q;
    }

    void PublisherComponent::publish_quaternion()
    {
        time_ += 0.1;

        const double roll = normalize_angle(std::sin(time_) * kPi / 2.0);
        const double pitch = normalize_angle(std::cos(time_) * kPi / 4.0);
        const double yaw = normalize_angle(time_);

        auto msg = rpy_to_quaternion(roll, pitch, yaw);

        publisher_->publish(msg);

        RCLCPP_INFO(
            this->get_logger(),
            "Publish RPY: roll=%.3f, pitch=%.3f, yaw=%.3f | Quaternion: w=%.3f, x=%.3f, y=%.3f, z=%.3f",
            roll, pitch, yaw,
            msg.w, msg.x, msg.y, msg.z);
    }

}  // namespace basic_topic

RCLCPP_COMPONENTS_REGISTER_NODE(basic_topic::PublisherComponent)
