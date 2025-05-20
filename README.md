# IITM-INSTA-INFLUENCER-FEST
C++ mini project simulates the IITM InstaInfluencer Fest using a directed graph of event dependencies. It employs DFS for cycle detection, SCC analysis, and topological sorting to manage event flow, and uses dynamic programming on the DAG to compute the maximum hype score—showcasing advanced algorithms and OOP.

Storyline:

  It’s March 2025. Following the massive success of Shaastra, IIT Madras is hosting its first-ever InstaInfluencer Fest, a high-energy gathering of Gen-Z influencers, meme        lords, podcasters, and livestreamers.

Events include:
    
    Viral dance-offs 🕺
    Meme wars 😂
    Live podcast sessions 🎙️
    Streaming challenges 🎮

Objectives Implemented

A directed graph, where:
  Nodes represent influencer events.
  Directed edges denote that one event must occur before another.
  Each event has a hype score, representing its popularity on campus.

The Project's main focus is to:
      
      Detect circular dependencies (conflicts in event scheduling).
      Identify clusters of mutually dependent events (Strongly Connected Components).
      Generate a valid event schedule if possible (topological sort).
      Maximize the total hype score across a valid path of events.


Input

      N = number of events
      M = number of dependencies
      Array of size N for hype scores
      M edges of dependencies
      Q queries (each asking one of 4 types)

      Query Type	Description
        
        1	Check if any cycles (circular dependencies) exist.
        2	Output number of Strongly Connected Components (SCCs) and the size of the largest SCC.
        3	Provide a valid topological order of events (lexicographically sorted if possible). Output "NO" if cycles exist.
        4	Compute the maximum hype score from a valid path in the condensed DAG of SCCs.

Algorithms & Techniques Used in the project
This project showcases multiple advanced graph algorithms and object-oriented design patterns:

  Algorithms/methods Implemented:
    
          Cycle Detection via Depth-First Search (DFS)
          SCC Detection using Kosaraju’s Algorithm or Tarjan's Algorithm
          Topological Sorting with lexicographical control using Kahn’s Algorithm + Min-Heap
          Longest Path in DAG for computing the maximum hype using:
          DAG condensation of SCCs
          Dynamic Programming over topological order
          A custom Comparator Functor to control priority and lexicographic ordering
          
  Object-Oriented Architecture:
    
    Abstract base class GraphAlgorithm with a pure virtual Query() method
    Derived classes:
    isCycle – cycle detection
    indepComponent – SCC computation
    validOrder – topological sorting
    maxHype – max hype path in DAG
[IITM InstaInfluencer Fest.pdf](https://github.com/user-attachments/files/20349519/IITM.InstaInfluencer.Fest.pdf)


