def count_24bit_numbers(file_path):
    with open(file_path, 'r') as file:
        data = file.read()

    # Extract numbers from the file content
    data = data.split('\n')[1]
    numbers = list(map(int, data.split(',')))
    total = len(numbers)
    # Define the 24-bit range
    min_24bit = -2**23
    max_24bit = 2**23 - 1

    # Count numbers within the 24-bit range
    count = sum(min_24bit <= num <= max_24bit for num in numbers)

    return count,total

file_path = '/Users/yasantha-mac/projects/IntelliBee/Data-Collecting-Device/Data-Collected/Raw-Data/Sample-02/2024-10-29-10-48-37.txt'
count,total = count_24bit_numbers(file_path)
print(f"Count of numbers representable using 24 bits: {count} from total of {total}")