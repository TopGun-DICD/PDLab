#pragma once

#include <QPlainTextEdit>
#include <QSyntaxHighlighter>

class LineNumberArea;

class LuaSyntaxHighlighter : public QSyntaxHighlighter {
	Q_OBJECT
public:
	LuaSyntaxHighlighter(QTextDocument* parent = nullptr);

	void addHighlightningRule(QRegularExpression pattern, QTextCharFormat format);
	void highlightBlock(const QString& text);
};

class LuaCodeEditor : public QPlainTextEdit	{
	Q_OBJECT
	/*----------Variables declaration----------*/
private:
	LineNumberArea* lineNumberArea;
	QList<QTextEdit::ExtraSelection> extraSelections;
	LuaSyntaxHighlighter* highlighter = nullptr; 

public:
	bool isConsoleWidget;

	/*----------Functions declaration----------*/
public:
	LuaCodeEditor(bool isConsole = false, QWidget* parent = nullptr);

public:
	void		lineNumberAreaPaintEvent(QPaintEvent* ev);
	qint32	lineNumberAreaWidth();
	void		clearExtraSelections();

private:
	void resizeEvent(QResizeEvent* event);
	void keyPressEvent(QKeyEvent* event);

	/*----------Slots declaration----------*/
private slots:
	void	slotUpdateLineNumberAreaWidth(qint32 newBlockCount);
	void	slotHighlightCurrentLine();
	void	slotUpdateLineNumberArea(const QRect &, qint32);
	void	slotOnTextChanged();
};

class LineNumberArea : public QWidget
{
	Q_OBJECT

private:
	LuaCodeEditor* codeEditor;

public:
	LineNumberArea(LuaCodeEditor* editor) : QWidget(editor){
		codeEditor = editor;
	}
	QSize sizeHint() const Q_DECL_OVERRIDE{
		return QSize(codeEditor->lineNumberAreaWidth(), 0);
	}

protected:
	void paintEvent(QPaintEvent* ev) Q_DECL_OVERRIDE {
		codeEditor->lineNumberAreaPaintEvent(ev);
	}

};
