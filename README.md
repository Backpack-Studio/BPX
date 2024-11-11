# BPX - Backpack Pixel

**BPX** (Backpack Pixel) is a lightweight and versatile C++ library for image manipulation, designed to support a wide range of image processing needs. Part of the Backpack ecosystem, BPX offers a straightforward API for image loading, processing, and saving, with support for various pixel formats and blending modes.

BPX can be used independently or alongside other Backpack libraries like Backpack Mathematics, providing a powerful toolkit for graphics applications, image processing, and more.

## Features

- **Load/Save Images:** Supports multiple image formats, including JPEG, PNG, BMP, PSD, GIF, and more.
- **Pixel Manipulation:** Directly manipulate individual pixels, colors, and regions with ease.
- **Color Operations:** Includes functions for brightness, contrast, saturation, grayscale, and inversion.
- **Geometric Drawing Primitives:** Draw lines, rectangles, and circles with customizable thickness, gradients, and blending modes.
- **Customizable Blending Modes:** Blend images and colors using various blending techniques.
- **Efficient Memory Management:** Flexible image creation from static memory or external sources with ownership handling.

## Table of Contents
1. [Installation](#installation)
2. [Usage](#usage)
3. [API Reference](#api-reference)
    - [Image Loading & Creation](#image-loading--creation)
    - [Pixel Manipulation](#pixel-manipulation)
    - [Color Operations](#color-operations)
    - [Geometric Primitives](#geometric-primitives)
4. [Examples](#examples)

---

## Installation

### Requirements
- **C++17** or newer _(currently C++11 or newer, but not guaranteed)_
- Optional: [SDL2](https://libsdl.org/download-2.0.php) (for building examples)

### Building the Library

Clone the repository and navigate to the project folder:
```bash
git clone https://github.com/Backpack-Studio/BPX.git
cd BPX
```

Use the provided `CMakeLists.txt` to build BPX. To build the library alone:
```bash
mkdir build
cd build
cmake ..
make
```

### Building Examples

To build the SDL example, ensure SDL2 is installed. The CMake script will automatically detect SDL2 and build the example if it’s available. Otherwise, it will skip the example with a warning.

To enable the example manually:
```bash
cmake -DBUILD_EXAMPLES=ON ..
make
```

---

## Usage

### Basic Image Loading and Saving

Load an image from a file, manipulate it, and save it back:

```cpp
#include <BPX/BPX.hpp>

int main() {
    bpx::Image image("input.jpg");
    image = bpx::brightness(image, 1.2f); // Increase brightness
    bpx::write_png(image, "output.png");  // Save as PNG
    return 0;
}
```

### Creating a Blank Image

Create an image filled with a specific color:

```cpp
bpx::Image blank_image(100, 100, bpx::RED); // Solid red, 100x100
```

### Manipulating Pixels

Set individual pixel colors:

```cpp
image.set(10, 10, bpx::GREEN);      // Set pixel at (10, 10) to green
```

---

## API Reference

### Image Loading & Creation

#### `Image(const std::string& file_path, bool flip_vertically = false)`
Loads an image from a file. Supported formats:
- JPEG, PNG, BMP, PSD, GIF, HDR, PIC, and PNM.

The `flip_vertically` parameter is useful for texture loading in OpenGL.

#### `Image(int w, int h, Color color = BLANK, PixelFormat format = PixelFormat::RGBA_U8)`
Creates a solid-colored image of specified dimensions and pixel format.

#### `Image(const void* pixels, int w, int h, PixelFormat format)`
Copies pixel data from an existing buffer into a new image.

#### `Image(void* pixels, int w, int h, PixelFormat format, bool owned)`
Uses existing pixel data without copying. If `owned` is true, memory is freed when the `Image` is destroyed.

---

### Pixel Manipulation

#### `Color get(int x, int y) const`
Retrieves the color of the pixel at coordinates `(x, y)`.

#### `Image& set(int x, int y, Color color)`
Sets the color of the pixel at `(x, y)`.

---

### Color Operations

#### `Image& brightness(Image& image, float factor)`
Adjusts brightness. A factor > 1 increases brightness, while a factor < 1 reduces it.

#### `Image& contrast(Image& image, float factor)`
Adjusts contrast, with `factor = 1` leaving contrast unchanged.

#### `Image& saturation(Image& image, float factor)`
Adjusts saturation. A factor > 1 increases saturation, and a factor < 1 reduces it.

#### `Image& invert(Image& image)`
Inverts colors in the image, producing a negative effect.

---

### Geometric Primitives

#### Drawing Lines

```cpp
bpx::line(image, 10, 10, 100, 100, 2, bpx::Color(255, 0, 0));
```

#### Drawing Rectangles

```cpp
bpx::rectangle(image, 20, 20, 80, 80, bpx::Color(0, 0, 255));
```

#### Drawing Circles

```cpp
bpx::circle(image, 50, 50, 25, bpx::Color(0, 255, 0));
```

---

## Examples

### Example: Creating an Image with a Gradient

```cpp
#include <BPX/BPX.hpp>

int main() {
    bpx::Image gradient_image(200, 200);
    bpx::Color start_color(255, 0, 0);
    bpx::Color end_color(0, 0, 255);

    for (int y = 0; y < gradient_image.height(); ++y) {
        float t = static_cast<float>(y) / gradient_image.height();
        bpx::Color color = bpx::lerp(start_color, end_color, t);
        for (int x = 0; x < gradient_image.width(); ++x) {
            gradient_image.set(x, y, color);
        }
    }

    bpx::write_png(gradient_image, "gradient.png");
    return 0;
}
```

### Example: Displaying an Image Using SDL2

```cpp
#include <BPX/BPX.hpp>
#include <SDL2/SDL.h>

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("BPX Example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);

    SDL_Surface* sdl_surface = SDL_GetWindowSurface(window);

    bpx::Image bpx_surface = {
        sdl_surface->pixels,
        sdl_surface->w, sdl_surface->h,
        bpx::PixelFormat::BGRA_U8, false
    };

    bpx::Image image("input.jpg");

    bpx::draw(
        bpx_surface, 0, 0, bpx_surface.width(), bpx_surface.height(),
        image, 0, 0, image.width(), image.height()
    );

    SDL_UpdateWindowSurface(window);
    SDL_Delay(2000); // Display for 2 seconds

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
```

---

## License

This library is released under the **zlib License**. See the [LICENSE](LICENSE) file for more details.
