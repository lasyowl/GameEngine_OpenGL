#include "Text.h"
#include "Font.h"

using namespace std;
using namespace glm;

Text::Text() : currentCharIndex(0), numChars(0), fontSize(0.5f), totalHeight(0.0f), isActive(false) {
	currentString = msgInfo.begin();
	color_first = vec4(0.1f, 0.1f, 0.1f, 1.0f);
	color_end = vec4(0.1f, 0.1f, 0.1f, 1.0f);
}

Text::Text(const glm::vec2 &origin, const glm::vec2 &size) : currentCharIndex(0), numChars(0), fontSize(0.5f), totalHeight(0.0f), isActive(true) {
	this->origin = origin;
	this->size = size;
	currentString = msgInfo.begin();
	color_first = vec4(0.1f, 0.1f, 0.1f, 1.0f);
	color_end = vec4(0.1f, 0.1f, 0.1f, 1.0f);
}

Text::~Text() {

}

void Text::Activate() {
	isActive = true;
}

void Text::Deactivate() {
	isActive = false;
}

void Text::SetFont(Font *font) {
	this->font = font;
}

void Text::SetFontSize(const float &size) {
	this->fontSize = size;
}

void Text::SetSize(const vec2 &size) {
	this->size = size;
}

void Text::SetOrigin(const vec2 &origin) {
	this->origin = origin;
}

void Text::DeleteMessage(const int &index) {
	std::list<MsgInfo>::iterator target = msgInfo.begin();
	if (target == msgInfo.end()) {
		numChars = 0;
		return;
	}

	for (int i = 0; i < index; i++) {
		target++;
	}

	totalHeight -= target->msgHeight;
	numChars -= target->message.size();
	msgInfo.erase(target);
	currentString = msgInfo.begin();
}

void Text::AddMessage(wstring msg) {
	MsgInfo temp;
	temp.visible = true;
	temp.message = msg;
	temp.charOffset.resize(temp.message.size());
	temp.vao.resize(temp.message.size());
	temp.color.resize(temp.message.size());
	list<MsgInfo>::iterator iter = msgInfo.end();
	if (iter != msgInfo.begin()) {
		iter--;
		temp.charOffset[0] = vec2(0, 0) + vec2(0, 0);
		temp.msgOffset = vec2(0, 0) + vec2(0, 0);
		temp.msgHeight = font->lineHeight;
	}
	else {
		temp.msgOffset = vec2(0, 0);
		temp.charOffset[0] = vec2(0, 0);
		temp.msgHeight = font->lineHeight;
	}
	temp.charOffset[0].y = font->fontInfo[temp.message[0]].newOffset.y;

	for (int i = 1; i < temp.charOffset.size(); i++) {
		int previousChar = temp.message[i - 1];
		int currentChar = temp.message[i];
		FontInfo *pFontInfo = &font->fontInfo[previousChar];
		FontInfo *cFontInfo = &font->fontInfo[currentChar];
		float xOffset = pFontInfo->offset.x;
		vec2 spacing;
		if (font->hasKernings) {
			if (font->kernings.count(previousChar) > 0) {
				map<int, int>::iterator iter = font->kernings[previousChar].spacing.find(temp.message[i]);
				if (iter != font->kernings[previousChar].spacing.end())
					spacing = vec2(pFontInfo->xAdvance + font->spacing.x - xOffset + iter->second, 0);
				else spacing = vec2(pFontInfo->xAdvance + font->spacing.x - xOffset, 0);
			}
			else spacing = vec2(pFontInfo->xAdvance + font->spacing.x - xOffset, 0);
		}
		else spacing = vec2(pFontInfo->xAdvance + font->spacing.x - xOffset, 0);

		temp.charOffset[i] = temp.charOffset[i - 1] + vec2(spacing.x, 0);
		if (temp.charOffset[i].x + cFontInfo->size.x > size.x / fontSize) {
			for (int j = 0; j < i; j++) {
				temp.charOffset[j].y += font->lineHeight;
			}
			temp.charOffset[i].x = 0;
			temp.msgHeight += font->lineHeight;
		}
		temp.charOffset[i].y = font->fontInfo[temp.message[i]].newOffset.y;
	}

	for (int i = 0; i < temp.charOffset.size(); i++) {
		temp.vao[i] = font->GetVao(temp.message[i]);
	}

	PaintText(&temp);

	iter = msgInfo.begin();
	while (iter != msgInfo.end()) {
		iter->msgOffset.y += temp.msgHeight;
		for (int i = 0; i < iter->charOffset.size(); i++) {
			iter->charOffset[i].y += temp.msgHeight;
		}
		++iter;
	}
	totalHeight += temp.msgHeight;

	iter = msgInfo.begin();
	while (iter != msgInfo.end()) {
		if (iter->msgOffset.y > size.y / fontSize) {
			iter->visible = false;
		}
		else {
			iter->visible = true;
			break;
		}
		++iter;
	}

	msgInfo.push_back(temp);
	numChars += temp.message.size();

	currentString = msgInfo.begin();
	currentCharIndex = 0;
}

void Text::InsertChar(const int &character) {
	list<MsgInfo>::iterator iter = msgInfo.end();
	if(iter != msgInfo.begin())
		iter--;
	else {
		wstring newStr;
		newStr += character;
		AddMessage(newStr);
		return;
	}
	iter->message.resize(iter->message.size() + 1);
	iter->charOffset.resize(iter->message.size() + 1);
	iter->vao.resize(iter->message.size() + 1);
	iter->color.resize(iter->message.size() + 1);
	int currentIndex = iter->message.size() - 1;
	int previousChar = iter->message[currentIndex - 1];
	int currentChar = character;
	iter->message[currentIndex] = currentChar;

	FontInfo *pFontInfo = &font->fontInfo[previousChar];
	FontInfo *cFontInfo = &font->fontInfo[currentChar];
	float xOffset = pFontInfo->offset.x;
	vec2 spacing;
	if (font->hasKernings) {
		if (font->kernings.count(previousChar) > 0) {
			map<int, int>::iterator iter = font->kernings[previousChar].spacing.find(currentChar);
			if (iter != font->kernings[previousChar].spacing.end())
				spacing = vec2(pFontInfo->xAdvance + font->spacing.x - xOffset + iter->second, 0);
			else spacing = vec2(pFontInfo->xAdvance + font->spacing.x - xOffset, 0);
		}
		else spacing = vec2(pFontInfo->xAdvance + font->spacing.x - xOffset, 0);
	}
	else spacing = vec2(pFontInfo->xAdvance + font->spacing.x - xOffset, 0);

	iter->charOffset[currentIndex] = iter->charOffset[currentIndex - 1] + vec2(spacing.x, 0);
	if (iter->charOffset[currentIndex].x + cFontInfo->size.x > size.x / fontSize) {
		iter->msgOffset.y += font->lineHeight;
		iter->charOffset[currentIndex].x = 0;
		iter->msgHeight += font->lineHeight;
		totalHeight += font->lineHeight;
	}
	iter->charOffset[currentIndex].y = iter->msgOffset.y + font->fontInfo[iter->message[currentIndex]].newOffset.y;
	iter->vao[currentIndex] = font->GetVao(iter->message[currentIndex]);

	PaintText(&*iter);

	if (totalHeight > size.y / fontSize)
		iter->visible = false;

	numChars++;
	currentString = msgInfo.begin();
	currentCharIndex = 0;
}

void Text::DeleteChar() {
	list<MsgInfo>::iterator iter = msgInfo.end();
	if (iter != msgInfo.begin()) {
		iter--;
	}
	else {
		return;
	}
	if (iter->message.size() == 1) {
		DeleteMessage(0);
		return;
	}
	iter->message.resize(iter->message.size() - 1);
	iter->charOffset.resize(iter->message.size() - 1);
	iter->vao.resize(iter->message.size() - 1);
	iter->color.resize(iter->message.size() - 1);
	
	PaintText(&*iter);
	numChars--;
}

void Text::UpdateMsg() {
	currentCharIndex++;
	if (currentCharIndex == currentString->message.size()) {
		currentCharIndex = 0;
		currentString++;
		if (currentString == msgInfo.end())
			currentString = msgInfo.begin();
	}
}

void Text::ResetMsgPtr() {
	currentCharIndex = 0;
	currentString = msgInfo.begin();
}

GLuint Text::GetVao() {
	return currentString->vao[currentCharIndex];
}

void Text::SetColor(const vec4 &color_first, const vec4 &color_end) {
	this->color_first = color_first;
	this->color_end = color_end;
	this->color_delta = color_end - color_first;

	list<MsgInfo>::iterator iter;
	for (iter = msgInfo.begin(); iter != msgInfo.end(); ++iter) {
		PaintText(&*iter);
	}
}

void Text::PaintText(MsgInfo *msgInfo) {
	float msgLength = msgInfo->message.size();
	float invMsgLength = 1.0f / msgLength;
	for (int i = 0; i < msgLength; i++) {
		msgInfo->color[i] = color_first + color_delta * invMsgLength * (float)i;
	}
}

wstring Text::GetCurrentMessage() {
	if (!msgInfo.empty())
		return msgInfo.begin()->message;
	else return wstring();
}