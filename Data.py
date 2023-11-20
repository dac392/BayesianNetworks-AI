import pandas as pd
import matplotlib.pyplot as plt

def process_alpha_k(row, alphas):
    if row['bot_type'] == 'Probabilistic':
        return alphas[row['alpha_k']]
    else:
        return row['alpha_k']

def main():
    # Load the data
    file_path = 'data.txt'  # Update the path if needed
    column_names = ['ship_uid', 'bot_uid', 'bot_name', 'bot_type', 'alpha_k', 'total_actions']
    data = pd.read_csv(file_path, names=column_names)

    # Define the alphas array
    alphas = [0.05, 0.10, 0.15, 0.20, 0.25, 0.30, 0.35, 0.40, 0.45, 0.50, 
              0.55, 0.60, 0.65, 0.70, 0.75, 0.80, 0.85, 0.90, 0.95, 1.00]

    # Process the alpha/k values
    data['alpha_k'] = data.apply(lambda row: process_alpha_k(row, alphas), axis=1)

    # Generate and save graphs for each bot
    for bot_name in data['bot_name'].unique():
        bot_df = data[data['bot_name'] == bot_name]
        plt.figure(figsize=(10, 6))
        plt.scatter(bot_df['alpha_k'], bot_df['total_actions'])
        plt.title(f"Total Actions vs Alpha/K for {bot_name}")
        plt.xlabel("Alpha/K Value")
        plt.ylabel("Total Actions")
        plt.savefig(f"{bot_name}_actions_graph.png")
        plt.close()

if __name__ == "__main__":
    main()
