# 🎯 Connect Four Final Project

Welcome to our Board Game AI Final Project!  
We developed an autonomous Connect Four game using Bobcat UI and a custom Min-Max AI agent.

---

## 📜 Overview
This project is a two-player Connect Four game that can be played:
- Human vs Human
- Human vs AI (autonomous agent)

The AI uses a Min-Max search strategy to make rational moves based on the current board state.

---

## 🛠️ Technologies Used
- C++
- Bobcat UI (FLTK-based frontend library)
- Custom data structures (ArrayList, LinkedList, Queue, Stack, etc.)

---

## 🎮 How to Play
1. Choose "New Game" from the menu to start.
2. Click on a column to drop your disc.
3. The disc falls to the lowest available spot.
4. The first player to connect four discs in a row (vertically, horizontally, or diagonally) wins.
5. If the board fills up without a winner, the game ends in a tie.

---

## ⚙️ Settings
- Change board size (e.g., 5x5, 6x7, 7x8).
- Enable or disable AI opponent.

Settings can be accessed through the "Settings" option in the main menu.

---

## 👥 Team Contributions

| Member Name | Role | Main Contributions |
|:------------|:-----|:--------------------|
| Chinomso Nwadike | Game Logic Developer | - Implemented core Connect Four rules (drop piece, win conditions) 
| Adyan Zaman | Frontend UI Developer | - Built and updated Connect Four grid interface using Bobcat UI
| Elson Liang | AI Developer | - Developed Min-Max based AI agent for autonomous gameplay
| Khaleb Lee | Settings + Integration + Testing | - Created Settings interface (board size options, AI toggle)
