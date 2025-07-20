# Carto Tools

## Overview

`Carto Tools` is a C++ application for performing geospatial tasks on GeoJSON files, such as intersection reports, GeoJSON similarity calculations, and area error calculations.

---

## Prerequisites

Before building the project, ensure you have the following software installed:

### For macOS

```bash
brew install cmake make boost
```

### For Linux

```bash
sudo apt update
sudo apt install cmake make libboost-all-dev
```

### For Docker Ubuntu

Build the Docker image using the provided `Dockerfile`:

```bash
docker build -t carto-tools .
```

Create and run the Docker container with Volume Mounting:

```bash
docker run -it --name carto-container -v $(pwd):/app carto-tools
```

For consecutive runs, start the container:

```bash
docker start -ai carto-container
```

---

## Build and Install Instructions

To build and install the project, navigate to the project directory and use the following single command:
```bash
cmake -B build && make -C build && make install -C build
```

---

## Usage

The application supports multiple tasks via command-line arguments. Below are the available commands:

### 1. **Intersection Report**
Generate a report of intersections in a GeoJSON file. The report includes:

- The number of self-intersections.
- The number of intersections with other polygons. (overlapping polygons intersections)
- SVG visualization of the intersections (if any).

```bash
carto --intersection --map <geojson_file>
```
**Example:**
```bash
carto --intersection --map ./data/belgium/belgium_intersections.geojson
```

### 2. **Map Similarity Calculation**
Calculate similarity metrics (Frechet Distance, Hausdorff Distance, and Symmetric Difference) between two GeoJSON files.

Both GeoJSON files must share at least one common property within their features (shared identifier, name). For example, the two files could represent the same map before and after a cartogram transformation, such as one generated at [go-cart.io](https://go-cart.io).

```bash
carto --similarity --map_1 <geojson_file_1> --map_2 <geojson_file_2>
```
**Example:**
```bash
carto --similarity --map_1 ./data/belgium/belgium.geojson --map_2 ./data/belgium/belgium_cartogram.geojson
```

### 3. **Area Error Calculation**
Calculate the Max Relative Area Error and the Area Weighted Mean Error of a GeoJSON file compared to a target area CSV file. The target area CSV file template can be generated using the `--create_csv` command described below. Simply fill in the empty column with the target area values.

```bash
carto --area_error --map <geojson_file> --target_area_csv <csv_file>
```
**Example:**
```bash
carto --area_error --map ./data/belgium/belgium_cartogram.geojson --target_area_csv ./data/belgium/belgium_population.csv
```

### 4. **Generate Target CSV Template**
Generate a CSV file template for target area values. The CSV file will contain one column that is empty for the user to fill in the target area values. The completed CSV file can be used as input for the `--area_error` command.

```bash
carto --create_csv --map <geojson_file>
```
**Example:**
```bash
carto --create_csv --map ./data/belgium/belgium.geojson
```

### 5. **Using Shell Scripts**

The repository includes several shell scripts to automate common tasks. These scripts assume that your analysis data is located in a specific directory. By default, the scripts will look for files in the current directory unless a different directory is specified as an argument.

### Assumptions
- The scripts assume that the GeoJSON files follow a naming convention: `<base_name>_input.geojson`, `<base_name>_cartogram.geojson`, and `<base_name>_fcarto.geojson`.
- The `area_error.sh` script also assumes the presence of a target area CSV file named `<base_name>.csv`.

#### similarity.sh
This script calculates similarity metrics between `_input.geojson` files and their corresponding `_cartogram.geojson` and `_fcarto.geojson` files.

```bash
./similarity.sh [directory]
```
**Example:**
```bash
./similarity.sh ./data/belgium
```

#### intersections.sh
This script generates intersection reports for `_input.geojson`, `_cartogram.geojson`, and `_fcarto.geojson` files.

```bash
./intersections.sh [directory]
```
**Example:**
```bash
./intersections.sh ./data/belgium
```

#### area_error.sh
This script calculates area error metrics for `_input.geojson`, `_cartogram.geojson`, and `_fcarto.geojson` files using a corresponding target area CSV file.

```bash
./area_error.sh [directory]
```
**Example:**
```bash
./area_error.sh ./data/belgium
```

---

## Contributing

If you encounter any problems, have feature requests, or have suggestions, please create an issue in the repository.

---

## License

This project is licensed under the MIT License.
