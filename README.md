# Maze Escape Game — DFS-Generated 3D Maze (C++ & raylib)

A fast, minimal **C++/raylib** project that generates a **perfect maze** using an **iterative Depth-First Search (DFS)** (a.k.a. *recursive backtracker*) and renders it in 3D. You have **2:30** to find the exit — beat the clock or lose the run.

---

## Overview

**Maze Escape Game** is a small teaching project that blends **procedural generation** with **real-time rendering**:
- Implements **perfect-maze** generation (single unique path between any two cells) with an **iterative DFS** using an explicit stack.
- Renders the maze as textured walls and floor tiles in **raylib** with a top-down “map” view toggle for orientation.
- Uses clean, OOP-style separation between the maze model (`Maze`) and the 3D renderer/holder (`Maze3D`).

---

## Game Loop & Objective

- Start at the maze’s origin and **reach the goal** within **2 minutes 30 seconds**.
- If you arrive before time’s up → **Win screen**. Otherwise → **Lose screen**.
- Each run seeds the generator from the current time, producing a **different maze** on (nearly) every start.

---

## Controls

- **W A S D** — move forward/left/back/right  
- **Mouse** — look around (first-person)  
- **Tab** — toggle **top-down** (bird’s-eye) map to help orientation

---

## Screenshots (Placeholders)
- First-person gameplay (corridors & turns)  
<img width="801" height="482" alt="Screenshot 2025-09-01 201011" src="https://github.com/user-attachments/assets/1dba9d6d-e471-4cbe-8895-13d85cc3c656" />
- Top-down map with player marker  
<img width="801" height="480" alt="Screenshot 2025-09-01 200733" src="https://github.com/user-attachments/assets/75a48bb8-4a16-447b-a578-21e3e169f8bf" />
- **Win** screen  
<img width="801" height="480" alt="Screenshot 2025-09-01 201539" src="https://github.com/user-attachments/assets/a9893640-6487-45b3-b9f1-9b75bc3e2a36" />
- **Lose** screen
<img width="803" height="479" alt="Screenshot 2025-09-01 201202" src="https://github.com/user-attachments/assets/763683ec-7c5a-4c79-b36c-a69baf76f010" />

## Why DFS?

---

**Randomized DFS** (recursive backtracker) is simple, fast, and produces aesthetically pleasing layouts:
-Ensures connectivity with no cycles (i.e., a tree over the grid graph → a perfect maze).
-Very compact state: only a visited[] array and an explicit stack.
-Drawback: tends to make long corridors with fewer junctions; that can be desirable for time-challenge gameplay.

Other options you can explore later: Prim, Kruskal, Wilson, Aldous–Broder, Eller, Sidewinder — each yields a distinct “feel”.
