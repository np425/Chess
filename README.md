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
- Moves are handled with [Algebraic Notation](https://en.wikipedia.org/wiki/Algebraic_notation_(chess))
- After game ends, you can save the game in PGN format to a game and play again or quit the game

---

# Move Notation Steps
1. [Piece Movement Notation](#Piece Movement Notation) or [Castling Notation](#Castling Notation)
2. [Optional Checks](#Optional Checks)
3. [Comment](#Comment)

## Piece Movement Notation
1. Upper case letter given for the [selected piece](#Pieces Denotation), if it's a pawn this step can be skipped and pawn is assumed
2. Optional origin coordinates: file and/or rank where the origin square of the piece to be moved is. Used to disambiguate move, when more than one piece can move to the target square.
3. Optional capture symbolic (`x`): indicates whether opponent's piece resides in target square. If no piece was selected (pawn assumed), then file must be given first.
4. Target coordinates: file and rank where the piece will be moved
5. If piece to be moved is a pawn, promotion symbolic may be given (`=P`), where P is [Pieces Denotation](#Pieces Denotation), piece may not be king or pawn.

### Examples
- e4 or Pe4 (pawn goes to e4 square)
- Bxe4 (Bishop takes e4 square)
- Qxf7# (Queen takes f7 with checkmate)
- Pe8=Q or e8=Q (Pawn moves to e8 and promote to a queen)

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

---
## Chess Game Steps
1. Board setup
2. Moves Processing

---
### Board Setup
1. Ensure set up fits the board
2. Set up the pieces
3. Invalidate game variables: en passant positions, castling rights.
4. Validate Initial Position

---
### Validate Initial Position

1. Find king positions: ensure exactly two kings are on the board.
2. Ensure kings are not together
3. Ensure no pawns on last ranks: 1st and 8th.
4. Check Validation

---
#### Check Validation
1. Invalidate previous check
2. Find checks by Getting Protecting Pieces for the king square of the enemy.
3. Ensure both players do not have checks.
4. Ignore 2 maximum checks
5. Ignore Discovered Check Rules

---
#### Getting Protecting Pieces
1. Getting Bishop, Queen, Rook Protecting Pieces
2. Getting Knight Protecting Pieces
3. Getting Pawn Protecting Pieces

---
#### Discovered Check Rules:
Must be two opposite movement pieces, except pawns, **and must be able to move back to "cover" the second check piece**  
valid setup:
- Bishop-Queen
- Bishop-Rook
- Bishop-Knight,
- Knight-Rook,
- Knight-Queen,
- Rook-Queen

---
### Moves Processing
Players alternate every move, for each player (each move):
1. Check for checkmate if there are checks, check for stalemate if there are none
2. Retrieve move notation string from player as input.
3. Translate it into a structural variant using notation parser.
4. Select valid moves that match the notation
5. Ensure that there is only one valid move that matches the notation.
6. Execute the move

