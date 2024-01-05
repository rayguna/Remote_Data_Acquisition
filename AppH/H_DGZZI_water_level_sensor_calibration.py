import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from scipy.interpolate import interp1d

def convert_to_mm(x_ml):
    """Convert from mL water to mm"""
    return 35 * np.array(x_ml) / max(x_ml)

# Sensor readings and corresponding water levels
sensor_data = {
    'x_ml': [0, 3.75, 7.5, 11.25, 15, 22.5, 30, 45, 60, 75, 90, 105, 120, 135],
    'y_wl': [0, 150, 190, 220, 250, 340, 370, 420, 450, 470, 470, 480, 480, 490]
}

# Create a DataFrame from the sensor data
df = pd.DataFrame(sensor_data)

# Convert sensor readings to mm
df['x_mm'] = convert_to_mm(df['x_ml'])

# Interpolate water level given sensor readings
interpolated_func = interp1d(df['y_wl'], df['x_mm'], kind='linear', fill_value='extrapolate')

# Scatter plot of experimental data and interpolation line
plt.scatter(df['x_mm'], df['y_wl'], marker='x', color='red', label='Experimental')
plt.plot(interpolated_func(df['y_wl']), df['y_wl'], label='Interpolation')

# Labeling and legend
plt.xlabel('Water level in mm', fontweight='bold', fontsize=12)
plt.ylabel('Water level reading', fontweight='bold', fontsize=12)
plt.legend()

# Save the plot as an image with specified DPI and size
plt.savefig('wl_calibration_figure.png', dpi=300, figsize=(6, 4))  # Change the filename as needed

# Display the plot
plt.show()
