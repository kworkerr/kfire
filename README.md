# kfire

A terminal-based ASCII fire simulator featuring 256-color ANSI support and configurable intensity.

## Features

- ASCII fire animation rendered directly in the terminal.
- 256-color ANSI palette support.
- Eight selectable color themes.
- Adjustable fire intensity.
- Lightweight implementation in C.
- No external runtime dependencies beyond the system's C library.

## Requirements

- A C compiler, such as `cc` or `clang`.
- A Unix-like operating system.
- A terminal with ANSI escape sequence and 256-color support.

## Build

```sh
./build.sh
```

## Usage

Start the simulator with the default settings:

```sh
./kfire
```

Set the fire intensity:

```sh
./kfire --intensity 5.0
```

Select a color theme:

```sh
./kfire --color 2
```

Combine options:

```sh
./kfire --color 2 --intensity 5.0
```

### Color Themes

The `--color` option accepts values from `1` to `8`, representing the available color themes.

### Intensity

The `--intensity` option accepts numeric values greater than `1`. Higher values produce stronger flames by reducing the cooling effect.

Press `Ctrl+C` to exit the simulator.

## License

See [LICENSE](LICENSE) for license details.
