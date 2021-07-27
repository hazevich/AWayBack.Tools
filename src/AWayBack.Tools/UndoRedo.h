#pragma once

#include <vector>

namespace AWayBack
{
    struct UndoRedoCommand
    {
        virtual ~UndoRedoCommand() = default;

        virtual void Undo() = 0;
        virtual void Execute() = 0;
    };

    class UndoRedoHistory
    {
    public:
        void AddCommand(UndoRedoCommand* command);
        void Undo();
        void Redo();

    private:
        std::vector<UndoRedoCommand*> _undoCommands;
        std::vector<UndoRedoCommand*> _redoCommands;
    };
}