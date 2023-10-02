import time
from memory_profiler import profile


# This function calculates buckets for characters in the input string T
def getBuckets(T):
    count = {}  # A dictionary to count the occurrence of each character
    buckets = {}  # A dictionary to store the ranges (buckets) for each character
    for c in T:
        count[c] = count.get(c, 0) + 1  # Count the occurrences of each character
    start = 0
    for c in sorted(count.keys()):  # Sort the characters
        buckets[c] = (start, start + count[c])  # Assign a range (bucket) for each character
        start += count[c]  # Update the start position for the next character
    return buckets

#@profile
# This is the main SAIS algorithm function
def sais(T):
    t = ["_"] * len(T)  # Initialize an array 't' with the same length as T, filled with "_"

    t[len(T) - 1] = "S"  # Mark the last character as "S"

    # Determine whether each character is "S" or "L" based on comparisons with the next character
    for i in range(len(T) - 1, 0, -1):
        if T[i - 1] == T[i]:
            t[i - 1] = t[i]
        else:
            t[i - 1] = "S" if T[i - 1] < T[i] else "L"

    # Calculate buckets for characters in the input string T
    buckets = getBuckets(T)

    count = {}  # Initialize a dictionary to count characters
    SA = [-1] * len(T)  # Initialize the suffix array with -1 values
    LMS = {}  # Initialize a dictionary to store LMS substring positions
    end = None

    # Iterate through the input string T backward
    for i in range(len(T) - 1, 0, -1):
        if t[i] == "S" and t[i - 1] == "L":
            # For S-type character followed by L-type character (LMS substring)
            revoffset = count[T[i]] = count.get(T[i], 0) + 1  # Count occurrences of the character
            # Calculate the position for this LMS substring and update the suffix array
            SA[buckets[T[i]][1] - revoffset] = i

            if end is not None:
                LMS[i] = end  # Store the end position of this LMS substring
            end = i

    LMS[len(T) - 1] = len(T) - 1  # Last character is always an LMS substring
    count = {}  # Reset the count dictionary

    # Iterate through the suffix array to place LMS substrings in their correct positions
    for i in range(len(T)):
        if SA[i] >= 0:
            if t[SA[i] - 1] == "L":
                symbol = T[SA[i] - 1]
                offset = count.get(symbol, 0)
                # Calculate the position for this LMS substring and update the suffix array
                SA[buckets[symbol][0] + offset] = SA[i] - 1
                count[symbol] = offset + 1  # Update the count of this character

    count = {}  # Reset the count dictionary

    # Iterate through the suffix array again to refine positions of S-type characters
    for i in range(len(T) - 1, 0, -1):
        if SA[i] > 0:
            if t[SA[i] - 1] == "S":
                symbol = T[SA[i] - 1]
                revoffset = count[symbol] = count.get(symbol, 0) + 1
                # Calculate the position for this S-type character and update the suffix array
                SA[buckets[symbol][1] - revoffset] = SA[i] - 1

    namesp = [-1] * len(T)  # Initialize a list to store LMS substring names
    name = 0
    prev = None

    # Assign unique names to LMS substrings and store them in 'namesp'
    for i in range(len(SA)):
        if t[SA[i]] == "S" and t[SA[i] - 1] == "L":
            if prev is not None and T[SA[prev]:LMS[SA[prev]]] != T[SA[i]:LMS[SA[i]]]:
                name += 1
            prev = i
            namesp[SA[i]] = name

    names = []
    SApIdx = []

    # Collect LMS substring names and their positions
    for i in range(len(T)):
        if namesp[i] != -1:
            names.append(namesp[i])
            SApIdx.append(i)

    # If there are more than one unique names, recursively call 'sais' to sort them
    if name < len(names) - 1:
        names = sais(names)

    names = list(reversed(names))  # Reverse the sorted names

    SA = [-1] * len(T)  # Reset the suffix array
    count = {}  # Reset the count dictionary

    # Place LMS substrings in their correct positions in the suffix array
    for i in range(len(names)):
        pos = SApIdx[names[i]]
        revoffset = count[T[pos]] = count.get(T[pos], 0) + 1
        SA[buckets[T[pos]][1] - revoffset] = pos

    count = {}  # Reset the count dictionary

    # Refine positions of S-type characters in the suffix array
    for i in range(len(T)):
        if SA[i] >= 0:
            if t[SA[i] - 1] == "L":
                symbol = T[SA[i] - 1]
                offset = count.get(symbol, 0)
                # Calculate the position for this S-type character and update the suffix array
                SA[buckets[symbol][0] + offset] = SA[i] - 1
                count[symbol] = offset + 1

    count = {}  # Reset the count dictionary

    # Refine positions of S-type characters again in the suffix array
    for i in range(len(T) - 1, 0, -1):
        if SA[i] > 0:
            if t[SA[i] - 1] == "S":
                symbol = T[SA[i] - 1]
                revoffset = count[symbol] = count.get(symbol, 0) + 1
                # Calculate the position for this S-type character and update the suffix array
                SA[buckets[symbol][1] - revoffset] = SA[i] - 1

    return SA  # Return the generated suffix array

# Function to read a file and return its content as a string
def read_file(filename):
    try:
        with open(filename, 'r', encoding='utf-8') as file:
            content = file.read()
        return content
    except FileNotFoundError:
        print(f"File '{filename}' not found.")
        return None

# Get the input filename from the user
filename = "./Libros/Book4.txt"

# Read the content of the file
file_content = read_file(filename)

if file_content:
    # Measure the execution time
    start_time = time.time()

    # Convert the file content into a list of character codes
    T = [ord(c) for c in file_content]

    # Generate the suffix array for the input string
    SA = sais(T)

    # Calculate the execution time
    end_time = time.time()
    execution_time = end_time - start_time

    # Print the suffix array and execution time
    print("Suffix Array:")
    print(SA)
    print(f"Execution Time: {execution_time:.6f} seconds")
    
    # for iSuffix in SA:
    #   substring = file_content[iSuffix:]
    #   print(substring)
