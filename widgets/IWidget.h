#pragma once

class IWidget
{
	
protected:

    int updateTime; 
	int id;
	
public:

	virtual ~IWidget() {}
	virtual bool update() = 0; // true if need rerendering
	virtual void render() = 0;
	virtual void getRect(int & x, int & y, int & w, int & h) = 0;
	virtual void setId(int id) { this->id = id; }
	virtual int getUpdateTime() { return this->updateTime; }
	virtual int getWidth() { return 0; }  // for custom mode
	virtual int getHeight() { return 0; }  // for custom mode
	virtual bool isVisible() { return true; };
	virtual bool isTransparent() { return false; };
};
