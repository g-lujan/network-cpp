#pragma once

#ifndef _BUFFER_HPP__
#define _BUFFER_HPP__

template <typename T> class MutableBuffer {
public:
  MutableBuffer() noexcept : data_(0), size_(0) {}
  MutableBuffer(T *data, std::size_t size) noexcept : data_(data), size_(size) {}
  inline T *data() const noexcept { return data_; }
  inline std::size_t size() const noexcept { return size_; }

private:
  T *data_;
  std::size_t size_;
};

template <typename T> class FixedBuffer {
public:
  FixedBuffer() noexcept : data_(0), size_(0) {}
  FixedBuffer(const T *data, std::size_t size) : data_(data), size_(size) {}
  FixedBuffer(const MutableBuffer<T> &b) noexcept : data_(b.data()), size_(b.size()) {}

  inline const T *data() const noexcept { return data_; }
  inline std::size_t size() const noexcept { return size_; }

private:
  const T *data_;
  std::size_t size_;
};

#endif