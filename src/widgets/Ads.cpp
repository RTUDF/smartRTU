#include "Ads.h"

bool Ads::Load(const char * jsonFile)
{

}

Ads::Ads(int id, const char *jsonFile)
{
    this.File = jsonFile;
    Load(File);
    this.id = id;
}

Ads::~Ads()
{}

bool Ads::update()
{
    return checkFileData();
}

int Ads::getId()
{
    return id;
}

bool checkFileData()
{
    return false; // true - file changed;
}