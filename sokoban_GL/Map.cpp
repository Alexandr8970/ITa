#include "Map.h"

static char *texture_box_path               = "textures/box.bmp";
static char *texture_box_on_goal_path       = "textures/box_on_goal.bmp";
static char *texture_loader_on_goal_path    = "textures/loader_on_goal.bmp";
static char *texture_wall_path              = "textures/wall.bmp";
static char *texture_goal_path              = "textures/goal.bmp";
static char *texture_empty_path             = "textures/empty.bmp";
static char *texture_loader_path            = "textures/loader.bmp";
static char *texture_fon_path               = "textures/fon.bmp";

//-----------------------------------------------------------------------------
Map::Map()
    : mCurrentLevel(-1)
     ,mStartLevel(-1)
     ,mEndLevel(-1)
     ,mLoaderPosRow(0)
     ,mLoaderPosColumn(0)
     ,mViewWidth(0)
     ,mViewHeight(0)
     ,mInternalViewWidth(0)
     ,mInternalViewHeight(0)
     ,mMapObjectSize(0)
     ,mStartDrawX(0)
     ,mStartDrawY(0)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
Map::~Map()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void Map::setViewSize(const int &aWidth, const int &aHeight)
//-----------------------------------------------------------------------------
{
    mViewWidth = aWidth;
    mViewHeight = aHeight;

    mInternalViewWidth = mInternalViewHeight = (mViewWidth * 0.7);

    mStartDrawX = (mViewWidth - (mInternalViewWidth))/2;
    mStartDrawY = (mViewHeight - (mViewHeight - (mInternalViewWidth)));
}

//-----------------------------------------------------------------------------
void Map::loadLevels(const std::string &aMapPath)
//-----------------------------------------------------------------------------
{
    _load_from_file(aMapPath);
    _load_textures();
}

//-----------------------------------------------------------------------------
void Map::setLevel(int aLevel)
//-----------------------------------------------------------------------------
{
    if((aLevel >= mStartLevel) && (aLevel <= mEndLevel))
    {
        mLevelData = mInitLevelData;
        mCurrentLevel = aLevel;
        mMapObjectSize = (mLevelData[mCurrentLevel].mColumns > mLevelData[mCurrentLevel].mRows)
                         ? mInternalViewWidth / mLevelData[mCurrentLevel].mColumns
                         : mInternalViewWidth / mLevelData[mCurrentLevel].mRows;

        mLoaderPosRow    = mLevelData[mCurrentLevel].mLoaderRowPos;
        mLoaderPosColumn = mLevelData[mCurrentLevel].mLoaderColPos;
    }
    else
        mCurrentLevel = -1;
}

//-----------------------------------------------------------------------------
const int &Map::getStartLevel() const
//-----------------------------------------------------------------------------
{
    return mStartLevel;
}

//-----------------------------------------------------------------------------
const int &Map::getEndLevel() const
//-----------------------------------------------------------------------------
{
    return mEndLevel;
}

//-----------------------------------------------------------------------------
void Map::loaderMove(eMoveDirectiron aDirection)
//-----------------------------------------------------------------------------
{
    if(_isMoving(aDirection))
        _loader_move(aDirection);
}

//-----------------------------------------------------------------------------
void Map::_loader_move(eMoveDirectiron aDirection)
//-----------------------------------------------------------------------------
{
    int data_pos = (mLoaderPosRow * mLevelData[mCurrentLevel].mColumns + mLoaderPosColumn);
    mLevelData[mCurrentLevel].mData[data_pos] = MV_EMPTY;

    switch(aDirection)
    {
        case MD_UP:
        {
            mLoaderPosRow -= 1;
            break;
        }

        case MD_DOWN:
        {
            mLoaderPosRow += 1;
            break;
        }

        case MD_RIGHT:
        {
            mLoaderPosColumn += 1;
            break;
        }

        case MD_LEFT:
        {
            mLoaderPosColumn -= 1;
            break;
        }
    }
    data_pos = (mLoaderPosRow * mLevelData[mCurrentLevel].mColumns + mLoaderPosColumn);
    mLevelData[mCurrentLevel].mData[data_pos] = MV_LOADER;
}

//-----------------------------------------------------------------------------
bool Map::_isMoving(eMoveDirectiron aDirection)
//-----------------------------------------------------------------------------
{
    switch(aDirection)
    {
        case MD_UP:
        {
            if(!_isEmpty(mLoaderPosColumn, mLoaderPosRow - 1))
                return false;
            break;
        }

        case MD_DOWN:
        {
            if(!_isEmpty(mLoaderPosColumn, mLoaderPosRow + 1))
                return false;
            break;
        }

        case MD_RIGHT:
        {
            if(!_isEmpty(mLoaderPosColumn + 1, mLoaderPosRow ))
                return false;
            break;
        }

        case MD_LEFT:
        {
            if(!_isEmpty(mLoaderPosColumn - 1, mLoaderPosRow ))
                return false;
            break;
        }
    }
    return true;
}

//-----------------------------------------------------------------------------
bool Map::_isEmpty(const int &aColumn, const int &aRow)
//-----------------------------------------------------------------------------
{
    int data_pos = (aRow * mLevelData[mCurrentLevel].mColumns + aColumn);
    if(mLevelData[mCurrentLevel].mData[data_pos] != MV_EMPTY)
        return false;

    return true;
}

//-----------------------------------------------------------------------------
void Map::draw()
//-----------------------------------------------------------------------------
{
    if((mCurrentLevel < mStartLevel) || (mCurrentLevel > mEndLevel))
        return;

    int index = 0;
    // column - X
    // row    - Y
    for(int row = 0; row < mLevelData[mCurrentLevel].mRows; ++row)
    {
        for(int col = 0; col < mLevelData[mCurrentLevel].mColumns; ++col)
        {
            _draw_map_object(col, row, (eMapValue)mLevelData[mCurrentLevel].mData[index]);
            ++index;
        }
    }
}

//-----------------------------------------------------------------------------
void Map::_draw_map_object(const int &aColumn, const int &aRow, eMapValue aMapObjType)
//-----------------------------------------------------------------------------
{
    GLuint map_obj;

    switch(aMapObjType)
    {
        case MV_WALL:
        {
            map_obj = texture_wall;
            break;
        }

        case MV_LOADER:
        {
            map_obj = texture_loader;
            break;
        }

        case MV_LOADER_ON_GOAL:
        {
            map_obj = texture_loader_on_goal;
            break;
        }

        case MV_BOX:
        {
            map_obj = texture_box;
            break;
        }

        case MV_BOX_ON_GOAL:
        {
            map_obj = texture_box_on_goal;
            break;
        }

        case MV_GOAL:
        {
            map_obj = texture_goal;
            break;
        }

        case MV_EMPTY:
        {
            map_obj = texture_empty;
            break;
        }
    }

    // column - X
    // row    - Y
    int g_x = mStartDrawX + aColumn * mMapObjectSize;
    int g_y = mStartDrawY - (aRow * mMapObjectSize)  - mMapObjectSize;

    glBindTexture(GL_TEXTURE_2D, map_obj);
    glBegin(GL_QUADS);
        glTexCoord2i(0, 1); 
        glVertex2i(g_x, g_y + mMapObjectSize); 
        
        glTexCoord2i(1, 1); 
        glVertex2i(g_x + mMapObjectSize, g_y + mMapObjectSize); 
        
        glTexCoord2i(1, 0); 
        glVertex2i(g_x + mMapObjectSize, g_y);
        
        glTexCoord2i(0, 0); 
        glVertex2i(g_x, g_y);
    glEnd();
}

//-----------------------------------------------------------------------------
void Map::_load_from_file(const std::string &aMapPath)
//-----------------------------------------------------------------------------
{
    ifstream in_file;
    in_file.open(aMapPath, ios::in | ios::app | ios::binary);

    mLevelData.clear();
    mLevelData.resize(1);

    while(!in_file.eof())
    {
        sLevelData data;
        _read_level_info(in_file, data);
        _read_level_data(in_file, data);
        mLevelData.push_back(data);
    }
    in_file.close();

    mStartLevel = mLevelData[1].mLevel;
    mEndLevel   = mLevelData[mLevelData.size() - 1].mLevel;

    mInitLevelData = mLevelData;
}

//-----------------------------------------------------------------------------
void Map::_read_level_info(ifstream &aFile, sLevelData &aLevel)
//-----------------------------------------------------------------------------
{
    int count_param = 0;
    std::string tmpStr;

    char str_line[256];
    std::string firstLevelLine;

    aFile.getline(str_line, 256);
    firstLevelLine = str_line;

    for(int i = 0; i < firstLevelLine.size(); ++i)
    {
        if((firstLevelLine[i] == ',') || (i == firstLevelLine.size() - 1))
        {
            switch(count_param)
            {
                case 0:
                {
                    aLevel.mLevel = atoi(tmpStr.c_str());
                    break;
                }

                case 1:
                {
                    aLevel.mColumns = atoi(tmpStr.c_str());
                    break;
                }

                case 2:
                {
                    aLevel.mRows = atoi(tmpStr.c_str());
                    break;
                }
            }

            tmpStr = "";
            ++count_param;
        }
        else
            tmpStr += firstLevelLine[i];
    }
}

//-----------------------------------------------------------------------------
void Map::_read_level_data(ifstream &aFile, sLevelData &aLevel)
//-----------------------------------------------------------------------------
{
    char str_line[256];
    std::string firstLevelLine;

    aLevel.mData.resize(aLevel.mColumns * aLevel.mRows, ' ');
    int start_index = 0;

    for(int i = 0; i < aLevel.mRows; ++i)
    {
        aFile.getline(str_line, 256);
        firstLevelLine  = str_line;

        start_index = i * aLevel.mColumns;
        for(int j = 0; j < firstLevelLine.size() - 1; ++j)
        {
            aLevel.mData[start_index] = firstLevelLine[j];
            if(firstLevelLine[j] == MV_LOADER)
            {
                aLevel.mLoaderRowPos = i;
                aLevel.mLoaderColPos = j;
            }
            ++start_index;
        }
    }
}

//-----------------------------------------------------------------------------
void Map::_load_textures()
//-----------------------------------------------------------------------------
{
    _load_texture_box();
    _load_texture_box_on_goal();
    _load_texture_loader_on_goal();
    _load_texture_wall();
    _load_texture_goal();
    _load_texture_empty();
    _load_texture_loader();
    _load_texture_fon();
}

//-----------------------------------------------------------------------------
void Map::_load_texture_box()
//-----------------------------------------------------------------------------
{
    AUX_RGBImageRec *texture;
    texture = auxDIBImageLoad(texture_box_path);
    glGenTextures(1, &texture_box);
    glBindTexture(GL_TEXTURE_2D, texture_box);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, texture->sizeX, texture->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, texture->data);
}

//-----------------------------------------------------------------------------
void Map::_load_texture_box_on_goal()
//-----------------------------------------------------------------------------
{
    AUX_RGBImageRec *texture;
    texture = auxDIBImageLoad(texture_box_on_goal_path);
    glGenTextures(1, &texture_box_on_goal);
    glBindTexture(GL_TEXTURE_2D, texture_box_on_goal);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, texture->sizeX, texture->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, texture->data);
}

//-----------------------------------------------------------------------------
void Map::_load_texture_loader_on_goal()
//-----------------------------------------------------------------------------
{
    AUX_RGBImageRec *texture;
    texture = auxDIBImageLoad(texture_loader_on_goal_path);
    glGenTextures(1, &texture_loader_on_goal);
    glBindTexture(GL_TEXTURE_2D, texture_loader_on_goal);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, texture->sizeX, texture->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, texture->data);
}

//-----------------------------------------------------------------------------
void Map::_load_texture_wall()
//-----------------------------------------------------------------------------
{
    AUX_RGBImageRec *texture;
    texture = auxDIBImageLoad(texture_wall_path);
    glGenTextures(1, &texture_wall);
    glBindTexture(GL_TEXTURE_2D, texture_wall);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, texture->sizeX, texture->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, texture->data);
}

//-----------------------------------------------------------------------------
void Map::_load_texture_goal()
//-----------------------------------------------------------------------------
{
    AUX_RGBImageRec *texture;
    texture = auxDIBImageLoad(texture_goal_path);
    glGenTextures(1, &texture_goal);
    glBindTexture(GL_TEXTURE_2D, texture_goal);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, texture->sizeX, texture->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, texture->data);
}

//-----------------------------------------------------------------------------
void Map::_load_texture_empty()
//-----------------------------------------------------------------------------
{
    AUX_RGBImageRec *texture;
    texture = auxDIBImageLoad(texture_empty_path);
    glGenTextures(1, &texture_empty);
    glBindTexture(GL_TEXTURE_2D, texture_empty);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, texture->sizeX, texture->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, texture->data);
}

//-----------------------------------------------------------------------------
void Map::_load_texture_loader()
//-----------------------------------------------------------------------------
{
    AUX_RGBImageRec *texture;
    texture = auxDIBImageLoad(texture_loader_path);
    glGenTextures(1, &texture_loader);
    glBindTexture(GL_TEXTURE_2D, texture_loader);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, texture->sizeX, texture->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, texture->data);
}

//-----------------------------------------------------------------------------
void Map::_load_texture_fon()
//-----------------------------------------------------------------------------
{
    AUX_RGBImageRec *texture;
    texture = auxDIBImageLoad(texture_fon_path);
    glGenTextures(1, &texture_fon);
    glBindTexture(GL_TEXTURE_2D, texture_fon);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, texture->sizeX, texture->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, texture->data);
}
