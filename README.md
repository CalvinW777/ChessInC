# ChessInC

ChessInc is a fully functional chess engine and graphical interface written in **C using Raylib**.  
The project focuses on implementing **complete chess rules and move validation from scratch**, with a clean and modular architecture.

The goal of ChessInC is to serve as a learning project for:

- Game engine logic
- Chess rule implementation
- Graphics programming with Raylib
- Clean C project structure

---

# Features

## Core Chess Rules
- Full legal move generation
- Turn-based play
- Check detection
- Checkmate detection
- Stalemate detection

## Piece Movement
- Pawns (single move, double move, captures)
- Knights
- Bishops
- Rooks
- Queen
- King

## Special Rules
- Castling
- En passant
- Pawn promotion

## Game Logic
- Illegal move prevention (moves that leave the king in check)
- Legal move highlighting
- Automatic checkmate/stalemate detection

## GUI
- Chessboard rendered with **Raylib**
- Piece textures
- Square highlighting
- Click-based movement

---

# Tech Stack

Language: C

## Running on macOS

This project was developed on Windows and includes a prebuilt `chess.exe`.
macOS cannot run `.exe` files, but the project can be compiled easily because it is written in C using **raylib**, which is cross-platform.

### 1. Install Homebrew (if you don't have it)

Open Terminal and run:

```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

### 2. Install raylib

```bash
brew install raylib
```

### 3. Clone or download the repository

```bash
git clone https://github.com/YOUR_USERNAME/ChessInc.git
cd ChessInc
```

Or download the ZIP from GitHub and open the folder in Terminal.

### 4. Compile the project

```bash
gcc main.c Functions.c -o chess -lraylib
```

### 5. Run the game

```bash
./chess
```

If the assets folder is present in the project directory, the chess pieces will load automatically.

### Folder structure required

```
ChessInc
│
├─ main.c
├─ Functions.c
├─ Functions.h
├─ chess (compiled executable)
└─ assets
    ├─ white-pawn.png
    ├─ white-rook.png
    ├─ white-knight.png
    ├─ white-bishop.png
    ├─ white-queen.png
    ├─ white-king.png
    ├─ black-pawn.png
    ├─ black-rook.png
    ├─ black-knight.png
    ├─ black-bishop.png
    ├─ black-queen.png
    └─ black-king.png
```

