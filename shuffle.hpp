
#include <iostream>
#include <vector>
#include <cstddef>
#include <cstring>
#include <stdexcept>
#include <boost/preprocessor/cat.hpp>

#define DEFAULT_BLOCK_SIZE 1024

namespace Shuffle {
namespace detail {

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

}


// SHUFFLE -- TEMPLATED VERSION//
template<uint8_t DT> 
void shuffle(const unsigned char* data, unsigned char* target, size_t data_sz, uint16_t blocksize=DEFAULT_BLOCK_SIZE) {
  // variables
  blocksize -= blocksize % DT;
  if(!blocksize)
    throw std::logic_error("Block size cannot be smaller than the datatype size.");
  size_t fullblocks = data_sz / blocksize;
  uint16_t per_block = blocksize / DT;
  // general blocks
  for(size_t b=0; b<fullblocks; ++b)
    for(size_t j=0; j<per_block; ++j)
      detail::unroll_shuffle<DT>(data, target, b*blocksize, per_block, DT, blocksize, j);
  // last block
  per_block = (data_sz - fullblocks*blocksize) / DT;
  for(size_t j=0; j<per_block; ++j)
    detail::unroll_shuffle<DT>(data, target, blocksize*fullblocks, per_block, DT, per_block*DT, j);
  // copy what's left
  size_t last = blocksize*fullblocks + per_block*DT;
  memcpy(target + last, data + last, data_sz-last);
}

template<> 
void shuffle<1>(const unsigned char* data, unsigned char* target, size_t data_sz, uint16_t blocksize) { memcpy(target, data, data_sz); }

template<> 
void shuffle<0>(const unsigned char* data, unsigned char* target, size_t data_sz, uint16_t blocksize) { memcpy(target, data, data_sz); }

// SHUFFLE -- GENERAL VERSION//
void shuffle(const unsigned char* data, unsigned char* target, size_t data_sz, uint16_t datatype_size, uint16_t blocksize=DEFAULT_BLOCK_SIZE) {
  // variables
  blocksize -= blocksize % datatype_size;
  if(!blocksize)
    throw std::logic_error("Block size cannot be smaller than the datatype size.");
  size_t fullblocks = data_sz / blocksize;
  uint16_t per_block = blocksize / datatype_size;
  // general blocks
  for(size_t b=0; b<fullblocks; ++b)
    for(size_t j=0; j<per_block; ++j)
      for(uint16_t i=0; i<datatype_size; ++i)
        target[b*blocksize + ((j+1)*blocksize + j - (datatype_size-i)*per_block) % blocksize] = data[b*blocksize + j*datatype_size + i];
  // last block
  per_block = (data_sz - fullblocks*blocksize) / datatype_size;
  for(size_t j=0; j<per_block; ++j)
    for(uint16_t i=0; i<datatype_size; ++i)
      target[blocksize*fullblocks + ((j+1)*per_block*datatype_size + j - (datatype_size-i)*per_block) % (per_block*datatype_size)] = data[blocksize*fullblocks + j*datatype_size + i];
  // copy what's left
  size_t last = blocksize*fullblocks + per_block*datatype_size;
  memcpy(target + last, data + last, data_sz-last);
}

template<typename T>
T shuffle(const T& data, uint16_t blocksize=DEFAULT_BLOCK_SIZE) {
  size_t datatype_size = sizeof(typename T::value_type);
  size_t sz = data.size();
  // allocate target
  T result = T(sz);
  sz *= datatype_size;
  switch(datatype_size) {
    case 0:
    case 1:
      shuffle<1>(reinterpret_cast<const unsigned char*>(data.data()), reinterpret_cast<unsigned char*>(result.data()), sz, blocksize);
      break;
    case 2:
      shuffle<2>(reinterpret_cast<const unsigned char*>(data.data()), reinterpret_cast<unsigned char*>(result.data()), sz, blocksize);
      break;
    case 4:
      shuffle<4>(reinterpret_cast<const unsigned char*>(data.data()), reinterpret_cast<unsigned char*>(result.data()), sz, blocksize);
      break;
    case 8:
      shuffle<8>(reinterpret_cast<const unsigned char*>(data.data()), reinterpret_cast<unsigned char*>(result.data()), sz, blocksize);
      break;
    case 16:
      shuffle<16>(reinterpret_cast<const unsigned char*>(data.data()), reinterpret_cast<unsigned char*>(result.data()), sz, blocksize);
      break;
    case 32:
      shuffle<32>(reinterpret_cast<const unsigned char*>(data.data()), reinterpret_cast<unsigned char*>(result.data()), sz, blocksize);
      break;
    case 64:
      shuffle<64>(reinterpret_cast<const unsigned char*>(data.data()), reinterpret_cast<unsigned char*>(result.data()), sz, blocksize);
      break;
    default:
      shuffle(reinterpret_cast<const unsigned char*>(data.data()), reinterpret_cast<unsigned char*>(result.data()), sz, datatype_size, blocksize);
  }
  return result;
}



// UNSHUFFLE -- TEMPLATED VERSION//
template<uint8_t DT> 
void unshuffle(const unsigned char* data, unsigned char* target, size_t data_sz, uint16_t blocksize=DEFAULT_BLOCK_SIZE) {
  // variables
  blocksize -= blocksize % DT;
  if(!blocksize)
    throw std::logic_error("Block size cannot be smaller than the datatype size.");
  size_t fullblocks = data_sz / blocksize;
  uint16_t per_block = blocksize / DT;
  // general blocks
  for(size_t b=0; b<fullblocks; ++b)
    for(size_t j=0; j<per_block; ++j)
      detail::unroll_unshuffle<DT>(data, target, b*blocksize, per_block, DT, blocksize, j);
  // last block
  per_block = (data_sz - fullblocks*blocksize) / DT;
  for(size_t j=0; j<per_block; ++j)
    detail::unroll_unshuffle<DT>(data, target, blocksize*fullblocks, per_block, DT, per_block*DT, j);
  // copy what's left
  size_t last = blocksize*fullblocks + per_block*DT;
  memcpy(target + last, data + last, data_sz-last);
}

template<> 
void unshuffle<1>(const unsigned char* data, unsigned char* target, size_t data_sz, uint16_t blocksize) { memcpy(target, data, data_sz); }

template<> 
void unshuffle<0>(const unsigned char* data, unsigned char* target, size_t data_sz, uint16_t blocksize) { memcpy(target, data, data_sz); }

// UNSHUFFLE -- GENERAL VERSION//
void unshuffle(const unsigned char* data, unsigned char* target, size_t data_sz, uint16_t datatype_size, uint16_t blocksize=DEFAULT_BLOCK_SIZE) {
  // variables
  blocksize -= blocksize % datatype_size;
  if(!blocksize)
    throw std::logic_error("Block size cannot be smaller than the datatype size.");
  size_t fullblocks = data_sz / blocksize;
  uint16_t per_block = blocksize / datatype_size;
  // general blocks
  for(size_t b=0; b<fullblocks; ++b)
    for(size_t j=0; j<per_block; ++j)
      for(uint16_t i=0; i<datatype_size; ++i)
        target[b*blocksize + j*datatype_size + i] = data[b*blocksize + ((j+1)*blocksize + j - (datatype_size-i)*per_block) % blocksize];
  // last block
  per_block = (data_sz - fullblocks*blocksize) / datatype_size;
  for(size_t j=0; j<per_block; ++j)
    for(uint16_t i=0; i<datatype_size; ++i)
      target[blocksize*fullblocks + j*datatype_size + i] = data[blocksize*fullblocks + ((j+1)*per_block*datatype_size + j - (datatype_size-i)*per_block) % (per_block*datatype_size)];
  // copy what's left
  size_t last = blocksize*fullblocks + per_block*datatype_size;
  memcpy(target + last, data + last, data_sz-last);
}

template<typename T>
T unshuffle(const T& data, uint16_t blocksize=DEFAULT_BLOCK_SIZE) {
  size_t datatype_size = sizeof(typename T::value_type);
  size_t sz = data.size();
  // allocate target
  T result = T(sz);
  sz *= datatype_size;
  switch(datatype_size) {
    case 0:
    case 1:
      unshuffle<1>(reinterpret_cast<const unsigned char*>(data.data()), reinterpret_cast<unsigned char*>(result.data()), sz, blocksize);
      break;
    case 2:
      unshuffle<2>(reinterpret_cast<const unsigned char*>(data.data()), reinterpret_cast<unsigned char*>(result.data()), sz, blocksize);
      break;
    case 4:
      unshuffle<4>(reinterpret_cast<const unsigned char*>(data.data()), reinterpret_cast<unsigned char*>(result.data()), sz, blocksize);
      break;
    case 8:
      unshuffle<8>(reinterpret_cast<const unsigned char*>(data.data()), reinterpret_cast<unsigned char*>(result.data()), sz, blocksize);
      break;
    case 16:
      unshuffle<16>(reinterpret_cast<const unsigned char*>(data.data()), reinterpret_cast<unsigned char*>(result.data()), sz, blocksize);
      break;
    case 32:
      unshuffle<32>(reinterpret_cast<const unsigned char*>(data.data()), reinterpret_cast<unsigned char*>(result.data()), sz, blocksize);
      break;
    case 64:
      unshuffle<64>(reinterpret_cast<const unsigned char*>(data.data()), reinterpret_cast<unsigned char*>(result.data()), sz, blocksize);
      break;
    default:
      unshuffle(reinterpret_cast<const unsigned char*>(data.data()), reinterpret_cast<unsigned char*>(result.data()), sz, datatype_size, blocksize);
  }
  return result;
}





// template<int8_t DT>
// std::vector<unsigned char> unshuffle(const std::vector<unsigned char>& data, uint16_t blocksize=DEFAULT_BLOCK_SIZE) {
//   size_t sz = data.size();
//   // allocate target
//   std::vector<unsigned char> result(sz);
//   // call pointer version
//   unshuffle<DT>(data.data(), result.data(), sz, blocksize);
//   return result;
// }




}
