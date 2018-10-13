//
//  version_text_sensor.cpp
//  esphomelib
//
//  Created by Otto Winter on 2018/10/12.
//  Copyright © 2018 Otto Winter. All rights reserved.
//


#include "esphomelib/defines.h"

#ifdef USE_VERSION_TEXT_SENSOR

#include "esphomelib/text_sensor/version_text_sensor.h"
#include "esphomelib/application.h"

ESPHOMELIB_NAMESPACE_BEGIN

namespace text_sensor {

void VersionTextSensor::setup() {
  if (App.get_compilation_time().empty()) {
    this->push_new_value(ESPHOMELIB_VERSION);
  } else {
    this->push_new_value(ESPHOMELIB_VERSION " " + App.get_compilation_time());
  }
}
float VersionTextSensor::get_setup_priority() const {
  return setup_priority::HARDWARE_LATE;
}
VersionTextSensor::VersionTextSensor(const std::string &name) : TextSensor(name) {}

} // namespace text_sensor

ESPHOMELIB_NAMESPACE_END

#endif //USE_VERSION_TEXT_SENSOR