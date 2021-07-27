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

    void UndoRedoHistory::AddCommand(UndoRedoCommand* command)
    {
        Clear(_redoCommands);

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
}
