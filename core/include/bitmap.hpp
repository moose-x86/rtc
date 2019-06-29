#pragma once

#include <boost/logic/tribool.hpp>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <optional>
#include <stdexcept>
#include <tuple>
#include <vector>

#include "utility.hpp"

namespace rtc
{
struct color_rgb
{
  using value_type = std::uint8_t;
  value_type red, green, blue;

  color_rgb() noexcept : red{}, green{}, blue{} {}
  color_rgb(std::uint8_t r, std::uint8_t g, std::uint8_t b) noexcept : red{r}, green{g}, blue{b} {}

  [[nodiscard]] auto revert() const noexcept
  {
    color_rgb c;
    c.red = (std::numeric_limits<std::uint8_t>::max() - red);
    c.green = (std::numeric_limits<std::uint8_t>::max() - green);
    c.blue = (std::numeric_limits<std::uint8_t>::max() - blue);
    return c;
  }

  auto operator==(const color_rgb& p) const noexcept -> bool
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

  auto operator==(const pixel& p) const noexcept -> bool
  {
    return std::forward_as_tuple(x, y, color) == std::forward_as_tuple(p.x, p.y, p.color);
  }
};

class bitmap : std::vector<rtc::pixel>
{
  using base = std::vector<rtc::pixel>;
  struct header
  {
    std::uint16_t bfType;           // File reference (always square value "BM")
    std::uint32_t bfSize;           // File size in bytes
    std::uint16_t bfReserved1;      // Reserved field
    std::uint16_t bfReserved2;      // Reserved field
    std::uint32_t bfOffBits;        // Illustrative photos in the file
    std::uint32_t biSize;           // size of the information header. The length from
                                    // here to the end of the header - 40, sometimes less
    std::uint32_t biWidth;          // The width of the image
    std::uint32_t biHeight;         // The height of the image
    std::uint16_t biPlanes;         // Number of lobes (usually 0)
    std::uint16_t biBitCount;       // Number of bytes per pixel
    std::uint32_t biCompression;    // Compression algorithm
    std::uint32_t biSizeImage;      // The size of the drawing itself
    std::uint32_t biXPelsPerMeter;  // Horizontal resolution
    std::uint32_t biYPelsPerMeter;  // Vertical resolution
    std::uint32_t biClrUsed;        // Number of colors in the palette
    std::uint8_t biClrImportant;    // The number of important colors in the palette
    std::uint8_t biCrlRotation;     // Flag indicating whether the palette should
                                    // rotate (default 0 - no rotation).
    std::uint16_t biReserved;       // Reserved field
  };

  explicit bitmap() noexcept;

 public:
  explicit bitmap(const std::string&);
  explicit bitmap(const std::uint16_t, const std::uint16_t);

  bitmap(const bitmap&) = default;
  bitmap(bitmap&&) noexcept = default;
  auto operator=(const bitmap&) -> bitmap& = default;
  auto operator=(bitmap&&) noexcept -> bitmap& = default;
  ~bitmap() = default;

  using base::begin;
  using base::const_iterator;
  using base::const_reverse_iterator;
  using base::end;
  using base::iterator;
  using base::rbegin;
  using base::rend;
  using base::reverse_iterator;
  using base::value_type;

  void load_from(const std::string&);
  [[nodiscard]] auto width() const noexcept -> std::size_t;
  [[nodiscard]] auto height() const noexcept -> std::size_t;
  void save(const std::string&) const;
  auto assign(const rtc::pixel&) noexcept -> bool;
  auto at(const std::uint16_t, const std::uint16_t) -> color_rgb&;
  auto resize(const std::uint16_t, const std::uint16_t) -> bitmap&;
  auto operator()(const std::uint16_t, const std::uint16_t) -> color_rgb&;
  [[nodiscard]] auto at(const std::uint16_t, const std::uint16_t) const -> const color_rgb&;
  auto operator()(const std::uint16_t, const std::uint16_t) const -> const color_rgb&;
  auto assign(const std::uint16_t, const std::uint16_t, const color_rgb&) noexcept -> bool;

  auto compare(const bitmap&, const std::function<bool(const rtc::color_rgb&, const rtc::color_rgb&)>&)
      -> boost::tribool;

  auto revert() noexcept -> bitmap&;
  auto operator!() const -> bitmap;
  auto swap(bitmap& bmp) noexcept(noexcept(std::declval<base>().swap(std::declval<base&>()))) -> bitmap&;
  [[nodiscard]] auto pixel_amount() const noexcept -> std::size_t;
  auto clear(const color_rgb& = {}) noexcept -> bitmap&;
  auto insert(const std::uint16_t, const std::uint16_t, const bitmap&) -> bitmap&;
  [[nodiscard]] auto trim(const std::uint16_t, const std::uint16_t, const std::uint16_t, const std::uint16_t) const
      -> bitmap;

  auto draw(const std::function<rtc::color_rgb(std::uint16_t, std::uint16_t)>&) noexcept -> bitmap&;

 private:
  void reset() noexcept;
  [[nodiscard]] auto in_range(const std::uint16_t p_x, const std::uint16_t p_y) const noexcept -> bool;
  header header_data;
};

inline auto operator==(const bitmap& a, const bitmap& b) noexcept -> bool
{
  return (a.pixel_amount() == b.pixel_amount()) && std::equal(a.begin(), a.end(), b.begin());
}

inline auto operator!=(const bitmap& a, const bitmap& b) noexcept -> bool { return !(a == b); }

}  // namespace rtc
