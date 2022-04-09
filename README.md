# Chess
Simple chess game written in C++ from scratch

---
## Compilation

### Unix (MacOS/Linux)
1. `cd src/`
2. Make a new folder for building, `cd folder/`
3. Build CMake project `cmake --build ..`
4. Compile chess `make`
5. Enjoy `./Chess`

### Windows
Just load the CMake project with IDE like Visual Studio or CLion

---
## How to Play
- When launching the game, you can select initial position: default, or from FEN or PGN format and describe the game
- Describe the game option allows you to add tags to the game such as names of white and black players.
- Moves are selected through [Algebraic Notation](#algebraic-notation-steps)
- After game ends, you can save the game in PGN format to a game and play again or quit the game

---
# Algebraic Notation Steps
1. [Piece Movement Notation](#piece-movement-notation) or [Castling Notation](#castling-notation)
2. [Optional Checks](#optional-checks)
3. [Comment](#comment)

## Piece Movement Notation
1. Upper case letter given for the [selected piece](#pieces-denotation), if it's a pawn this step can be skipped and pawn is assumed
2. Optional origin coordinates: file and/or rank where the origin square of the piece to be moved is. Used to disambiguate move, when more than one piece can move to the target square.
3. Optional capture symbolic (`x`): indicates whether opponent's piece resides in target square. If no piece was selected (pawn assumed), then file must be given first.
4. Target coordinates: file and rank where the piece will be moved
5. If piece to be moved is a pawn, promotion symbolic may be given (`=P`), where P is [Pieces Denotation](#pieces-denotation), piece may not be king or pawn.

### Examples
- `e4` or `Pe4` (pawn goes to e4 square)
- `Bxe4` (Bishop takes e4 square)
- `Qxf7#` (Queen takes f7 with checkmate)
- `Pe8=Q+` or `e8=Q+` (Pawn moves to e8, promotes to a queen with a check)

---
## Castling Notation
- `O-O` for king-side castling
- `O-O-O` for queen-side castling

---
## Optional Checks
- `+` could be put to denote that after executing the move, piece attacks the opponent's king
- or `#` could be put to denote that after executing the move, opponent has been checkmated.

---
# Comment
- `;` can be put to represent a comment which last to the end of the line
- or `{` could be put to represent a comment which lasts until `}`

---
# Pieces Denotation
- `P` = pawn
- `R` = rook
- `B` = bishop
- `Q` = queen
- `K` = king
- `N` = knight

## FEN format
https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation

## PGN format
https://en.wikipedia.org/wiki/Portable_Game_Notation
