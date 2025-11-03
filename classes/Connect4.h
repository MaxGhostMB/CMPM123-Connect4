#pragma once
#include "Game.h"

class Connect4 : public Game
{   
    public:
    Connect4();
    ~Connect4();

    void         setUpBoard() override;
    Player*      checkForWinner() override;
    bool         checkForDraw() override;
    std::string  initialStateString() override;
    std::string  stateString() override;
    void         setStateString(const std::string &s) override;
    bool         actionForEmptyHolder(BitHolder &holder) override;
    bool         canBitMoveFrom(Bit &bit, BitHolder &src) override;
    bool         canBitMoveFromTo(Bit &bit, BitHolder &src, BitHolder &dst) override;
    void         stopGame() override;
    void         bitMovedFromTo(Bit &bit, BitHolder &src, BitHolder &dst) override;


    // AI methods
    int          winnerFromState(const std::string &s);
    int          evaluateStateForPlayer(const std::string &s, int player);
    int          getLowestEmptyRow(const std::string &state, int col);

    std::string  applyMove(const std::string &state, int x, int y, int player);
    int          negamax(const std::string&, int, int, int, int);
    Player*      ownerAt(int, int ) const;
    void         updateAI() override;
    bool         gameHasAI() override { return true; } // Set to true when AI is implemented
    Grid*        getGrid() override { return _grid; }
    int          evaluateAIBoard(Player*);

    private:
    // Helpers
    // ChessSquare* NextEmptyY(const int);
    // char         posAt(int, int, std::string);
    Bit*         PieceForPlayer(Player*);

    static const int RED_PLAYER = 0;
    static const int YELLOW_PLAYER = 1;
    static const int MAX_DEPTH = 7;
    static const int WIDTH = 7;
    static const int HEIGHT = 6;
    
    Grid*        _grid;

};