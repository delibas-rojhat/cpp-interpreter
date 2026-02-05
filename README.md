# Witcher Tracker – C++ Implementation

A C++ implementation of an interpreter-based inventory and event tracking system inspired by *The Witcher* universe. The program simulates Geralt's inventory management, alchemy knowledge, and monster encounters through a command-based interpreter.

## Overview
The system processes textual commands representing actions, knowledge acquisition, encounters, and queries. It maintains internal state for inventory items, potion formulas, bestiary knowledge, and encounter results. Inputs are validated according to strict grammar rules, and appropriate responses are generated for each valid command.

## Features
- Command interpreter with strict grammar validation
- Inventory management for ingredients, potions, and trophies
- Potion brewing system based on learned formulas
- Bestiary tracking with potion and sign effectiveness
- Monster encounter simulation
- Trading system between trophies and ingredients
- Query system for inventory, alchemy, and bestiary data
- Detection of invalid inputs and commands

## Core Functionalities
Geralt can loot ingredients, brew potions if formulas and ingredients are available, learn potion formulas and monster weaknesses, encounter monsters and collect trophies, trade trophies for ingredients, and query current knowledge and inventory status through interpreter commands.

## Project Structure
main.cpp – Program entry point  
WitcherTracker.* – Core interpreter and system logic  
Item / Potion classes – Domain entities  
Utils.hpp – Helper utilities  

## Build & Run
Compile with a standard C++ compiler:

```bash
g++ *.cpp -std=c++17 -o witchertracker
./witchertracker

## Purpose
The goal of this project is to develop practical experience in building an interpreter-based system using C++. It focuses on structured input parsing, managing complex program state, and designing modular software components. The project also emphasizes clean architecture, object-oriented design, and handling real-time command processing within a consistent rule-based system.
``
