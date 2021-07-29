#pragma once

#include <vector>

namespace AWayBack
{
    struct UndoRedoCommand
    {
        virtual ~UndoRedoCommand() = default;

        virtual void Undo() = 0;
        virtual void Execute() = 0;
        virtual const char* GetType() = 0;

        virtual bool Merge(UndoRedoCommand& undoRedoCommand)
        {
            return false;
        }
    };

    class UndoRedoHistory
    {
    public:
        void ExecuteCommand(UndoRedoCommand* command);
        void Undo();
        void Redo();
        void Reset();

    private:
        std::vector<UndoRedoCommand*> _undoCommands;
        std::vector<UndoRedoCommand*> _redoCommands;
    };
}