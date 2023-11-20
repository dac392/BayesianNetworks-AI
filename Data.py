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

    # Initialize a figure for the combined graph
    plt.figure(figsize=(15, 10))

    # Generate and save line graphs for each bot and add to the combined graph
    for i, bot_name in enumerate(data['bot_name'].unique(), start=1):
        bot_df = data[data['bot_name'] == bot_name]
        avg_total_actions = bot_df.groupby('alpha_k')['total_actions'].mean()

        # Individual line graph
        plt.figure(figsize=(10, 6))
        avg_total_actions.plot(kind='line')
        plt.title(f"Average Total Actions vs Alpha/K for {bot_name}")
        plt.xlabel("Alpha/K Value")
        plt.ylabel("Average Total Actions")
        plt.savefig(f"{bot_name}_avg_actions_line_graph.png")
        plt.close()

        # Add to combined graph
        plt.subplot(3, 3, i)
        avg_total_actions.plot(kind='line', title=bot_name)
        plt.xlabel("Alpha/K Value")
        plt.ylabel("Avg Total Actions")

    # Save the combined graph
    plt.tight_layout()
    plt.savefig("combined_avg_actions_line_graph.png")
    plt.close()

if __name__ == "__main__":
    main()
