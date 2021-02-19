#include "checkersai.h"
#include "checkers.h"

#include <ctime>
#include <cstdlib>
#include <climits>

extern Checkers* MainWindow;

struct CheckersAI::Impl {
    enum {win = INT_MAX, lose = INT_MIN};

    int currentValue = lose;

    struct Pieces {
        unsigned int p1Reg = 0;
        unsigned int p2Reg = 0;
        unsigned int p1King = 0;
        unsigned int p2King = 0;
    };

    class Node {
    public:
        Node(Move move, std::shared_ptr<Node> parent)
            :  mParent(parent),
               mMove(move)
        {}
        std::vector<std::shared_ptr<Node>> children;
        std::shared_ptr<Node> mParent;
        Move mMove;
        int stateValue;
    };

    std::vector<Coords> calculateAllValidMovesForPiece(const CheckersLogic& state, const Coords& piece) const;
    int randomNumber(unsigned int max) const;
    int stateEvaluator(const CheckersLogic& state) const;
    Pieces numberOfEachPlayerPieces(const CheckersLogic& state) const;
    Move calculateBestMove(const CheckersLogic& state, unsigned int movesLookAhead);
    std::vector<Coords> moveablePieces(const CheckersLogic& state) const;
    CheckersLogic doMoveSequence(const CheckersLogic& initialState, const std::shared_ptr<Node>& node) const;
    CheckersLogic doMoveSequence(const CheckersLogic& initialState, const std::vector<Move>& moves) const;
    void calculateMovesForNode(const CheckersLogic& initialState, const std::shared_ptr<Node>& node) const;
    int minimax(const CheckersLogic& initialState, const std::shared_ptr<Node>& node, unsigned int depth, int min, int max) const;
    void minimaxRoot(const CheckersLogic& initialState, const std::shared_ptr<Node>& root, unsigned int depth);
    Move bestMove(const std::shared_ptr<Node>& root) const;
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
    return pimpl->calculateBestMove(state, 3);
}

std::vector<Coords> CheckersAI::Impl::calculateAllValidMovesForPiece(const CheckersLogic& state, const Coords& piece) const {
    std::vector<Coords> possibleDest;
    possibleDest.reserve(8);

    if(state.getBoard()[piece.x][piece.y] != player2Piece) {
        possibleDest.push_back(CheckersLogic::movePos(piece, upLeftPos));
        possibleDest.push_back(CheckersLogic::movePos(piece, upRightPos));
        possibleDest.push_back(CheckersLogic::movePos(piece, upLeftPos, 2));
        possibleDest.push_back(CheckersLogic::movePos(piece, upRightPos, 2));
    }

    if(state.getBoard()[piece.x][piece.y] != player1Piece) {
        possibleDest.push_back(CheckersLogic::movePos(piece, downLeftPos));
        possibleDest.push_back(CheckersLogic::movePos(piece, downRightPos));
        possibleDest.push_back(CheckersLogic::movePos(piece, downLeftPos, 2));
        possibleDest.push_back(CheckersLogic::movePos(piece, downRightPos, 2));
    }

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
int CheckersAI::Impl::randomNumber(unsigned int max) const {
    srand(time(NULL));
    return rand() % max;
}

CheckersAI::CheckersAI() :
    pimpl(new Impl)
{}

CheckersAI::~CheckersAI() = default;

// Higher is better for player 2
int CheckersAI::Impl::stateEvaluator(const CheckersLogic& state) const {
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

CheckersAI::Impl::Pieces CheckersAI::Impl::numberOfEachPlayerPieces(const CheckersLogic& state) const {
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

    std::shared_ptr<Node> root {new Node({{0,0},{0,0}}, nullptr)};

    minimaxRoot(state, root, movesLookAhead * 2);

//    auto itr = std::max_element(root->children.begin(), root->children.end(),
//             [&] (const std::shared_ptr<Node>& lhs, const std::shared_ptr<Node>& rhs)
//    { return lhs->stateValue < rhs->stateValue; });

//    currentValue = itr->get()->stateValue;

    return bestMove(root);
}

std::vector<Coords> CheckersAI::Impl::moveablePieces(const CheckersLogic& state) const {
    if(state.hasCapturedThisTurn())
        return {state.getSelectedPiece()};

    std::vector<Coords> result;

    for(int i = 0; i < MainWindow->number_of_squares_in_board; ++i)
        for(int j = 0; j < MainWindow->number_of_squares_in_board; ++j) {
            if(state.isMoveable({i, j}))
                result.push_back({i, j});
        }

    return result;
}

CheckersLogic CheckersAI::Impl::doMoveSequence(const CheckersLogic& initialState, const std::shared_ptr<CheckersAI::Impl::Node>& node) const {
    std::vector<Move> moves;
    auto currentNode {node};

    while(currentNode->mParent) {
        moves.push_back(currentNode->mMove);
        currentNode = currentNode->mParent;
    }

    return doMoveSequence(initialState, moves);
}

// Assumes moves are in reverse order
CheckersLogic CheckersAI::Impl::doMoveSequence(const CheckersLogic& initialState, const std::vector<Move>& moves) const {
    auto newState {initialState};

    for(auto itr = moves.rbegin(); itr != moves.rend(); ++itr) {
        newState.selectPiece((*itr).start);
        newState.movePiece((*itr).destination);
    }

    return newState;
}

void CheckersAI::Impl::calculateMovesForNode(const CheckersLogic& initialState, const std::shared_ptr<CheckersAI::Impl::Node>& node) const {
    for(auto& p : moveablePieces(doMoveSequence(initialState, node))) {
        for(auto& d : calculateAllValidMovesForPiece(doMoveSequence(initialState, node), p)) {
            node->children.emplace_back(new Node({p, d}, node));
        }
    }
}

int CheckersAI::Impl::minimax(const CheckersLogic& initialState, const std::shared_ptr<CheckersAI::Impl::Node>& node, unsigned int depth, int min, int max) const {
    auto nodeState = doMoveSequence(initialState, node);

    if(depth == 0 || (nodeState.getGameState() != current))
        return stateEvaluator(nodeState);

    auto pieces = moveablePieces(doMoveSequence(initialState, node));

    if(nodeState.isPlayer1Turn()) {
        int value = max;

        for(auto& piece : pieces) {
            auto moves = calculateAllValidMovesForPiece(doMoveSequence(initialState, node), piece);

            for(auto& move : moves) {
                std::shared_ptr<CheckersAI::Impl::Node> child {new Node({piece, move}, node)};

                int childValue = minimax(initialState, child, depth - 1, min, value);

                if(childValue < value) value = childValue;

                if(value < min) return min;
            }
        }

        return value;
    } else {
        int value = min;

        for(auto& piece : pieces) {
            auto moves = calculateAllValidMovesForPiece(doMoveSequence(initialState, node), piece);

            for(auto& move : moves) {
                std::shared_ptr<CheckersAI::Impl::Node> child {new Node({piece, move}, node)};

                int childValue = minimax(initialState, child, depth - 1, value, max);

                if(childValue > value) value = childValue;

                if(value > max) return max;
            }
        }

        return value;
    }
}

void CheckersAI::Impl::minimaxRoot(const CheckersLogic& initialState, const std::shared_ptr<CheckersAI::Impl::Node>& root, unsigned int depth) {
    calculateMovesForNode(initialState, root);

    int value = currentValue;

    for(auto& child : root->children) {
        child->stateValue = minimax(initialState, child, depth - 1, value, win);

        if(child->stateValue > value) value = child->stateValue;
    }

    root->stateValue = value;
}

Move CheckersAI::Impl::bestMove(const std::shared_ptr<CheckersAI::Impl::Node>& root) const {
    std::vector<Move> bestMoves;

    for(auto& child : root->children)
        if(child->stateValue == root->stateValue)
            bestMoves.push_back(child->mMove);

    return bestMoves[randomNumber(bestMoves.size())];
}
