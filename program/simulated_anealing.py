import math
import sys, os
from random import random, randint, shuffle

def read_instance(file_name):
    f = open(file_name, "r")
    lines = f.readlines()
    f.close()
    n = int(lines[0])
    w = []
    t = []
    for i in xrange(1, n+1): #n+2):
        ar = lines[i].split()
        ar = map(lambda x: float(x), ar)
        w.append(ar)
    for i in xrange(n+1, 2*n+1): #n+2, 2*n+2):
        ar = lines[i].split()
        ar = map(lambda x: float(x), ar)
        t.append(ar)
    return (n, w, t)

def rand_neightbour_2(x):
    n = len(x)
    y = list(x)
    for i in xrange(n/10):
        i, j = randint(0, n-1), randint(0, n-2)
        if j >= i:
            j += 1
        y[i], y[j] = y[j], y[i]
        #y = rand_neightbour(x)
    return y
    


def rand_neightbour(x):
    n = len(x)
    y = list(x)
    i, j = randint(0, n-1), randint(0, n-2)
    if j >= i:
        j += 1
    y[i], y[j] = x[j], x[i]
    return y

def select_best_neightbour(x, instance):
    n = len(x)    
    best_f_y = f(x, instance)
    best_y = x
    for i in xrange(0, n):
        for j in xrange(i+1, n):
            y = list(x)
            y[i], y[j] = x[j], x[i]
            f_y = f(y, instance)
            if f_y < best_f_y:
                best_f_y = f_y
                best_y = y
    return y
                

def f(x, instance):
    return c(x, 0, instance)
    
def c(x, t, instance):
    time_trace = []
    n, w, dt = instance
    m = len(x)
    if len(x) == 0:
        return 0
    elif len(x) == 1:
        a = dt[x[0]-1][0]
        b = dt[x[0]-1][1]
        if(a <= t and t <= b):
            return 0
        elif(a >= t):
            return 0
        else:
            return 1 
    else:
        w_time = 0
        head = x[0]
        if len(x) >= 2:
            x1 = x[0]
            x2 = x[1]
            w_time = w[x1-1][x2-1]
        tail = x[1:]
        c1, c2, c3 = 1000000, 1000000, 1000000
        if(dt[head-1][0] <= t and t <= dt[head-1][1]): #trafilismy w przedzial            
            c1 = c(tail, t + w_time, instance) 
        if(dt[head-1][0] >  t): # czekamy 
            c2 = c(tail, dt[head-1][0] + w_time, instance)
        # nieczekam
        c3 = c(tail, t + w_time, instance) + 1 
        return min(c1, c2, c3)

def save_results(file_name, trace, instance):
    xs = map(lambda x: f(x, instance), trace)
    fi = open(file_name, "w")
    for i in xrange(len(trace)):
        fi.write(str(i+1)+"\t"+str(xs[i])+"\n")
    fi.close()

def temperature(x):
    return (math.exp(x) - math.exp(0))/(math.exp(1) - math.exp(0))

def simulated_anealing(instance):
    trace = []
    (n, w, t) = instance
    s = [i for i in xrange(1, n+1)]
    shuffle(s)
    trace.append(s)
    k, k_max = 0, n*n*n
    print "starting point: ", s
    while k < k_max:
        s1 = s
        s2 = rand_neightbour_2(s) #select_best_neightbour(s, instance) #rand_neightbour(s)
        if f(s1, instance) > f(s2, instance):
            s1, s2 = s2, s1
        if temperature(k / float(k_max)) > random():
            s = s1
        else:
            s = s2
        trace.append(s)
        print s, f(s, instance), temperature(k / float(k_max))
        k += 1
    return (s, trace)

#instance = read_instance("instances/rc_201.1.txt") #sample.in")
#print read_instance("instances/rc_201.1.txt") #sample.in")
#print f([1,2,3,4,5], read_instance("sample.in"))
#result =  simulated_anealing(instance)
#(best, trace) = result
#print f(result[0], instance)
#save_results("data.dat", trace, instance)

for file_name in os.listdir("./instances/"):
    print "File: ", file_name
    path = os.path.join("./instances/", file_name)
    instance = read_instance(path)
    results = simulated_anealing(instance)
    (best, trace) = results
    output_path = os.path.join("./outputs/", file_name+".out")
    print "Output file: ", output_path
    save_results(output_path, trace, instance)

