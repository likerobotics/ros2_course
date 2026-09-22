#include "diffdrive_arduino_hardware/diffdrive_arduino_hardware.hpp"

#include <cmath>
#include <cstring>
#include <sstream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>

#include "pluginlib/class_list_macros.hpp"

namespace diffdrive_arduino_hardware
{

using hardware_interface::CallbackReturn;
using hardware_interface::return_type;

CallbackReturn MecanumArduinoHardware::on_init(
  const hardware_interface::HardwareInfo & info)
{
  if (hardware_interface::SystemInterface::on_init(info) != CallbackReturn::SUCCESS) {
    RCLCPP_ERROR(logger_, "Base SystemInterface::on_init() failed");
    return CallbackReturn::ERROR;
  }

  if (info_.joints.empty()) {
    RCLCPP_ERROR(logger_, "No joints specified in hardware info");
    return CallbackReturn::ERROR;
  }

  const auto & params = info_.hardware_parameters;

  if (params.find("port") == params.end() ||
      params.find("baudrate") == params.end() ||
      params.find("ticks_per_rev") == params.end())
  {
    RCLCPP_ERROR(
      logger_,
      "Parameters 'port', 'baudrate', and 'ticks_per_rev' are required in ros2_control hardware block");
    return CallbackReturn::ERROR;
  }

  const std::string port = params.at("port");
  int baudrate = std::stoi(params.at("baudrate"));
  ticks_per_rev_ = std::stod(params.at("ticks_per_rev"));

  if (ticks_per_rev_ <= 0.0) {
    RCLCPP_ERROR(logger_, "ticks_per_rev must be > 0 (got %f)", ticks_per_rev_);
    return CallbackReturn::ERROR;
  }

  if (params.find("reverse_wheels") != params.end()) {
    reverse_wheels_ = (params.at("reverse_wheels") == "true");
  }

  // Resize state / command vectors to number of joints
  const std::size_t n = info_.joints.size();
  hw_velocities_.assign(n, 0.0);
  hw_commands_.assign(n, 0.0);
  last_encoder_ticks_.assign(n, 0);

  // Open serial using serial::Serial
  if (!open_serial(port, baudrate)) {
    RCLCPP_ERROR(logger_, "Failed to open serial port %s", port.c_str());
    return CallbackReturn::ERROR;
  }

  RCLCPP_INFO(
    logger_,
    "Initialized MecanumArduinoHardware with %zu joints on port %s @ %d baud, ticks_per_rev=%f",
    n, port.c_str(), baudrate, ticks_per_rev_);

  return CallbackReturn::SUCCESS;
}

std::vector<hardware_interface::StateInterface>
MecanumArduinoHardware::export_state_interfaces()
{
  std::vector<hardware_interface::StateInterface> state_interfaces;
  state_interfaces.reserve(info_.joints.size());

  for (std::size_t i = 0; i < info_.joints.size(); ++i) {
    state_interfaces.emplace_back(
      hardware_interface::StateInterface(
        info_.joints[i].name,
        hardware_interface::HW_IF_VELOCITY,
        &hw_velocities_[i]));
  }

  return state_interfaces;
}

std::vector<hardware_interface::CommandInterface>
MecanumArduinoHardware::export_command_interfaces()
{
  std::vector<hardware_interface::CommandInterface> command_interfaces;
  command_interfaces.reserve(info_.joints.size());

  for (std::size_t i = 0; i < info_.joints.size(); ++i) {
    command_interfaces.emplace_back(
      hardware_interface::CommandInterface(
        info_.joints[i].name,
        hardware_interface::HW_IF_VELOCITY,
        &hw_commands_[i]));
  }

  return command_interfaces;
}

return_type MecanumArduinoHardware::read(
  const rclcpp::Time & /*time*/,
  const rclcpp::Duration & period)
{
  if (!serial_.isOpen()) {
    RCLCPP_ERROR(logger_, "Serial port not open in read()");
    return return_type::ERROR;
  }

  // Try to get as many lines as are available; use the last ENC line
  std::string line;
  std::vector<long long> ticks;
  bool got_enc = false;

  while (read_serial_line(line)) {
    if (line.rfind("ENC ", 0) == 0) {  // starts with "ENC "
      std::vector<long long> t;
      if (parse_encoder_line(line, t)) {
        ticks = std::move(t);
        got_enc = true;
      }
    }
  }

  if (!got_enc) {
    // No new encoder data this cycle – keep previous velocities
    return return_type::OK;
  }

  if (ticks.size() != hw_velocities_.size()) {
    RCLCPP_WARN(
      logger_,
      "ENC line contained %zu ticks but %zu joints configured",
      ticks.size(), hw_velocities_.size());
    return return_type::OK;
  }

  const double dt = period.seconds();
  if (dt <= 0.0) {
    return return_type::OK;
  }

  const double rad_per_tick = 2.0 * M_PI / ticks_per_rev_;

  for (std::size_t i = 0; i < hw_velocities_.size(); ++i) {
    const long long dticks = ticks[i] - last_encoder_ticks_[i];
    last_encoder_ticks_[i] = ticks[i];

    const double dtheta = static_cast<double>(dticks) * rad_per_tick;  // [rad]
    double vel = dtheta / dt;                                          // [rad/s]

    if (reverse_wheels_) {
      vel = -vel;
    }

    hw_velocities_[i] = vel;
  }

  return return_type::OK;
}

return_type MecanumArduinoHardware::write(
  const rclcpp::Time & /*time*/,
  const rclcpp::Duration & /*period*/)
{
  if (!serial_.isOpen()) {
    RCLCPP_ERROR(logger_, "Serial port not open in write()");
    return return_type::ERROR;
  }

  std::ostringstream ss;
  ss << "SET ";

  for (std::size_t i = 0; i < hw_commands_.size(); ++i) {
    double vel_cmd = hw_commands_[i];  // [rad/s]

    if (reverse_wheels_) {
      vel_cmd = -vel_cmd;
    }

    ss << vel_cmd;
    if (i + 1 < hw_commands_.size()) {
      ss << ' ';
    }
  }
  ss << '\n';

  if (!write_serial(ss.str())) {
    RCLCPP_ERROR(logger_, "Failed to write SET command to serial");
    return return_type::ERROR;
  }

  return return_type::OK;
}

// ---------- Serial helpers using serial::Serial ----------

bool MecanumArduinoHardware::open_serial(const std::string & port, int baudrate)
{
  try {
    close_serial();  // close if already open

    serial_.setPort(port);
    serial_.setBaudrate(static_cast<uint32_t>(baudrate));

    // Small timeout so read() is effectively non-blocking in a cycle
    serial::Timeout to = serial::Timeout::simpleTimeout(10);  // 10 ms
    serial_.setTimeout(to);

    serial_.open();

    if (!serial_.isOpen()) {
      RCLCPP_ERROR(logger_, "serial::Serial failed to open port %s", port.c_str());
      return false;
    }

    // *** KEY PART: emulate what a terminal does ***

    // Assert DTR/RTS so the Arduino sees a "real" host
    serial_.setDTR(true);
    serial_.setRTS(true);

    // Give the board a moment to reset / start up (many boards auto-reset on DTR)
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    // Flush any junk that may have been sent during reset
    serial_.flushInput();
    serial_.flushOutput();

    RCLCPP_INFO(logger_, "Opened serial port %s", port.c_str());
    return true;
  }
  catch (const serial::IOException & e) {
    RCLCPP_ERROR(logger_, "IOException while opening %s: %s", port.c_str(), e.what());
    return false;
  }
  catch (const std::exception & e) {
    RCLCPP_ERROR(logger_, "Exception while opening %s: %s", port.c_str(), e.what());
    return false;
  }
}


void MecanumArduinoHardware::close_serial()
{
  if (serial_.isOpen()) {
    serial_.close();
    RCLCPP_INFO(logger_, "Closed serial port");
  }
}

bool MecanumArduinoHardware::write_serial(const std::string & data)
{
  if (!serial_.isOpen()) {
    return false;
  }

  try {
    serial_.write(reinterpret_cast<const uint8_t *>(data.data()), data.size());
    return true;
  }
  catch (const serial::IOException & e) {
    RCLCPP_ERROR(logger_, "serial::write() failed: %s", e.what());
    return false;
  }
}

bool MecanumArduinoHardware::read_serial_line(std::string & line)
{
  if (!serial_.isOpen()) {
    return false;
  }

  // Non-blocking-ish: read whatever is available, accumulate into rx_buffer_
  try {
    const size_t available = serial_.available();
    if (available > 0) {
      std::string chunk = serial_.read(available);
      rx_buffer_.append(chunk);
    }
  }
  catch (const serial::IOException & e) {
    RCLCPP_ERROR(logger_, "serial::read() failed: %s", e.what());
    return false;
  }

  // Look for newline in rx_buffer_
  auto pos = rx_buffer_.find('\n');
  if (pos != std::string::npos) {
    line = rx_buffer_.substr(0, pos);
    rx_buffer_.erase(0, pos + 1);

    // Trim CR if present
    if (!line.empty() && line.back() == '\r') {
      line.pop_back();
    }

    return true;
  }

  return false;
}

bool MecanumArduinoHardware::parse_encoder_line(
  const std::string & line,
  std::vector<long long> & ticks_out)
{
  // Expected format: "ENC t_fl t_fr t_rr t_rl"
  ticks_out.clear();

  std::istringstream iss(line);
  std::string tag;
  iss >> tag;
  if (tag != "ENC") {
    return false;
  }

  long long value = 0;
  while (iss >> value) {
    ticks_out.push_back(value);
  }

  if (ticks_out.empty()) {
    RCLCPP_WARN(logger_, "ENC line has no values: '%s'", line.c_str());
    return false;
  }

  return true;
}

}  // namespace diffdrive_arduino_hardware

PLUGINLIB_EXPORT_CLASS(
  diffdrive_arduino_hardware::MecanumArduinoHardware,
  hardware_interface::SystemInterface)
