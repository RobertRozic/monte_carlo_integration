from scipy import random
import numpy as np
import matplotlib.pyplot as plt
import time
import multiprocessing as mp

start_time = time.time()

a = 0
b = 10
N = int(input('Unesite velicinu uzorka:'))
M = int(input('Unesite broj racunanja:'))


def func(x):
    return pow(x, 2)+7*np.cos(x)


def monte_carlo():
    xrand = random.uniform(a, b, N)
    integral = sum(func(xrand))
    answer = (b - a) / float(N) * integral
    return answer

areas = list()
# Sekvencijalno
for i in range(M):
    areas.append(monte_carlo())

print("Sekvencijalno: --- %s seconds ---" % (time.time() - start_time))
print("Priblizan rezultat:", np.mean(areas))

# Paralelno
start_time = time.time()
pool = mp.Pool(6)

future_results = [pool.apply_async(monte_carlo) for i in range(M)]
areas = [f.get() for f in future_results]

print("Paralelno --- %s seconds ---" % (time.time() - start_time))
print("Priblizan rezultat:", np.mean(areas))

# Graf
plt.title("Distribucija izracunatih podrucja")
plt.hist(areas, bins=30, ec='black')
plt.xlabel("Podrucja")
plt.show()
