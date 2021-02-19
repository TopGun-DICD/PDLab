#include "LuaCodeEditor.hpp"

#include <QRegularExpression>
#include <QPainter>

LuaSyntaxHighlighter::LuaSyntaxHighlighter(QTextDocument* parent) : QSyntaxHighlighter(parent) {
}

void LuaSyntaxHighlighter::addHighlightningRule(QRegularExpression pattern, QTextCharFormat format) {
}

void LuaSyntaxHighlighter::highlightBlock(const QString& text) {
	QRegularExpression expr("\\b(and|break|do|else|elseif|end|false|for|function|if|in|local|nil|not|or|repeat|return|then|true|until|while)\\b");

	QTextCharFormat format;
	format.setForeground(QColor(Qt::blue).lighter(130));

	QRegularExpressionMatch match = expr.match(text);
	while (match.hasMatch()) {
		setFormat(match.capturedStart(), match.capturedLength(), format); 
		match = expr.match(text, match.capturedEnd());
	}
}

LuaCodeEditor::LuaCodeEditor(bool isConsole, QWidget* parent) : QPlainTextEdit(parent) {
	isConsoleWidget = isConsole;
	setParent(parent);
	setLineWrapMode(QPlainTextEdit::NoWrap);
	setFont(QFont("Consolas"));
	if (!isConsole){
		lineNumberArea = new LineNumberArea(this);
		connect(this, SIGNAL(blockCountChanged(qint32)), this, SLOT(slotUpdateLineNumberAreaWidth(qint32)));
		connect(this, SIGNAL(updateRequest(QRect, qint32)), this, SLOT(slotUpdateLineNumberArea(QRect, qint32)));
		connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(slotHighlightCurrentLine()));
		slotUpdateLineNumberAreaWidth(0);
	}
	else{
		setReadOnly(true);
	}

	setShortcutEnabled(1);

	connect(this, SIGNAL(textChanged()), this, SLOT(slotOnTextChanged()));
	highlighter = new LuaSyntaxHighlighter(this->document());
}

qint32 LuaCodeEditor::lineNumberAreaWidth(){
	/*qint32 digits = 1;
	qint32 max = qMax(1, blockCount());
	while (max >= 10) {
		max /= 10;
		++digits;
	}
	
	qint32 space = 20 + fontMetrics().horizontalAdvance(QLatin1Char('9')) + digits;

	return space;	 */
	return 35;
}
void LuaCodeEditor::clearExtraSelections(){
	extraSelections.clear();
}
void LuaCodeEditor::slotUpdateLineNumberAreaWidth(qint32){
	setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}
void LuaCodeEditor::slotUpdateLineNumberArea(const QRect &rect, qint32 dy){
	if (dy)
		lineNumberArea->scroll(0, dy);
	else
		lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

	if (rect.contains(viewport()->rect()))
		slotUpdateLineNumberAreaWidth(0);
}

void LuaCodeEditor::resizeEvent(QResizeEvent *event) {
	QPlainTextEdit::resizeEvent(event);

	if (!isConsoleWidget) {
		QRect cr = contentsRect();
		lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
	}
}

void LuaCodeEditor::keyPressEvent(QKeyEvent* event)	{
	switch(event->key()) {
		case Qt::Key_Up:
			return;
		case Qt::Key_Return:
			//qDebug() << "Ok";
			break;
	}
	QPlainTextEdit::keyPressEvent(event);
}

void LuaCodeEditor::slotHighlightCurrentLine(){
	QList<QTextEdit::ExtraSelection> tmpExtraSelections = extraSelections;
	if (!isReadOnly()) {
		QTextEdit::ExtraSelection selection;
		QColor lineColor = QColor(Qt::yellow).lighter(180);
		selection.format.setBackground(lineColor);
		selection.format.setProperty(QTextFormat::FullWidthSelection, true);
		selection.cursor = textCursor();
		selection.cursor.clearSelection();
		tmpExtraSelections.prepend(selection);
	}
	setExtraSelections(tmpExtraSelections);
}

void LuaCodeEditor::lineNumberAreaPaintEvent(QPaintEvent* event) {
	QPainter painter(lineNumberArea);
	painter.fillRect(event->rect(), QColor(Qt::gray).lighter(150));

	QTextBlock block = firstVisibleBlock();
	qint32 blockNumber = block.blockNumber();
	qint32 top = (qint32)blockBoundingGeometry(block).translated(contentOffset()).top();
	qint32 bot = top + (qint32)blockBoundingRect(block).height();

	while (block.isValid() && top <= event->rect().bottom()) {
		if (block.isVisible() && bot >= event->rect().top()) {
			QString num = QString::number(blockNumber + 1);
			painter.setPen(QColor(100, 100, 100));
			painter.setFont(font());
			painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(), Qt::AlignRight, num);
		}

		block = block.next();
		top = bot;
		bot = top + (qint32)blockBoundingRect(block).height();
		++blockNumber;
	}
}

void LuaCodeEditor::slotOnTextChanged(){
	document()->setModified(true);
}

