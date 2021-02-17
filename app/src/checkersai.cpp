#include "checkersai.h"
#include "checkers.h"

#include <ctime>
#include <cstdlib>
#include <climits>

extern Checkers* MainWindow;

struct CheckersAI::Impl {
    enum {win = INT_MAX, lose = INT_MIN};

    struct Pieces {
        unsigned int p1Reg = 0;
        unsigned int p2Reg = 0;
        unsigned int p1King = 0;
        unsigned int p2King = 0;
    };

    std::vector<Coords> calculateAllValidMovesForPiece(const CheckersLogic& state, const Coords& piece);
    int randomNumber(unsigned int max);
    int stateEvaluator(const CheckersLogic& state);
    Pieces numberOfEachPlayerPieces(const CheckersLogic& state);
    Move calculateBestMove(const CheckersLogic& state, unsigned int movesLookAhead);
    std::vector<Coords> moveablePieces(const CheckersLogic& state);
    void doMoveSequence(CheckersLogic& state, std::vector<Move> moveSequence);

    class Node {
    public:
        Node(CheckersLogic state, Move move, std::shared_ptr<Node> parent)
            :  mParent(parent),
               mMove(move),
               mState(state)
        {}
        std::vector<std::shared_ptr<Node>> children;
        std::shared_ptr<Node> mParent;
        Move mMove;
        CheckersLogic mState;
        int stateValue;
    };

    void doMoveSequence(CheckersLogic& initialState, std::shared_ptr<Node> node);
    void calculateMovesForNode(std::shared_ptr<Node> node);
    int minimax(std::shared_ptr<Node> node, unsigned int depth, int min, int max);
    bool isLeafNode(std::shared_ptr<Node> node);
};

// chooses move randomly
Move CheckersAI::calculateMoveVeryEasy(const CheckersLogic& state) const {
    // choose a piece to move
    Coords pieceToMove;

    if(state.hasCapturedThisTurn())
        pieceToMove = state.getSelectedPiece();
    else {
        auto moveablePieces = pimpl->moveablePieces(state);
        pieceToMove = moveablePieces[pimpl->randomNumber(moveablePieces.size())];
    }


    // choose a move for the selected piece
    auto possibleMoves = pimpl->calculateAllValidMovesForPiece(state, pieceToMove);

    Coords destination = possibleMoves[pimpl->randomNumber(possibleMoves.size())];

    return {pieceToMove, destination};
}

Move CheckersAI::calculateMoveEasy(const CheckersLogic& state) const {
    return pimpl->calculateBestMove(state, 1);
}

std::vector<Coords> CheckersAI::Impl::calculateAllValidMovesForPiece(const CheckersLogic& state, const Coords& piece) {
    std::vector<Coords> possibleDest;
    possibleDest.reserve(8);

    possibleDest.push_back(CheckersLogic::movePos(piece, upLeftPos));
    possibleDest.push_back(CheckersLogic::movePos(piece, upRightPos));
    possibleDest.push_back(CheckersLogic::movePos(piece, downLeftPos));
    possibleDest.push_back(CheckersLogic::movePos(piece, downRightPos));
    possibleDest.push_back(CheckersLogic::movePos(piece, upLeftPos, 2));
    possibleDest.push_back(CheckersLogic::movePos(piece, upRightPos, 2));
    possibleDest.push_back(CheckersLogic::movePos(piece, downLeftPos, 2));
    possibleDest.push_back(CheckersLogic::movePos(piece, downRightPos, 2));

    std::vector<Coords> result;

    for(auto& destination : possibleDest) {
        bool outOfBounds = destination.x < 0
                        || destination.y < 0
                        || destination.x >= state.number_of_squares_in_board
                        || destination.y >= state.number_of_squares_in_board;

        if(outOfBounds) continue;

        if(state.isValidMove(piece, destination))
            result.push_back(destination);
    }

    return result;
}

// Random number from 0 to max - 1
int CheckersAI::Impl::randomNumber(unsigned int max) {
    srand(time(NULL));
    return rand() % max;
}

CheckersAI::CheckersAI() :
    pimpl(new Impl)
{}

CheckersAI::~CheckersAI() = default;

// Higher is better for player 2
int CheckersAI::Impl::stateEvaluator(const CheckersLogic& state) {
    auto gameState = state.getGameState();

    if(gameState == player2Wins)
        return win;
    else if(gameState == player1Wins)
        return lose;

    auto numOfPieces = numberOfEachPlayerPieces(state);

    const int kingValue = 2;
    int value = numOfPieces.p2Reg - numOfPieces.p1Reg - (kingValue * numOfPieces.p1King) + (kingValue * numOfPieces.p2King);

    return value;
}

CheckersAI::Impl::Pieces CheckersAI::Impl::numberOfEachPlayerPieces(const CheckersLogic &state) {
    Pieces count;

    for(auto& row : state.getBoard()) {
        for(int s : row) {
            if(s == empty)
                continue;

            switch (s) {
                case player1Piece:
                    ++count.p1Reg;
                    break;
                case player2Piece:
                    ++count.p2Reg;
                    break;
                case player1KingPiece:
                    ++count.p1King;
                    break;
                default:
                    ++count.p2King;
            }
        }
    }

    return count;
}

// Assumes valid move exists
Move CheckersAI::Impl::calculateBestMove(const CheckersLogic& state, unsigned int movesLookAhead) {
    if(movesLookAhead == 0) movesLookAhead = 1;

    // generate tree of all possible first moves
    std::shared_ptr<Node> root {new Node(state, {{0,0},{0,0}}, nullptr)};

    std::vector<std::shared_ptr<Node>> leafs {{root}};
    for(unsigned int i = 0; i < movesLookAhead * 2; ++i) {
        for(auto& leaf : leafs) {
            if(leaf->mState.getGameState() != current)
                continue;

            calculateMovesForNode(leaf);
        }

        std::vector<std::shared_ptr<Node>> newLeafs;
        for(auto& n : leafs)
            newLeafs.insert(newLeafs.end(), n->children.begin(), n->children.end());
        leafs.swap(newLeafs);
    }

    minimax(root, movesLookAhead * 2, lose, win);

    // pick best state
    int highestState = lose;
    std::vector<Move> bestMoves;
    for(auto& child : root->children) {
        if(child->stateValue > highestState) {
            highestState = child->stateValue;
            bestMoves.clear();
            bestMoves.push_back(child->mMove);
        } else if(child->stateValue == highestState)
            bestMoves.push_back(child->mMove);
    }

    auto bestMove = bestMoves[randomNumber(bestMoves.size())];

    return bestMove;
}

std::vector<Coords> CheckersAI::Impl::moveablePieces(const CheckersLogic& state) {
    if(state.hasCapturedThisTurn())
        return {state.getSelectedPiece()};

    std::vector<Coords> list;

    for(int i = 0; i < MainWindow->number_of_squares_in_board; ++i)
        for(int j = 0; j < MainWindow->number_of_squares_in_board; ++j)
            if(state.isMoveable({i, j}))
                list.push_back({i, j});

    return list;
}

void CheckersAI::Impl::doMoveSequence(CheckersLogic& state, std::vector<Move> moveSequence) {
    for(auto& move : moveSequence) {
        state.selectPiece(move.start);
        state.movePiece(move.destination);
    }
}

void CheckersAI::Impl::doMoveSequence(CheckersLogic& initialState, std::shared_ptr<Node> node) {
    std::vector<Move> moves;

    while(node->mParent)
        moves.push_back(node->mMove);

    doMoveSequence(initialState, moves);
}

void CheckersAI::Impl::calculateMovesForNode(std::shared_ptr<CheckersAI::Impl::Node> node) {
    for(auto& p : moveablePieces(node->mState)) {
        for(auto& d : calculateAllValidMovesForPiece(node->mState, p)) {
            auto newState {node->mState};
            doMoveSequence(newState, {{p, d}});
            node->children.emplace_back(new Node(newState, {p, d}, node));
        }
    }
}

int CheckersAI::Impl::minimax(std::shared_ptr<CheckersAI::Impl::Node> node, unsigned int depth, int min, int max) {
    if(depth == 0 || isLeafNode(node)) {
        node->stateValue = stateEvaluator(node->mState);
        return node->stateValue;
    }

    if(node->mState.isPlayer1Turn()) {
        int value = max;

        for(auto& child : node->children) {
            int childValue = minimax(child, depth - 1, min, value);

            if(childValue < value) value = childValue;

            if(value < min) {
                node->stateValue = min;
                return min;
            }
        }

        node->stateValue = value;
        return value;
    } else {
        int value = min;

        for(auto& child : node->children) {
            int childValue = minimax(child, depth - 1, value, max);

            if(childValue > value) value = childValue;

            if(value > max) {
                node->stateValue = max;
                return max;
            }
        }

        node->stateValue = value;
        return value;
    }
}

bool CheckersAI::Impl::isLeafNode(std::shared_ptr<CheckersAI::Impl::Node> node) {
    return node->children.empty();
}
