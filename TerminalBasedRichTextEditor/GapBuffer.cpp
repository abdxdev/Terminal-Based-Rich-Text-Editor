#include "GapBuffer.h"

GapBuffer::GapBuffer(int initialSize) : buffer(initialSize, '\0'), gapStart(0), gapEnd(initialSize) {}

void GapBuffer::expandBuffer() {
    int newSize = buffer.size() * 2;
    vector<char> newBuffer(newSize, '\0');
    for (int i = 0; i < gapStart; ++i)
        newBuffer[i] = buffer[i];
    for (int i = gapEnd; i < buffer.size(); ++i)
        newBuffer[newSize - (buffer.size() - i)] = buffer[i];
    gapEnd += newSize - buffer.size();
    buffer = move(newBuffer);
}

void GapBuffer::moveGap(int position) {
    if (position < gapStart) 
        while (gapStart > position) 
            buffer[--gapEnd] = buffer[--gapStart];
    else if (position > gapStart) 
        while (gapStart < position) 
            buffer[gapStart++] = buffer[gapEnd++];
}

void GapBuffer::insert(char c) {
    if (gapStart == gapEnd)
        expandBuffer();
    buffer[gapStart++] = c;
}

void GapBuffer::insert(const string& text) {
    for (char c : text) {
        insert(c);
    }
}

void GapBuffer::deleteCharLeft() {
    if (gapStart > 0)
        --gapStart;
}

void GapBuffer::deleteCharRight() {
    if (gapEnd < buffer.size())
        ++gapEnd;
}

void GapBuffer::deleteChars(int count) {
    while (count-- > 0 && gapStart > 0) {
        --gapStart;
    }
}

void GapBuffer::moveCursor(int position) {
    if (position < 0 || position > buffer.size() - (gapEnd - gapStart)) {
        throw "Invalid cursor position";
        return;
    }
    moveGap(position);
}

void GapBuffer::moveCursorRelative(int offset) {
    moveCursor(gapStart + offset);
}

void GapBuffer::saveToFile(const string& filename) const {
    ofstream file(filename);
    if (file.is_open()) {
        for (int i = 0; i < gapStart; ++i)
            file << buffer[i];
        for (int i = gapEnd; i < buffer.size(); ++i)
            file << buffer[i];
        file.close();
    }
}

void GapBuffer::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (file.is_open()) {
        string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
        buffer = vector<char>(content.begin(), content.end());
        gapStart = content.size();
        gapEnd = buffer.size();
        file.close();
    }
}

int GapBuffer::find(const string& text) const {
    string combinedBuffer;
    combinedBuffer.reserve(buffer.size() - (gapEnd - gapStart));
    combinedBuffer.append(buffer.begin(), buffer.begin() + gapStart);
    combinedBuffer.append(buffer.begin() + gapEnd, buffer.end());

    int pos = combinedBuffer.find(text);
    return (pos != string::npos) ? static_cast<int>(pos) : -1;
}

void GapBuffer::replace(const string& oldText, const string& newText) {
    int pos = find(oldText);
    if (pos != -1) {
        moveCursor(pos);
        deleteChars(oldText.size());
        insert(newText);
    }
}

void GapBuffer::copy(int start, int length) {
    clipboard.clear();
    for (int i = start; i < start + length && i < gapStart; ++i)
        clipboard += buffer[i];
    for (int i = gapEnd; i < gapEnd + length && i < buffer.size(); ++i)
        clipboard += buffer[i];
}

void GapBuffer::paste() {
    insert(clipboard);
}

void GapBuffer::selectText(int start, int end) {
    selection = {start, end};
}

void GapBuffer::deleteSelection() {
    if (selection.first != -1 && selection.second != -1) {
        moveCursor(selection.first);
        deleteChars(selection.second - selection.first);
        selection = {-1, -1};
    }
}

void GapBuffer::display() const {
    for (int i = 0; i < gapStart; ++i)
        cout << buffer[i];
    for (int i = gapEnd; i < buffer.size(); ++i)
        cout << buffer[i];
    cout << '\n';
}

string GapBuffer::getDebugText() const {
	string text;
	text.reserve(buffer.size() - (gapEnd - gapStart));
	text.append(buffer.begin(), buffer.begin() + gapStart);
    text.append("_");
	text.append(buffer.begin() + gapEnd, buffer.end());
	return text;
}

int GapBuffer::getLength() const {
	return buffer.size() - (gapEnd - gapStart);
}

int GapBuffer::getCursorPosition() const {
    return gapStart;
}

void GapBuffer::undo() {
    if (!undoStack.empty()) {
		pair<int, string> operation = undoStack.top();
		undoStack.pop();
		moveCursor(operation.first);
		deleteChars(operation.second.size());
		redoStack.push(operation);
    }
}

void GapBuffer::redo() {
    if (!redoStack.empty()) {
		pair<int, string> operation = redoStack.top();
        redoStack.pop();
        moveCursor(operation.first);
        insert(operation.second);
        undoStack.push(operation);
    }
}
