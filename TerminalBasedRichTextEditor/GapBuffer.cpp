#include "GapBuffer.h"

GapBuffer::GapBuffer(int initialSize) : buffer(initialSize, '\0'),
                                        gapStart(0),
                                        gapEnd(initialSize),
                                        inSequence(false),
                                        insertStartPosition(-1),
                                        isRedoOperation(false),
                                        visualCursorPosition(0) {
}

void GapBuffer::expandBuffer() {
    int newSize = (buffer.size() == 0) ? 10 : buffer.size() * 2;
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

    if (!isRedoOperation) {
        while (!redoStack.empty()) {
            redoStack.pop();
        }
    }

    if (insertStartPosition == -1) {
        insertStartPosition = gapStart;
    }

    buffer[gapStart++] = c;
    currentInsertBuffer += c;

    if (c == '\033') {
        inSequence = true;
    } else if (inSequence && isalpha(c)) {
        inSequence = false;
    } else if (!inSequence && c != '\033') {
        visualCursorPosition++;
    }

    if (c == '.' || c == '!' || c == '?' || c == '\n' || c == ' ') {
        commitInsertOperation();
    }
}

void GapBuffer::insert(const string& text) {
    for (char c : text) {
        insert(c);
    }
}

void GapBuffer::commitInsertOperation() {
    if (!currentInsertBuffer.empty()) {
        undoStack.push(make_pair(insertStartPosition, currentInsertBuffer));
        currentInsertBuffer.clear();
        insertStartPosition = -1;
    }
}

void GapBuffer::deleteCharLeft() {
    if (gapStart > 0) {

        if (isInAnsiSequence(gapStart - 1)) {

            int seqStart = gapStart - 1;
            while (seqStart > 0 && buffer[seqStart] != '\033') {
                seqStart--;
            }
            int seqLength = gapStart - seqStart;
            gapStart = seqStart;
        } else {
            --gapStart;
            --visualCursorPosition;
        }
    }
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

    int bufferPos = getBufferPosition(position);

    if (bufferPos < 0 || bufferPos > buffer.size() - (gapEnd - gapStart))
        return;

    moveGap(bufferPos);
    visualCursorPosition = getVisualPosition(gapStart);
}

void GapBuffer::moveCursorRelative(int offset) {
    moveCursor(gapStart + offset);
    // visualCursorPosition += offset;
}

bool GapBuffer::isInAnsiSequence(int pos) const {
    int seqStart = pos;
    while (seqStart >= 0) {
        if (buffer[seqStart] == '\033') {
            int seqEnd = seqStart + 1;
            while (seqEnd < buffer.size() && seqEnd < gapStart) {
                if (isalpha(buffer[seqEnd])) {
                    return pos >= seqStart && pos <= seqEnd;
                }
                seqEnd++;
            }
            return pos >= seqStart;
        } else if (seqStart == pos && pos > 0) {
            seqStart--;
        } else {
            break;
        }
    }
    return false;
}

int GapBuffer::getBufferPosition(int visualPos) const {
    int visual = 0;
    int buffer = 0;

    while (visual < visualPos && buffer < getLength()) {
        if (!isInAnsiSequence(buffer))
            visual++;
        buffer++;
    }

    return buffer;
}

int GapBuffer::getVisualPosition(int bufferPos) const {
    int visual = 0;
    for (int i = 0; i < bufferPos; i++)
        if (!isInAnsiSequence(i))
            visual++;
    return visual;
}

int GapBuffer::getAnsiSequenceLength(int start) const {
    if (buffer[start] != '\033') {
        return 0;
    }

    int length = 1;
    while (start + length < buffer.size() && start + length < gapStart) {
        if (isalpha(buffer[start + length])) {
            return length + 1;
        }
        length++;
    }
    return length;
}

int GapBuffer::getBufferCursorPosition() const {
    return gapStart;
}

int GapBuffer::getVisualCursorPosition() const {
    return visualCursorPosition;
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

void GapBuffer::display(int from = 0, int to = -1) const {
    if (to == -1) {
        for (int i = 0; i < gapStart; ++i) {
            cout << buffer[i];
        }
        for (int i = gapEnd; i < buffer.size(); ++i) {
            cout << buffer[i];
        }
    } else {
        int visualPos = 0;
        for (int i = 0; i < buffer.size(); i++) {
            if (i >= gapStart && i < gapEnd)
                continue;

            if (!isInAnsiSequence(i)) {
                if (visualPos >= from && visualPos < to) {
                    cout << buffer[i];
                }
                visualPos++;
            } else {
                cout << buffer[i];
            }
        }
    }
}

string GapBuffer::getDebugText() const {
    string text;
    text.reserve(buffer.size() - (gapEnd - gapStart));
    text.append(buffer.begin(), buffer.begin() + gapStart);

    for (int i = gapStart; i < gapEnd; ++i) {
        text += '_';
    }

    text.append(buffer.begin() + gapEnd, buffer.end());
    return text;
}

string GapBuffer::getLine() const {
    string line;
    line.reserve(buffer.size() - (gapEnd - gapStart));
    line.append(buffer.begin(), buffer.begin() + gapStart);
    line.append(buffer.begin() + gapEnd, buffer.end());
    return line;
}

int GapBuffer::getLength() const {
    return buffer.size() - (gapEnd - gapStart);
}

void GapBuffer::undo() {
    commitInsertOperation();

    if (!undoStack.empty()) {
        auto operation = undoStack.top();
        undoStack.pop();
        redoStack.push(operation);
        moveGap(operation.second.length() + operation.first);
        for (size_t i = 0; i < operation.second.length(); ++i)
            deleteCharLeft();
    }
}

void GapBuffer::redo() {
    if (!redoStack.empty()) {
        auto operation = redoStack.top();
        redoStack.pop();
        isRedoOperation = true;
        moveGap(operation.first);
        insert(operation.second);
        isRedoOperation = false;
    }
}

string GapBuffer::getTextAfterCursor() const {
    string result;
    for (int i = gapEnd; i < buffer.size(); i++) {
        result += buffer[i];
    }
    return result;
}

void GapBuffer::deleteToEnd() {
    buffer.erase(buffer.begin() + gapStart, buffer.end());
    gapEnd = buffer.size();
}
