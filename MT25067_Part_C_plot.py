import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# Load data
df = pd.read_csv('MT25067_Part_C_CSV.csv')

# Set plot style
sns.set(style="whitegrid")

# 1. Execution Time Plot
plt.figure(figsize=(10, 6))
sns.barplot(x="Task", y="Execution_Time_Sec", hue="Program", data=df)
plt.title("Execution Time Comparison (Processes vs Threads)")
plt.savefig("MT25067_Part_C_Time_Plot.png")
plt.close()

# 2. CPU Usage Plot
plt.figure(figsize=(10, 6))
sns.barplot(x="Task", y="Avg_CPU_Usage", hue="Program", data=df)
plt.title("Average CPU Usage Comparison")
plt.savefig("MT25067_Part_C_CPU_Plot.png")
plt.close()

print("Plots generated: Time_Plot.png, CPU_Plot.png")