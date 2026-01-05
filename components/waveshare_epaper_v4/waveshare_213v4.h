#pragma once

#include "waveshare_epaper.h"

namespace esphome {
namespace waveshare_epaper {

class WaveshareEPaper2P13InV4 : public WaveshareEPaper {
 public:
  void setup() override;
  void display() override;
  void dump_config() override;

 protected:
  void set_window_(int t, int b);
  int get_width_internal() override;
  int get_height_internal() override;
  void partial_update_();
  void full_update_();
};

}
}
