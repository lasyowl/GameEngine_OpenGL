#ifndef __TEXT_H
#define __TEXT_H

#include <list>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <gl/glew.h>

class Font;

struct MsgInfo {
	bool visible;
	float msgHeight;
	glm::vec2 msgOffset;
	std::wstring message;
	std::vector<glm::vec2> charOffset;
	std::vector<GLuint> vao;
	std::vector<glm::vec4> color;
};

class Text {
public:
	Text();
	Text(const glm::vec2 &origin, const glm::vec2 &size);
	~Text();

	void Activate();
	void Deactivate();
	void SetFont(Font *font);
	void SetSize(const glm::vec2 &size);
	void SetFontSize(const float &size);
	void SetOrigin(const glm::vec2 &origin);
	void SetColor(const glm::vec4 &color_first, const glm::vec4 &color_end);
	void InsertChar(const int &character);
	void AddMessage(std::wstring msg);
	void DeleteMessage(const int &index);
	void DeleteChar();
	void UpdateMsg();
	std::wstring GetCurrentMessage();
	void ResetMsgPtr();
	GLuint GetVao();

	int numChars;
	std::list<MsgInfo>::iterator currentString;
	int currentCharIndex;
	std::list<MsgInfo> msgInfo;
	Font *font;
	glm::vec2 origin; // Left-down point coord
	glm::vec2 size;
	float fontSize;
	glm::vec4 color_first, color_end, color_delta;
	float totalHeight;
	bool isActive;

private:
	void PaintText(MsgInfo *msgInfo);
};
#endif