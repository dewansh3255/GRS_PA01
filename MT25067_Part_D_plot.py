import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# Set style
sns.set(style="whitegrid")

# --- LOAD DATA ---
# We assume both CSVs are present.
try:
    df_c = pd.read_csv('MT25067_Part_C_CSV.csv')
    df_d = pd.read_csv('MT25067_Part_D_CSV.csv')
except FileNotFoundError as e:
    print(f"Error: Could not find CSV files. Make sure you ran both shell scripts. {e}")
    exit()

# --- PLOT 1: Part C Execution Time ---
plt.figure(figsize=(10, 6))
sns.barplot(x="Task", y="Execution_Time_Sec", hue="Program", data=df_c)
plt.title("Part C: Execution Time Comparison")
plt.ylabel("Time (seconds)")
plt.savefig("MT25067_Part_C_Time_Plot.png")
plt.close()
print("Generated: MT25067_Part_C_Time_Plot.png")

# --- PLOT 2: Part C CPU Usage (Restored) ---
plt.figure(figsize=(10, 6))
sns.barplot(x="Task", y="Avg_CPU_Usage", hue="Program", data=df_c)
plt.title("Part C: Average CPU Usage")
plt.ylabel("CPU Usage (%)")
plt.ylim(0, 105) # Cap at 105% for clarity
plt.savefig("MT25067_Part_C_CPU_Plot.png")
plt.close()
print("Generated: MT25067_Part_C_CPU_Plot.png")

# --- PLOT 3: Part D Scalability ---
plt.figure(figsize=(10, 6))

# Filter for CPU task in Part D (assuming we ran 'cpu' for scalability)
# If you ran other tasks, adjust the filter.
data_a = df_d[(df_d['Program'] == 'Program_A') & (df_d['Task'] == 'cpu')]
data_b = df_d[(df_d['Program'] == 'Program_B') & (df_d['Task'] == 'cpu')]

plt.plot(data_a['Count'], data_a['Execution_Time_Sec'], marker='o', label='Program A (Processes)', linewidth=2)
plt.plot(data_b['Count'], data_b['Execution_Time_Sec'], marker='x', linestyle='--', label='Program B (Threads)', linewidth=2)

plt.title("Part D: Scalability (CPU Task pinned to 1 Core)")
plt.xlabel("Number of Workers")
plt.ylabel("Execution Time (seconds)")
plt.legend()
plt.grid(True)
plt.savefig("MT25067_Part_D_Scalability_Plot.png")
plt.close()
print("Generated: MT25067_Part_D_Scalability_Plot.png")