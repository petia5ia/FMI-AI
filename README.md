# FMI-AI - Artificial Intelligence Homeworks (C++)

This repository contains my homework assignments from the **Artificial Intelligence** course at **Sofia University "St. Kliment Ohridski"**, Faculty of Mathematics and Informatics (FMI).  
All solutions are implemented in **C++** and cover core AI paradigms including:
- Heuristic search (A*, IDA*)
- Constraint satisfaction (Backtracking)
- Evolutionary algorithms (Genetic Algorithm)
- Adversarial search (Minimax)
- Probabilistic machine learning (Naive Bayes)

## Contents

### HW1 - Sliding Puzzle
Implementation of heuristic search algorithms for solving the Sliding Puzzle problem in C++.

Includes:
- **A\*** search for the 3×3 puzzle (8-puzzle)
- **IDA\*** (Iterative Deepening A\*) for both 3×3 and 4×4 (15-puzzle)

The implementations use the **Manhattan distance heuristic** and reconstruct the sequence of moves leading to the goal state.

This assignment focuses on:
- State representation
- Heuristic search
- Memory-efficient search strategies

---

### HW2 - N Queens
C++ implementation of the classic **N-Queens constraint satisfaction problem**.

The solution uses a **backtracking algorithm** to place queens row by row while checking column and diagonal conflicts.  
The program efficiently prunes invalid configurations and generates valid board arrangements.

This assignment focuses on:
- Constraint satisfaction problems (CSP)
- Backtracking search
- Conflict detection and pruning
- Recursive problem solving

---

### HW3 - Traveling Salesman Problem (TSP)
C++ implementation of the **Traveling Salesman Problem** using a **Genetic Algorithm** approach.

The solution includes:
- Population initialization
- Fitness evaluation based on tour distance
- Selection strategies
- Crossover operations
- Iterative evolution over generations

This assignment focuses on:
- Optimization problems
- Evolutionary algorithms
- Heuristic search techniques
- Stochastic solution strategies

---

### HW4 - Tic-Tac-Toe (Game AI)

C++ implementation of a Tic-Tac-Toe game with an AI opponent based on the **Minimax algorithm**.

The AI evaluates possible game states recursively and selects optimal moves to either win or force a draw.

This assignment focuses on:
- Adversarial search
- Game tree exploration
- Recursive decision-making
- Board state evaluation

---

### HW5 - Naive Bayes Classifier

C++ implementation of a **Naive Bayes classifier** for supervised learning tasks.

The program calculates prior and conditional probabilities from training data and applies Bayes’ theorem to classify new instances.

This assignment focuses on:
- Probabilistic reasoning
- Supervised learning
- Statistical classification
- Bayes’ theorem application

---

These projects were developed for academic purposes.
