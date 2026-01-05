#pragma once

#include "esphome/core/component.h"
#include "esphome/components/display/display_buffer.h"
#include "esphome/components/spi/spi.h"

namespace esphome {
namespace waveshare_epaper_v4 {

class WaveshareEPaperV4 : public PollingComponent, public display::DisplayBuffer, public spi::SPIDevice<spi::BIT_ORDER_MSB_FIRST, spi::CLOCK_POLARITY_LOW, spi::CLOCK_PHASE_LEADING, spi::DATA_RATE_4MHZ> {
 public:
  void set_dc_pin(GPIOPin *dc) { dc_pin_ = dc; }
  void set_reset_pin(GPIOPin *reset) { reset_pin_ = reset; }
  void set_busy_pin(GPIOPin *busy) { busy_pin_ = busy; }

  float get_setup_priority() const override { return setup_priority::PROCESSOR; }
  void setup() override;
  void update() override;
  void display();

 protected:
  int get_width_internal() override { return 122; }
  int get_height_internal() override { return 250; }
  
  void command(uint8_t value);
  void data(uint8_t value);
  void reset_();
  void wait_until_idle_();

  GPIOPin *dc_pin_;
  GPIOPin *reset_pin_;
  GPIOPin *busy_pin_;
};

}
}
