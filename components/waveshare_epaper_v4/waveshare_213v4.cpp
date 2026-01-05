#include "waveshare_213v4.h"
#include "esphome/core/log.h"

namespace esphome {
namespace waveshare_epaper {

static const char *const TAG = "waveshare_2.13v4";

void WaveshareEPaper2P13InV4::setup() {
  this->init_internal_(this->get_buffer_length_());
  this->reset_();
  this->wait_until_idle_();

  this->command(0x12); // SW Reset
  this->wait_until_idle_();

  this->command(0x01); // Driver output control
  this->data(0xF9); this->data(0x00); this->data(0x00);

  this->command(0x11); // Data entry mode
  this->data(0x03);

  this->set_window_(0, this->get_height_internal());
  
  this->command(0x3C); // Border Waveform
  this->data(0x05);

  this->command(0x18); // Temperature sensor
  this->data(0x80);

  this->wait_until_idle_();
}

void WaveshareEPaper2P13InV4::full_update_() {
  this->write_buffer_(0x24, 0, this->get_height_internal());
  this->write_buffer_(0x26, 0, this->get_height_internal()); // Display Base
  
  this->command(0x22); 
  this->data(0xF7); // V4 Full Update Code
  this->command(0x20); // Master Activation
  this->wait_until_idle_();
  this->is_busy_ = false;
}

void WaveshareEPaper2P13InV4::partial_update_() {
  this->command(0x3C); // Border for partial
  this->data(0x80);

  this->write_buffer_(0x24, 0, this->get_height_internal());
  
  this->command(0x22);
  this->data(0xFF); // V4 Partial Update Code
  this->command(0x20);
  this->wait_until_idle_();
  this->is_busy_ = false;
}

void WaveshareEPaper2P13InV4::display() {
  if (this->is_busy_) return;
  this->is_busy_ = true;
  
  // Decide between partial and full refresh
  bool partial = this->at_update_ != 0;
  this->at_update_ = (this->at_update_ + 1) % this->full_update_every_;
  
  if (partial) {
    this->partial_update_();
  } else {
    this->full_update_();
  }
}

void WaveshareEPaper2P13InV4::set_window_(int t, int b) {
  this->command(0x44); // RAM X
  this->data(0x00); this->data(0x0F); // 128 pixels
  this->command(0x45); // RAM Y
  this->data(t & 0xFF); this->data((t >> 8) & 0xFF);
  this->data(b & 0xFF); this->data((b >> 8) & 0xFF);
}

int WaveshareEPaper2P13InV4::get_width_internal() { return 122; }
int WaveshareEPaper2P13InV4::get_height_internal() { return 250; }

void WaveshareEPaper2P13InV4::dump_config() {
  LOG_DISPLAY("", "Waveshare E-Paper", this);
  ESP_LOGCONFIG(TAG, "  Model: 2.13inV4");
}

} // namespace waveshare_epaper
} // namespace esphome
