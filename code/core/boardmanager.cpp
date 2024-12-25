#include "BoardManager.h"
#include "board.h"

BoardManager::BoardManager(QObject *parent)
    : QObject(parent), currentBoard_(nullptr)
{
}

BoardManager& BoardManager::instance()
{
    // 静态局部变量保证单例模式
    static BoardManager instance;
    return instance;
}

bool BoardManager::IsInitialized() const
{
    return !boardPool_.isEmpty();
}

void BoardManager::AddBoard(const QString& name, std::shared_ptr<Board> board)
{
    if (!board) {
        qWarning() << "Cannot add a null Board with name:" << name;
        return;
    }

    if (boardPool_.contains(name)) {
        qWarning() << "Board with name" << name << "already exists. Skipping.";
        return;
    }

    boardPool_.insert(name, board);

    // 如果当前没有活动的 Board，自动设置为新添加的 Board
    if (!currentBoard_) {
        currentBoard_ = board;
    }

    qDebug() << "Board added with name:" << name;
}

bool BoardManager::SwitchToBoard(const QString& name)
{
    if (!boardPool_.contains(name)) {
        qWarning() << "Board with name" << name << "does not exist.";
        return false;
    }

    auto newBoard = boardPool_.value(name);
    if (currentBoard_ == newBoard) {
        qDebug() << "Already on the Board with name:" << name;
        return true;
    }

    currentBoard_ = newBoard;
    qDebug() << "Switched to Board with name:" << name;

    return true;
}

std::shared_ptr<Board> BoardManager::GetCurrentBoard() const
{
    return currentBoard_;
}
