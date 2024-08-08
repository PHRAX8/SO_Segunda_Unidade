import sys
import matplotlib.pyplot as plt

# Function to read the file and extract numbers
def read_file(filename):
    try:
        with open(filename, 'r') as file:
            data = [float(line.strip()) for line in file]
        return data
    except Exception as e:
        print(f"Error reading file: {e}")
        sys.exit(1)

# Plotting function
def plot_data(data):
    x_values = [i * 10 for i in range(1, len(data) + 1)]
    y_values = data

    plt.figure(figsize=(10, 5))
    plt.plot(x_values, y_values, marker='o', linestyle='None')
    plt.title('Número de Processos x Variância do Tempo Médio de Execução')
    #plt.title('Número de Processos x Média do Tempo Médio de Execução')
    plt.xlabel('Número de Processos')
    plt.ylabel('Média do Tempo Médio de Execução')
    plt.grid(True)
    plt.show()

# Main function to handle CLI input
def main():
    if len(sys.argv) != 2:
        print("Usage: python script.py <filename>")
        sys.exit(1)

    filename = sys.argv[1]
    data = read_file(filename)
    plot_data(data)

if __name__ == "__main__":
    main()
