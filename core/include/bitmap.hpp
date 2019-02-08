#pragma once

#include <fstream>
#include <iostream>
#include <cstring>
#include <stdexcept>
#include <cstdint>
#include <cstdlib>
#include <limits>
#include <vector>
#include <tuple>
#include <functional>
#include "utility.hpp"

namespace rtc
{

struct color_rgb
{
  using value_type = std::uint8_t;
  value_type red, green, blue;

  color_rgb() noexcept : red{}, green{}, blue{} {}
  color_rgb(std::uint8_t r,
            std::uint8_t g,
            std::uint8_t b) noexcept : red{r}, green{g}, blue{b} {}

  auto revert() const noexcept
  {
    color_rgb c;
    c.red   = (std::numeric_limits<std::uint8_t>::max() - red);
    c.green = (std::numeric_limits<std::uint8_t>::max() - green);
    c.blue  = (std::numeric_limits<std::uint8_t>::max() - blue);
    return c;
  }

  bool operator==(const color_rgb& p) const noexcept
  {
    return p.red == red && p.green == green && p.blue == blue;
  }
};


struct pixel
{
  const std::uint16_t x, y;
  color_rgb color;

  pixel() noexcept : x{}, y{}, color{} {}
  pixel(std::uint16_t x, std::uint16_t y, const color_rgb& c = {}) noexcept : x{x}, y{y}, color{c} {}

  bool operator==(const pixel& p) const noexcept
  {
    return std::forward_as_tuple(x, y, color) == std::forward_as_tuple(p.x, p.y, p.color);
  }
};

class bitmap : std::vector<rtc::pixel>
{
  using base = std::vector<rtc::pixel>;
  struct header
  {
    std::uint16_t bfType; // File reference (always square value "BM")
    std::uint32_t bfSize; // File size in bytes
    std::uint16_t bfReserved1; // Reserved field
    std::uint16_t bfReserved2; // Reserved field
    std::uint32_t bfOffBits; // Illustrative photos in the file
    std::uint32_t biSize; // size of the information header. The length from here to the end of the header - 40, sometimes less
    std::uint32_t biWidth; // The width of the image
    std::uint32_t biHeight; // The height of the image
    std::uint16_t biPlanes; // Number of lobes (usually 0)
    std::uint16_t biBitCount; // Number of bytes per pixel
    std::uint32_t biCompression;  // Compression algorithm
    std::uint32_t biSizeImage; // The size of the drawing itself
    std::uint32_t biXPelsPerMeter;  // Horizontal resolution
    std::uint32_t biYPelsPerMeter;  // Vertical resolution
    std::uint32_t biClrUsed; // Number of colors in the palette
    std::uint8_t  biClrImportant; // The number of important colors in the palette
    std::uint8_t  biCrlRotation; // Flag indicating whether the palette should rotate (default 0 - no rotation).
    std::uint16_t biReserved; // Reserved field
  };

public:
  explicit bitmap() noexcept;
  explicit bitmap(const std::string&);
  explicit bitmap(const std::uint16_t, const std::uint16_t);

  bitmap(const bitmap&) = default;
  bitmap(bitmap&&) noexcept = default;
  bitmap& operator=(const bitmap&) = default;
  bitmap& operator=(bitmap&&) noexcept = default;
  ~bitmap() = default;

  using base::begin;
  using base::end;
  using base::rbegin;
  using base::rend;
  using base::iterator;
  using base::value_type;
  using base::const_iterator;
  using base::reverse_iterator;
  using base::const_reverse_iterator;

  void load_from(const std::string&);
  std::size_t width() const noexcept;
  std::size_t height() const noexcept;
  void save(const std::string&) const;
  bool assign(const rtc::pixel&) noexcept;
  color_rgb& at(const std::uint16_t, const std::uint16_t);
  bitmap& resize(const std::uint16_t, const std::uint16_t);
  color_rgb& operator()(const std::uint16_t, const std::uint16_t);
  const color_rgb& at(const std::uint16_t, const std::uint16_t) const;
  const color_rgb& operator()(const std::uint16_t, const std::uint16_t) const;
  bool assign(const std::uint16_t, const std::uint16_t, const color_rgb&) noexcept;

  bitmap& revert() noexcept;
  bitmap operator!() const;
  bitmap& swap(bitmap& bmp) noexcept(noexcept(std::declval<base>().swap(std::declval<base&>())));
  std::size_t pixel_amount() const noexcept;
  bitmap& clear(const color_rgb& = {}) noexcept;
  bitmap& insert(const std::uint16_t, const std::uint16_t, const bitmap&);
  bitmap trim(const std::uint16_t, const std::uint16_t, const std::uint16_t, const std::uint16_t) const;

  bitmap& draw(const std::function<rtc::color_rgb(std::uint16_t, std::uint16_t)>&) noexcept;

private:
  void reset() noexcept;
  bool in_range(const std::uint16_t p_x, const std::uint16_t p_y) const noexcept;
  header header_data;
};

inline bool operator==(const bitmap& a, const bitmap& b) noexcept
{
  return ( a.pixel_amount() == b.pixel_amount() ) && std::equal(a.begin(), a.end(), b.begin());
}

inline bool operator!=(const bitmap& a, const bitmap& b) noexcept
{
  return !(a == b);
}

}

