import numpy as np
import pandas as pd
import time

# Load data
start = time.time()
data = pd.read_csv("test.csv", header = 0)
end = time.time()
print "Data Loading time:", 1000*(end - start), "ms"

# Dump data
start = time.time()
data.to_csv("pandas_output.csv")
end = time.time()
print "Data Dumping time:", 1000*(end - start), "ms"

# Sort data
start = time.time()
data.sort_values(by = "orig_destination_distance")
end = time.time()
print "Data Sorting time:", 1000*(end - start), "ms"

# Filter data
start = time.time()
d = data[data["site_name"] < 10]
end = time.time()
print "Data Filtering time:", 1000*(end - start), "ms"

# Get data
start = time.time()
d = data.iloc[range(0, 100000), [3, 4, 5, 6, 7]]  
end = time.time()
print "Data Getting time:", 1000*(end - start), "ms"

# Set data
start = time.time()
for i in xrange(data.count()[0]):
	data.set_value(i, 0, 0);  
end = time.time()
print "Data Setting time:", 1000*(end - start), "ms"


# Row append data
start = time.time()
data.append(data);
end = time.time()
print "Data row appending time:", 1000*(end - start), "ms"





