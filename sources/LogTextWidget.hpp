#pragma once

#include <QPlainTextEdit>
#include <QSyntaxHighlighter>

class LogSyntaxHighlighter : public QSyntaxHighlighter {
	Q_OBJECT
public:
	LogSyntaxHighlighter(QTextDocument* parent = nullptr);

	void addHighlightningRule(QRegularExpression pattern, QTextCharFormat format);
	void highlightBlock(const QString& text);
};

class LogTextWidget : public QPlainTextEdit	{
	Q_OBJECT
private:
	QList<QTextEdit::ExtraSelection> extraSelections;
	LogSyntaxHighlighter* highlighter = nullptr;
public:
	LogTextWidget(bool isConsole = false, QWidget* parent = nullptr);
public:
	void clearExtraSelections();
private:
	void resizeEvent(QResizeEvent* event);
private slots:
	void slotHighlightCurrentLine();
	void slotOnTextChanged();
};
