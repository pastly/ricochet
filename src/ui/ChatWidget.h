#ifndef CHATWIDGET_H
#define CHATWIDGET_H

#include <QWidget>

class QDateTime;
class ContactUser;

class ChatWidget : public QWidget
{
Q_OBJECT
public:
	ContactUser * const user;

	static ChatWidget *widgetForUser(ContactUser *user);

	~ChatWidget();

	void addChatMessage(ContactUser *user, const QDateTime &when, const QString &text, int identifier = 0);

private slots:
	void sendInputMessage();
	void messageReply();

private:
	static QHash<ContactUser*,ChatWidget*> userMap;

	class QTextEdit *textArea;
	class QLineEdit *textInput;

	explicit ChatWidget(ContactUser *user);

	void createTextArea();
	void createTextInput();

	void scrollToBottom();

	bool findBlockIdentifier(int identifier, class QTextBlock &block);
};

#endif // CHATWIDGET_H
