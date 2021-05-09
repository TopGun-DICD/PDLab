#include "LogTextWidget.hpp"

#include <QRegularExpression>
#include <QPainter>

// https://doc.qt.io/archives/qq/qq21-syntaxhighlighter.html
// https://doc.qt.io/qt-5/qtwidgets-richtext-syntaxhighlighter-example.html

LogSyntaxHighlighter::LogSyntaxHighlighter(QTextDocument* parent) : QSyntaxHighlighter(parent) {
}

void LogSyntaxHighlighter::addHighlightningRule(QRegularExpression pattern, QTextCharFormat format) {
}

void LogSyntaxHighlighter::highlightBlock(const QString& text) {
	/*
	QRegularExpression expr("\\b(and|break|do|else|elseif|end|false|for|function|if|in|local|nil|not|or|repeat|return|then|true|until|while)\\b");

	QTextCharFormat format;
	format.setForeground(QColor(Qt::blue).lighter(130));

	QRegularExpressionMatch match = expr.match(text);
	while (match.hasMatch()) {
		setFormat(match.capturedStart(), match.capturedLength(), format); 
		match = expr.match(text, match.capturedEnd());
	}
	*/
	enum { NormalState = -1, InsideCStyleComment };

	int state = previousBlockState();
	int start = 0;

	for (int i = 0; i < text.length(); ++i) {

		if (text.mid(i, 7) == "[error]") {
			setFormat(i, text.length() - i, Qt::red);
			break;
		}

		if (state == InsideCStyleComment) {
			if (text.mid(i, 2) == "*/") {
				state = NormalState;
				setFormat(start, i - start + 2, Qt::blue);
			}
		}
		else {
			if (text.mid(i, 2) == "//") {
				setFormat(i, text.length() - i, Qt::red);
				break;
			}
			else if (text.mid(i, 2) == "/*") {
				start = i;
				state = InsideCStyleComment;
			}
		}
	}
	if (state == InsideCStyleComment)
		setFormat(start, text.length() - start, Qt::blue);

	setCurrentBlockState(state);
}

LogTextWidget::LogTextWidget(bool isConsole, QWidget* parent) : QPlainTextEdit(parent) {
	//isConsoleWidget = isConsole;
	setParent(parent);
	setLineWrapMode(QPlainTextEdit::NoWrap);
	//setFont(QFont("Consolas"));
	if (!isConsole){
		connect(this, SIGNAL(blockCountChanged(qint32)), this, SLOT(slotUpdateLineNumberAreaWidth(qint32)));
		connect(this, SIGNAL(updateRequest(QRect, qint32)), this, SLOT(slotUpdateLineNumberArea(QRect, qint32)));
		connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(slotHighlightCurrentLine()));
	}
	else{
		setReadOnly(true);
	}

	setShortcutEnabled(1);

	connect(this, SIGNAL(textChanged()), this, SLOT(slotOnTextChanged()));
	highlighter = new LogSyntaxHighlighter(this->document());
}

void LogTextWidget::clearExtraSelections(){
	extraSelections.clear();
}

void LogTextWidget::resizeEvent(QResizeEvent *event) {
	QPlainTextEdit::resizeEvent(event);

	/*if (!isConsoleWidget) {
		QRect cr = contentsRect();
		lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
	}*/
}

void LogTextWidget::slotHighlightCurrentLine(){
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

void LogTextWidget::slotOnTextChanged(){
	document()->setModified(true);
}

