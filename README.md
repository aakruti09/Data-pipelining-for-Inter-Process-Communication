# Data pipelining for Inter-Process Communication [IPC]

- Built data pipeline code between three programs 
-- Program 1: Generates random coordinates and street names for street maps in C++. 
-- Program 2: Identifies intersection and creates unidirectional graph in Python, based on coordinates given by program 1.
-- Program 3: Calculates shortest path from any vertex to other using C++, from the graph generated in the above program. 
- Parent program: Created data pipeline between above three code using pipe(), fork(), dup2(), and exec() functions to debug multiple programs concurrently and build data pipelines for inter-process communication. 
- Execution flow is: Parent program (a3ece650.cpp) - Program 1 (rgen.cpp) - Program 2 (a1ece650.py-intesect_point_new.py) - graph.txt - Program 3 (a2ece650.cpp)
