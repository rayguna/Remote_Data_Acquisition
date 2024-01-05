import pandas as pd
from bokeh.plotting import figure, save
from bokeh.layouts import gridplot
from easygui import fileopenbox, enterbox, ynbox, filesavebox
import os
import webbrowser

# Function to read data from the CSV file
def read_csv_file():
    file_path = fileopenbox(msg="Select CSV file", filetypes=["*.csv"])
    if file_path:
        return pd.read_csv(file_path), file_path
    else:
        return None, None

# Get data from the CSV file
data, file_path = read_csv_file()

if data is not None:
    # Get all columns except the first one (timestamp)
    value_columns = [col for col in data.columns if col != 'Timestamp']

    # Convert the timestamp column to datetime
    data['Timestamp'] = pd.to_datetime(data['Timestamp'])

    # Create plots dynamically based on the number of value columns
    plots = []
    for col in value_columns:
        p = figure(x_axis_type='datetime', title=col, x_axis_label='Timestamp', y_axis_label='Value')
        p.line(data['Timestamp'], data[col], legend_label=col, line_width=2)
        plots.append(p)

    # Arrange the plots in a grid layout with a title
    grid = gridplot(plots, ncols=2, plot_width=400, plot_height=400)

    # Save the grid of plots to an HTML file
    file_name = os.path.splitext(os.path.basename(file_path))[0] + "_plots.html"
    while os.path.exists(file_name):
        choice = ynbox(msg="File already exists. Do you want to rename it?", choices=("Yes", "No"))
        if choice:
            file_name = filesavebox(msg="Select a new location and filename for the plot", default=file_name)
        else:
            break

    if file_name:  # If the user selected a file name
        save(grid, filename=file_name, title="CSV Plots", resources="cdn")

        # Open the HTML file
        webbrowser.open(file_name)
