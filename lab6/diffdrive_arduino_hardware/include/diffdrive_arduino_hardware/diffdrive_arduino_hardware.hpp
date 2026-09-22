#ifndef DIFFDRIVE_ARDUINO_HARDWARE__DIFFDRIVE_ARDUINO_HARDWARE_HPP_
#define DIFFDRIVE_ARDUINO_HARDWARE__DIFFDRIVE_ARDUINO_HARDWARE_HPP_

#include <string>
#include <vector>

#include "hardware_interface/system_interface.hpp"
#include "hardware_interface/types/hardware_interface_type_values.hpp"
#include "hardware_interface/handle.hpp"

#include "rclcpp/rclcpp.hpp"

// NEW: use wjwwood/serial
#include <serial/serial.h>

namespace diffdrive_arduino_hardware
{

class MecanumArduinoHardware : public hardware_interface::SystemInterface
{
public:
  RCLCPP_SHARED_PTR_DEFINITIONS(MecanumArduinoHardware)

  hardware_interface::CallbackReturn on_init(
    const hardware_interface::HardwareInfo & info) override;

  std::vector<hardware_interface::StateInterface> export_state_interfaces() override;

  std::vector<hardware_interface::CommandInterface> export_command_interfaces() override;

  hardware_interface::return_type read(
    const rclcpp::Time & time,
    const rclcpp::Duration & period) override;

  hardware_interface::return_type write(
    const rclcpp::Time & time,
    const rclcpp::Duration & period) override;

private:
  // ---- Serial helpers, now implemented with serial::Serial ----
  bool open_serial(const std::string & port, int baudrate);
  void close_serial();
  bool write_serial(const std::string & data);
  bool read_serial_line(std::string & line);

  // Parse ENC line: "ENC t_fl t_fr t_rr t_rl"
  bool parse_encoder_line(
    const std::string & line,
    std::vector<long long> & ticks_out);

  // ---- ROS 2 control state / command storage ----
  std::vector<double> hw_velocities_;          // wheel angular velocity [rad/s]
  std::vector<double> hw_commands_;            // wheel angular velocity commands [rad/s]
  std::vector<long long> last_encoder_ticks_;  // last raw tick values

  // Params
  double ticks_per_rev_ {0.0};
  bool reverse_wheels_ {false};          // optional global inversion

  // Serial (wjwwood/serial)
  serial::Serial serial_;
  std::string rx_buffer_;                // accumulate serial bytes into lines

  rclcpp::Logger logger_ {rclcpp::get_logger("MecanumArduinoHardware")};
};

}  // namespace diffdrive_arduino_hardware

#endif  // DIFFDRIVE_ARDUINO_HARDWARE__DIFFDRIVE_ARDUINO_HARDWARE_HPP_
