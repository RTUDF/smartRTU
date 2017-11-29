#pragma once

class Ads
{
private:
    int id; //~
    char File[100];
protected:
    virtual bool Load(const char * jsonFile);
    bool checkFileData(); // true - file changed;
public:
    Ads(int id, const char *jsonFile);
    ~Ads();
    virtual bool update();
    virtual void render() = 0;
    virtual const char * getCaption() { return "Par mums"; }
    virtual bool isActual() { returm true; }
    virtual int getId();    //~
};
