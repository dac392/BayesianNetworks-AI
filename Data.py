import pandas as pd
import matplotlib.pyplot as plt

def process_alpha_k(row, alphas):
    if row['bot_type'] == 'Probabilistic':
        return alphas[row['alpha_k']]
    else:
        return row['alpha_k']

def compare_bots(data, bot_names):
    comparisons = {}
    for bot_name in bot_names:
        bot_df = data[data['bot_name'] == bot_name]
        mean_actions = bot_df['total_actions'].mean()
        comparisons[bot_name] = mean_actions
    print(f"Comparison for {', '.join(bot_names)}:")
    for bot, mean_actions in comparisons.items():
        print(f"{bot} - Mean Total Actions: {mean_actions}")
    print("\n")

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

    # Bot groups for combined graphs and comparisons
    bot_groups = {
        "combined_bot1_bot2_avg_actions_line_graph.png": ["bot1", "bot2"],
        "combined_bot3_bot4_avg_actions_line_graph.png": ["bot3", "bot4"],
        "combined_bot5_bot6_avg_actions_line_graph.png": ["bot5", "bot6"],
        "combined_bot7_bot8_bot9_avg_actions_line_graph.png": ["bot7", "bot8", "bot9"]
    }

    # Generate and save individual and combined line graphs, and print comparisons
    for filename, bots in bot_groups.items():
        plt.figure(figsize=(10, 6))
        for bot_name in bots:
            bot_df = data[data['bot_name'] == bot_name]
            avg_total_actions = bot_df.groupby('alpha_k')['total_actions'].mean()
            avg_total_actions.plot(kind='line', label=bot_name)

        compare_bots(data, bots)
       
        plt.title("Average Total Actions vs Alpha/K")
        plt.xlabel("Alpha/K Value")
        plt.ylabel("Average Total Actions")
        plt.legend()
        plt.savefig(filename)
        plt.close()

if __name__ == "__main__":
    main()