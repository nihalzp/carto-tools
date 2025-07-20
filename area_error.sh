#!/bin/bash

# Define the directory where your files are located
DIRECTORY="${1:-.}"

# Loop over each input.geojson file
for input_file in "$DIRECTORY"/*_input.geojson; do
  # Extract the base name without the extension and suffix
  base_name=$(basename "$input_file" "_input.geojson")

  # Define the corresponding target area CSV file
  target_area_csv="${DIRECTORY}/${base_name}.csv"

  # Define the corresponding cartogram and fcarto files
  cartogram_file="${DIRECTORY}/${base_name}_cartogram.geojson"
  fcarto_file="${DIRECTORY}/${base_name}_fcarto.geojson"

  # Check if the target area CSV file exists
  if [[ -f "$target_area_csv" ]]; then
    echo "Running area error calculation for $base_name (input)..."
    carto --area_error --map "$input_file" --target_area_csv "$target_area_csv"

    echo "Running area error calculation for $base_name (cartogram)..."
    carto --area_error --map "$cartogram_file" --target_area_csv "$target_area_csv"

    echo "Running area error calculation for $base_name (fcarto)..."
    carto --area_error --map "$fcarto_file" --target_area_csv "$target_area_csv"
  else
    echo "Missing target area CSV file for $base_name. Skipping..."
  fi
done