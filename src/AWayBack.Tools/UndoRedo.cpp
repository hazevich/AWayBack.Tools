#include "UndoRedo.h"

namespace AWayBack
{
    void Clear(std::vector<UndoRedoCommand*>& commands)
    {
        if (commands.empty()) return;

        for (UndoRedoCommand* command : commands)
        {
            delete command;
        }

        commands.clear();
    }

    void UndoRedoHistory::ExecuteCommand(UndoRedoCommand* command, bool lockMerging)
    {
        Clear(_redoCommands);

        command->IsMergingLocked = lockMerging;

        if (!_undoCommands.empty())
        {
            UndoRedoCommand* latestCommand = _undoCommands.back();
            
            if (latestCommand->GetType() == command->GetType() && !latestCommand->IsMergingLocked && latestCommand->Merge(*command))
            {
                latestCommand->IsMergingLocked = command->IsMergingLocked;
                delete command;
                latestCommand->Execute();
                return;
            }
        }

        _undoCommands.push_back(command);
        command->Execute();
    }

    void UndoRedoHistory::Undo()
    {
        if (_undoCommands.empty()) return;

        UndoRedoCommand* latestCommand = _undoCommands.back();
        _undoCommands.pop_back();
        _redoCommands.push_back(latestCommand);

        latestCommand->Undo();
    }

    void UndoRedoHistory::Redo()
    {
        if (_redoCommands.empty()) return;

        UndoRedoCommand* latestCommand = _redoCommands.back();
        _redoCommands.pop_back();
        _undoCommands.push_back(latestCommand);

        latestCommand->Execute();
    }

    void UndoRedoHistory::Reset()
    {
        Clear(_undoCommands);
        Clear(_redoCommands);
    }

    bool UndoRedoHistory::CanUndo()
    {
        return !_undoCommands.empty();
    }

    bool UndoRedoHistory::CanRedo()
    {
        return !_redoCommands.empty();
    }

}
