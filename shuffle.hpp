

#include <stdint.h>
#include <iostream>
#include <vector>
#include <string.h>

namespace Shuffle {

template<uint8_t i>
void unroll_shuffle(const unsigned char* data, unsigned char* target, size_t block_start, uint16_t per_block, uint8_t dtypesize, uint32_t block_size, size_t j) {
  target[block_start + ((j+1)*block_size + j - i*per_block) % block_size] = data[block_start + j*dtypesize + (dtypesize-i)];
  unroll_shuffle<i-1>(data, target, block_start, per_block, dtypesize, block_size, j);
}
template<>
void unroll_shuffle<0>(const unsigned char* data, unsigned char* target, size_t block_start, uint16_t per_block, uint8_t dtypesize, uint32_t block_size, size_t j) { }

template<uint8_t i>
void unroll_unshuffle(const unsigned char* data, unsigned char* target, size_t block_start, uint16_t per_block, uint8_t dtypesize, uint32_t block_size, size_t j) {
  target[block_start + j*dtypesize + (dtypesize-i)] = data[block_start + ((j+1)*block_size + j - i*per_block) % block_size];
  unroll_unshuffle<i-1>(data, target, block_start, per_block, dtypesize, block_size, j);
}
template<>
void unroll_unshuffle<0>(const unsigned char* data, unsigned char* target, size_t block_start, uint16_t per_block, uint8_t dtypesize, uint32_t block_size, size_t j) { }

template<uint8_t DT> 
void shuffle(const unsigned char* data, unsigned char* target, size_t data_sz, uint16_t blocksize=1024) {
  // variables
  blocksize -= blocksize % DT;
  size_t fullblocks = data_sz / blocksize;
  uint16_t per_block = blocksize / DT;
  // general blocks
  for(size_t b=0; b<fullblocks; ++b)
    for(size_t j=0; j<per_block; ++j)
      unroll_shuffle<DT>(data, target, b*blocksize, per_block, DT, blocksize, j);
  // last block
  per_block = (data_sz - fullblocks*blocksize) / DT;
  for(size_t j=0; j<per_block; ++j)
    unroll_shuffle<DT>(data, target, blocksize*fullblocks, per_block, DT, per_block*DT, j);
  // copy what's left
  size_t last = blocksize*fullblocks + per_block*DT;
  memcpy(target + last, data + last, data_sz-last);
}

template<> 
void shuffle<1>(const unsigned char* data, unsigned char* target, size_t data_sz, uint16_t blocksize) {
  memcpy(target, data, data_sz);
}

template<uint8_t DT> 
void unshuffle(const unsigned char* data, unsigned char* target, size_t data_sz, uint16_t blocksize=1024) {
  // variables
  blocksize -= blocksize % DT;
  size_t fullblocks = data_sz / blocksize;
  uint16_t per_block = blocksize / DT;
  // general blocks
  for(size_t b=0; b<fullblocks; ++b)
    for(size_t j=0; j<per_block; ++j)
      unroll_unshuffle<DT>(data, target, b*blocksize, per_block, DT, blocksize, j);
  // last block
  per_block = (data_sz - fullblocks*blocksize) / DT;
  for(size_t j=0; j<per_block; ++j)
    unroll_unshuffle<DT>(data, target, blocksize*fullblocks, per_block, DT, per_block*DT, j);
  // copy what's left
  size_t last = blocksize*fullblocks + per_block*DT;
  memcpy(target + last, data + last, data_sz-last);
}

template<> 
void unshuffle<1>(const unsigned char* data, unsigned char* target, size_t data_sz, uint16_t blocksize) {
  memcpy(target, data, data_sz);
}

template<int8_t DT>
std::vector<unsigned char> shuffle(const std::vector<unsigned char>& data, uint16_t blocksize=1024) {
  size_t sz = data.size();
  // allocate target
  std::vector<unsigned char> result(sz);
  // call pointer version
  shuffle<DT>(data.data(), result.data(), sz, blocksize);
  return result;
}

template<int8_t DT>
std::vector<unsigned char> unshuffle(const std::vector<unsigned char>& data, uint16_t blocksize=1024) {
  size_t sz = data.size();
  // allocate target
  std::vector<unsigned char> result(sz);
  // call pointer version
  unshuffle<DT>(data.data(), result.data(), sz, blocksize);
  return result;
}

template<typename T>
T shuffle(const T& data, uint16_t blocksize=1024) {
  size_t datatype_size = sizeof(typename T::value_type);
  
  
  return T();
}




}
