#!/bin/bash

# Define the directory where your files are located
DIRECTORY="${1:-.}"

# Loop over each input.geojson file
for input_file in "$DIRECTORY"/*_input.geojson; do
  # Extract the base name without the extension and suffix
  base_name=$(basename "$input_file" "_input.geojson")

  # Define the corresponding cartogram and fcarto files
  cartogram_file="${DIRECTORY}/${base_name}_cartogram.geojson"
  fcarto_file="${DIRECTORY}/${base_name}_fcarto.geojson"

  # Check if both files exist
  if [[ -f "$cartogram_file" && -f "$fcarto_file" ]]; then
    echo "Running intersection for $base_name (input)..."
    carto --intersection --map "$input_file"

    echo "Running intersection for $base_name (cartogram)..."
    carto --intersection --map "$cartogram_file"

    echo "Running intersection for $base_name (fcarto)..."
    carto --intersection --map "$fcarto_file"
  else
    echo "Missing files for $base_name. Skipping..."
  fi
done
