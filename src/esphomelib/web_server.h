//
//  web_server.h
//  esphomelib
//
//  Created by Otto Winter on 14.04.18.
//  Copyright © 2018 Otto Winter. All rights reserved.
//

#ifndef ESPHOMELIB_WEB_SERVER_H
#define ESPHOMELIB_WEB_SERVER_H

#include "esphomelib/component.h"
#include "esphomelib/controller.h"
#include "esphomelib/switch_/switch.h"
#include "esphomelib/defines.h"

#include <vector>

#ifdef USE_WEB_SERVER

#include <ESPAsyncWebServer.h>

ESPHOMELIB_NAMESPACE_BEGIN

/// Internal helper struct that is used to parse incoming URLs
struct UrlMatch {
  std::string domain; ///< The domain of the component, for example "sensor"
  std::string id; ///< The id of the device that's being aceesed, for example "living_room_fan"
  std::string method; ///< The method that's being called, for example "turn_on"
  bool valid; ///< Whether this match is valid
};

/** This class allows users to create a web server with their ESP nodes.
 *
 * Behind the scenes it's using AsyncWebServer to set up the server. It exposes 3 things:
 * an index page under '/' that's used to show a simple web interface (the css/js is hosted
 * by esphomelib.com by default), an event source under '/events' that automatically sends
 * all state updates in real time + the debug log. Lastly, there's an REST API available
 * under the '/light/...', '/sensor/...', ... URLs. A full documentation for this API
 * can be found under https://esphomelib.com/web-api/index.html.
 *
 * Additionally, the web server is advertised via mDNS.
 */
class WebServer : public StoringController, public Component, public AsyncWebHandler {
 public:
  /// Initialize the web server with the specified port
  explicit WebServer(uint16_t port);

  /** Set the URL to the CSS <link> that's sent to each client. Defaults to
   * https://esphomelib.com/_static/webserver-v1.min.css
   *
   * @param css_url The url to the web server stylesheet.
   */
  void set_css_url(const char *css_url);

  /** Set the URL to the script that's embedded in the index page. Defaults to
   * https://esphomelib.com/_static/webserver-v1.min.js
   *
   * @param js_url The url to the web server script.
   */
  void set_js_url(const char *js_url);

  /// Set the web server port.
  void set_port(uint16_t port);

  // ========== INTERNAL METHODS ==========
  // (In most use cases you won't need these)
  /// Setup the internal web server and register handlers.
  void setup() override;

  /// MQTT setup priority.
  float get_setup_priority() const override;

  /// Handle an index request under '/'.
  void handle_index_request(AsyncWebServerRequest *request);

#ifdef USE_SENSOR
  /// Internally register a sensor and set a callback on state changes.
  void register_sensor(sensor::Sensor *obj) override;

  /// Handle a sensor request under '/sensor/<id>'.
  void handle_sensor_request(AsyncWebServerRequest *request, UrlMatch match);

  /// Dump the sensor state with its value as a JSON string.
  std::string sensor_json(sensor::Sensor *obj, float value);
#endif

#ifdef USE_SWITCH
  /// Internally register a switch and set a callback on state changes.
  void register_switch(switch_::Switch *obj) override;

  /// Handle a switch request under '/switch/<id>/</turn_on/turn_off/toggle>'.
  void handle_switch_request(AsyncWebServerRequest *request, UrlMatch match);

  /// Dump the switch state with its value as a JSON string.
  std::string switch_json(switch_::Switch *obj, bool value);
#endif

#ifdef USE_BINARY_SENSOR
  /// Internally register a binary sensor and set a callback on state changes.
  void register_binary_sensor(binary_sensor::BinarySensor *obj) override;

  /// Handle a binary sensor request under '/binary_sensor/<id>'.
  void handle_binary_sensor_request(AsyncWebServerRequest *request, UrlMatch match);

  /// Dump the binary sensor state with its value as a JSON string.
  std::string binary_sensor_json(binary_sensor::BinarySensor *obj, bool value);
#endif

#ifdef USE_FAN
  /// Internally register a fan and set a callback on state changes.
  void register_fan(fan::FanState *obj) override;

  /// Handle a fan request under '/fan/<id>/</turn_on/turn_off/toggle>'.
  void handle_fan_request(AsyncWebServerRequest *request, UrlMatch match);

  /// Dump the fan state as a JSON string.
  std::string fan_json(fan::FanState *obj);
#endif

#ifdef USE_LIGHT
  /// Internally register a light and set a callback on state changes.
  void register_light(light::LightState *obj) override;

  /// Handle a light request under '/light/<id>/</turn_on/turn_off/toggle>'.
  void handle_light_request(AsyncWebServerRequest *request, UrlMatch match);

  /// Dump the light state as a JSON string.
  std::string light_json(light::LightState *obj);
#endif

  /// Override the web handler's canHandle method.
  bool canHandle(AsyncWebServerRequest *request) override;
  /// Override the web handler's handleRequest method.
  void handleRequest(AsyncWebServerRequest *request) override;
  /// This web handle is not trivial.
  bool isRequestHandlerTrivial() override;

 protected:
  uint16_t port_;
  AsyncWebServer *server_;
  AsyncEventSource events_{"/events"};
  const char *css_url_{nullptr};
  const char *js_url_{nullptr};
};

ESPHOMELIB_NAMESPACE_END

#endif //USE_WEB_SERVER

#endif //ESPHOMELIB_WEB_SERVER_H
