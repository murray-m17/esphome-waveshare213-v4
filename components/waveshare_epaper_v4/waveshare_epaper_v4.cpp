#include "waveshare_epaper_v4.h"
#include "esphome/core/log.h"

namespace esphome {
namespace waveshare_epaper_v4 {

void WaveshareEPaperV4::setup() {
  this->spi_setup();
  this->dc_pin_->setup();
  this->reset_pin_->setup();
  this->busy_pin_->setup();

  this->init_internal_(this->get_buffer_length_());
  
  // Start Init Sequence from your Python file
  this->reset_();
  this->wait_until_idle_();
  this->command(0x12); // SW Reset
  this->wait_until_idle_();

  this->command(0x01); // Driver output control
  this->data(0xF9); this->data(0x00); this->data(0x00); //

  this->command(0x11); // Data entry mode
  this->data(0x03);    // Portrait mode

  this->command(0x3C); // Border Waveform
  this->data(0x05);

  this->command(0x18); // Temp sensor
  this->data(0x80);
  this->wait_until_idle_();
}

void WaveshareEPaperV4::display() {
  this->command(0x24); // Write RAM
  this->write_array(this->buffer_, this->get_buffer_length_());

  this->command(0x22); // Display Update Control
  this->data(0xF7);    // V4 Full Update
  this->command(0x20); // Activate
  this->wait_until_idle_();
}

void WaveshareEPaperV4::update() {
  this->do_update_();
  this->display();
}

void WaveshareEPaperV4::command(uint8_t value) {
  this->dc_pin_->digital_write(false);
  this->enable();
  this->write_byte(value);
  this->disable();
}

void WaveshareEPaperV4::data(uint8_t value) {
  this->dc_pin_->digital_write(true);
  this->enable();
  this->write_byte(value);
  this->disable();
}

void WaveshareEPaperV4::reset_() {
  this->reset_pin_->digital_write(true);
  delay(20);
  this->reset_pin_->digital_write(false);
  delay(2);
  this->reset_pin_->digital_write(true);
  delay(20);
}

void WaveshareEPaperV4::wait_until_idle_() {
  while (this->busy_pin_->digital_read()) { delay(10); }
}

}  // namespace waveshare_epaper_v4
}  // namespace esphome
