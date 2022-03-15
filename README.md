# Chess
Simple chess game written in C++

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

