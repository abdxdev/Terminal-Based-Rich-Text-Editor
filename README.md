# Terminal-Based Text Editor

![alt text](https://github.com/abdxdev/TerminalBasedRichTextEditor/blob/master/screenshots/auto-suggestion.png?raw=true)
![alt text](https://github.com/abdxdev/TerminalBasedRichTextEditor/blob/master/screenshots/color.png?raw=true)

A high-performance terminal-based rich text editor designed for speed and functionality. It supports autosuggestions, undo/redo, and customizable shortcuts. The editor leverages advanced data structures to ensure smooth and efficient text editing.

### Features

1. **Autosuggestions**: Real-time autocomplete powered by Trie.
2. **Undo/Redo**: Stack-based undo/redo operatio.
3. **GapBuffer Efficiency**: Smooth editing and fast cursor movement.
4. **Custom Shortcuts**: Bind frequently used actions for faster workflow.

### Data Structures Used

1. **GapBuffer**: For efficient text editing and cursor movement within a line.
2. **GapBufferLinkedList**: Extends GapBuffer to support multi-line text editing.
3. **Trie**: Powers the autosuggestion system for real-time word predictions.
4. **Stack**: Implements undo/redo functionality.

### Getting Started

1. Install a **Nerd Font** for enhanced terminal visuals from [nerdfonts.com](https://www.nerdfonts.com/font-downloads).
2. Clone the repository and build the project with **C++14** support:
   ```bash
   git clone https://github.com/abdxdev/TerminalBasedRichTextEditor.git
   cd TerminalBasedRichTextEditor
   start TerminalBasedRichTextEditor.sln
   ```
   Ensure your compiler supports **C++14**.
3. Launch the editor in a terminal supporting **Nerd Fonts**. 
> _Note: You need to set the font in your terminal settings._

### Keyboard Shortcuts

#### File Operations:

- `Ctrl+S`: Save file
- `ESC`: Quit editor

#### Editing:

- `Ctrl+Z`: Undo
- `Ctrl+Y`: Redo
- `Arrow Keys`: Move cursor

#### Suggestions:

- `Tab` or `Enter`: Apply suggestion
- `Esc`: Dismiss suggestion

#### Formatting:

- `F1`: Change foreground color
- `F2`: Change background color
- `F3`: Toggle autosuggestions
- `F4`: Refresh screen
- `F5`: Change cursor style
- `Ctrl+B`: Toggle bold
- `Ctrl+I`: Toggle italics
- `Ctrl+U`: Toggle underline
- `Ctrl+T`: Toggle strikethrough

### License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
