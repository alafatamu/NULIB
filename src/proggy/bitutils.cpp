#include "proggy/bitutils.hpp"

namespace bitutils{
  uint8_t firstbyte(uint16_t word_16){
    return (word_16>>8)&0xff;
  }
}