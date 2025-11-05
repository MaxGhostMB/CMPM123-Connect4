#include "Connect4.h"

Connect4::Connect4() {
    _grid = new Grid(7, 6);
}

Connect4::~Connect4() {
    delete _grid;
}

void Connect4::setUpBoard() {
    setNumberOfPlayers(2);
    _gameOptions.rowX = WIDTH;
    _gameOptions.rowY = HEIGHT;

    // Initialize all squares
    _grid->initializeSquares(80, "square.png");
    Player* redPlayer = getPlayerAt(RED_PLAYER);
    Player* yellowPlayer = getPlayerAt(YELLOW_PLAYER);
    if (gameHasAI()){
        if (_gameOptions.AIPlayer == 1) {
            setAIPlayer(YELLOW_PLAYER);
        }
    }
    startGame();
}

Bit* Connect4::PieceForPlayer(Player* player)
{
    // depending on playerNumber load the "x.png" or the "o.png" graphic
    Bit *bit = new Bit();
    // should possibly be cached from player class?
    bit->LoadTextureFromFile(player == getPlayerAt(YELLOW_PLAYER) ? "yellow.png" : "red.png");
    bit->setOwner(player);
    return bit;
}

Player* Connect4::checkForWinner() {
    Player* winner = nullptr;

    int array[] = { 3,2,4,1,5,0,6 };

    for (int i = 0; i < 7; ++i) {
        int x = array[i];
        for (int y = 0; y < HEIGHT; ++y) {
            ChessSquare* square = _grid->getSquare(x, y);
            if (!square->bit()) continue; // empty square
            Player* owner = square->bit()->getOwner();
            if (!owner) continue;

            // Horizontal
            if (x + 3 < WIDTH &&
                _grid->getSquare(x + 1, y)->bit() &&
                _grid->getSquare(x + 2, y)->bit() &&
                _grid->getSquare(x + 3, y)->bit() &&
                _grid->getSquare(x + 1, y)->bit()->getOwner() == owner &&
                _grid->getSquare(x + 2, y)->bit()->getOwner() == owner &&
                _grid->getSquare(x + 3, y)->bit()->getOwner() == owner)
                return owner;

            // Vertical
            if (y + 3 < HEIGHT &&
                _grid->getSquare(x, y + 1)->bit() &&
                _grid->getSquare(x, y + 2)->bit() &&
                _grid->getSquare(x, y + 3)->bit() &&
                _grid->getSquare(x, y + 1)->bit()->getOwner() == owner &&
                _grid->getSquare(x, y + 2)->bit()->getOwner() == owner &&
                _grid->getSquare(x, y + 3)->bit()->getOwner() == owner)
                return owner;

            // Diagonal down-right
            if (x + 3 < WIDTH && y + 3 < HEIGHT &&
                _grid->getSquare(x + 1, y + 1)->bit() &&
                _grid->getSquare(x + 2, y + 2)->bit() &&
                _grid->getSquare(x + 3, y + 3)->bit() &&
                _grid->getSquare(x + 1, y + 1)->bit()->getOwner() == owner &&
                _grid->getSquare(x + 2, y + 2)->bit()->getOwner() == owner &&
                _grid->getSquare(x + 3, y + 3)->bit()->getOwner() == owner)
                return owner;

            // Diagonal up-right
            if (x + 3 < WIDTH && y - 3 >= 0 &&
                _grid->getSquare(x + 1, y - 1)->bit() &&
                _grid->getSquare(x + 2, y - 2)->bit() &&
                _grid->getSquare(x + 3, y - 3)->bit() &&
                _grid->getSquare(x + 1, y - 1)->bit()->getOwner() == owner &&
                _grid->getSquare(x + 2, y - 2)->bit()->getOwner() == owner &&
                _grid->getSquare(x + 3, y - 3)->bit()->getOwner() == owner)
                return owner;
        }
    }

    return nullptr; // no winner found
}

bool Connect4::checkForDraw() {
    std::string state = stateString();
    for (int i = 0; i < 42; i++) {
        if (state[i] == '0') {
            return false;
        }
    }
    return true;
}

std::string Connect4::initialStateString() {
    std::string state(42, '0');
    return state;
}

std::string Connect4::stateString() {
    std::string state;
    _grid->forEachSquare([&state, this](ChessSquare* square, int x, int y) {
        Bit* bit = square->bit();
        if (!bit) {
            state += '0';
        } else if (bit->getOwner() == getPlayerAt(RED_PLAYER)) {
            state += '1';
        } else {
            state += '2';
        }
    });
    // std::cout << state << std::endl;
    return state;
}

void Connect4::setStateString(const std::string &s) {
    if (s.length() != 42) return;
    _grid->setStateString(s);
    // Recreate pieces from state
    size_t index = 0;
    _grid->forEachEnabledSquare([&](ChessSquare* square, int x, int y) {
        if (index < s.length()) {
            int pieceType = s[index++] - '0';
            if (pieceType != 0) {
                Bit* piece = PieceForPlayer(getPlayerAt(pieceType));
                piece->setPosition(square->getPosition());
                square->setBit(piece);
            }
        }
    });
}

void Connect4::bitMovedFromTo(Bit &bit, BitHolder &src, BitHolder &dst) {}

bool Connect4::actionForEmptyHolder(BitHolder &holder) {
    if (holder.bit()) return false;

    ChessSquare* square = static_cast<ChessSquare*>(&holder);
    int x = square->getColumn();
    int y = square->getRow();
    Player* currentPlayer = getCurrentPlayer();

    ImVec2 pos = _grid->getSquare(x,0)->getPosition();

    // Place the piece 
    Bit* newPiece = PieceForPlayer(currentPlayer);
    newPiece->setPosition(pos);
    _grid->getSquare(x,0)->setBit(newPiece);
    // std::cout << "hello\n";
    for (int i = 5; i >= 0; i--) {
        if (_grid->getSquare(x, i)->empty()) {
            ChessSquare *finalplace = _grid->getSquare(x, i); //get square at top of colum
            finalplace->setBit(newPiece);
            pos = finalplace->getPosition();
            newPiece->moveTo(pos);
            break;
        }
    }
    endTurn();
    return true;
}

bool Connect4::canBitMoveFrom(Bit &bit, BitHolder &src) {
    return false; // can't move after being placed
}

bool Connect4::canBitMoveFromTo(Bit &bit, BitHolder &src, BitHolder &dst) {
    return false; // can't move after being placed 
}

void Connect4::stopGame() {
    _grid->forEachSquare([](ChessSquare* square, int x, int y) {
        square->destroyBit();
    });
}

Player* Connect4::ownerAt(int x, int y) const
{
    if (x < 0 || x >= 7 || y < 0 || y >= 6) {
        return nullptr;
    }

    auto square = _grid->getSquare(x, y);
    if (!square || !square->bit()) {
        return nullptr;
    }
    return square->bit()->getOwner();
}

int Connect4::winnerFromState(const std::string &state) {
    int array[] = { 3,2,4,1,5,0,6 };

    auto At = [&](int x, int y) -> char {
        if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return '0';
        return state[y * WIDTH + x];
    };

    for (int i = 0; i < 7; i++) {
        int x = array[i];
        for (int y = 0; y < HEIGHT; y++) {
            char player = At(x, y);
            if (player == '0') continue;
            // check horizontals
            if (x + 3 < WIDTH && 
                At(x+1, y) == player &&
                At(x+2, y) == player && 
                At(x+3, y) == player) {
                return (player == '1') ? 0 : 1;
            }
            // check verticals
            if (y + 3 < HEIGHT && 
                At(x, y+1) == player && 
                At(x, y+2) == player && 
                At(x, y+3) == player) {
                return (player == '1') ? 0 : 1;
            }
            // check diag down-right
            if (x + 3 < WIDTH && y + 3 < HEIGHT &&
                At(x+1, y+1) == player && 
                At(x+2, y+2) == player && 
                At(x+3, y+3) == player) {
                return (player == '1') ? 0 : 1;
            }
            // check diag up-right
            if (x + 3 < WIDTH && y - 3 >= 0 &&
                At(x+1, y-1) == player && 
                At(x+2, y-2) == player && 
                At(x+3, y-3) == player) {
                return (player == '1') ? 0 : 1;
            }
        }
    }
    if (state.find('0') == std::string::npos) {
        return 2;
    }
    return -1; 
}

int Connect4::evaluateStateForPlayer(const std::string &state, int player) {
    int winner = winnerFromState(state);
    if (winner == player) {
        return +100000;
    }
    if (winner == 1 - player) {
        return -100000;
    }
    if (winner == 2) {
        return 0;
    }

    // best way to access the grid 
    auto At = [&](int x, int y) -> char {
        if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return '0';
        return state[y * WIDTH + x];
    };

    int score = 0;
    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            char c = At(x, y);
            if (c == '0') continue;

            int _player = (c == '1') ? 0 : 1;
            int factor = (_player == player) ? 1 : -1;

            // Check all 4 directions
            std::array<std::pair<int,int>,4> directions = {{{1,0}, {0,1}, {1,1}, {1,-1}}};

            for (auto [dx, dy] : directions) {
                int count = 1;
                bool blocked = false;

                for (int step = 1; step < 4; ++step) {
                    char next = At(x + dx*step, y + dy*step);
                    if (next == c) {
                        count++;
                    }
                    else if (next != '0') {
                        blocked = true; 
                        break; 
                    }
                    else break; // open space stops count
                }

                if (blocked) continue;

                // Weight longer sequences more heavily
                if (count == 2) {
                    score += factor * 10;
                } else if (count == 3) {
                    score += factor * 50;
                } else if (count == 4) {
                    score += factor * 100000;
                }
            }
        }
    }

    return score;
}

std::string Connect4::applyMove(const std::string &state, int x, int y, int player) {
    std::string s = state;
    s[y * WIDTH + x] = (player == 0 ? '1' : '2');
    return s;
}
int Connect4::getLowestEmptyRow(const std::string &state, int col) {
    for (int y = 5; y >= 0; y--) {
        if (state[y * 7 + col] == '0') {
            return y;
        }
    }
    return -1;
}

int Connect4::negamax(const std::string &state, int depth, int player, int alpha, int beta) {
    int eval = evaluateStateForPlayer(state, player);

    if (eval == 100000 || eval == -100000 || depth == 0 || winnerFromState(state) == 2) {
        // std::cout << "depth: " << depth << std::endl;
        return eval;
    }

    int best = -10000000;
    for (int x = 0; x < WIDTH; x++) {
        int y = getLowestEmptyRow(state, x);
        if (y == -1) continue;
        std::string next = applyMove(state, x, y, player);
        int val = -negamax(next, depth - 1, 1 - player, -beta, -alpha);
        best = std::max(best, val);
        alpha = std::max(alpha, val);
        if (alpha >= beta) break;
    }
    return best;
}

void Connect4::updateAI() {
    std::string state = stateString();
    int bestVal = -10000000;
    int bestCol = -1;

    for (int x = 0; x < WIDTH; x++) {
        int y = getLowestEmptyRow(state, x);
        if (y == -1) continue;
        std::string next = applyMove(state, x, y, AI_PLAYER);
        int val = -negamax(next, MAX_DEPTH - 1, 1 - AI_PLAYER, -10000000, 10000000);
        if (val > bestVal) {
            bestVal = val;
            bestCol = x;
        }
    }

    if (bestCol != -1) {
        BitHolder* square = _grid->getSquare(bestCol, 0);
        actionForEmptyHolder(*square);
    }
}