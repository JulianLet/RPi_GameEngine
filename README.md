# Pi Game Engine by Julez

A lightweight game engine for the **Raspberry Pi Pico W**, built from scratch. This project demonstrates an Entity-Component-System (ECS) architecture with events, physics, and dynamic collision detection, along with sample games.



## Features

Entity-Component-System (ECS): Modular design to separate data, behavior, and logic.
Event system: Easily handle game events, collisions, and input.
Physics & Collision Detection: Dynamic collisions, basic physics simulation.



## Sample games included:

Pong: Classic game to play against the computer.
Jump & Run (WIP): Early prototype for a platformer demo.



## Quick build

Run this inside the root folder
mkdir -p build && cd build && cmake -G "Ninja" .. && ninja

or this inside the empty build folder
cmake -G "Ninja" .. && ninja
