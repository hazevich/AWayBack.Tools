#pragma once

#include <vector>

namespace AWayBack
{
    struct UndoRedoCommand
    {
        virtual ~UndoRedoCommand() = default;

        bool IsMergingLocked = false;

        virtual void Undo() = 0;
        virtual void Execute() = 0;
        virtual const char* GetType() = 0;

        virtual bool Merge(UndoRedoCommand& other)
        {
            return false;
        }
    };

    class UndoRedoHistory
    {
    public:
        void ExecuteCommand(UndoRedoCommand* command, bool lockMerging = false);
        void Undo();
        void Redo();
        void Reset();

    private:
        std::vector<UndoRedoCommand*> _undoCommands;
        std::vector<UndoRedoCommand*> _redoCommands;
    };
}