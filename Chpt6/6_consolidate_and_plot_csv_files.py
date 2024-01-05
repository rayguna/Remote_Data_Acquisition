import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.dates as mdates
import glob

y_labels=["°C", "%", "ppm", "arbitrary", "°C", "%", "V", "V"]

def plot_csv_data(directory, rows=1, cols=1):
    dfs = pd.DataFrame()

    for file in glob.glob(f"{directory}/*.csv"):
        df_temp = pd.read_csv(file, index_col=0, parse_dates=["Timestamp"])
        dfs = pd.concat([dfs, df_temp], axis=0)

    num_plots = rows * cols
    num_cols = min(len(dfs.columns), cols)
    num_rows = - (-len(dfs.columns) // num_cols)
    
    fig, axs = plt.subplots(num_rows, num_cols, figsize=(15, 7))
    axs = axs.flatten()

    # Filter data: remove data point where turbidity reading is zero
    dfs = dfs[dfs['esp:tur'] > 0]
    
    for i, col in enumerate(dfs.columns):
        if i >= len(axs):
            break

        ax = axs[i]
        ax.plot(dfs.index, dfs[col])
        ax.set_xlabel('Date', fontweight='bold', fontsize=10)
        ax.set_ylabel(y_labels[i], fontweight='bold', fontsize=10)
        ax.set_title(col, fontweight='bold', fontsize=12)
        ax.xaxis.set_major_formatter(mdates.DateFormatter('%Y-%m-%d'))
        ax.tick_params(axis='x', rotation=45)

    for j in range(len(dfs.columns), len(axs)):
        axs[j].axis('off')

    plt.tight_layout()
    
    # Save figure as an image with specified dimensions and resolution
    plt.savefig('celer_opus_hydroponics_data.png', dpi=300, figsize=(6, 4))  # Change the filename as needed

    plt.show()

# Check if a directory was selected
plot_csv_data("data", rows=2, cols=4)  # Change rows and cols as needed
