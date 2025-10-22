# Function Plotter (C++17 + Cairo)

A simple command-line utility for plotting mathematical functions.  
Generates PNG images using the **Cairo** graphics library.

## Features

- Plot one or multiple functions using `--expr`.
- Generate PNG images (`--png output.png`).
- Adjustable axis ranges (`--min-x`, `--max-x`, `--min-y`, `--max-y`).
- Custom image size (`--img-width`, `--img-height`).
- Control the number of samples (`--samples`).
- Supports basic operations `+ - * /` and the `sin()` function.
- Call help with input options using `-h` or `--help`.

## Example Usage

```bash
# Build
./build.sh

# Run with PNG output
./func_plotter.exe --expr "sin(x)" --png plot.png --min-x -6.28 --max-x 6.28

# Run without PNG (CSV output to console)
./func_plotter.exe --expr "x*x" --samples 50
```
## License

© 2025 [Yehor Nesterenko]. All rights reserved.  
This project was created as part of an academic assignment and is shared **for portfolio and demonstration purposes only**.  
Unauthorized use, reproduction, or distribution of this code is prohibited.
